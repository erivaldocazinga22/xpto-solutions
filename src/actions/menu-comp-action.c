#include "../include/structs.h"
#include "../include/utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int obterProximoComponenteId() {

  FILE *arq = fopen("componentes.txt", "r");
  if (arq == NULL)
    return 1;

  int ultimoId = 0;
  char linha[512];
  while (fgets(linha, sizeof(linha), arq) != NULL) {
    int idTemp;
    if (sscanf(linha, "%d;", &idTemp) == 1) {
      if (idTemp > ultimoId) {
        ultimoId = idTemp;
      }
    }
  }
  fclose(arq);
  return ultimoId + 1;
}

void salvarComponente(Componente novoComponente) {
  FILE *arq = fopen("componentes.txt", "a");
  if (arq == NULL) {
    printf("Erro ao abrir o ficheiro.\n");
    return;
  }

  int id = obterProximoComponenteId();
  fprintf(arq, "%d;%d;%d;%d;%s;%s;%s;%s\n", id, novoComponente.idForncedor,
          novoComponente.idFabricante, novoComponente.idPostoTrabalho,
          novoComponente.tipo, novoComponente.designacao,
          novoComponente.condicao, novoComponente.observacao);
  fclose(arq);
}

void InserirComponente() {
  int error = 0;
  printf("\n============ INSERIR COMPONENTE =============\n");
  Componente novoComponente;
  getchar();

  do {
    printf("ID Fornecedor: ");
    scanf("%d", &novoComponente.idForncedor);
    getchar();

    error = fornecedorAlreadyExists(novoComponente.idForncedor);
    if (!error) {
      printf("Id do Forncedor nao existe.\n");
    }
  } while (!error);

  error = 0;
  do {
    printf("ID Fabricante: ");
    scanf("%d", &novoComponente.idFabricante);
    getchar();

    error = fabricanteAlreadyExists(novoComponente.idFabricante);
    if (!error) {
      printf("Id do Fabricante nao existe.\n");
    }
  } while (!error);

  do {
    printf("ID Posto Trabalho: ");
    scanf("%d", &novoComponente.idPostoTrabalho);
    getchar();

    error = postoAlreadyExists(novoComponente.idPostoTrabalho);
    if (!error) {
      printf("Id do Posto de trabalho nao existe.\n");
    }
  } while (!error);

  printf("Tipo: ");
  fgets(novoComponente.tipo, sizeof(novoComponente.tipo), stdin);
  strtok(novoComponente.tipo, "\n");

  printf("Designação: ");
  fgets(novoComponente.designacao, sizeof(novoComponente.designacao), stdin);
  strtok(novoComponente.designacao, "\n");

  printf("Condição: ");
  fgets(novoComponente.condicao, sizeof(novoComponente.condicao), stdin);
  strtok(novoComponente.condicao, "\n");

  printf("Observação: ");
  fgets(novoComponente.observacao, sizeof(novoComponente.observacao), stdin);
  strtok(novoComponente.observacao, "\n");

  salvarComponente(novoComponente);
}

void ListarComponentes() {
  FILE *arq = fopen("componentes.txt", "r");
  if (arq == NULL) {
    printf("Erro ao abrir o ficheiro.\n");
    return;
  }

  printf("\n============ LISTA DE COMPONENTES =============\n");

  int id, idFornecedor, idFabricante, idPostoTrabalho;
  char tipo[100], designacao[100], condicao[100], observacao[255];

  while (fscanf(arq, "%d;%d;%d;%d;%99[^;];%99[^;];%99[^;];%254[^\n]\n", &id,
                &idFornecedor, &idFabricante, &idPostoTrabalho, tipo,
                designacao, condicao, observacao) == 8) {
    printf("\nID: %d\nFornecedor: %d\nFabricante: %d\nPosto: %d\nTipo: "
           "%s\nDesignação: %s\nCondição: %s\nObservação: %s\n",
           id, idFornecedor, idFabricante, idPostoTrabalho, tipo, designacao,
           condicao, observacao);
  }
  fclose(arq);
}

void ApagarComponente() {
  printf("\n============ APAGAR COMPONENTE =============\n");
  int optionId;
  printf("Digite o id do componente: ");
  scanf("%d", &optionId);

  FILE *arq = fopen("componentes.txt", "r");
  if (arq == NULL) {
    printf("Erro ao abrir o ficheiro.\n");
    return;
  }

  FILE *temp = fopen("temp.txt", "w");
  if (temp == NULL) {
    printf("Erro ao criar o ficheiro temporário.\n");
    fclose(arq);
    return;
  }

  int id, idFornecedor, idFabricante, idPostoTrabalho;
  char tipo[100], designacao[100], condicao[100], observacao[255];
  int encontrado = 0;

  while (fscanf(arq, "%d;%d;%d;%d;%99[^;];%99[^;];%99[^;];%254[^\n]\n", &id,
                &idFornecedor, &idFabricante, &idPostoTrabalho, tipo,
                designacao, condicao, observacao) == 8) {
    if (id != optionId) {
      fprintf(temp, "%d;%d;%d;%d;%s;%s;%s;%s\n", id, idFornecedor, idFabricante,
              idPostoTrabalho, tipo, designacao, condicao, observacao);
    } else {
      encontrado = 1;
    }
  }

  fclose(arq);
  fclose(temp);

  if (encontrado) {
    remove("componentes.txt");             // Apaga o antigo
    rename("temp.txt", "componentes.txt"); // Renomeia o novo
    printf("Componente com ID %d apagado com sucesso.\n", optionId);
  } else {
    remove("temp.txt"); // Não encontrado, apaga temp
    printf("Componente com ID %d não encontrado.\n", optionId);
  }
}

void AlterarComponente() {
  printf("\n============ ALTERAR COMPONENTE =============\n");
  int optionId;
  printf("Digite o id do componente que deseja alterar: ");
  scanf("%d", &optionId);
  getchar(); // limpar o \n que fica no buffer

  FILE *arq = fopen("componentes.txt", "r");
  if (arq == NULL) {
    printf("Erro ao abrir o ficheiro.\n");
    return;
  }

  FILE *temp = fopen("temp.txt", "w");
  if (temp == NULL) {
    printf("Erro ao criar ficheiro temporário.\n");
    fclose(arq);
    return;
  }

  int id, idFornecedor, idFabricante, idPostoTrabalho;
  char tipo[100], designacao[100], condicao[100], observacao[255];
  int encontrado = 0;

  while (fscanf(arq, "%d;%d;%d;%d;%99[^;];%99[^;];%99[^;];%254[^\n]\n", &id,
                &idFornecedor, &idFabricante, &idPostoTrabalho, tipo,
                designacao, condicao, observacao) == 8) {
    if (id == optionId) {
      Componente novoComponente;

      printf("\nNova designacao: ");
      fgets(novoComponente.designacao, sizeof(novoComponente.designacao),
            stdin);
      strtok(novoComponente.designacao, "\n"); // remove \n

      printf("Novo tipo: ");
      fgets(novoComponente.tipo, sizeof(novoComponente.tipo), stdin);
      strtok(novoComponente.tipo, "\n");

      printf("Nova condicao: ");
      fgets(novoComponente.condicao, sizeof(novoComponente.condicao), stdin);
      strtok(novoComponente.condicao, "\n");

      printf("Nova descrição: ");
      fgets(novoComponente.observacao, sizeof(novoComponente.observacao),
            stdin);
      strtok(novoComponente.observacao, "\n");

      fprintf(temp, "%d;%d;%d;%d;%s;%s;%s;%s\n", id, idFornecedor, idFabricante,
              idPostoTrabalho, novoComponente.tipo, novoComponente.designacao,
              novoComponente.condicao, novoComponente.observacao);
      encontrado = 1;
    } else {
      fprintf(temp, "%d;%d;%d;%d;%s;%s;%s;%s\n", id, idFornecedor, idFabricante,
              idPostoTrabalho, tipo, designacao, condicao, observacao);
    }
  }

  fclose(arq);
  fclose(temp);

  if (encontrado) {
    remove("componentes.txt");
    rename("temp.txt", "componentes.txt");
    printf("Componente com ID %d alterado com sucesso.\n", optionId);
  } else {
    remove("temp.txt");
    printf("Componente com ID %d não encontrado.\n", optionId);
  }
}
void PesquisarComponentes() {
  printf("\n============ PESQUISAR COMPONENTE =============\n");
  char termo[100];
  printf(
      "Digite o designacao, condição ou parte da observação para pesquisar: ");
  getchar(); // limpar \n pendente
  fgets(termo, sizeof(termo), stdin);
  strtok(termo, "\n"); // tirar o \n no final

  FILE *arq = fopen("componentes.txt", "r");
  if (arq == NULL) {
    printf("Erro ao abrir o ficheiro.\n");
    return;
  }

  int id, idFornecedor, idFabricante, idPostoTrabalho;
  char tipo[100], designacao[100], condicao[100], observacao[255];
  int encontrado = 0;

  while (fscanf(arq, "%d;%d;%d;%d;%99[^;];%99[^;];%99[^;];%254[^\n]\n", &id,
                &idFornecedor, &idFabricante, &idPostoTrabalho, tipo,
                designacao, condicao, observacao) == 8) {
    if (strstr(designacao, termo) != NULL || strstr(condicao, termo) != NULL ||
        strstr(observacao, termo) != NULL) {
      printf("\nID: %d\n", id);
      printf("\nID Fabricante: %d\n", idFabricante);
      printf("\nID Fornecerdor: %d\n", idFornecedor);
      printf("\nID Posto de trabalho: %d\n", idPostoTrabalho);
      printf("Nome: %s\n", designacao);
      printf("Tipo: %s\n", tipo);
      printf("Condiçã: %s\n", condicao);
      printf("Observação: %s\n", observacao);
      encontrado = 1;
    }
  }

  fclose(arq);

  if (!encontrado) {
    printf("\nNenhum componente encontrado com o termo \"%s\".\n", termo);
  }
}