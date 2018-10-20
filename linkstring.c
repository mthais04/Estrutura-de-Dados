/*
Versão 2 de Listas Simples (Contínuas)
*/
#include "linkstring.h"

/*
 alocação dinâmica de nós
*/

Node *MakeNode(Entry item){
	Node *nodepointer;
	if ((nodepointer = malloc(sizeof(Node))) == NULL)
		Error("Memória esgotada.");
	else {
		nodepointer->info = item;
		nodepointer->next = NULL;
		nodepointer->previous = NULL;
	}
	return nodepointer;
}


/*
 inicialização
*/

void CreateList(List *buffer){
	if (buffer == NULL) {
		Error("Impossível inicializar a lista.");
	}
	buffer->count = 0;
	buffer->current = NULL;
	buffer->currentpos = -1;
}

void ReadList(List *buffer){
	char line[MAXLINE];
	Boolean proceed = TRUE;
	
	if(!ListEmpty(buffer)){
		printf("Buffer não está vazio; a leitura irá destruí-lo. "
		"Ok em prosseguir?");
		if ((proceed = UserSaysYes()) == TRUE)
			ClearList(buffer);
	}

	if(proceed){
		CreateList(buffer);
		fseek(buffer->fpi, 0L, 0); //reseta para o início do arquivo
		while(fgets(line,MAXLINE,buffer->fpi))
			InsertList(ListSize(buffer),line,buffer);
		buffer->currentpos = 0;
		buffer->current = buffer->head;
	}
}

void FindString(List *buffer){
	int n;
	Node *p;
	char *s, *t, target[MAXLINE];
	if(ListEmpty(buffer))
		printf("Buffer vazio; não pode procurar.\n");
	else{
		printf("String para procurar? ");
		fflush(stdin);
		fgets(target, MAXLINE, stdin);
		target[strlen(target) - 1] = '\0'; //drop newline charcter
		for(p = buffer->current, n = buffer->currentpos; p; p = p->next, n++)
			if((s = strstr(p->entry, target)) != NULL)
				break;
		if(!p)
			printf("String não foi encontrada.\n");
		else{
			buffer->current = p;
			buffer->currentpos = n;
			printf("%2d: %s", n, p->entry);
			printf("   ");
			for(t = p->entry; t<s; t++)
				putchar('^');
			putchar('\n');
		}
	}
}
		

/*
 reinicialização
*/

void ClearList(List *buffer){
	Node *np;
	SetPosition(buffer, 0);
	while (!ListEmpty(buffer)) {
		np = buffer->current;
		buffer->current = np->next;
		buffer->current->previous = NULL;
		np->next = NULL;
		free(np);
	}
	buffer->count = 0;
	buffer->current = NULL;
	buffer->currentpos = -1;
}


/*
 operações de situação (status)
*/

Boolean ListEmpty(const List *buffer){
	return buffer->current == NULL;
}

Boolean ListFull(const List *buffer){
	return FALSE;
}

int ListSize(const List *buffer){
	return buffer->count;
}


/*
operações de manipulação
*/
void AddList(List *buffer, Entry item){
	Node *np = MakeNode(item);
	AddNode(buffer, np);
}

void AddNode(List *buffer, Node *np){
	if (np == NULL)
		Error("Tentativa de adicionar um nó não existente.");
	else {
		np->next = NULL;
		np->previous = NULL;
		if (buffer->current == NULL){
			buffer->current = np;
			buffer->currentpos = 0;
		} else {
			SetPosition(buffer, buffer->count -1);
			buffer->current->next = np;
			np->previous = buffer->current;
		}
		buffer->count++;
	}
}

void SetPosition(List *buffer, Position p){
	if (p < 0 || p >= ListSize(buffer))
		Error("Tentativa de acessar para uma posição fora da lista.");
	else {
		if (buffer->currentpos < p)
			for (; buffer->currentpos != p; buffer->currentpos++)
				buffer->current = buffer->current->next;
		else if (buffer->currentpos > p)
			for (; buffer->currentpos != p; buffer->currentpos--)
				buffer->current = buffer->current->previous;
	}
}

void InsertList(List *buffer, Entry item, Position p){
	Node *newnode, *following;
	if (p < 0 || p > ListSize(buffer))
		Error("Tentativa de inserir em uma posição inválida da lista.");
	else {
		newnode = MakeNode(item);
		if (p == 0) {
			if (buffer->count > 0) {
				SetPosition(buffer, 0);
				newnode->next = buffer->current;
				buffer->current->previous = newnode;
			}
		} else {
			SetPosition(buffer, p - 1);
			following = buffer->current->next;
			/* Insere entre o atual e o seguinte. */
			newnode->next = following;
			newnode->previous = buffer->current;
			buffer->current->next = newnode;
			if (following)
				following->previous = newnode;
		}
		buffer->current = newnode;
		buffer->currentpos = p;
		buffer->count++;
	}
}

void DeleteList(List *buffer, Entry *item, Position p){
	Node *old_node, *next, *previous;
	if (p < 0 || p >= ListSize(buffer))
		Error("Tentativa de remoção de uma posição inválida da lista.");
	else {
		SetPosition(buffer, p);
		old_node = buffer->current;
		next = old_node->next;
		previous = old_node->previous;
		if (next) {
			next->previous = previous;
			buffer->current = next;
		} else {
			buffer->current = previous;
			buffer->currentpos--;
		}
		if (previous)
			previous->next = next;
		buffer->count--;
		*item = old_node->info;
		old_node->next = NULL;
		old_node->previous = NULL;
		free(old_node);
	}
}

void RetrieveList(List *buffer, Entry *item, Position p){
	if (p < 0 || p >= ListSize(buffer))
		Error("Tentativa de recuperar o valor de uma posição inválida da lista.");
	else {
		SetPosition(buffer, p);
		*item = buffer->current->info;
	}
}

void ReplaceList(List *buffer, Entry item, Position p){
	if (p < 0 || p >= ListSize(buffer))
		Error("Tentativa de substituição em uma posição inválida da lista.");
	else {
		SetPosition(buffer, p);
		buffer->current->info = item;
	}
}

/*
percorre e visita
*/
void TraverseList(List *buffer, void (*Visit)(Entry)){
	int currentpos = buffer->currentpos;
	Node *current = buffer->current;
	SetPosition(buffer, 0);
	Node *np = buffer->current;
	while (np != NULL) {
		(*Visit)(np->info);
		np = np->next;
	}
	//SetPosition(list, currentpos);
	buffer->currentpos = currentpos;
	buffer->current = current;
}
