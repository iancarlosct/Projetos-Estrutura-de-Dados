#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_COMMENT_SIZE 1000 // Tamanho máximo de um comentário
#define MAX_IP 100            // Quantidade máxima de variáveis

typedef struct node
{
    // NÓ DA ÁRVORE

    int var_index;       // Índice da variável do nó na lista de interpretação parcial
    bool value;          // Valor atribuído a variável desse nesse nó (true ou false)
    struct node *left;
    struct node *right;
} node;

typedef struct tree
{
    // HEADER DA ÁRVORE

    int var_amount;      // Quantidade de variáveis da fórmula  
    node *root;          // Raiz da árvore (não guarda uma variável)
    int interp[MAX_IP];  // Lista de interpretação parcial
} tree;

node *create_node(int var_index, bool value)
{
    // FUNÇÃO -> CRIA UM NOVO NÓ
    // var_index -> índice da variável do nó na lista de interpretação parcial
    // value -> valor booleano a ser atribuído a variável do nó
    // RETORNA -> NOVO NÓ

    node *new_node = (node*) malloc(sizeof(node));
    new_node->var_index = var_index;
    new_node->value = value;
    new_node->left = NULL;
    new_node->right = NULL;
    return new_node;
}

node *add_node(tree *header, node *curr, int var_index, bool value)
{
    // FUNÇÃO -> ADICIONAR UM NOVO NÓ NA ÁRVORE
    // header -> cabeçalho da árvore
    // curr -> nó atual, onde será adicionado um novo nó no left ou right
    // var_index -> índice da variável do nó na lista de interpretação parcial
    // value -> valor booleano a ser atribuído a variável do nó
    // RETORNA -> NOVO NÓ

    node *new_node = create_node(var_index, value);
    if(value) header->interp[var_index] = 1;
    else header->interp[var_index] = -1;

    if(curr == NULL)
    {
        if(header->root->left == NULL) header->root->left = new_node;           
        else header->root->right = new_node;
    }
    else
    {
        if(value) curr->left = new_node;
        else curr->right = new_node;
    }

    return new_node;
}

bool satisfies(int cla, int lit, int formula[cla][lit], int interp[MAX_IP])
{
    // FUNÇÃO -> TESTAR SE A FÓRMULA COM A INTERPRETAÇÃO PARCIAL ATUAL É VERDADEIRA
    // cla -> número de cláusulas
    // lit -> número de variáveis (literais)
    // formula -> matriz onde guardamos a fórmula
    // interp -> lista da interpretação parcial
    // RETORNA -> TRUE se a fórmula for verdadeira com a IP (SATISFATÍVEL)
    // RETORNA -> FALSE se a fórmula for inconclusiva com a IP (INCONCLUSIVA != CONTRADIÇÃO)

    for(int i = 0; i < cla; i++)
    {
        bool satisfied = false;
        for(int j = 0; j < lit; j++)
        {
            if(formula[i][j] == 2)
            {
                satisfied = true;
                break;
            }
            if(formula[i][j] == interp[j] && interp[j] != 0) 
            {
                satisfied = true;
                break;
            }
        }
        if(!satisfied) return false;
    }
    return true;
}

bool contradicts(int cla, int lit, int formula[cla][lit], int interp[MAX_IP])
{
    // FUNÇÃO -> TESTAR SE A FÓRMULA COM A INTERPRETAÇÃO PARCIAL ATUAL É FALSA
    // cla -> número de cláusulas
    // lit -> número de variáveis (literais)
    // formula -> matriz onde guardamos a fórmula
    // interp -> lista da interpretação parcial
    // RETORNA -> TRUE se a fórmula for falsa com a IP (CONTRADIÇÃO PARCIAL)
    // RETORNA -> FALSE se a fórmula for inconclusiva com a IP (INCONCLUSIVA != SATISFATÍVEL)

    for(int i = 0; i < cla; i++)
    {
        bool contradicted = true;
        for(int j = 0; j < lit; j++)
        {
            if(formula[i][j] == 2) 
            {
                contradicted = false;
                break;
            }
            if(formula[i][j] != 0 && interp[j] == 0) 
            {
                contradicted = false;
                break;
            }
            if(formula[i][j] == interp[j] && interp[j] != 0) 
            {
                contradicted = false;
                break;
            }
        }
        if(contradicted) return true;
    }
    return false;
}

bool sat(int cla, int lit, int formula[cla][lit], tree *header, node *curr, int var_index)
{ 
    // FUNÇÃO -> REALIZAR A LÓGICA DE RESOLUÇÃO DO SAT
    // cla -> número de cláusulas
    // lit -> número de variáveis (literais)
    // formula -> matriz onde guardamos a fórmula
    // header -> cabeçalho da árvore
    // curr -> nó atual, onde será adicionado um novo nó no left ou right
    // var_index -> índice da variável do nó na lista de interpretação parcial
    // RETORNA -> TRUE se a fórmula for SATISFATÍVEL
    // RETORNA -> FALSE se a fórmula for CONTRADIÇÃO

    if(satisfies(cla, lit, formula, header->interp)) return true;
    if(contradicts(cla, lit, formula, header->interp)) return false;

    if(var_index >= lit) return false;

    node *node_t = add_node(header, curr, var_index, true);
    if(sat(cla, lit, formula, header, node_t, var_index+1)) return true;
    
    node *node_f = add_node(header, curr, var_index, false);
    if(sat(cla, lit, formula, header, node_f, var_index+1)) return true;
    
    header->interp[var_index] = 0;

    return false;
}

tree *initialize_tree(int var_amount)
{
    // FUNÇÃO -> INICIALIZAR A ÁRVORE (CABEÇALHO)
    // var_amount -> quantidade de variáveis da fórmula
    // RETORNO -> NOVA ÁRVORE (CABEÇALHO)

    tree *new_tree = (tree*) malloc(sizeof(tree));
    new_tree->root = (node*) malloc(sizeof(node));
    new_tree->root->var_index = -1;
    new_tree->root->left = NULL;
    new_tree->root->right = NULL;

    new_tree->var_amount = var_amount;
    memset(new_tree->interp, 0, sizeof(new_tree->interp));

    return new_tree;
}   

int main()
{  
    char cmd;                           // Guarda o Char que determina se a linha é comentário (c) ou início da fórmula (p)
    char comment[MAX_COMMENT_SIZE];     // Guarda o Comentário atual conforme lê os comentários
    char form[10];                      // Guarda o tipo de fórmula de entrada (Tem que ser CNF)
    int lit, cla;                       // Guarda a quantidade de Variáveis (lit) e Cláusulas (cla)
    

    // REALIZA A LEITURA DO CABEÇALHO DO ARQUIVO DE ENTRADA
    while(scanf(" %c", &cmd) != EOF)
    {
        if(cmd == 'c')
        {
            fgets(comment, sizeof(comment), stdin);
            comment[strlen(comment)-1] = '\0';
            printf("=== %s ===\n", comment+1);
        }
        if(cmd == 'p')
        {
            scanf("%s", form);
            if(strcmp(form, "cnf") != 0) 
            {
                printf("Invalid Format, Please Use CNF Format\n");
                return 0;
            }
            scanf("%d %d", &lit, &cla);
            if(lit <= 0)
            {
                printf("Literals Number Invalid, Please Use a Valid Amount\n");
                return 0;
            }
            if(cla <= 0)
            {
                printf("Clauses Number Invalid, Please Use a Valid Amount\n");
                return 0;
            }
            break;
        }
    }

    
    int formula[cla][lit];                  // Matriz onde a fórmula é guardada
    memset(formula, 0, sizeof(formula));    // Zera todos os elementos da matriz

    // REALIZA A LEITURA DA FÓRMULA E ELIMINA AS CLÁUSULAS COM TAUTOLOGIA
    for(int i = 0; i < cla; i++)
    {
        int aux, tautology = 0;
    
        while(1)
        {
            scanf("%d", &aux);

            if(aux == 0) break; // Fim da cláusula

            if(aux > 0) 
            {
                if (formula[i][aux-1] == -1) 
                {
                    tautology = 1;
                }
                formula[i][aux-1] = 1;
            } 

            if(aux < 0) 
            {
                if (formula[i][-aux-1] == 1) 
                {
                    tautology = 1;
                }
                formula[i][-aux-1] = -1;
            }
        }   

        // Em caso de tautologia da cláusula: (A || ~A) -> Sempre TRUE
        if(tautology)
        {
            memset(formula[i], 0, lit * sizeof(int)); // Zera a cláusula
            formula[i][0] = 2;                        // Primeiro elemento da cláusula recebe 2 para identificar as cláusulas com tautologia na função satisfies() e contradicts()
        }
    }

    // PRINT DA FÓRMULA NA MATRIZ
    printf("Fórmula na Memória:\n");
    for(int i = 0; i < cla; i++)
    {
        for(int j = 0; j < lit; j++)
        {
            printf("%d ", formula[i][j]);
        }
        printf("\n");
    }
    printf("\n");

    // INICIALIZA A ÁRVORE (CABEÇALHO)
    tree *header = initialize_tree(lit);
    
    // CHAMA SAT (RES = TRUE para SATISFATÍVEL) && (RES == FALSE para CONTRADIÇÃO) 
    bool res = sat(cla, lit, formula, header, header->root, 0);

    // PRINT DO RESULTADO E DA INTERPRETAÇÃO PARCIAL EM CASO DE FÓRMULA SATISFATÍVEL
    if(res) 
    {
        printf("Satisfatível\n");
        for(int i = 0; i < MAX_IP; i++)
        {
            if(header->interp[i] != 0)
            {
                printf("X%d: %d ", i+1, header->interp[i]);
            }
        }
        printf("\n");
    } 
    else
    {
        printf("Contradição\n");
    }

    return 0;
}
// CSA Campeão do Brasileirão em 2030