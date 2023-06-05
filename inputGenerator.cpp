#include <chrono>
#include <random>
#include <fstream>
#include <string>
#include <boost/random.hpp>

using namespace std;

void inputGenerator(int n, int m) {
    ofstream inputFile;
    inputFile.open("input_"+to_string(n)+"_"+to_string(m));
    inputFile << n << " " << m << endl;

    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    default_random_engine generator (seed);

    // Definindo distribuição normal com média de 4 e desvio padrão de 1
    normal_distribution<double> distribution_dif(4, 1.0);

    uniform_int_distribution<int> distribution_hr(0, 23);
    uniform_int_distribution<int> distribution_cat(1, m);

    vector<int> maxFilmes(m); // Vetor para armazenar o máximo de filmes por categoria
    for (int i = 0; i < m; i++) {
        maxFilmes[i] = distribution_cat(generator); // Gerando o máximo de filmes para cada categoria
        inputFile << maxFilmes[i] << " "; // Escrevendo o valor no arquivo de entrada
    }
    inputFile << endl;

    for (int i = 0; i < n; i++) {
        int hora_inicio = distribution_hr(generator);
        double dif_media = distribution_dif(generator);
        int hora_fim = ((int)hora_inicio + (int)round(dif_media)) % 24;
        int categoria = distribution_cat(generator);

        inputFile << hora_inicio << " " << hora_fim << " " << categoria << endl;
    }


    inputFile.close();
}


int main(int argc, char *argv[]){
    int n_filmes_inicial = 1;
    int n_filmes_final = 25;

    int n_cat_inicial = 1;
    int n_cat_final = 10;

    for (int n_filme=n_filmes_inicial; n_filme<=n_filmes_final; n_filme++){
        for (int n_cat=n_cat_inicial; n_cat<=n_cat_final; n_cat++){
            inputGenerator(n_filme, n_cat);
        }
    }
    return 0;
}