#include<iostream>
#include<vector>
#include<algorithm>

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

// Falta verificação de dia
// Testar com geração de input (ter certeza que o máximo de categorias está funcionando)

int main() {
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
        todos_filmes.push_back({i, this_h_inicio, this_h_fim, this_categoria, this_h_fim - this_h_inicio});
    }

    //ordenar
    sort(todos_filmes.begin(), todos_filmes.end(), heuristica_por_duracao);
    for(int i = 0; i < n_filme; i++){
        cout << todos_filmes[i].h_inicio << " " << todos_filmes[i].h_fim << " " << todos_filmes[i].categoria << '\n';
    }
    int horario = 0;
    for(auto& this_filme : todos_filmes){
        //Duas verificações: se o horario é menor que o horario inicial 
        //                   se a categoria não estoura o máximo daquela categoria
        this_id = this_filme.id;
        this_h_inicio=this_filme.h_inicio;
        this_h_fim=this_filme.h_fim;
        this_categoria=this_filme.categoria;

        if (horario <= this_h_inicio && max_categorias[this_categoria] - 1 >= 0){
            horario = this_h_fim;
            max_categorias[this_categoria] -= 1;
            mochila.push_back({this_id, this_h_inicio, this_h_fim, this_categoria});
        }
    }
    //ordenando para imprimir
    sort(mochila.begin(), mochila.end(), [](auto& i, auto&j){return i.id < j.id;});
    cout << "Horário final: " << horario << '\n';
    for(auto& el: mochila){
        cout << el.id << " ";
    }
    cout << '\n';
    return 0;
}