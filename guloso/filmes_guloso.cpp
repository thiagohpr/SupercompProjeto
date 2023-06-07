#include<iostream>
#include <chrono>
#include<vector>
#include<algorithm>
#include <map>
#include <fstream>

using namespace std;

struct filme {
    int id;
    int h_inicio;
    int h_fim;
    int categoria;
    int duracao;
};

bool heuristica_por_final(filme a, filme b){
    return a.h_fim < b.h_fim; // ordenando pelo horário de término do filme
}

bool heuristica_por_duracao(filme a, filme b){
    return a.duracao < b.duracao; // ordenando pela duração do filme
}

void generateOuput(vector<filme> mochila, int n_filme, int n_cat){
    ofstream outputFile;
    outputFile.open("./outputs/output_gul_"+to_string(n_filme)+"_"+to_string(n_cat));
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
    map<int, bool>::iterator it;

    for(it=agenda.begin(); it!=agenda.end(); ++it){
    cout << it->first << " => " << it->second << '\n';
    }
    cout << "-----------------" << '\n';
    return agenda;
}

int main() {

    map<int, bool> agenda;

    int n_filme = 0;
    int n_cat = 0;
    vector<filme> mochila;
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

    //ordenar
    sort(todos_filmes.begin(), todos_filmes.end(), heuristica_por_final);


    for(auto& this_filme : todos_filmes){

        if (verify_agenda(agenda, this_filme) && max_categorias[this_filme.categoria-1] - 1 >= 0){
            max_categorias[this_filme.categoria-1] -= 1;
            agenda = fill_agenda(agenda, this_filme);
            mochila.push_back(this_filme);

        }
        
    }

    generateOuput(mochila, n_filme, n_cat);

    cout << '\n';
    return 0;
}


