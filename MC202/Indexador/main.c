#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#include "funcoes_index.h"


int main(void) {

  int atualizado=0, desatualizado=0, obsoleto=0;
  char cmd[2], nivel[10], estado[14];
  long x;

  Index* head = NULL;

  while(1){
    if (scanf("%s", cmd) != 1) exit(1);

    if (strcmp(cmd,"i") == 0) {
      if (scanf("%ld %s %s", &x, nivel, estado) != 3) exit(1);
      df_insert(&head, x, nivel, estado, &atualizado, &desatualizado, &obsoleto);
    }

    else if (strcmp(cmd,"a") == 0) {
      if (scanf("%ld %s %s", &x, nivel, estado) != 3) exit(1);
      df_atualizar(&head, x, nivel, estado, &atualizado, &desatualizado, &obsoleto);
    }

    else if (strcmp(cmd,"b") == 0) {
      if (scanf(" %ld", &x) != 1) exit(1);
      df_search(head, x);
    }

    else if (strcmp(cmd,"l") == 0) {
      df_print(head);
    }

    else if (strcmp(cmd,"t") == 0) {
      df_print_estados(atualizado, desatualizado, obsoleto);
    }

    else if (strcmp(cmd,"f") == 0) {
      df_free(head);
      return 0;
    }
  }
}