#include <iostream>
#include <vector>
#include <algorithm>
#include <thrust/host_vector.h>
#include <thrust/device_vector.h>
#include <thrust/transform_reduce.h>
#include <fstream>


#define RESET   "\033[0m"
#define BLACK   "\033[30m"      /* Black */
#define RED     "\033[31m"      /* Red */
#define GREEN   "\033[32m"      /* Green */
#define YELLOW  "\033[33m"      /* Yellow */
#define BLUE    "\033[34m"      /* Blue */
#define MAGENTA "\033[35m"      /* Magenta */
#define CYAN    "\033[36m"      /* Cyan */
#define WHITE   "\033[37m"      /* White */
#define BOLDBLACK   "\033[1m\033[30m"      /* Bold Black */
#define BOLDRED     "\033[1m\033[31m"      /* Bold Red */
#define BOLDGREEN   "\033[1m\033[32m"      /* Bold Green */
#define BOLDYELLOW  "\033[1m\033[33m"      /* Bold Yellow */
#define BOLDBLUE    "\033[1m\033[34m"      /* Bold Blue */
#define BOLDMAGENTA "\033[1m\033[35m"      /* Bold Magenta */
#define BOLDCYAN    "\033[1m\033[36m"      /* Bold Cyan */
#define BOLDWHITE   "\033[1m\033[37m"      /* Bold White */

struct filme {
    int id;
    int h_inicio;
    int h_fim;
    int categoria;
    int duracao;
};

using namespace std;

void generateOuput(vector<filme> mochila, int n_filme, int n_cat){
    ofstream outputFile;
    outputFile.open("../outputs2/output_exau_"+to_string(n_filme)+"_"+to_string(n_cat));
    outputFile << n_filme << " " << n_cat << endl;

    double tempo_tela=0;

    int n_mochila = 0;
    for(auto& this_film : mochila){
        tempo_tela +=this_film.duracao;
        n_mochila += 1;
    }
    double media = tempo_tela/(double)n_mochila;
    outputFile << n_mochila << " " << tempo_tela << " " << media << endl;
    outputFile.close();

}

void output_visual(vector<filme> mochila){
    cout<<"\n\n\n";
    
    cout << "|-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|" << endl;
    cout << "|00\t01\t02\t03\t04\t05\t06\t07\t08\t09\t10\t11\t12\t13\t14\t15\t16\t17\t18\t19\t20\t21\t22\t23\t|" << endl;
    int agora = 0;
    int numero_de_espacos = 0;
    int numero_de_filme = 0;
    for(auto& this_film : mochila){

        agora = this_film.h_inicio;
        numero_de_espacos = agora;
        numero_de_filme = this_film.h_fim - this_film.h_inicio;
        for(int i = 0; i<numero_de_espacos; i++){
            cout<<"\t";
        }

        for(int i = 0; i<numero_de_filme; i++){
            if(this_film.categoria==1){
                cout << RED;
            }
            else if (this_film.categoria==2){
                cout << GREEN;
            }
            else if (this_film.categoria==3){
                cout << MAGENTA;
            }
            else if (this_film.categoria==4){
                cout << YELLOW;
            }
            else if (this_film.categoria==5){
                cout << BLUE;
            }
            else if (this_film.categoria==6){
                cout << MAGENTA;
            }
            else if (this_film.categoria==7){
                cout << CYAN;
            }
            
            cout<<"|"<<"|"<<"|"<<"|"<<"|"<<"|"<<"|"<<"|";
            cout << RESET;
        }
        cout<<endl;
        
    }
    cout << "|-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|" << endl;

    // for(auto& this_film : mochila){
    //     cout << this_film.h_inicio << " " << this_film.h_fim << " " << this_film.id << '\n';
    // }
}

void get_combinations(vector<filme> movies, vector<vector<filme>> &combinations)
{
  int slent = pow(2, movies.size());

  for (int i = 0; i < slent; i++)
  {
    vector<filme> temp;
    for (int j = 0; j < movies.size(); j++)
    {
      if ((i & int(pow(2, j))))
      {
        temp.push_back(movies[j]);
        if (temp.size() > 25) break;
      }
    }
    if (temp.size() > 0)
    {
      combinations.push_back(temp);
    }
  }

  sort(combinations.begin(), combinations.end(), [](auto& i, auto& j){return i.size() > j.size();});

  return;             
}


// Functor for verifying agenda constraints
struct VerifyAgenda {
    const thrust::device_vector<bool>& agenda;

    __host__ __device__
    bool operator()(const filme& this_filme) const {
        if (this_filme.h_inicio == this_filme.h_fim)
            return !agenda[this_filme.h_inicio];

        for (int i = this_filme.h_inicio; i < this_filme.h_fim; i++) {
            if (agenda[i] == true) {
                return false;
            }
        }
        return true;
    }
};

// Functor for filling the agenda
struct FillAgenda {
    thrust::device_vector<bool>& agenda;

    __host__ __device__
    void operator()(const filme& this_filme) const {
        if (this_filme.h_inicio == this_filme.h_fim)
            agenda[this_filme.h_inicio] = true;
        else {
            for (int i = this_filme.h_inicio; i < this_filme.h_fim; i++) {
                agenda[i] = true;
            }
        }
    }
};

// Functor for calculating the duration of a film
struct CalculateDuration {
    __host__ __device__
    int operator()(const filme& this_filme) const {
        return this_filme.duracao;
    }
};

int main() {
    int n_filme = 0;
    int n_cat = 0;
    vector<filme> todos_filmes;

    vector<int> max_categorias;

    cin >> n_filme >> n_cat;

    todos_filmes.reserve(n_filme);
    int this_max_cat = 0;
    for (int i = 0; i < n_cat; i++) {
        cin >> this_max_cat;
        max_categorias.push_back(this_max_cat);
    }

    int this_h_inicio = 0;
    int this_h_fim;
    int this_categoria = 0;

    for (int i = 0; i < n_filme; i++) {
        cin >> this_h_inicio >> this_h_fim >> this_categoria;
        if (this_h_fim < this_h_inicio) {
            this_h_fim += 24;
        }
        todos_filmes.push_back({ i, this_h_inicio, this_h_fim, this_categoria, this_h_fim - this_h_inicio });
    }

    vector<vector<filme>> todas_possibilidades;
    get_combinations(todos_filmes, todas_possibilidades);

    double melhor_tempo_tela = 0;
    vector<filme> melhor_solucao;

    thrust::host_vector<bool> melhor_agenda_host(24, false);
    thrust::device_vector<bool> melhor_agenda = melhor_agenda_host;

    for (auto& solucao : todas_possibilidades) {
        thrust::device_vector<bool> agenda(24, false);
        thrust::device_vector<int> max_categorias_copia = max_categorias;
        thrust::device_vector<filme> d_solucao = solucao;

        // Verify agenda and subtract category counts
        auto valid_solution_end = thrust::remove_if(d_solucao.begin(), d_solucao.end(), VerifyAgenda{ agenda });
        valid_solution_end = thrust::remove_if(d_solucao.begin(), valid_solution_end, [&max_categorias_copia] __device__(const filme& this_filme) {
            return max_categorias_copia[this_filme.categoria - 1]-- <= 0;
        });
        d_solucao.erase(valid_solution_end, d_solucao.end());

        // Fill agenda for valid films
        thrust::for_each(d_solucao.begin(), d_solucao.end(), FillAgenda{ agenda });

        // Calculate total duration
        double tempo_tela = thrust::transform_reduce(d_solucao.begin(), d_solucao.end(), CalculateDuration(), 0, thrust::plus<int>());

        if (tempo_tela > melhor_tempo_tela) {
            melhor_tempo_tela = tempo_tela;
            melhor_solucao = solucao;
            melhor_agenda = agenda;
        }
    }

    // generateOuput(melhor_solucao, n_filme, n_cat);

    cout << '\n';
    return 0;
}
