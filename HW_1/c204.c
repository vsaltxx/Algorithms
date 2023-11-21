/* ******************************* c204.c *********************************** */
/*  Předmět: Algoritmy (IAL) - FIT VUT v Brně                                 */
/*  Úkol: c204 - Převod infixového výrazu na postfixový (s využitím c202)     */
/*  Referenční implementace: Petr Přikryl, listopad 1994                      */
/*  Přepis do jazyka C: Lukáš Maršík, prosinec 2012                           */
/*  Upravil: Kamil Jeřábek, září 2019                                         */
/*           Daniel Dolejška, září 2021                                       */
/* ************************************************************************** */
/*
** Implementujte proceduru pro převod infixového zápisu matematického výrazu
** do postfixového tvaru. Pro převod využijte zásobník (Stack), který byl
** implementován v rámci příkladu c202. Bez správného vyřešení příkladu c202
** se o řešení tohoto příkladu nepokoušejte.
**
** Implementujte následující funkci:
**
**    infix2postfix ... konverzní funkce pro převod infixového výrazu
**                      na postfixový
**
** Pro lepší přehlednost kódu implementujte následující pomocné funkce:
**    
**    untilLeftPar ... vyprázdnění zásobníku až po levou závorku
**    doOperation .... zpracování operátoru konvertovaného výrazu
**
** Své řešení účelně komentujte.
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako procedury
** (v jazyce C procedurám odpovídají funkce vracející typ void).
**
**/

#include "c204.h"

//bool solved;

/**
 * Pomocná funkce untilLeftPar.
 * Slouží k vyprázdnění zásobníku až po levou závorku, přičemž levá závorka bude
 * také odstraněna.
 * Pokud je zásobník prázdný, provádění funkce se ukončí.
 *
 * Operátory odstraňované ze zásobníku postupně vkládejte do výstupního pole
 * znaků postfixExpression.
 * Délka převedeného výrazu a též ukazatel na první volné místo, na které se má
 * zapisovat, představuje parametr postfixExpressionLength.
 *
 * Aby se minimalizoval počet přístupů ke struktuře zásobníku, můžete zde
 * nadeklarovat a používat pomocnou proměnnou typu char.
 *
 * @param stack Ukazatel na inicializovanou strukturu zásobníku
 * @param postfixExpression Znakový řetězec obsahující výsledný postfixový výraz
 * @param postfixExpressionLength Ukazatel na aktuální délku výsledného postfixového výrazu
 */
void untilLeftPar( Stack *stack, char *postfixExpression, unsigned *postfixExpressionLength ) {

    char tmp;                                               // temporary variable for storing a character from the stack
    Stack_Top(stack, &tmp);                                 // take a character from the top of the stack and store it in a temporary variable

    while (tmp != '(') {
        postfixExpression[*postfixExpressionLength] = tmp;  // add the character to the end of the output string at the PostfixExpressionLength position (first free position)
        (*postfixExpressionLength)++;                       // we must increase the length of the output string
        Stack_Pop(stack);                                   // remove the character from the stack
        Stack_Top(stack, &tmp);                             // take a character from the top of the stack and store it in a temporary variable
    }

    Stack_Pop(stack);                                       // remove the left parenthesis from the stack

}

/**
 * Pomocná funkce doOperation.
 * Zpracuje operátor, který je předán parametrem c po načtení znaku ze
 * vstupního pole znaků.
 *
 * Dle priority předaného operátoru a případně priority operátoru na vrcholu
 * zásobníku rozhodneme o dalším postupu.
 * Délka převedeného výrazu a taktéž ukazatel na první volné místo, do kterého
 * se má zapisovat, představuje parametr postfixExpressionLength, výstupním
 * polem znaků je opět postfixExpression.
 *
 * @param stack Ukazatel na inicializovanou strukturu zásobníku
 * @param c Znak operátoru ve výrazu
 * @param postfixExpression Znakový řetězec obsahující výsledný postfixový výraz
 * @param postfixExpressionLength Ukazatel na aktuální délku výsledného postfixového výrazu
 */
void doOperation( Stack *stack, char c, char *postfixExpression, unsigned *postfixExpressionLength ) {

    if (Stack_IsEmpty(stack)) { // if the stack is empty, we put the character at the top of the stack
        Stack_Push(stack, c);
    }
    else {
        char tmp;
        Stack_Top(stack, &tmp);                                                 // find out what character is on the top of the stack

        if (tmp == '(') {
            Stack_Push(stack, c);                                               // put the left parenthesis on the top of the stack
        } else if ((tmp == '+' || tmp == '-') && (c == '*' || c =='/')) {
            Stack_Push(stack, c);                                               // if the top of the stack is + or - and the character read is * or /, put the character at the top of the stack
        } else {                                                                // if the top of the stack is * or / and the character read is + or -
            postfixExpression[*postfixExpressionLength] = tmp;                  // remove the character at the top of the stack and insert it on the output string
            (*postfixExpressionLength)++;                                       // increase the length of the output string
            Stack_Pop(stack);                                                   // remove the character from the stack
            doOperation(stack, c, postfixExpression, postfixExpressionLength);  // recursive function call. if the top of the stack is * or / and the character loaded is + or -. we remove the character at the top of the stack and insert it on the output string
        }
    }

}

/**
 * Konverzní funkce infix2postfix.
 * Čte infixový výraz ze vstupního řetězce infixExpression a generuje
 * odpovídající postfixový výraz do výstupního řetězce (postup převodu hledejte
 * v přednáškách nebo ve studijní opoře).
 * Paměť pro výstupní řetězec (o velikosti MAX_LEN) je třeba alokovat. Volající
 * funkce, tedy příjemce konvertovaného řetězce, zajistí korektní uvolnění zde
 * alokované paměti.
 *
 * Tvar výrazu:
 * 1. Výraz obsahuje operátory + - * / ve významu sčítání, odčítání,
 *    násobení a dělení. Sčítání má stejnou prioritu jako odčítání,
 *    násobení má stejnou prioritu jako dělení. Priorita násobení je
 *    větší než priorita sčítání. Všechny operátory jsou binární
 *    (neuvažujte unární mínus).
 *
 * 2. Hodnoty ve výrazu jsou reprezentovány jednoznakovými identifikátory
 *    a číslicemi - 0..9, a..z, A..Z (velikost písmen se rozlišuje).
 *
 * 3. Ve výrazu může být použit předem neurčený počet dvojic kulatých
 *    závorek. Uvažujte, že vstupní výraz je zapsán správně (neošetřujte
 *    chybné zadání výrazu).
 *
 * 4. Každý korektně zapsaný výraz (infixový i postfixový) musí být uzavřen
 *    ukončovacím znakem '='.
 *
 * 5. Při stejné prioritě operátorů se výraz vyhodnocuje zleva doprava.
 *
 * Poznámky k implementaci
 * -----------------------
 * Jako zásobník použijte zásobník znaků Stack implementovaný v příkladu c202.
 * Pro práci se zásobníkem pak používejte výhradně operace z jeho rozhraní.
 *
 * Při implementaci využijte pomocné funkce untilLeftPar a doOperation.
 *
 * Řetězcem (infixového a postfixového výrazu) je zde myšleno pole znaků typu
 * char, jenž je korektně ukončeno nulovým znakem dle zvyklostí jazyka C.
 *
 * Na vstupu očekávejte pouze korektně zapsané a ukončené výrazy. Jejich délka
 * nepřesáhne MAX_LEN-1 (MAX_LEN i s nulovým znakem) a tedy i výsledný výraz
 * by se měl vejít do alokovaného pole. Po alokaci dynamické paměti si vždycky
 * ověřte, že se alokace skutečně zdrařila. V případě chyby alokace vraťte namísto
 * řetězce konstantu NULL.
 *
 * @param infixExpression vstupní znakový řetězec obsahující infixový výraz k převedení
 *
 * @returns znakový řetězec obsahující výsledný postfixový výraz
 */
char *infix2postfix( const char *infixExpression ) {

    unsigned postfixExpressionLength = 0;                // initialization of the length of the output string

    Stack *stack = (Stack *)malloc(sizeof(Stack));  // initialization of the stack to store there operators
    if (stack == NULL) {                                // check if the memory allocation was successful
        return NULL;
    }

    char *postfixExpression = (char *)malloc(MAX_LEN * sizeof(char));   // initialization of the output string
    if (postfixExpression == NULL) {
        free(stack);                                // failure of the first malloc, release the memory of the first malloc
        return NULL;
    }

    Stack_Init(stack);                                  // stack initialization

    int i = 0;
    while (infixExpression[i] != '\0'){

        if ((infixExpression[i] >= '0' && infixExpression[i] <= '9') ||
            (infixExpression[i] >= 'a' && infixExpression[i] <= 'z') ||
            (infixExpression[i] >= 'A' && infixExpression[i] <= 'Z'))           // if the character is a number or a letter
        {
            postfixExpression[postfixExpressionLength] = infixExpression[i];    // add it to the end of the output string
            postfixExpressionLength++;
        }
        else if (infixExpression[i] == '(')                                     // if the character is a left parenthesis
        {
            Stack_Push(stack, infixExpression[i]);                              // add it to the top of the stack
        }
        else if (infixExpression[i] == ')')                                     // if the character is a right parenthesis
        {
            untilLeftPar(stack, postfixExpression, &postfixExpressionLength);    // we call the function to empty the stack to the left parenthesis
        }
        else if (infixExpression[i] == '+' || infixExpression[i] == '-' || infixExpression[i] == '*' || infixExpression[i] == '/') // if the character is an operator
        {
            doOperation(stack, infixExpression[i], postfixExpression, &postfixExpressionLength); // we call the function to process the operator according to its priority
        }
        else if (infixExpression[i] == '=')                                     // if the character is an equal sign (the end of the expression)
        {
            // we empty the stack to the end of the expression
            while (!Stack_IsEmpty(stack)){

                Stack_Top(stack, &(postfixExpression[postfixExpressionLength])); // we take the character from the top of the stack and store it in the output string
                postfixExpressionLength++;
                Stack_Pop(stack);   // we remove the character from the stack
            }
            postfixExpression[postfixExpressionLength] = '=';   // we add the equal sign to the end of the output string
            postfixExpressionLength++;                          // we increase the length of the output string
            postfixExpression[postfixExpressionLength] = '\0';  // we add '\0' to the end of the output string

            Stack_Dispose(stack);   // we release the memory of the stack
            break;
        }
        else // if the character is sth else
        {
            return NULL;
        }
        i++;
    }
	return postfixExpression;
}


/**
 * Pomocná metoda pro vložení celočíselné hodnoty na zásobník.
 *
 * Použitá implementace zásobníku aktuálně umožňuje vkládání pouze
 * hodnot o velikosti jednoho byte (char). Využijte této metody
 * k rozdělení a postupné vložení celočíselné (čtyřbytové) hodnoty
 * na vrchol poskytnutého zásobníku.
 *
 * @param stack ukazatel na inicializovanou strukturu zásobníku
 * @param value hodnota k vložení na zásobník
 */
void expr_value_push( Stack *stack, int value ) {

    Stack_Push(stack, (char)(value & 0xFF));            // vložení nejnižšího bytu hodnoty na vrchol zásobníku
    Stack_Push(stack, (char)((value >> 8) & 0xFF));     // vložení druhého nejnižšího bytu hodnoty na vrchol zásobníku
    Stack_Push(stack, (char)((value >> 16) & 0xFF));    // vložení druhého nejvyššího bytu hodnoty na vrchol zásobníku
    Stack_Push(stack, (char)((value >> 24) & 0xFF));    // vložení nejvyššího bytu hodnoty na vrchol zásobníku
}

/**
 * Pomocná metoda pro extrakci celočíselné hodnoty ze zásobníku.
 *
 * Využijte této metody k opětovnému načtení a složení celočíselné
 * hodnoty z aktuálního vrcholu poskytnutého zásobníku. Implementujte
 * tedy algoritmus opačný k algoritmu použitému v metodě
 * `expr_value_push`.
 *
 * @param stack ukazatel na inicializovanou strukturu zásobníku
 * @param value ukazatel na celočíselnou proměnnou pro uložení
 *   výsledné celočíselné hodnoty z vrcholu zásobníku
 */
void expr_value_pop( Stack *stack, int *value ) {

    char tmp;
    Stack_Top(stack, &tmp);
	*value = 0;

    *value |= ((unsigned char)tmp) << 24;   // read the highest byte of the value from the stack
    Stack_Pop(stack);                       // remove the highest byte of the value from the stack
    Stack_Top(stack, &tmp);
    *value |= ((unsigned char)tmp) << 16;   // read the second highest byte of the value from the stack
    Stack_Pop(stack);                       // remove its value from the stack
    Stack_Top(stack, &tmp);
    *value |= ((unsigned char)tmp) << 8;    // read the second lowest byte of the value from the stack
    Stack_Pop(stack);                       // remove its value from the stack
    Stack_Top(stack, &tmp);
    *value |= ((unsigned char)tmp);         // read the lowest byte of the value from the stack
    Stack_Pop(stack);                       // remove its value from the stack
}

bool isOperator(char c) {
    return (c == '+' || c == '-' || c == '*' || c == '/');
}
/**
 * Tato metoda provede vyhodnocení výrazu zadaném v `infixExpression`,
 * kde hodnoty proměnných použitých v daném výrazu jsou definovány
 * v poli `variableValues`.
 *
 * K vyhodnocení vstupního výrazu využijte implementaci zásobníku
 * ze cvičení c202. Dále také využijte pomocných funkcí `expr_value_push`,
 * respektive `expr_value_pop`. Při řešení si můžete definovat libovolné
 * množství vlastních pomocných funkcí.
 *
 * Předpokládejte, že hodnoty budou vždy definovány
 * pro všechy proměnné použité ve vstupním výrazu.
 *
 * @param infixExpression vstpní infixový výraz s proměnnými
 * @param variableValues hodnoty proměnných ze vstupního výrazu
 * @param variableValueCount počet hodnot (unikátních proměnných
 *   ve vstupním výrazu)
 * @param value ukazatel na celočíselnou proměnnou pro uložení
 *   výsledné hodnoty vyhodnocení vstupního výrazu
 *
 * @return výsledek vyhodnocení daného výrazu na základě poskytnutých hodnot proměnných
 */
bool eval( const char *infixExpression, VariableValue variableValues[], int variableValueCount, int *value ) {

    char *postfixExpression = malloc((MAX_LEN + 1) * sizeof(char)); // initialization of the output string
    if (postfixExpression == NULL) {                                    // check if the memory allocation was successful
        return false;
    }

    postfixExpression = infix2postfix(infixExpression);              // convert the input string to postfix

    Stack *stack = (Stack *)malloc(sizeof(Stack));              // initialization of the stack to store there operators
    if (stack == NULL) {
        return false;
    }
    Stack_Init(stack);                                              // initialization of the stack

    int operand;
    for (int i = 0; postfixExpression[i] != '\0'; i++) {
        char current_Char = postfixExpression[i];

        if ((current_Char >= '0' && current_Char <= '9') || (current_Char >= 'a' && current_Char <= 'z') || (current_Char >= 'A' && current_Char <= 'Z')) {
            bool found = false;                                     // flag for checking if the variable was found
            for (int j = 0; j < variableValueCount; j++) {          // search for the variable in the array of variables
                if (variableValues[j].name == current_Char) {
                    operand = variableValues[j].value;
                    found = true;                                   // the variable was found
                    break;
                }
            }
            if (!found) {                                           // the variable was not found
                free(postfixExpression);                        // release the memory of the output string
                Stack_Dispose(stack);                               // release the memory of the stack
                return false;                                       // we didn't find the variable
            }
            expr_value_push(stack, operand);                  // we put the value of the variable on the stack
        }
        else if (isOperator(current_Char)) {                    // if the character is an operator

            int operand1, operand2;

            // order is important for the division operation
            expr_value_pop(stack, &operand2);
            expr_value_pop(stack, &operand1);

            switch (current_Char) {
                case '+':
                    expr_value_push(stack, operand1 + operand2);
                    break;
                case '-':
                    expr_value_push(stack, operand1 - operand2);
                    break;
                case '*':
                    expr_value_push(stack, operand1 * operand2);
                    break;
                case '/':
                    expr_value_push(stack, operand1 / operand2);
                    break;
            }
        }

    }
    expr_value_pop(stack, value);                       // we take the result of the expression from the stack

    free(postfixExpression);                        // release the memory of the output string, we don't need it anymore
    Stack_Dispose(stack);                               // release the memory of the stack
    return true;
}

/* Konec c204.c */
