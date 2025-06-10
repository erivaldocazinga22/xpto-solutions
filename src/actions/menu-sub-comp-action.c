#include "../include/structs.h"
#include "../include/utils.h"
#include <stdio.h>

#define FILE_NAME "./data/componente-subcomponente.txt"

void associarComponentes() {
  ComponenteSubcomponente novoRelacionamento;

  int error = 0;
  do {
    printf("Digite o ID do componente: ");
    scanf("%d", &novoRelacionamento.idComponente);
    getchar();

    error = componenteAlreadyExists(novoRelacionamento.idComponente);
    if (!error)
      printf("ID do componente não existe.\n");
  } while (!error);

  do {
    printf("Digite o ID do Sub-Componente: ");
    scanf("%d", &novoRelacionamento.idSubComponente);
    getchar();

    error = componenteAlreadyExists(novoRelacionamento.idComponente);
    if (!error)
      printf("ID do sub-componente não existe.\n");
  } while (!error);

  // Salvar Relacionamento
  FILE *arq = fopen(FILE_NAME, "a");
  if (arq == NULL) {
    printf("Erro ao abrir o ficheiro.\n");
    return;
  }

  fprintf(arq, "%d;%d", novoRelacionamento.idComponente,
          novoRelacionamento.idSubComponente);

  fclose(arq);
}