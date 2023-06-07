#include<iostream>
#include <chrono>
#include<vector>
#include<algorithm>
#include <map>
#include <fstream>
#include <cmath>
#include <omp.h>

using namespace std;

struct filme {
    int id;
    int h_inicio;
    int h_fim;
    int categoria;
    int duracao;
};

void generateOuput(vector<filme> mochila, int n_filme, int n_cat){
    ofstream outputFile;
    outputFile.open("./outputs/output_parall_"+to_string(n_filme)+"_"+to_string(n_cat));
    outputFile << n_filme << " " << n_cat << endl;

    int n_mochila = 0;
    for(auto& this_film : mochila){
        n_mochila += 1;
    }

    outputFile << n_mochila << endl;
    outputFile.close();

}

bool verify_agenda(map<int, bool> agenda, filme this_filme){
    if (this_filme.h_inicio == this_filme.h_fim)
        return !agenda[this_filme.h_inicio];

    for (int i=this_filme.h_inicio; i<this_filme.h_fim; i++){
        if (agenda[i]==true){
            return false;
        }
    }
    return true;
}

map<int, bool> fill_agenda(map<int, bool> agenda, filme this_filme){
    if (this_filme.h_inicio == this_filme.h_fim)
        agenda[this_filme.h_inicio] = true;
    else{
        for (int i=this_filme.h_inicio; i<this_filme.h_fim; i++){
            agenda[i] = true;
        }  
    }

    return agenda;
}

void get_combinations(vector<filme> movies, vector<vector<filme>>& combinations)
{
  int slent = pow(2, movies.size());

  #pragma omp parallel for
  for (int i = 0; i < slent; i++)
  {
    vector<filme> temp;
    for (int j = 0; j < movies.size(); j++)
    {
      if ((i & int(pow(2, j))))
      {
        temp.push_back(movies[j]);
        if (temp.size() > 25)
          break;
      }
    }
    if (temp.size() > 0)
    {
      #pragma omp critical
      {
        combinations.push_back(temp);
      }
    }
  }

  sort(combinations.begin(), combinations.end(), [](auto& i, auto& j) { return i.size() > j.size(); });

  return;
}

int main() {

    int n_filme = 0;
    int n_cat = 0;
    vector<filme> todos_filmes;

    vector<int> max_categorias;

    cin >> n_filme >> n_cat;

    todos_filmes.reserve(n_filme);
    int this_max_cat=0;
    for(int i = 0; i < n_cat; i++){
        cin >> this_max_cat;
        max_categorias.push_back(this_max_cat);
    }

    int this_id = 0;
    int this_h_inicio=0;
    int this_h_fim;
    int this_categoria=0;

    for(int i = 0; i < n_filme; i++){
        cin >> this_h_inicio >> this_h_fim >> this_categoria;
        if (this_h_fim<this_h_inicio){
            this_h_fim += 24;
        }
        todos_filmes.push_back({i, this_h_inicio, this_h_fim, this_categoria, this_h_fim - this_h_inicio});
    }

    int n_max_film = 25;
    vector<vector<filme>> todas_possibilidades;

    get_combinations(todos_filmes, todas_possibilidades);

    double melhor_tempo_tela;
    vector<filme> melhor_solucao;
    map<int, bool> melhor_agenda;

    #pragma omp parallel for
    for (int i = 0; i < todas_possibilidades.size(); i++) {
        map<int, bool> agenda;
        vector<int> max_categorias_copia = max_categorias;
        vector<filme> mochila;

        bool valid_solution = true;

        for (auto& this_filme : todas_possibilidades[i]) {
            if (verify_agenda(agenda, this_filme) && max_categorias_copia[this_filme.categoria - 1] - 1 >= 0) {
                max_categorias_copia[this_filme.categoria - 1] -= 1;
                agenda = fill_agenda(agenda, this_filme);
                continue;
            }
            valid_solution = false;
        }

        if (valid_solution) {
            double tempo_tela = 0;
            for (auto& this_film : todas_possibilidades[i]) {
                tempo_tela += this_film.duracao;
            }

            #pragma omp critical
            {
                if (tempo_tela > melhor_tempo_tela) {
                    melhor_tempo_tela = tempo_tela;
                    melhor_solucao = todas_possibilidades[i];
                    melhor_agenda = agenda;
                }
            }
        }
    }

    
    cout << endl;
    generateOuput(melhor_solucao, n_filme, n_cat);

    cout << '\n';
    return 0;
}
