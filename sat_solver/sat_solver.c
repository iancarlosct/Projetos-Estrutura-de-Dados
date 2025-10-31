#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_COMMENT_SIZE 1000 // Maximum size of a comment line
#define MAX_IP 100            // Maximum size of the interpretation partial list

typedef struct node
{
    //Tree Node
    int var_index;      // Index of the variable in the interpretation partial list
    bool value;         // Boolean value assigned to the variable 
    struct node *left;  // Pointer to the left child (true branch)
    struct node *right; // Pointer to the right child (false branch)
} node;

typedef struct tree
{
    //Tree Header
    int var_amount;      // Amount of variables in the formula
    node *root;          // Pointer to the root node of the tree
    int interp[MAX_IP];  // List of the interpretation partial (1 = true, -1 = false, 0 = unassigned)
} tree;

/** 
 * Function -> Create a new node
 * @param var_index -> index of the variable in the interpretation partial list
 * @param value -> boolean value to be assigned to the variable
 *@return -> new node
 */
node *create_node(int var_index, bool value)
{
    node *new_node = (node*) malloc(sizeof(node));
    new_node->var_index = var_index;
    new_node->value = value;
    new_node->left = NULL;
    new_node->right = NULL;
    return new_node;
}

/**
 * Function -> Add a new node to the tree
 * @param header -> tree header
 * @param curr -> current node where the new node will be added as left or right
 * @param var_index -> index of the variable in the interpretation partial list
 * @param value -> boolean value to be assigned to the variable
 * @return -> new node added to the tree
 */
node *add_node(tree *header, node *curr, int var_index, bool value)
{
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

/**
 * Function -> Test if the formula with the current partial interpretation is true
 * @param cla -> number of clauses
 * @param lit -> number of variables (literals)
 * @param formula -> matrix where the formula is stored
 * @param interp -> list of the partial interpretation
 * @return -> TRUE if the formula is true with the IP (SATISFIABLE)
 * @return -> FALSE if the formula is inconclusive with the IP (INCONCLUSIVE != UNSATISFIABLE)
 */
bool satisfies(int cla, int lit, int formula[cla][lit], int interp[MAX_IP]) 
{
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

/**
 * Function -> Test if the formula with the current partial interpretation is false
 * @param cla -> number of clauses
 * @param lit -> number of variables (literals)
 * @param formula -> matrix where the formula is stored
 * @param interp -> list of the partial interpretation
 * @return -> TRUE if the formula is false with the IP (CONTRADICTION)
 * @return -> FALSE if the formula is inconclusive with the IP (INCONCLUSIVE != SATISFIABLE)
*/
bool contradicts(int cla, int lit, int formula[cla][lit], int interp[MAX_IP])
{
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

/**
 * Function -> Recursive function that builds the binary tree and tests the formula with the current partial interpretation
 * @param cla -> number of clauses
 * @param lit -> number of variables (literals)
 * @param formula -> matrix where the formula is stored
 * @param header -> tree header
 * @param curr -> current node where the new node will be added as left or right
 * @param var_index -> index of the variable in the interpretation partial list
 * @return -> TRUE if the formula is SATISFIABLE
 * @return -> FALSE if the formula is UNSATISFIABLE
 */
bool sat(int cla, int lit, int formula[cla][lit], tree *header, node *curr, int var_index)
{ 
    if(satisfies(cla, lit, formula, header->interp)) return true;
    if(contradicts(cla, lit, formula, header->interp)) return false;

    if(var_index >= lit) return false;

    node *node_t = add_node(header, curr, var_index, true);
    if(sat(cla, lit, formula, header, node_t, var_index+1)) return true;
    
    node *node_f = add_node(header, curr, var_index, false);
    if(sat(cla, lit, formula, header, node_f, var_index+1)) return true;
    
    header->interp[var_index] = 0; // Backtrack: unassign the variable

    return false;
}

/** 
 * Function -> Initialize the tree header
 * @param var_amount -> amount of variables in the formula
 * @return -> new tree header
 */
tree *initialize_tree(int var_amount)
{
    tree *new_tree = (tree*) malloc(sizeof(tree));
    new_tree->root = (node*) malloc(sizeof(node));
    new_tree->root->var_index = -1;
    new_tree->root->left = NULL;
    new_tree->root->right = NULL;
    
    new_tree->var_amount = var_amount;
    memset(new_tree->interp, 0, sizeof(new_tree->interp));
    
    return new_tree;
}   

/**
 * Function -> Print an error message and exit the program
 * @param message -> error message to be printed
 */
void throw_error(char message[]) {
  perror(message);
  exit(EXIT_FAILURE);
}

/** 
 * Function -> Read the input and extract the number of literals and clauses
 * @param lit -> pointer to store the number of literals
 * @param cla -> pointer to store the number of clauses
*/
void read_input(int *lit, int *cla)
{
    char cmd;                           // Command Character to identify the type of line being read
    char comment[MAX_COMMENT_SIZE];     // Keeps the comment line
    char form[10];                      // Keeps the format of the formula (should be "cnf")
    
    // Reads the input until it finds the "p cnf" line
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
            if(strcmp(form, "cnf") != 0) throw_error("Invalid Format, Please Use CNF Format");
            scanf("%d %d", lit, cla);
            if(lit <= 0) throw_error("Literals Number Invalid, Please Use a Valid Amount");
            if(cla <= 0) throw_error("Clauses Number Invalid, Please Use a Valid Amount");
            break;
        }
    }
    printf("\n");

    return;
}

/**
 * Function -> Read the formula and store it in a matrix
 * @param cla -> number of clauses
 * @param lit -> number of variables (literals)
 * @param formula -> matrix where the formula will be stored
 */
void read_formula(int cla, int lit, int formula[cla][lit])
{   
    memset(formula, 0, cla * lit * sizeof(int));    // Initialize the matrix with 0 (no literal in the clause)

    // Reads the formula and fills the matrix
    for(int i = 0; i < cla; i++) 
    {
        int aux, tautology = 0;
        while(1)
        {
            scanf("%d", &aux);
    
            if(aux == 0) break; // End of clause
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
    
        // In case of tautology, the clause is set to a special value (2) to be ignored in satisfies() and contradicts()
        if(tautology) 
        {
            memset(formula[i], 0, lit * sizeof(int)); // Clear the clause
            formula[i][0] = 2;                        // Set the first position to 2 to indicate tautology
        }
    }

    return;
}

/**
 * Function -> Print the formula stored in memory
 * @param cla -> number of clauses
 * @param lit -> number of variables (literals)
 * @param formula -> matrix where the formula is stored
 */
void print_formula(int cla, int lit, int formula[cla][lit])
{
    // Print the formula stored in memory
    printf("=== Fórmula na Memória: ===\n");
    for(int i = 0; i < cla; i++) 
    {
        for(int j = 0; j < lit; j++)
        {
            printf("%d ", formula[i][j]);
        }
        printf("\n");
    }
    printf("\n");
    
    return;
}

/**
 * Function -> Print the result of the SAT solver
 * @param res -> result of the SAT solver (TRUE = SATISFIABLE, FALSE = UNSATISFIABLE)
 * @param header -> tree header
 */
void print_result(bool res, tree *header)
{
    if(res) 
    {
        printf("SATISFIABLE\n");
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
        printf("UNSATISFIABLE\n");
    }

    return;
}

int main()
{  
    int lit, cla;                      
    read_input(&lit, &cla);
    int formula[cla][lit];            
    read_formula(cla, lit, formula);
    print_formula(cla, lit, formula);
    tree *header = initialize_tree(lit);
    bool res = sat(cla, lit, formula, header, header->root, 0);
    print_result(res, header);
    return 0;
}

// CSA Campeão Alagoano 2026
