#include "common.h"
#include <string.h>

void Warning (const char *message){
	fprintf(stderr, "Warning: %s\n", message);
}

void Error (const char *message){
	fprintf(stderr, "Error: %s\n", message);
	exit(1);
}

void Print(Entry item){
	printf("%s\n", item);
}


int stricmp(char const *str1, char const *str2) {
   for (;; str1++, str2++) {
      int d = tolower((unsigned char)*str1) - tolower((unsigned char)*str2);//erro aqui
      if (d != 0 || !*str1)
         return d;
   }
}
