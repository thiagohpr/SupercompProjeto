#include<iostream>
#include <chrono>
#include<vector>
#include<algorithm>
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

// Estrutura principal do objeto a ser estudado
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

// Função responsável por receber os Filmes Escolhidos e gerar um txt com as variáveis resposta
void generateOutput(vector<filme> mochila, int n_filme, int n_cat){
    ofstream outputFile;
    outputFile.open("../outputs/output_gul_"+to_string(n_filme)+"_"+to_string(n_cat)); // nome do arquivo a partir do input
    outputFile << n_filme << " " << n_cat << endl;

    double tempo_tela=0;
    int n_mochila = 0;

    // Contando filmes dentro do vetor
    for(auto& this_film : mochila){
        tempo_tela +=this_film.duracao;
        n_mochila += 1;
    }

    double media = tempo_tela/(double)n_mochila; // Calculando média de tempos dos filmes assistidos
    outputFile << n_mochila << " " << tempo_tela << " " << media << endl; // Escrevendo as variáveis no arquivo
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

// Função que verifica se o filme pode ser assistido
bool verify_agenda(map<int, bool> agenda, filme this_filme){
    // Se iniciar e acabar no mesmo horário, bastar retornar o invertido do booleando da posição
    if (this_filme.h_inicio == this_filme.h_fim) 
        return !agenda[this_filme.h_inicio];

    // Retornar Falso se algum horário do filme ter booleano true (já foi preenchido)
    for (int i=this_filme.h_inicio; i<this_filme.h_fim; i++){
        if (agenda[i]==true){
            return false;
        }
    }
    return true;
}

// Função que preenche os horários do filme na agenda
map<int, bool> fill_agenda(map<int, bool> agenda, filme this_filme){
    // Se iniciar e acabar no mesmo horário, preencher o horário para true
    if (this_filme.h_inicio == this_filme.h_fim)
        agenda[this_filme.h_inicio] = true;
    else{
        // Preencher todos os horários para true
        for (int i=this_filme.h_inicio; i<this_filme.h_fim; i++){
            agenda[i] = true;
        }  
    }

    return agenda;
}

// 
int main() {

    int n_filme = 0;
    int n_cat = 0;
    
    vector<filme> todos_filmes;
    vector<int> max_categorias;
    
    // Ler número de filmes e categorias do input
    cin >> n_filme >> n_cat;

    todos_filmes.reserve(n_filme);
    int this_max_cat=0;

    // Ler máximo de filmes para cada categoria e armazenar em um vetor
    for(int i = 0; i < n_cat; i++){
        cin >> this_max_cat;
        max_categorias.push_back(this_max_cat);
    }

    int this_h_inicio=0;
    int this_h_fim;
    int this_categoria=0;

    // Ler horário de início, fim e categoria de cada filme
    for(int i = 0; i < n_filme; i++){
        cin >> this_h_inicio >> this_h_fim >> this_categoria;
        if (this_h_fim<this_h_inicio){
            // Tratamento especial para os filmes de "madrugada" para que o horário final sempre seja maior que o inicial
            this_h_fim += 24;
        }
        todos_filmes.push_back({i, this_h_inicio, this_h_fim, this_categoria, this_h_fim - this_h_inicio});
    }

    //ordenar
    sort(todos_filmes.begin(), todos_filmes.end(), heuristica_por_final);

    vector<filme> mochila;
    map<int, bool> agenda; // O valor padrão do booleano é false, por isso basta declarar a variável

    for(auto& this_filme : todos_filmes){

        if (verify_agenda(agenda, this_filme) && max_categorias[this_filme.categoria-1] - 1 >= 0){
            max_categorias[this_filme.categoria-1] -= 1;
            agenda = fill_agenda(agenda, this_filme);
            mochila.push_back(this_filme);

        }
        
    }
    //ordenando para imprimir
    //sort(mochila.begin(), mochila.end(), [](auto& i, auto&j){return i.id < j.id;});

    //output_visual(mochila);
    generateOutput(mochila, n_filme, n_cat);

    cout << '\n';
    return 0;
}
// gerar arquivo de output com n_filmes, tempo de tela, tempo de execução, média da duração dos filmes, 

// Formato
// n_filmes_input    n_cat_input
// tempo_de_tela     média_filmes

