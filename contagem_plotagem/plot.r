library(ggplot2)

balanced = read.csv("teste_balanceado.csv", header=TRUE)
unbalanced = read.csv("teste_sequencia_crescente.csv", header=TRUE)
random = read.csv("teste_random.csv", header=TRUE)

title = labs(title = "Comparação de Desempenho entre Arvore de busca binária e Arvore AVL",
             x = "Tamanho da Entrada",
             y = "Quantidade de Operações",
             color = "Estrutura")

ggplot(balanced, aes(x=quantidade)) +
  geom_point(aes(y=arvore.de.busca.binaria, color="Arvore desbalanceada")) +
  geom_point(aes(y=arvore.avl, color="Arvore balanceada", alpha=0.5)) +
  title

ggplot(unbalanced, aes(x=quantidade)) +
  geom_point(aes(y=arvore.de.busca.binaria, color="Arvore desbalanceada")) +
  geom_point(aes(y=arvore.avl, color="Arvore balanceada")) +
  title

ggplot(random, aes(x=quantidade)) +
  geom_point(aes(y=arvore.de.busca.binaria, color="Arvore desbalanceada")) +
  geom_point(aes(y=arvore.avl, color="Arvore balanceada")) +
  title
