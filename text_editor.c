// compilar: gcc text_editor.c linkstring.c common.c -o text_editor.bin
//após executar o comando acima, escrever no terminal " ./text_editor.bin arquivo.txt "

/* Double Linked List Driver
Pré: Nenhuma.
Pós: Atuam como um programa de demonstração orientado por um menu para listas.
*/

#include "linkstring.h"
#include <ctype.h>        /* Usado para converter maiúsculas em minúsculas. */
#include <string.h>

void Introduction(void);
void Help(void);
void GetCommand(char *command, List *buffer);
void DoCommand(char, List *buffer);
void OpenFiles(int argc, char *argv[], List *buffer);
void DisplayCurrentLine(const List *buffer);

int main(int argc, char *argv[]){
	char command;
	List buffer;

   CreateList(&buffer);
	OpenFiles(argc, argv, &buffer);

    /* loop infinito; DoCommand() irá chamar exit() para finalizar o programa. */
   while (TRUE) {
      GetCommand(&command, &buffer);
      DoCommand(command, &buffer);
   }
   return 1;           /* Este comando nunca será executado. */
}

/*Help:
Pré: Nenhuma.
Pós: Uma tela de ajuda para o programa a ser impressa. */
void Introduction(void){
    printf("\t===================================\n");
    printf("\tIniciando Double Linked List String Driver\n");
    printf("\t===================================\n\n");
}

/*Help:
Pré: Nenhuma.
Pós: Uma tela de ajuda para o programa a ser impressa. */
void Help(void){
    printf("\nEste programa permite que um comando seja inserido em cada linha.\n"
        "Por exemplo, se o comando I é inserido na linha de comando\n"
        "então o programa irá pedir por uma cadeia de caracteres (string) e\n"
        "e imediatamente em seguira irá inserí-lo na lista.\n");
    printf("Os comando válidos são:\n"
        "\tB - Vai para o início da string\n"
        "\tC - Muda a string\n"
        "\tD - Deleta a linha\n"
        "\tE - Vai para o final da string\n"
        "\tF - Procura a string\n"
        "\tI - Insere uma nova linha\n"
	"\tP - Linha anterior\n"
	"\tL - Tamanho da string\n"
	"\tN - Próxima linha\n"
	"\tR - Leitura do arquivo\n"
	"\tS - Substitui linha\n"
	"\tV - Visualizar\n"
	"\tW - Escrever arquivo\n"
        "\tH - Esta tela de ajuda\n"
        "\tQ - Sair\n"
        "Pressione <Enter> para continuar.");
    while (getchar() != '\n')
        ;
}

/* GetCommand:
Pré: Nenhuma.
Pós: Recupera um comando válido do terminal e devolve o comando do desejado. */
void GetCommand(char *command, List *buffer) {
	if(ListSize(buffer) == 0)
		printf("O buffer está vazio. ");
	else
		DisplayCurrentLine(buffer);
	printf("Por favor, entre com um comando. ");
	do {
		while((*command=getchar()) == ' ' || *command == '\t' || *command == '\n')
			;
		*command = tolower(*command);
		if(strchr("bcdefgh?ilnpqrsvw", *command) == NULL)
			printf("Digite h para ajuda ou entre com um comando válido");
		else
			return;
	} while(1);
}

/* DoCommand:
Pré: command contém um comando válido.
Pós: Executa o comando na lista. */
void DoCommand(char command, List *buffer){

	switch(command){
	  /* case 'b':
	 	GoToBeginning(buffer);
	 	break;*/
	   case 'c':
	 	ChangeString(buffer);
	 	break;
	 /*  case 'd':
	  	DeleteLine(buffer);
	 	break;
	   case 'e': 
		GoToEndBuffer(buffer);
	 	break;*/
	   case 'f':
	 	FindString(buffer);
	 	break;
	   /*case 'g':
	 	GoToLine(buffer);
	 	break;
	   case '?':
	   case 'h':
	 	Help();
	 	break;*/
	   case 'i':
	 	InsertLine(buffer);
	 	break;
	   /*case 'l':
	 	Length(buffer);
	 	break;
	   case 'n':
	 	NextLine(buffer);
	 	break;
	   case 'p':
	 	PrecedingLine(buffer);
	 	break;
	   case 'q':
		break;*/
	   case 'r':
	 	ReadFile(buffer);
	 	break;
	   /*case 's':
	 	SubstituteLine(buffer);
	 	break;
	   case 'v':
	 	ViewBuffer(buffer);
	 	break;*/
	   case 'w':
	 	WriteFile(buffer);
	 	break;
   default:
      printf("Não faz nada.\n");
	}
}

void OpenFiles(int argc, char *argv[], List *buffer) {
   char *error_msg;
   if (argc != 2) {
      sprintf(error_msg, "O programa exige o nome do arquivo de texto como único argumento.\n"
            "\tExemplo de chamada: %s arquivo.txt\n", argv[0]);
      Error(error_msg);
   }
   printf("Carregando conteúdo do arquivo %s ...\n", argv[1]);
	;
   if((buffer->fpi = fopen(argv[1], "r+")) == NULL)
	buffer->fpi = fopen(argv[1], "w+");
   else
       ReadFile(buffer);
   //recuperar o conteúdo do arquivo e colocar no buffer
   printf("Conteúdo do arquivo %s carregado.\n", argv[1]);
}

void DisplayCurrentLine(const List *buffer){
   char *strtmp;
   sprintf(strtmp, "%d", buffer->count);
   sprintf(strtmp, "%%%dd", (int)strlen(strtmp));
   printf(strtmp, buffer->currentpos);
   Print(buffer->current->info);
}

Boolean UserSaysYes() {
   char confirm[4];
   fgets(confirm, 4, stdin);
   return (stricmp("y", confirm) == 0) || (stricmp("yes", confirm) == 0);
}
