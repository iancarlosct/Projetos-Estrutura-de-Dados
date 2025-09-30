#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_COMMENT_SIZE 1000
#define MAX_IP 100

typedef struct node
{
    int var;
    bool value;
    struct node *left;
    struct node *right;
    struct node *prev;
} node;

typedef struct tree
{
    int var_amount;
    node *root;
    int interp[MAX_IP];
} tree;

node *create_node(bool value, node *curr)
{
    node *new_node = (node*) malloc(sizeof(node));
    new_node->left = NULL;
    new_node->right = NULL;
    new_node->prev = curr;
    new_node->value = value;

    return new_node;
}

node *add_node(tree *partial_interp, node *curr, bool value)
{
    node *new_node = create_node(value, curr);

    // Falta Coisa
}

bool sat(int cla, int lit, int formula[cla][lit], tree *partial_interp, node *curr)
{ 
    if(satisfies(cla, lit, formula, curr)) return true;
    if(contradicts(cla, lit, formula, curr)) return false;
    
    curr = add_node(partial_interp, curr, true);
    if(sat(cla, lit, formula, partial_interp, curr)) return true;

    curr = add_node(partial_interp, curr, false);
    if(sat(cla, lit, formula, partial_interp, curr)) return false;
    
    return false;
}

tree *initialize_tree(int var_amount)
{
    tree *new_tree = (tree*) malloc(sizeof(tree));
    new_tree->root = (node*) malloc(sizeof(node));

    new_tree->var_amount = var_amount;
    new_tree->root->left = NULL;
    new_tree->root->right = NULL;
    new_tree->root->prev = NULL;
    new_tree->root->value = false;
    new_tree->root->var = 0;

    memset(new_tree->interp, 0, sizeof(new_tree->interp));

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
                if (formula[i][-aux-1] == 1) 
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
    
    sat(cla, lit, formula, partial_interpretation, NULL);

    return 0;
}
