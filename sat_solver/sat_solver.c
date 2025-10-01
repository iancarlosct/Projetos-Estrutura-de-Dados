#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_COMMENT_SIZE 1000
#define MAX_IP 100

typedef struct node
{
    bool value;
    struct node *left;
    struct node *right;
} node;

typedef struct tree
{
    int var_amount;
    int var_used;
    node *root;
    int interp[MAX_IP];
} tree;

node *create_node(bool value)
{
    node *new_node = (node*) malloc(sizeof(node));
    new_node->left = NULL;
    new_node->right = NULL;
    new_node->value = value;
    return new_node;
}

node *add_node(tree *header, node *curr, bool value)
{
    node *new_node = create_node(value);

    if(value == true) header->interp[header->var_used] = 1;
    else header->interp[header->var_used] = -1;
    header->var_used++;

    if(curr == NULL)
    {
        if(header->root->left) header->root->left = new_node;           
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
    int aux = 0;
    for(int i = 0; i < cla; i++)
    {
        for(int j = 0; j < lit; j++)
        {
            if(formula[i][j] == 2)
            {
                aux++;
                break;
            }
            if(formula[i][j] == interp[j] && interp[j] != 0) 
            {
                aux++;
                break;
            }
        }
    }

    if(aux == cla) return true;
    return false;
}

bool contradicts(int cla, int lit, int formula[cla][lit], int interp[MAX_IP])
{
    for(int i = 0; i < cla; i++)
    {
        bool contradiction = true;
        for(int j = 0; j < lit; j++)
        {
            if(formula[i][j] == 2) 
            {
                contradiction = false;
                break;
            }
            if(formula[i][j] != 0 && interp[j] == 0) 
            {
                contradiction = false;
                break;
            }
            if(formula[i][j] == interp[j] && interp[j] != 0) 
            {
                contradiction = false;
                break;
            }
        }
        if(contradiction) return true;
    }
    return false;
}

bool sat(int cla, int lit, int formula[cla][lit], tree *header, node *curr)
{ 
    if(satisfies(cla, lit, formula, header->interp)) return true;
    if(contradicts(cla, lit, formula, header->interp)) return false;

    if(header->var_used >= lit) return false;

    node *node_t = add_node(header, curr, true);
    if(sat(cla, lit, formula, header, node_t)) return true;

    header->var_used--;
    
    node *node_f = add_node(header, curr, false);
    if(sat(cla, lit, formula, header, node_f)) return true;

    header->var_used--;
    
    return false;
}

tree *initialize_tree(int var_amount)
{
    tree *new_tree = (tree*) malloc(sizeof(tree));
    new_tree->root = (node*) malloc(sizeof(node));

    new_tree->var_amount = var_amount;
    new_tree->var_used = 0;
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

    // Leitura da fórmula (com eliminação de clausulas com tautologias)
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
            formula[i][0] = 2;
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

    tree *header = initialize_tree(lit);
    
    bool res = sat(cla, lit, formula, header, NULL);

    if(res) 
    {
        printf("Satisfatível\n");
        for(int i = 0; i < MAX_IP; i++)
        {
            if(header->interp[i] == 0) break;
            printf("X%d: %d ", i+1, header->interp[i]);
        }
        printf("\n");
    } 
    else
    {
        printf("Contradição\n");
    }

    return 0;
}