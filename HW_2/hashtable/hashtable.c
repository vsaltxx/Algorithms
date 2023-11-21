/*
 * Tabulka s rozptýlenými položkami
 *
 * S využitím datových typů ze souboru hashtable.h a připravených koster
 * funkcí implementujte tabulku s rozptýlenými položkami s explicitně
 * zretězenými synonymy.
 *
 * Při implementaci uvažujte velikost tabulky HT_SIZE.
 */

#include "hashtable.h"
#include <stdlib.h>
#include <string.h>

int HT_SIZE = MAX_HT_SIZE;

/*
 * Rozptylovací funkce která přidělí zadanému klíči index z intervalu
 * <0,HT_SIZE-1>. Ideální rozptylovací funkce by měla rozprostírat klíče
 * rovnoměrně po všech indexech. Zamyslete sa nad kvalitou zvolené funkce.
 */
int get_hash(char *key) {
  int result = 1;
  int length = strlen(key);
  for (int i = 0; i < length; i++) {
    result += key[i];
  }
  return (result % HT_SIZE);
}

/*
 * Inicializace tabulky — zavolá sa před prvním použitím tabulky.
 */
void ht_init(ht_table_t *table) {
    if ((*table) == NULL) {     // check if the table is already initialized
        return;
    }
    for (int i = 0; i < HT_SIZE; i++) {
        (*table)[i] = NULL;
    }
}

/*
 * Vyhledání prvku v tabulce.
 *
 * V případě úspěchu vrací ukazatel na nalezený prvek; v opačném případě vrací
 * hodnotu NULL.
 */
ht_item_t *ht_search(ht_table_t *table, char *key) {

  if (table == NULL || key == NULL) {
    return NULL;
  }
  else {
    int index = get_hash(key);           // get the index of the key
    ht_item_t *item = (*table)[index];   // get the item at the index
    while (item != NULL) {
      if (strcmp(item->key, key) == 0) { // if the key of the item is the same as the key
        return item;                     // return the item
      }
      else {
        item = item->next;               // go to the next item
      }
    }
    return NULL;                         // return NULL
  }
}

/*
 * Vložení nového prvku do tabulky.
 *
 * Pokud prvek s daným klíčem už v tabulce existuje, nahraďte jeho hodnotu.
 *
 * Při implementaci využijte funkci ht_search. Pri vkládání prvku do seznamu
 * synonym zvolte nejefektivnější možnost a vložte prvek na začátek seznamu.
 */
void ht_insert(ht_table_t *table, char *key, float value) {
    if (table == NULL || key == NULL) {
        return;
    }
    else {
        ht_item_t *item = ht_search(table, key);  // search for the item
        if (item != NULL) {                       // the item was found
            item->value = value;                  // set the value of the item
            return;
        }
        else {                                    // the item was not found
            int index = get_hash(key);            // get the index of the key
            ht_item_t *new_item = (ht_item_t *)malloc(sizeof(ht_item_t));  // allocate memory for the new item
            if (new_item == NULL) {
                return;
            }
            new_item->key = key;                  // set the key of the new item
            new_item->value = value;              // set the value of the new item
            new_item->next = (*table)[index];     // set the next item of the new item
            (*table)[index] = new_item;           // set the new item to the table
        }
    }
}

/*
 * Získání hodnoty z tabulky.
 *
 * V případě úspěchu vrací funkce ukazatel na hodnotu prvku, v opačném
 * případě hodnotu NULL.
 *
 * Při implementaci využijte funkci ht_search.
 */
float *ht_get(ht_table_t *table, char *key) {
    if (table == NULL || key == NULL) {
        return NULL;
    }
    else {
        ht_item_t *item = ht_search(table, key);  // search for the item
        if (item != NULL) {                       // the item was found
            return (&(item->value));              // return the value of the item
        }
        else {                                    // the item was not found
            return NULL;                          // return NULL
        }
    }
}

/*
 * Smazání prvku z tabulky.
 *
 * Funkce korektně uvolní všechny alokované zdroje přiřazené k danému prvku.
 * Pokud prvek neexistuje, funkce nedělá nic.
 *
 * Při implementaci NEPOUŽÍVEJTE funkci ht_search.
 */
void ht_delete(ht_table_t *table, char *key) {

    if (table == NULL || key == NULL) {
        return;
    }
    else {
        int index = get_hash(key);
        ht_item_t *item = (*table)[index];
        if (item == NULL) {         // the item was not found
            return;
        }
        else {                      // the item was found
            if (strcmp(item->key, key) == 0) {      // the item is the first item
                (*table)[index] = item->next;       // set the next item of the table to the next item of the item
                free(item);
                return;
            }
            else {                                          // the item is not the first item
                ht_item_t *prev_item = item;                // set the previous item to the item
                item = item->next;                          // set the item to the next item
                while (item != NULL) {
                    if (strcmp(item->key, key) == 0) {      // the item is the item we are looking for
                        prev_item->next = item->next;
                        free(item);
                        return;
                    }
                    else {                                  // the item is not the item we are looking for
                        prev_item = item;
                        item = item->next;
                    }
                }
            }
        }
    }
}

/*
 * Smazání všech prvků z tabulky.
 *
 * Funkce korektně uvolní všechny alokované zdroje a uvede tabulku do stavu po 
 * inicializaci.
 */
void ht_delete_all(ht_table_t *table) {
    if (table == NULL) {
        return;
    }
    else {
        for (int i = 0; i < HT_SIZE; i++) {     // go through the table
            ht_item_t *item = (*table)[i];      // get the item at the index
            while (item != NULL) {
                ht_item_t *tmp = item;          // set the tmp to the current item
                item = item->next;              // go to the next item
                free(tmp);                  // free the current item
            }
            (*table)[i] = NULL;                 // set the item at the index to NULL
        }
    }
}
