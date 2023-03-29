import pandas as pd
import matplotlib.pyplot as plt
# a partir do dataframe montado, cria um gráfico 3D com n_filmes, n_categorias e um y como parâmetro

# def create_graphs(dataframe_geral, z_column, heuristica):
#     fig = plt.figure()
#     # syntax for 3-D projection
#     ax = fig.add_subplot(projection ='3d')
    
#     # defining axes
#     ax.scatter(dataframe_geral.n_filmes.astype(int), dataframe_geral.n_categorias.astype(int), dataframe_geral[z_column].astype(float))
    
#     # syntax for plotting
#     ax.set_title(f'Variável Resposta {z_column}')
#     ax.set_xlabel('Número de Filmes')
#     ax.set_ylabel('Número de Categorias')
#     ax.set_zlabel(f'{z_column}')

#     plt.savefig(f'{heuristica}_{z_column}.png')
#     plt.show(block=True)
    #plt.show(block=False)
    #plt.pause(0.001)
    #plt.show()

dataframe_geral = pd.read_csv('/home/user/SupercompProjeto/python_interpreter/all_programs.csv')

# tempo_ex
fig = plt.figure()
# syntax for 3-D projection
ax = fig.add_subplot(projection ='3d')

# defining axes
ax.scatter(dataframe_geral.n_filmes.astype(int), dataframe_geral.n_categorias.astype(int), dataframe_geral['tempo_ex'].astype(float))

# syntax for plotting
ax.set_title('Variável Resposta tempo_ex')
ax.set_xlabel('Número de Filmes')
ax.set_ylabel('Número de Categorias')
ax.set_zlabel('tempo_ex')
plt.show()


# n_assistidos
fig = plt.figure()
# syntax for 3-D projection
ax = fig.add_subplot(projection ='3d')

# defining axes
ax.scatter(dataframe_geral.n_filmes.astype(int), dataframe_geral.n_categorias.astype(int), dataframe_geral['n_assistidos'].astype(float))

# syntax for plotting
ax.set_title('Variável Resposta n_assistidos')
ax.set_xlabel('Número de Filmes')
ax.set_ylabel('Número de Categorias')
ax.set_zlabel('n_assistidos')
plt.show()


# tempo_tela
fig = plt.figure()
# syntax for 3-D projection
ax = fig.add_subplot(projection ='3d')

# defining axes
ax.scatter(dataframe_geral.n_filmes.astype(int), dataframe_geral.n_categorias.astype(int), dataframe_geral['tempo_tela'].astype(float))

# syntax for plotting
ax.set_title('Variável Resposta tempo_tela')
ax.set_xlabel('Número de Filmes')
ax.set_ylabel('Número de Categorias')
ax.set_zlabel('tempo_tela')
plt.show()


# media
fig = plt.figure()
# syntax for 3-D projection
ax = fig.add_subplot(projection ='3d')

# defining axes
ax.scatter(dataframe_geral.n_filmes.astype(int), dataframe_geral.n_categorias.astype(int), dataframe_geral['media'].astype(float))

# syntax for plotting
ax.set_title('Variável Resposta media')
ax.set_xlabel('Número de Filmes')
ax.set_ylabel('Número de Categorias')
ax.set_zlabel('media')
plt.show()