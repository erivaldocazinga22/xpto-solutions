#include "../include/structs.h"
#include "../include/utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int obterProximoPostoId() {
  FILE *arq = fopen("posto-de-trabalho.txt", "r");
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

void salvarPosto(PostoTrabalho novoPosto) {
  FILE *arq = fopen("posto-de-trabalho.txt", "a");
  if (arq == NULL) {
    printf("Erro ao abrir o ficheiro.\n");
    return;
  }

  int id = obterProximoPostoId();
  fprintf(arq, "%d;%d;%s;%s;%s;%s\n", id, novoPosto.idFunc, novoPosto.nome,
          novoPosto.local, novoPosto.seccao, novoPosto.descricao);
  fclose(arq);
}

void InserirPostoTrabalho() {
  printf("\n============ INSERIR POSTO DE TRABALHO =============\n");
  PostoTrabalho novoPosto;
  getchar();

  int error = 0;
  do {
    printf("ID do Funcionário: ");
    scanf("%d", &novoPosto.idFunc);
    getchar();

    error = funcionarioAlreadyExists(novoPosto.idFunc);
    if (!error) {
      printf("Id do Forncedor nao existe.\n");
    }
  } while (!error);

  printf("Nome do Posto: ");
  fgets(novoPosto.nome, sizeof(novoPosto.nome), stdin);
  strtok(novoPosto.nome, "\n");

  printf("Local: ");
  fgets(novoPosto.local, sizeof(novoPosto.local), stdin);
  strtok(novoPosto.local, "\n");

  printf("Seccao: ");
  fgets(novoPosto.seccao, sizeof(novoPosto.seccao), stdin);
  strtok(novoPosto.seccao, "\n");

  printf("Descrição: ");
  fgets(novoPosto.descricao, sizeof(novoPosto.descricao), stdin);
  strtok(novoPosto.descricao, "\n");

  salvarPosto(novoPosto);
}

void ListarPostosTrabalho() {
  FILE *arq = fopen("posto-de-trabalho.txt", "r");
  if (arq == NULL) {
    printf("Erro ao abrir o ficheiro.\n");
    return;
  }

  printf("\n============ LISTA DE POSTOS DE TRABALHO =============\n");

  int id, idFunc;
  char nome[100], local[100], seccao[100], descricao[100];

  while (fscanf(arq, "%d;%d;%99[^;];%99[^;];%99[^;];%99[^\n]\n", &id, &idFunc,
                nome, local, seccao, descricao) == 6) {
    printf("\nID: %d\nID Funcionário: %d\nNome: %s\nLocal: %s\nSeccao: "
           "%s\nDescrição: %s\n",
           id, idFunc, nome, local, seccao, descricao);
  }
  fclose(arq);
}

void AlterarPostoTrabalho() {
  printf("\n============ ALTERAR POSTO DE TRABALHO =============\n");
  int optionId;
  printf("Digite o id do posto que deseja alterar: ");
  scanf("%d", &optionId);
  getchar(); // limpar o \n que fica no buffer

  FILE *arq = fopen("posto-de-trabalho.txt", "r");
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

  int id, idFunc;
  char nome[100], local[100], seccao[100], descricao[200];
  int encontrado = 0;

  while (fscanf(arq, "%d;%d;%99[^;];%99[^;]%99[^;];%199[^\n]\n", &id, &idFunc,
                nome, local, seccao, descricao) == 6) {
    if (id == optionId) {
      PostoTrabalho novoPosto;

      printf("\nNovo nome: ");
      fgets(novoPosto.nome, sizeof(novoPosto.nome), stdin);
      strtok(novoPosto.nome, "\n"); // remove \n

      printf("Nova local: ");
      fgets(novoPosto.local, sizeof(novoPosto.local), stdin);
      strtok(novoPosto.local, "\n");

      printf("Nova seccao: ");
      fgets(novoPosto.seccao, sizeof(novoPosto.seccao), stdin);
      strtok(novoPosto.seccao, "\n");

      printf("Nova descrição: ");
      fgets(novoPosto.descricao, sizeof(novoPosto.descricao), stdin);
      strtok(novoPosto.descricao, "\n");

      fprintf(temp, "%d;%d;%s;%s;%s%s\n", id, idFunc, novoPosto.nome,
              novoPosto.local, novoPosto.seccao, novoPosto.descricao);
      encontrado = 1;
    } else {
      fprintf(temp, "%d;%d;%s;%s;%s%s\n", id, idFunc, nome, local, seccao,
              descricao);
    }
  }

  fclose(arq);
  fclose(temp);

  if (encontrado) {
    remove("posto-de-trabalho.txt");
    rename("temp.txt", "posto-de-trabalho.txt");
    printf("Posto de trabalho com ID %d alterado com sucesso.\n", optionId);
  } else {
    remove("temp.txt");
    printf("Posto de trabalho com ID %d não encontrado.\n", optionId);
  }
}

void ApagarPostoTrabalho() {
  printf("\n============ APAGAR POSTO DE TRABALHO =============\n");
  int optionId;
  printf("Digite o id do posto de trabalho: ");
  scanf("%d", &optionId);

  FILE *arq = fopen("posto-de-trabalho.txt", "r");
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

  int id, idFunc;
  char nome[100], local[100], seccao[100], descricao[200];
  int encontrado = 0;

  while (fscanf(arq, "%d;%d;%99[^;];%99[^;]%99[^;];%199[^\n]\n", &id, &idFunc,
                nome, local, seccao, descricao) == 6) {
    if (id != optionId) {
      fprintf(temp, "%d;%d;%s;%s;%s;%s\n", id, idFunc, nome, local, seccao,
              descricao);
    } else {
      encontrado = 1;
    }
  }

  fclose(arq);
  fclose(temp);

  if (encontrado) {
    remove("posto-de-trabalho.txt");             // Apaga o antigo
    rename("temp.txt", "posto-de-trabalho.txt"); // Renomeia o novo
    printf("Posoto de trabalho com ID %d apagado com sucesso.\n", optionId);
  } else {
    remove("temp.txt"); // Não encontrado, apaga temp
    printf("Posoto de trabalho com ID %d não encontrado.\n", optionId);
  }
}

void PesquisarPostoTrabalho() {
  printf("\n============ PESQUISAR POSTO DE TRABALHO =============\n");
  char termo[100];
  printf("Digite o nome, local ou parte da descrição para pesquisar: ");
  getchar(); // limpar \n pendente
  fgets(termo, sizeof(termo), stdin);
  strtok(termo, "\n"); // tirar o \n no final

  FILE *arq = fopen("posto-de-trabalho.txt", "r");
  if (arq == NULL) {
    printf("Erro ao abrir o ficheiro.\n");
    return;
  }

  int id, idFunc;
  char nome[100], local[100], seccao[100], descricao[200];
  int encontrado = 0;

  while (fscanf(arq, "%d;%d;%99[^;];%99[^;]%99[^;];%199[^\n]\n", &id, &idFunc,
                nome, local, seccao, descricao) == 6) {
    if (strstr(nome, termo) != NULL || strstr(local, termo) != NULL ||
        strstr(descricao, termo) != NULL) {
      printf("\nID: %d\n", id);
      printf("Nome: %s\n", nome);
      printf("Local: %s\n", local);
      printf("Descrição: %s\n", descricao);
      encontrado = 1;
    }
  }

  fclose(arq);

  if (!encontrado) {
    printf("\nNenhum posto de trabalho encontrado com o termo \"%s\".\n",
           termo);
  }
}