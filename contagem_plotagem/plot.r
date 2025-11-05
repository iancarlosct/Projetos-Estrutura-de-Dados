library(ggplot2)

unbalanced = read.csv("/home/nicolas/projects/estrutura_de_dados/Projetos-Estrutura-de-Dados/contagem_plotagem/teste_random.csv", header=TRUE)

title = labs(title = "Comparação de Desempenho entre Arvore de busca binária e Arvore AVL",
             x = "Tamanho da Entrada",
             y = "Quantidade de Operações",
             color = "Estrutura")

ggplot(unbalanced, aes(x=quantidade)) +
  # Adiciona os pontos da coluna arvore de busca binaria para o plot
  geom_point(aes(y=arvore.de.busca.binaria, color="Arvore desbalanceada")) +
  # Adiciona os pontos da coluna arvore avl para o plot
  geom_point(aes(y=arvore.avl, color="Arvore balanceada")) +
  title
