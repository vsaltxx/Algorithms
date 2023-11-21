/*
 * Binární vyhledávací strom — iterativní varianta
 *
 * S využitím datových typů ze souboru btree.h, zásobníku ze souboru stack.h 
 * a připravených koster funkcí implementujte binární vyhledávací 
 * strom bez použití rekurze.
 */

#include "../btree.h"
#include "stack.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * Inicializace stromu.
 *
 * Uživatel musí zajistit, že inicializace se nebude opakovaně volat nad
 * inicializovaným stromem. V opačném případě může dojít k úniku paměti (memory
 * leak). Protože neinicializovaný ukazatel má nedefinovanou hodnotu, není
 * možné toto detekovat ve funkci. 
 */
void bst_init(bst_node_t **tree) {
    if ((*tree) == NULL) {  // check if tree is already initialized
        return;
    }
    else {
        (*tree) = NULL;     // set tree pointer to NULL
    }
}

/*
 * Vyhledání uzlu v stromu.
 *
 * V případě úspěchu vrátí funkce hodnotu true a do proměnné value zapíše
 * hodnotu daného uzlu. V opačném případě funkce vrátí hodnotu false a proměnná
 * value zůstává nezměněná.
 * 
 * Funkci implementujte iterativně bez použité vlastních pomocných funkcí.
 */
bool bst_search(bst_node_t *tree, char key, int *value) {

    if (tree == NULL) {                 // check if the tree is empty
        return false;                   // wasn't found
    }
    else {
        while (tree != NULL) {          // while the tree is not empty
            if (tree->key == key) {     // the key was found
                *value = tree->value;   // set value to the value of the node
                return true;            // was found
            }
            else {
                if (key < tree->key) {  // the key is smaller than the key of the node
                    tree = tree->left;  // search in the left subtree
                }
                else {                  // the key is bigger than the key of the node
                    tree = tree->right; // search in the right subtree
                }
            }
        }
    return false;                   // wasn't found
    }
}

/*
 * Vložení uzlu do stromu.
 *
 * Pokud uzel se zadaným klíče už ve stromu existuje, nahraďte jeho hodnotu.
 * Jinak vložte nový listový uzel.
 *
 * Výsledný strom musí splňovat podmínku vyhledávacího stromu — levý podstrom
 * uzlu obsahuje jenom menší klíče, pravý větší. 
 *
 * Funkci implementujte iterativně bez použití vlastních pomocných funkcí.
 */
void bst_insert(bst_node_t **tree, char key, int value) {

    if ((*tree) == NULL) {
        (*tree) = (bst_node_t *)malloc(sizeof(bst_node_t));  // no --> allocate memory
        if ((*tree) == NULL) {
            return;                                         // allocation failed
        }
        else { //create new node
            (*tree)->key = key;
            (*tree)->value = value;
            (*tree)->left = NULL;
            (*tree)->right = NULL;
        }
    }
    else {
        bst_node_t *current = (*tree);                      // set the current node to the root
        while (current != NULL) {                           // while the tree is not empty
            if (current->key == key) {                      // the key was found
                current->value = value;                     // reset the value of the node
                return;
            } else {
                if (key < current->key) {                   // the key is smaller than the key of the node
                    if (current->left == NULL) {            // the left child is empty
                        current->left = (bst_node_t *) malloc(sizeof(bst_node_t)); // allocate memory
                        if (current->left == NULL) {        // allocation failed
                            return;
                        } else {  // add new data to the node and set the children to NULL
                            current->left->key = key;
                            current->left->value = value;
                            current->left->left = NULL;
                            current->left->right = NULL;
                            return;
                        }
                    } else {                                // the left child is not empty
                        current = current->left;            // search in the left subtree
                    }
                } else {
                    if (current->right == NULL) {           // the right child is empty
                        current->right = (bst_node_t *) malloc(sizeof(bst_node_t)); // allocate memory
                        if (current->right == NULL) {       // allocation failed
                            return;
                        } else {  // add new data to the node and set the children to NULL
                            current->right->key = key;
                            current->right->value = value;
                            current->right->left = NULL;
                            current->right->right = NULL;
                            return;
                        }
                    } else {                                // the right child is not empty
                        current = current->right;           // search in the right subtree
                    }
                }
            }
        }
    }
}

/*
 * Pomocná funkce která nahradí uzel nejpravějším potomkem.
 * 
 * Klíč a hodnota uzlu target budou nahrazené klíčem a hodnotou nejpravějšího
 * uzlu podstromu tree. Nejpravější potomek bude odstraněný. Funkce korektně
 * uvolní všechny alokované zdroje odstraněného uzlu.
 *
 * Funkce předpokládá, že hodnota tree není NULL.
 * 
 * Tato pomocná funkce bude využita při implementaci funkce bst_delete.
 *
 * Funkci implementujte iterativně bez použití vlastních pomocných funkcí.
 */
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree) {
    bst_node_t *current = (*tree);                      // set the current node to the root
    bst_node_t *previous = NULL;
    while (current->right != NULL) {
        previous = current;
        current = current->right;
    }
    target->key = current->key;                         // replace the key of the target node by the key of the rightmost node
    target->value = current->value;                     // replace the value of the target node
    if (previous == NULL) {
        (*tree) = current->left;
    }
    else {                                              // the previous node is not NULL
        previous->right = current->left;
    }
    free(current);
}

/*
 * Odstranění uzlu ze stromu.
 *
 * Pokud uzel se zadaným klíčem neexistuje, funkce nic nedělá.
 * Pokud má odstraněný uzel jeden podstrom, zdědí ho rodič odstraněného uzlu.
 * Pokud má odstraněný uzel oba podstromy, je nahrazený nejpravějším uzlem
 * levého podstromu. Nejpravější uzel nemusí být listem.
 * 
 * Funkce korektně uvolní všechny alokované zdroje odstraněného uzlu.
 * 
 * Funkci implementujte iterativně pomocí bst_replace_by_rightmost a bez
 * použití vlastních pomocných funkcí.
 */
void bst_delete(bst_node_t **tree, char key) {
    if ((*tree) == NULL) {                              // if the tree is empty
        return;                                         // wasn't found
    }
    while ((*tree) != NULL) {
        if (key < (*tree)->key) {
            tree = &(*tree)->left;                      // search in the left subtree
        }
        else if (key > (*tree)->key) {                  // the key is bigger than the key of the node
            tree = &(*tree)->right;                     // search in the right subtree
        }
        else {                                          // the key is equal to the key of the node
            if ((*tree)->left == NULL && (*tree)->right == NULL) { // the node is a leaf
                free((*tree));
                (*tree) = NULL;
                return;
            }
            else if ((*tree)->left == NULL) {           // the node has only the right child
                bst_node_t *tmp = (*tree);
                (*tree) = (*tree)->right;               // set the tree to the right child of the current node
                free(tmp);
                return;
            }
            else if ((*tree)->right == NULL) {          // the node has only the left child
                bst_node_t *tmp = (*tree);
                (*tree) = (*tree)->left;                // set the tree to the left child of the current node
                free(tmp);
                return;
            }
            else {                                      // the node has 2 children
                bst_replace_by_rightmost((*tree), &(*tree)->left); // replace the node by the rightmost node in the left subtree
                return;
            }
        }
    }
}

/*
 * Zrušení celého stromu.
 * 
 * Po zrušení se celý strom bude nacházet ve stejném stavu jako po 
 * inicializaci. Funkce korektně uvolní všechny alokované zdroje rušených 
 * uzlů.
 * 
 * Funkci implementujte iterativně s pomocí zásobníku a bez použití 
 * vlastních pomocných funkcí.
 */
void bst_dispose(bst_node_t **tree) {
    stack_bst_t stack;                   // stack of nodes
    stack_bst_init(&stack);         // initialize the stack
    do {
        if ((*tree) == NULL) {
            if (!stack_bst_empty(&stack)) {
                (*tree) = stack_bst_top(&stack);
                stack_bst_pop(&stack);
            }
        }
        else {
            if ((*tree)->right != NULL) {
                stack_bst_push(&stack, (*tree)->right);
            }
            bst_node_t *tmp = (*tree);
            (*tree) = (*tree)->left;    // search in the left subtree
            free(tmp);              // free the current node
        }
    } while ((*tree) != NULL || (!stack_bst_empty(&stack)));

}

/*
 * Pomocná funkce pro iterativní preorder.
 *
 * Prochází po levé větvi k nejlevějšímu uzlu podstromu.
 * Nad zpracovanými uzly zavolá bst_add_node_to_items a uloží je do zásobníku uzlů.
 *
 * Funkci implementujte iterativně s pomocí zásobníku a bez použití 
 * vlastních pomocných funkcí.
 */
void bst_leftmost_preorder(bst_node_t *tree, stack_bst_t *to_visit, bst_items_t *items) {
        while (tree != NULL) {
            stack_bst_push(to_visit, tree);   // push the node to the stack
            bst_add_node_to_items(tree, items);    // add the node to the items
            tree = tree->left;                           // search in the left subtree
        }
}

/*
 * Preorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte iterativně pomocí funkce bst_leftmost_preorder a
 * zásobníku uzlů a bez použití vlastních pomocných funkcí.
 */
void bst_preorder(bst_node_t *tree, bst_items_t *items) {

    stack_bst_t to_visit;                                // stack of nodes
    stack_bst_init(&to_visit);
    bst_leftmost_preorder(tree, &to_visit, items);
    while (!stack_bst_empty(&to_visit)) {
        tree = stack_bst_top(&to_visit);            // pop the node from the stack
        stack_bst_pop(&to_visit);                   // remove the node from the stack
        bst_leftmost_preorder(tree->right, &to_visit, items);
    }
}

/*
 * Pomocná funkce pro iterativní inorder.
 * 
 * Prochází po levé větvi k nejlevějšímu uzlu podstromu a ukládá uzly do
 * zásobníku uzlů.
 *
 * Funkci implementujte iterativně s pomocí zásobníku a bez použití 
 * vlastních pomocných funkcí.
 */
void bst_leftmost_inorder(bst_node_t *tree, stack_bst_t *to_visit)
{
    while (tree != NULL) {
        stack_bst_push(to_visit, tree);
        tree = tree->left;
    }
}

/*
 * Inorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte iterativně pomocí funkce bst_leftmost_inorder a
 * zásobníku uzlů a bez použití vlastních pomocných funkcí.
 */
void bst_inorder(bst_node_t *tree, bst_items_t *items) {
    stack_bst_t to_visit;
    stack_bst_init(&to_visit);
    bst_leftmost_inorder(tree, &to_visit);
    while (!stack_bst_empty(&to_visit)) {
        tree = stack_bst_top(&to_visit);
        stack_bst_pop(&to_visit);
        bst_add_node_to_items(tree, items);
        bst_leftmost_inorder(tree->right, &to_visit);
    }
}

/*
 * Pomocná funkce pro iterativní postorder.
 *
 * Prochází po levé větvi k nejlevějšímu uzlu podstromu a ukládá uzly do
 * zásobníku uzlů. Do zásobníku bool hodnot ukládá informaci, že uzel
 * byl navštíven poprvé.
 *
 * Funkci implementujte iterativně pomocí zásobníku uzlů a bool hodnot a bez použití
 * vlastních pomocných funkcí.
 */
void bst_leftmost_postorder(bst_node_t *tree, stack_bst_t *to_visit,
                            stack_bool_t *first_visit) {
    while (tree != NULL) {
        stack_bst_push(to_visit, tree);
        stack_bool_push(first_visit, true);
        tree = tree->left;
    }
}

/*
 * Postorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte iterativně pomocí funkce bst_leftmost_postorder a
 * zásobníku uzlů a bool hodnot a bez použití vlastních pomocných funkcí.
 */
void bst_postorder(bst_node_t *tree, bst_items_t *items) {

    bool from_left;

    stack_bst_t to_visit;
    stack_bool_t first_visit;
    stack_bst_init(&to_visit);
    stack_bool_init(&first_visit);

    bst_leftmost_postorder(tree, &to_visit, &first_visit);

    while (!stack_bst_empty(&to_visit)) {
        tree = stack_bst_top(&to_visit);
        from_left = stack_bool_top(&first_visit);
        stack_bool_pop(&first_visit);
        if (from_left) {
            stack_bool_push(&first_visit, false);
            bst_leftmost_postorder(tree->right, &to_visit, &first_visit);
        }
        else {
            stack_bst_pop(&to_visit);
            bst_add_node_to_items(tree, items);
        }
    }
}
