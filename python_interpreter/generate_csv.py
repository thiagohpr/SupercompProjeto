import pandas as pd
import subprocess
import time
from os import system
from os import listdir
from os.path import isfile, join
import matplotlib.pyplot as plt
from natsort import os_sorted

# roda os arquivos, cria um pandas dataframe com n_filme do input, n_categoria e o tempo de execucao

def run_inputs(dataframe_geral, heuristica, ex):
    input_path = '/home/user/SupercompProjeto/inputs'
    input_files = os_sorted([f for f in listdir(input_path) if isfile(join(input_path, f))])

    for input_f in input_files:
        system('clear')
        print(f'> Running Inputs For {heuristica}')
        print(f'> {input_f}')
        n_filmes = int(input_f.split('_')[1])
        n_cat = int(input_f.split('_')[2])
        
        with open(f'{input_path}/{input_f}') as f:
            start = time.perf_counter()
            proc = subprocess.run([ex], input=f.read(), text=True, capture_output=True)
            end = time.perf_counter()

        this_line = pd.DataFrame({'heur':[heuristica],
                                'n_filmes':[n_filmes],
                                'n_categorias':[n_cat],
                                'tempo_ex':[end-start],
                                'n_assistidos':[None],
                                'tempo_tela':[None],
                                'media':[None]})

        dataframe_geral = pd.concat([dataframe_geral, this_line])

    
    return dataframe_geral


# le os outputs e adiciona as 3 variáveis às linhas correspondentes de filme_categoria

def process_outputs(dataframe_geral):
    output_path = '/home/user/SupercompProjeto/outputs'
    output_files = os_sorted([f for f in listdir(output_path) if isfile(join(output_path, f))])

    for output_f in output_files:
        system('clear')
        print(f'> Processing Outputs')
        print(f'> {output_f}')
        heuristica = output_f.split('_')[1]
        n_filmes = int(output_f.split('_')[2])
        n_cat = int(output_f.split('_')[3])

        with open(f'{output_path}/{output_f}') as f:
            file_lines = f.readlines()
        
        n_assistidos = int(file_lines[1].split(' ')[0])
        tempo_tela = int(file_lines[1].split(' ')[1])
        media = float(file_lines[1].split(' ')[2][:-1])

        dataframe_geral.loc[(dataframe_geral.heur == heuristica)&(dataframe_geral.n_filmes == n_filmes)&(dataframe_geral.n_categorias == n_cat), 'n_assistidos'] = n_assistidos
        dataframe_geral.loc[(dataframe_geral.heur == heuristica)&(dataframe_geral.n_filmes == n_filmes)&(dataframe_geral.n_categorias == n_cat), 'tempo_tela'] = tempo_tela
        dataframe_geral.loc[(dataframe_geral.heur == heuristica)&(dataframe_geral.n_filmes == n_filmes)&(dataframe_geral.n_categorias == n_cat), 'media'] = media
    
# main chama as primerias funcoes e depois cria um gráfico 3d para cada variavel resposta n_filmes_assistidos, tempo_tela e média

def main():
    dataframe_geral = pd.DataFrame()

    heuristicas = {'aleat':'/home/user/SupercompProjeto/guloso_aleatorio/filmes_guloso_aleat',
                   'gul':'/home/user/SupercompProjeto/guloso/filmes_guloso',
                   }

    for heuristica, ex in heuristicas.items():
        dataframe_geral = pd.concat([dataframe_geral, run_inputs(dataframe_geral, heuristica, ex)])
        
    process_outputs(dataframe_geral)
    
    dataframe_geral.to_csv('all_programs.csv')
        
if __name__ == '__main__':
    main()