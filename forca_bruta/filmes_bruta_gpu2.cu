#include <iostream>
#include <algorithm>
#include <map>
#include <fstream>
#include <cmath>
#include <thrust/device_vector.h>
#include <thrust/host_vector.h>
#include <thrust/copy.h>
#include <thrust/device_ptr.h>

using namespace std;

struct filme {
    int id;
    int h_inicio;
    int h_fim;
    int categoria;
    int duracao;
};

void generateOuput(int n_adicionados, int n_filme, int n_cat) {
    ofstream outputFile;
    outputFile.open("./outputs/output_gpu_" + to_string(n_filme) + "_" + to_string(n_cat));
    outputFile << n_filme << " " << n_cat << endl;

    outputFile << n_adicionados << endl;
    outputFile.close();
}

struct get_combinations_max_movies 
{
   int *max_categorias;
   filme *todos_filmes;
   int n_filmes;
   int n_cat;


    get_combinations_max_movies(int _n_filmes, filme *_todos_filmes, int _n_cat ,int *_max_categorias) : n_filmes(_n_filmes), todos_filmes(_todos_filmes), n_cat(_n_cat) ,max_categorias(_max_categorias) {};
   __device__ __host__
   int operator() (int slent)
   {  

      bool agenda[30];
      for(int h = 0; h < 30; h++) {
        agenda[h] = false;
      }

      int max_categorias_cp[30];
      for(int h = 0; h <= n_cat; h++) {
        max_categorias_cp[h] = max_categorias[h];
      }

      int n_filmes_assistidos = 0;
      for(int i = 0; i < n_filmes; i++)
      {
          if ((slent & int(pow(2, i))))
          {
              // Verificação da agenda
              if(todos_filmes[i].h_inicio == todos_filmes[i].h_fim) {
                if(agenda[todos_filmes[i].h_inicio]) return -1;
                agenda[todos_filmes[i].h_inicio] = true;
              } 
              else {
                for(int j = todos_filmes[i].h_inicio; j < todos_filmes[i].h_fim; j++) {
                  if(agenda[j]) return -1;
                  agenda[j] = true;
                }
              }
              if(max_categorias_cp[todos_filmes[i].categoria] == 0) return -1;
              max_categorias_cp[todos_filmes[i].categoria] --;
              n_filmes_assistidos++;
          }
      }
      return n_filmes_assistidos;
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

    thrust::device_vector<int> cats_lim_gpu(max_categorias);

    thrust::device_vector<filme> mov_gpu(todos_filmes);

    thrust::device_vector<int> mov_count(pow(2, todos_filmes.size()));
    
    thrust::counting_iterator<int> comb(0);

    thrust::transform(comb, comb + pow(2, todos_filmes.size()), mov_count.begin(), get_combinations_max_movies(todos_filmes.size(), thrust::raw_pointer_cast(mov_gpu.data()), n_cat ,thrust::raw_pointer_cast(cats_lim_gpu.data())));

    int best = *thrust::max_element(mov_count.begin(), mov_count.end());

    generateOuput(best, n_filme, n_cat);

    cout << "best: " <<  best << endl;
    return 0;
}

