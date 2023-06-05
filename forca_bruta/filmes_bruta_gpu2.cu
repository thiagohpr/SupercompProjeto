#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <fstream>
#include <cmath>
#include <thrust/device_vector.h>
#include <thrust/host_vector.h>
#include <thrust/copy.h>

using namespace std;

struct filme {
    int id;
    int h_inicio;
    int h_fim;
    int categoria;
    int duracao;
};

void generateOuput(vector<filme> mochila, int n_filme, int n_cat) {
    ofstream outputFile;
    outputFile.open("../outputs2/output_exau_" + to_string(n_filme) + "_" + to_string(n_cat));
    outputFile << n_filme << " " << n_cat << endl;

    double tempo_tela = 0;

    int n_mochila = 0;
    for (auto& this_film : mochila) {
        tempo_tela += this_film.duracao;
        n_mochila += 1;
    }
    double media = tempo_tela / (double)n_mochila;
    outputFile << n_mochila << " " << tempo_tela << " " << media << endl;
    outputFile.close();
}

struct verify_agenda_functor {
    const thrust::device_vector<bool>& agenda;
    verify_agenda_functor(const thrust::device_vector<bool>& _agenda) : agenda(_agenda) {}

    __host__ __device__
    bool operator()(const filme& this_filme) {
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

struct fill_agenda_functor {
    thrust::device_vector<bool>& agenda;
    fill_agenda_functor(thrust::device_vector<bool>& _agenda) : agenda(_agenda) {}

    __host__ __device__
    void operator()(const filme& this_filme) {
        if (this_filme.h_inicio == this_filme.h_fim)
            agenda[this_filme.h_inicio] = true;
        else {
            for (int i = this_filme.h_inicio; i < this_filme.h_fim; i++) {
                agenda[i] = true;
            }
        }
    }
};

struct CalculateDuration {
    __host__ __device__
    int operator()(const filme& this_filme) const {
        return this_filme.duracao;
    }
};

void get_combinations(const vector<filme>& movies, vector<vector<filme>>& combinations) {
    int slent = pow(2, movies.size());

    for (int i = 0; i < slent; i++) {
        vector<filme> temp;
        for (int j = 0; j < movies.size(); j++) {
            if ((i & int(pow(2, j)))) {
                temp.push_back(movies[j]);
                if (temp.size() > 25) break;
            }
        }
        if (temp.size() > 0) {
            combinations.push_back(temp);
        }
    }

    sort(combinations.begin(), combinations.end(), [](const auto& i, const auto& j) { return i.size() > j.size(); });
}

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

    for (auto& solucao : todas_possibilidades) {
        thrust::device_vector<filme> d_solucao(solucao.begin(), solucao.end());
        thrust::device_vector<bool> agenda(24, false);
        thrust::device_vector<int> max_categorias_copia(max_categorias.begin(), max_categorias.end());

        bool valid_solution = true;

        verify_agenda_functor verify_functor(agenda);
        fill_agenda_functor fill_functor(agenda);

        auto valid_solution_end = thrust::remove_if(d_solucao.begin(), d_solucao.end(), verify_functor);

        if (valid_solution_end != d_solucao.end()) {
            valid_solution = false;
        }

        if (valid_solution) {
            double tempo_tela = thrust::transform_reduce(d_solucao.begin(), d_solucao.end(),CalculateDuration(), 0.0, thrust::plus<double>());

            if (tempo_tela > melhor_tempo_tela) {
                melhor_tempo_tela = tempo_tela;
                thrust::copy(d_solucao.begin(), d_solucao.end(), melhor_solucao.begin());
            }
        }
    }

    generateOuput(melhor_solucao, n_filme, n_cat);

    cout << '\n';
    return 0;
}
