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

void ReadFile(List *buffer){
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

void InsertLine(List *buffer){

	int newline;
	char line[MAXLINE];

	if(ListFull(buffer))
		Error("Buffer está cheio; nenhuma inserção possível.");

	else{
		printf("Onde você quer inserir uma nova linha? ");
		scanf("%d", &newline);
		if(newline < 0 || newline > ListSize(buffer)){
			printf("Essa linha não existe.");
		}
		else{
			printf("|: ");
			fflush(stdin);
			fgets(line,MAXLINE,stdin);
			InsertList(newline,line,buffer);
		}
	}
}

void ChangeString(List *buffer)
{
	int n, len;
	char *s, rep[MAXLINE], text[MAXLINE], temp[MAXLINE];
	printf("Qual linha para mudar? ");
	scanf("%d", &n); //colocar o numero da linha
	if(n < 0 || n >= ListSize(buffer))
		printf("Nenhuma linha a processar.");
	else{
		SetPosition(n, buffer);
		printf("Texto a substituir? ");
		fflush(stdin);
		fgets(text, MAXLINE, stdin); //introduza o alvo
		text[strlen(text) - 1] = '\0'; //solta o caracter na nova linha
		if((s = strstr(buffer->current->entry, text)) == NULL)
			printf("String não encontrada.");
		else{
			printf("Substituição de texto? ");
			fflush(stdin);
			fgets(rep, MAXLINE, stdin); //introduz a substituição
			rep[strlen(rep) - 1] = '\0';
			len = s - buffer->current->entry;
			strncpy(temp, buffer->current->entry, len);
			temp[len]= '\0';
			strcat(temp, rep);
			strcat(temp, buffer->current->entry + len +strlen(text));
			free(buffer->current->entry);
			buffer->current->entry = malloc(strlen(temp) + 1);
			strcpy(buffer->current->entry, temp);
		}
	}
}

void WriteFile(List *buffer)
{
	List *fpi;
	fpi = fopen("arquivo.txt", "w");
	char frase[MAXLINE];
	printf("Entre com a string a ser gravada no arquivo: ");
	fgets(frase, MAXLINE, stdin);
	fprintf(fpi, "%s\0", frase);
	fclose(fpi);
	return 0;
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
