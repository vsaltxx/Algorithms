/*
 * Použití binárních vyhledávacích stromů.
 *
 * S využitím Vámi implementovaného binárního vyhledávacího stromu (soubory ../iter/btree.c a ../rec/btree.c)
 * implementujte funkci letter_count. Výstupní strom může být značně degradovaný (až na úroveň lineárního seznamu) 
 * a tedy implementujte i druhou funkci (bst_balance), která strom, na požadavek uživatele, vybalancuje.
 * Funkce jsou na sobě nezávislé a tedy automaticky NEVOLEJTE bst_balance v letter_count.
 * 
 */
#include "../btree.h"
#include <stdio.h>
#include <stdlib.h>


/**
 * Vypočítání frekvence výskytů znaků ve vstupním řetězci.
 * 
 * Funkce inicilializuje strom a následně zjistí počet výskytů znaků a-z (case insensitive), znaku 
 * mezery ' ', a ostatních znaků (ve stromu reprezentováno znakem podtržítka '_'). Výstup je v 
 * uložen ve stromu.
 * 
 * Například pro vstupní řetězec: "abBccc_ 123 *" bude strom po běhu funkce obsahovat:
 * 
 * key | value
 * 'a'     1
 * 'b'     2
 * 'c'     3
 * ' '     2
 * '_'     5
 * 
 * Pro implementaci si můžete v tomto souboru nadefinovat vlastní pomocné funkce.
*/
void increment_or_create(bst_node_t **tree, char key) {
    int value;
    if (bst_search(*tree, key, &value)) {
        // key exists in the tree, increment the value
        //bst_delete(tree, key);
        bst_insert(tree, key, value + 1);
    }
    else {
        // key does not exist in the tree, create a new node with value 1
        bst_insert(tree, key, 1);
    }
}

void letter_count(bst_node_t **tree, char *input) {

    bst_node_t **letter_frequency_tree = tree;
    // initialize the tree
    bst_init(letter_frequency_tree);

    // process each character in the input string
    for (int i = 0; input[i] != '\0'; i++) {
        char c = input[i];
        if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) {
            // convert the character to lowercase
            if (c >= 'A' && c <= 'Z') {
                c += 'a' - 'A';
            }
            increment_or_create(letter_frequency_tree, c);
        }
        else if (input[i] == ' ') {
            increment_or_create(letter_frequency_tree, ' ');
        }
        else {
            increment_or_create(letter_frequency_tree, '_');
        }
    }
}



/**
 * Vyvážení stromu.
 * 
 * Vyvážený binární vyhledávací strom je takový binární strom, kde hloubka podstromů libovolného uzlu se od sebe liší maximálně o jedna.
 * 
 * Předpokládejte, že strom je alespoň inicializován. K získání uzlů stromu využijte vhodnou verzi vámi naimplmentovaného průchodu stromem.
 * Následně můžete například vytvořit nový strom, kde pořadím vkládaných prvků zajistíte vyváženost.
 *  
 * Pro implementaci si můžete v tomto souboru nadefinovat vlastní pomocné funkce. Není nutné, aby funkce fungovala *in situ* (in-place).
*/

void build_balanced_tree(bst_node_t **tree, bst_node_t **nodes, int start, int end) {
    if (start > end) {
        *tree = NULL;
        return;
    }

    int mid = (start + end) / 2;
    *tree = nodes[mid];

    // Recursive calls to build left and right subtrees
    build_balanced_tree(&((*tree)->left), nodes, start, mid - 1);
    build_balanced_tree(&((*tree)->right), nodes, mid + 1, end);
}


void bst_balance(bst_node_t **tree) {
    // Initialize an array to store nodes in sorted order
    bst_items_t items;
    items.nodes = NULL;
    items.capacity = 0;
    items.size = 0;

    // Copy nodes into the array
    bst_inorder(*tree, &items);

    // Build a balanced tree from the sorted array
    build_balanced_tree(tree,items.nodes, 0, items.size - 1);

    // Free the allocated memory
    free(items.nodes);
}