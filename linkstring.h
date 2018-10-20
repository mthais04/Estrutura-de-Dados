/*
Versão 2 de Listas Simples (Contínuas)
*/
#include "common.h"

typedef struct node {
	Entry info, entry;
	struct node *next;
	struct node *previous;

} Node;

typedef struct buffer {
	int count;
	Node *current;
	Node *head;
	Position currentpos;
	FILE *fpi;
} List;

/*
 alocação dinâmica de nós
*/
Node *MakeNode(Entry item);

/*
inicialização
*/

void CreateList(List *buffer);
void ReadList(List *buffer);
void FindString(List *buffer);

/*
reinicialização
*/

void ClearList(List *buffer);


/*
 operações de situação (status)
*/

Boolean ListEmpty(const List *buffer);

Boolean ListFull(const List *buffer);

int ListSize(const List *buffer);


/*
operações de manipulação
*/
void AddList(List *buffer, Entry item);

void AddNode(List *buffer, Node *np);

void SetPosition(List *buffer, Position p);

void InsertList(List *buffer, Entry item, Position p);

void DeleteList(List *buffer, Entry *item, Position p);

void RetrieveList(List *buffer, Entry *item, Position p);

void ReplaceList(List *buffer, Entry item, Position p);


/*
percorre e visita
*/
void TraverseList(List *buffer, void (*Visit)(Entry));
