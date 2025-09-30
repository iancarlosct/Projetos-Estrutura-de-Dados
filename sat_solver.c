#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_COMMENT_SIZE 1000
#define MAX_VARS 100

typedef struct node
{
    int var;
    bool value;
    struct node *left;
    struct node *right;
} node;

typedef struct tree
{
    int var_amount;
    bool res;
    node *root;
} tree;

bool satisfies(int formula[][], tree *partial_interp)
{
//
}

bool contradicts(int formula[][], tree *partial_interp)
{
// 
}

int choose_var(tree *partial_interp)
{
//
}

node  *add_node(tree *partial_interp, bool value)
{
//
}

bool *sat(int formula[][], tree *partial_interp)
{
    if(satisfies(formula, partial_interp)) return true;
    if(contradicts(formula, partial_interp)) return false;

    int var = choose_var(partial_interp);
    if(var == -1) return false;

    node *new_node = add_node(partial_interp, true);
    if(sat(formula, partial_interp)) return true;

    new_node = add_node(partial_interp, false);
    if(sat(formula, partial_interp)) return false;

    return false;
}

tree *initialize_tree(int var_amount)
{
    tree *new_tree = (tree*) malloc(sizeof(tree));
    new_tree->var_amount = var_amount;
    new_tree->root = NULL;
    return new_tree;
}   

int main()
{   
    char cmd;
    char comment[MAX_COMMENT_SIZE];
    char form[10];
    int lit, cla; // Variáveis, Cláusulas
    
    // Leitura dos Comentários e do tipo de formato
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
            break;
        }
    }

    // Leitura da fórmula (com eliminação de tautologias)
    int formula[cla][lit];
    memset(formula, 0, sizeof(formula));

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
                if (formula[i][aux-1] == 1) 
                {
                    tautology = 1;
                }
                formula[i][-aux-1] = -1;
            }
        }   

        //Zera a cláusula em caso de Tautologia
        if(tautology)
        {
            memset(formula[i], 0, lit * sizeof(int));
        }
    }

    // Print da matriz
    for(int i = 0; i < cla; i++)
    {
        for(int j = 0; j < lit; j++)
        {
            printf("%d ", formula[i][j]);
        }
        printf("\n");
    }

    tree *partial_interpretation = initialize_tree(lit);
    
    sat(formula, partial_interpretation);

    return 0;
}
