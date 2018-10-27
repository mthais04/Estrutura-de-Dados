#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define MAXLINE 10

typedef enum boolean {FALSE, TRUE} Boolean;

typedef int Position;

typedef char *Entry;

/* Warning: exibe uma mensagem de alerta.
Pré: message é a mensagem de alerta para exibir.
Pós: A mensagem de alerta é impressa no stderr. */
void Warning (const char *message);

/* Error: exibe uma mensagem de erro.
Pré: message é a mensagem de erro para exibir.
Pós: A mensaem de alerta é impressa no stderr e o programa é encerrado. */
void Error (const char *message);

/* Print: exibe um Entry.
Pré: item contém uma entrada válida da lista (um caracter).
Pós: Imprime o valor de item. */
void Print(Entry item);

/*
Compares the string pointed to, by str1 to the string pointed to by str2, ignoring case.
*/
int stricmp(char const *a, char const *b);
