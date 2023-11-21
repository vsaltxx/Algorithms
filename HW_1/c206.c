/* ******************************* c206.c *********************************** */
/*  Předmět: Algoritmy (IAL) - FIT VUT v Brně                                 */
/*  Úkol: c206 - Dvousměrně vázaný lineární seznam                            */
/*  Návrh a referenční implementace: Bohuslav Křena, říjen 2001               */
/*  Vytvořil: Martin Tuček, říjen 2004                                        */
/*  Upravil: Kamil Jeřábek, září 2020                                         */
/*           Daniel Dolejška, září 2021                                       */
/*           Daniel Dolejška, září 2022                                       */
/* ************************************************************************** */
/*
** Implementujte abstraktní datový typ dvousměrně vázaný lineární seznam.
** Užitečným obsahem prvku seznamu je hodnota typu int. Seznam bude jako datová
** abstrakce reprezentován proměnnou typu DLList (DL znamená Doubly-Linked
** a slouží pro odlišení jmen konstant, typů a funkcí od jmen u jednosměrně
** vázaného lineárního seznamu). Definici konstant a typů naleznete
** v hlavičkovém souboru c206.h.
**
** Vaším úkolem je implementovat následující operace, které spolu s výše
** uvedenou datovou částí abstrakce tvoří abstraktní datový typ obousměrně
** vázaný lineární seznam:
**
**      DLL_Init ........... inicializace seznamu před prvním použitím,
**      DLL_Dispose ........ zrušení všech prvků seznamu,
**      DLL_InsertFirst .... vložení prvku na začátek seznamu,
**      DLL_InsertLast ..... vložení prvku na konec seznamu,
**      DLL_First .......... nastavení aktivity na první prvek,
**      DLL_Last ........... nastavení aktivity na poslední prvek,
**      DLL_GetFirst ....... vrací hodnotu prvního prvku,
**      DLL_GetLast ........ vrací hodnotu posledního prvku,
**      DLL_DeleteFirst .... zruší první prvek seznamu,
**      DLL_DeleteLast ..... zruší poslední prvek seznamu,
**      DLL_DeleteAfter .... ruší prvek za aktivním prvkem,
**      DLL_DeleteBefore ... ruší prvek před aktivním prvkem,
**      DLL_InsertAfter .... vloží nový prvek za aktivní prvek seznamu,
**      DLL_InsertBefore ... vloží nový prvek před aktivní prvek seznamu,
**      DLL_GetValue ....... vrací hodnotu aktivního prvku,
**      DLL_SetValue ....... přepíše obsah aktivního prvku novou hodnotou,
**      DLL_Previous ....... posune aktivitu na předchozí prvek seznamu,
**      DLL_Next ........... posune aktivitu na další prvek seznamu,
**      DLL_IsActive ....... zjišťuje aktivitu seznamu.
**
** Při implementaci jednotlivých funkcí nevolejte žádnou z funkcí
** implementovaných v rámci tohoto příkladu, není-li u funkce explicitně
 * uvedeno něco jiného.
**
** Nemusíte ošetřovat situaci, kdy místo legálního ukazatele na seznam
** předá někdo jako parametr hodnotu NULL.
**
** Svou implementaci vhodně komentujte!
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako procedury
** (v jazyce C procedurám odpovídají funkce vracející typ void).
**
**/

#include "c206.h"

bool error_flag;
bool solved;

/**
 * Vytiskne upozornění na to, že došlo k chybě.
 * Tato funkce bude volána z některých dále implementovaných operací.
 */
void DLL_Error(void) {
    printf("*ERROR* The program has performed an illegal operation.\n");
    error_flag = true;
}

/**
 * Provede inicializaci seznamu list před jeho prvním použitím (tzn. žádná
 * z následujících funkcí nebude volána nad neinicializovaným seznamem).
 * Tato inicializace se nikdy nebude provádět nad již inicializovaným seznamem,
 * a proto tuto možnost neošetřujte.
 * Vždy předpokládejte, že neinicializované proměnné mají nedefinovanou hodnotu.
 *
 * @param list Ukazatel na strukturu dvousměrně vázaného seznamu
 */
void DLL_Init( DLList *list ) {

    list->firstElement = NULL;
    list->activeElement = NULL;
    list->lastElement = NULL;
}

/**
 * Zruší všechny prvky seznamu list a uvede seznam do stavu, v jakém se nacházel
 * po inicializaci.
 * Rušené prvky seznamu budou korektně uvolněny voláním operace free.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Dispose( DLList *list ) {

    DLLElementPtr tmp = list->firstElement; // Create a temporary pointer to the first element of the list

    while (list->firstElement != NULL) {    // While the first element of the list exists
        list->firstElement = list->firstElement->nextElement; // Set the first element of the list to the next element of the list
        free(tmp);                      // Free the temporary pointer
        tmp = list->firstElement;           // Set the temporary pointer to the new first element of the list
    }

    list->activeElement = NULL;
    list->lastElement = NULL;
}

/**
 * Vloží nový prvek na začátek seznamu list.
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení na začátek seznamu
 */
void DLL_InsertFirst( DLList *list, int data ) {

    DLLElementPtr newElement =(DLLElementPtr) malloc(sizeof(struct DLLElement)); // Create a new element of the list and allocate memory for it
    if (newElement == NULL) {                               // If the allocation of memory for the new element failed
        DLL_Error();
        return;
    }

    newElement->data = data;
    newElement->previousElement = NULL;
    newElement->nextElement = list->firstElement;           // Set the next element of the new element to the current first element of the list

    if (list->firstElement != NULL) {                       // if the list is not empty (the first element of the list exists)
        list->firstElement->previousElement = newElement;   // Set the pointer 'previous' of the first element of the list to the new element
    } else {                                                // if the list is empty (the first element of the list doesn't exist)
        list->lastElement = newElement;                     // Set the 'last element' pointer of the list to the new element
    }

    list->firstElement = newElement;                        // Set the 'first element' pointer of the list to the new element
}

/**
 * Vloží nový prvek na konec seznamu list (symetrická operace k DLL_InsertFirst).
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení na konec seznamu
 */
void DLL_InsertLast( DLList *list, int data ) {

    DLLElementPtr newElement =(DLLElementPtr)malloc(sizeof(struct DLLElement));
    if (newElement == NULL) {
        DLL_Error();
        return;
    }

    newElement->data = data;
    newElement->nextElement = NULL;                     // newElement is now the last element of the list

    if (list->firstElement == NULL) {                   // the list is empty

        newElement->previousElement = NULL;
        list->firstElement = newElement;                // newElement is now the first element of the list and the last as well
    }
    else {                                              // is not empty
        newElement->previousElement = list->lastElement;
        list->lastElement->nextElement = newElement;    // the last element of the list is now pointing to the newElement
    }
    list->lastElement = newElement;                     // newElement is now the last element of the list
}

/**
 * Nastaví první prvek seznamu list jako aktivní.
 * Funkci implementujte jako jediný příkaz, aniž byste testovali,
 * zda je seznam list prázdný.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_First( DLList *list ) {
    list->activeElement = list->firstElement;
}

/**
 * Nastaví poslední prvek seznamu list jako aktivní.
 * Funkci implementujte jako jediný příkaz, aniž byste testovali,
 * zda je seznam list prázdný.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */

void DLL_Last( DLList *list ) {
    list->activeElement = list->lastElement;
}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu prvního prvku seznamu list.
 * Pokud je seznam list prázdný, volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetFirst( DLList *list, int *dataPtr ) {
    if (list->firstElement == NULL) {                       // if the first element of the list doesn't exist
        DLL_Error();
        return;
    }

    *dataPtr = list->firstElement->data;
}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu posledního prvku seznamu list.
 * Pokud je seznam list prázdný, volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetLast( DLList *list, int *dataPtr ) {
    if (list->lastElement == NULL) {                        // if the last element of the list doesn't exist
        DLL_Error();
        return;
    }

    *dataPtr = list->lastElement->data;
}

/**
 * Zruší první prvek seznamu list.
 * Pokud byl první prvek aktivní, aktivita se ztrácí.
 * Pokud byl seznam list prázdný, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteFirst( DLList *list ) {

    if (list->firstElement == NULL) {                       // If the list is empty
        return;
    }
    else {
        DLLElementPtr elementPtr = list->firstElement;

        if (list->activeElement == list->firstElement) {    // If the active element is the first element
            list->activeElement = NULL;
        }
        if (list->firstElement == list->lastElement) {      // If the list has only one element
            list->lastElement = NULL;
            list->firstElement = NULL;
        }
        else {
            list->firstElement = list->firstElement->nextElement;
            list->firstElement->previousElement = NULL;
        }
        free(elementPtr);
    }
}

/**
 * Zruší poslední prvek seznamu list.
 * Pokud byl poslední prvek aktivní, aktivita seznamu se ztrácí.
 * Pokud byl seznam list prázdný, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteLast( DLList *list ) {

    if (list->lastElement == NULL){
        return;
    }
    else{
        DLLElementPtr elementPtr = list->lastElement;

        if (list->activeElement == list->lastElement) {     // If the active element is the last element
            list->activeElement = NULL;
        }
        if (list->firstElement == list->lastElement) {      // If the list  has only one element
            list->firstElement = NULL;
            list->lastElement = NULL;
        }
        else {
            list->lastElement = list->lastElement->previousElement;
            list->lastElement->nextElement = NULL;
        }
        free(elementPtr);
    }
}

/**
 * Zruší prvek seznamu list za aktivním prvkem.
 * Pokud je seznam list neaktivní nebo pokud je aktivní prvek
 * posledním prvkem seznamu, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteAfter( DLList *list ) {

    if (list->activeElement == NULL || list->activeElement == list->lastElement) {
        return;
    }
    else {
        DLLElementPtr tmp = list->activeElement->nextElement;           // Create a temporary pointer to the element to be deleted

        if (tmp == list->lastElement) {                                 // If the element to be deleted is the last element of the list
            list->activeElement->nextElement = NULL;
            list->lastElement = list->activeElement;
        }
        else {                                                          // If the element to be deleted is not the last element of the list
            tmp->nextElement->previousElement = list->activeElement;
            list->activeElement->nextElement = tmp->nextElement;
        }
        free(tmp);
    }
}

/**
 * Zruší prvek před aktivním prvkem seznamu list .
 * Pokud je seznam list neaktivní nebo pokud je aktivní prvek
 * prvním prvkem seznamu, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteBefore( DLList *list ) {

    if (list->activeElement ==  NULL || list->activeElement == list->firstElement) {
        return;
    }
    else {
        DLLElementPtr tmp = list->activeElement->previousElement;           // Create a temporary pointer to the element to be deleted

        if (tmp == list->firstElement){                                     // If the element to be deleted is the first element of the list
            list->activeElement->previousElement = NULL;
            list->firstElement = list->activeElement;
        }
        else {                                                              // If the element to be deleted is between two elements in the middle of the list
            list->activeElement->previousElement = tmp->previousElement;
            tmp->previousElement->nextElement = list->activeElement;
        }
        free(tmp);
    }
}

/**
 * Vloží prvek za aktivní prvek seznamu list.
 * Pokud nebyl seznam list aktivní, nic se neděje.
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení do seznamu za právě aktivní prvek
 */
void DLL_InsertAfter( DLList *list, int data ) {

    if (list->activeElement == NULL) {
        return;
    }
    else {
        DLLElementPtr newElement = (DLLElementPtr)malloc(sizeof(struct DLLElement));
        if (newElement == NULL){
            DLL_Error();
            return;
        }
        else {
            newElement->data = data;

            if (list->activeElement == list->lastElement) {                         // If the active element is the last element of the list
                list->lastElement->nextElement = newElement;                        // Insert the new element after the last element
                newElement->previousElement = list->activeElement;                  // Set the 'previous' element pointer of the new element to the 'active' element
                newElement->nextElement = NULL;                                     // Set the 'next' element pointer of the new element to NULL (now it's the last element)
                list->lastElement = newElement;                                     // Change the 'last element' pointer of the list to the new element
            }
            else {                                                                  // Not the last
                newElement->nextElement = list->activeElement->nextElement;
                newElement->previousElement = list->activeElement;
                list->activeElement->nextElement->previousElement = newElement;
                list->activeElement->nextElement = newElement;
            }
        }
    }

}

/**
 * Vloží prvek před aktivní prvek seznamu list.
 * Pokud nebyl seznam list aktivní, nic se neděje.
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení do seznamu před právě aktivní prvek
 */
void DLL_InsertBefore( DLList *list, int data ) {

    if (list->activeElement == NULL) {
        return;
    }
    else {
        DLLElementPtr newElement = (DLLElementPtr)malloc(sizeof(struct DLLElement));
        if (newElement == NULL) {
            DLL_Error();
            return;
        }
        newElement->data = data;

        if (list->activeElement == list->firstElement){         // If the active element is the first element of the list
            newElement->nextElement = list->activeElement;      // Insert the new element before the first element
            newElement->previousElement = NULL;                 // Set the 'previous' element pointer of the new element to NULL (now it's the first element)
            list->activeElement->previousElement = newElement;  // Set the 'previous' element pointer of the first element to the new element
            list->firstElement = newElement;                    // Change the 'first element' pointer of the list to the new element
        }
        else {                                                  // Not the first
            newElement->nextElement = list->activeElement;
            newElement->previousElement = list->activeElement->previousElement;
            list->activeElement->previousElement->nextElement = newElement;
            list->activeElement->previousElement = newElement;
        }
    }

}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu aktivního prvku seznamu list.
 * Pokud seznam list není aktivní, volá funkci DLL_Error ().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetValue( DLList *list, int *dataPtr ) {
    if (list->activeElement == NULL) {
        DLL_Error();
        return;
    }
    else {
        *dataPtr = list->activeElement->data;
    }
}

/**
 * Přepíše obsah aktivního prvku seznamu list.
 * Pokud seznam list není aktivní, nedělá nic.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Nová hodnota právě aktivního prvku
 */
void DLL_SetValue( DLList *list, int data ) {
    if (list->activeElement == NULL) {
        return;
    }
    else {
        list->activeElement->data = data;
    }
}

/**
 * Posune aktivitu na následující prvek seznamu list.
 * Není-li seznam aktivní, nedělá nic.
 * Všimněte si, že při aktivitě na posledním prvku se seznam stane neaktivním.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Next( DLList *list ) {
    if (list->activeElement == NULL){
        return;
    }
    else {
        list->activeElement = list->activeElement->nextElement;
    }
}


/**
 * Posune aktivitu na předchozí prvek seznamu list.
 * Není-li seznam aktivní, nedělá nic.
 * Všimněte si, že při aktivitě na prvním prvku se seznam stane neaktivním.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Previous( DLList *list ) {
    if (list->activeElement == NULL) {
        return;
    }
    else {
        list->activeElement = list->activeElement->previousElement;
    }
}

/**
 * Je-li seznam list aktivní, vrací nenulovou hodnotu, jinak vrací 0.
 * Funkci je vhodné implementovat jedním příkazem return.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 *
 * @returns Nenulovou hodnotu v případě aktivity prvku seznamu, jinak nulu
 */
int DLL_IsActive( DLList *list ) {
    return list->activeElement == NULL ? 0 : 1;
}

/* Konec c206.c */
