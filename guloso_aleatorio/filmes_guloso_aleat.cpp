#include<iostream>
#include <chrono>
#include<vector>
#include<algorithm>
#include <random>
#include <map>
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
    outputFile.open("../outputs/output_aleat_"+to_string(n_filme)+"_"+to_string(n_cat));
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

    for(auto& this_film : mochila){
        cout << this_film.h_inicio << " " << this_film.h_fim << " " << this_film.id << '\n';
    }
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

    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    default_random_engine generator(seed);
    uniform_int_distribution<int> distribution(1,4);

    int i = 1;
    int n_filme_copy = n_filme;

    for(auto& this_filme : todos_filmes){
        int prob = distribution(generator); // gera número

        if (verify_agenda(agenda, this_filme) && max_categorias[this_filme.categoria-1] - 1 >= 0){
            max_categorias[this_filme.categoria-1] -= 1;
            agenda = fill_agenda(agenda, this_filme);
            mochila.push_back(this_filme);

        }
        if (prob==4 && i<n_filme_copy){
            uniform_int_distribution<int> distribution_id(i, n_filme_copy-1);
            int id_aleat = distribution_id(generator);
            filme filme_aleat = todos_filmes[id_aleat];

            if (verify_agenda(agenda, filme_aleat) && max_categorias[filme_aleat.categoria-1] - 1 >= 0){
                max_categorias[filme_aleat.categoria-1] -= 1;
                agenda = fill_agenda(agenda, filme_aleat);
                mochila.push_back(filme_aleat);
                todos_filmes.erase(todos_filmes.begin()+id_aleat-1);
                n_filme_copy = n_filme_copy - 1;
            
            }

        }
        i++;
        
    }
    
    //output_visual(mochila);
    generateOuput(mochila, n_filme, n_cat);

    cout << '\n';
    return 0;
}
// gerar arquivo de output com n_filmes, tempo de tela, tempo de execução, média da duração dos filmes, 

// Formato
// n_filmes_input    n_cat_input
// tempo_de_tela     média_filmes

