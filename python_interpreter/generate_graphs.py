import pandas as pd
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D 

dataframe_geral = pd.read_csv('all_programs.csv')

dataframe_guloso = dataframe_geral.loc[dataframe_geral.heur=='gul']
dataframe_aleatorio = dataframe_geral.loc[dataframe_geral.heur=='aleat']

colors = ['blue', 'green']
heuristicas = ['Gulosa', 'Gulosa Aleatória']


targets = {'Tempo de Execução':'tempo_ex', 
           'Quantidade Assistidos':'n_assistidos', 
           'Tempo Total de Tela':'tempo_tela', 
           'Média de Duração':'media'}
lista_maximos = [0.11, 26, 31, 6]

e = 0
for pretty_name, z_column in targets.items():
    for i,df in enumerate([dataframe_guloso, dataframe_aleatorio]):
        fig = plt.figure()
        # syntax for 3-D projection
        ax = fig.add_subplot(111, projection ='3d')
        
        # defining axes
        ax.scatter(df.n_filmes.astype(int), df.n_categorias.astype(int), df[z_column].astype(float), color = colors[i])
        
        # syntax for plotting
        ax.set_title(f'3D {heuristicas[i]} para {pretty_name}')
        ax.set_xlabel('Número de Filmes')
        ax.set_ylabel('Número de Categorias')
        ax.set_zlabel(f'{pretty_name}')
        ax.axes.set_zlim3d(bottom=0, top=lista_maximos[e]) 
        plt.savefig(f'3d-{z_column}-{heuristicas[i].lower()}.png')

    fig, axs = plt.subplots(2, 2)
    fig.suptitle(f'Vistas Laterais da Variável {pretty_name}\nGulosa           Gulosa Aleatória')
    plt.setp(axs, ylim=(0, lista_maximos[e]))

    for i,df in enumerate([dataframe_guloso, dataframe_aleatorio]):
        axs[0, i].scatter(df.n_filmes.astype(int), df[z_column].astype(float), color = colors[i])

        axs[1, i].scatter(df.n_categorias.astype(int), df[z_column].astype(float), color = colors[i])
        
    plt.savefig(f'expl_{z_column}.png')

    e += 1

    


# printar os melhores reusltados (maximizam cada variável resposta)!
        