#include "../include/structs.h"
#include "../include/utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILE_NAME "./data/posto-de-trabalho.txt"
#define FILE_NAME_TEMP "./data/temp.txt"

int obterProximoPostoId() {
  FILE *arq = fopen(FILE_NAME, "r");
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
  // Verificar se a pasta "data" existe, se não deve criar
  folderDataAlreadyExists();

  FILE *arq = fopen(FILE_NAME, "a");
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
  FILE *arq = fopen(FILE_NAME, "r");
  if (arq == NULL) {
    printf("Erro ao abrir o ficheiro.\n");
    return;
  }

  printf("\n============ LISTA DE POSTOS DE TRABALHO =============\n");

  int id, idFunc;
  char nome[100], local[100], seccao[100], descricao[100];
  int encontrou = 0;
  while (fscanf(arq, "%d;%d;%99[^;];%99[^;];%99[^;];%99[^\n]\n", &id, &idFunc,
                nome, local, seccao, descricao) == 6) {
    printf("\nID: %d\nID Funcionário: %d\nNome: %s\nLocal: %s\nSeccao: "
           "%s\nDescrição: %s\n",
           id, idFunc, nome, local, seccao, descricao);
    encontrou = 1;
  }

  if (!encontrou) {
    printf("\nNenhum posto de trabalho encontrado.\n");
  }

  fclose(arq);
}

void AlterarPostoTrabalho() {
  printf("\n============ ALTERAR POSTO DE TRABALHO =============\n");
  int optionId;
  printf("Digite o ID do posto que deseja alterar: ");
  scanf("%d", &optionId);
  getchar(); // limpar o \n

  FILE *arq = fopen(FILE_NAME, "r");
  if (arq == NULL) {
    printf("Erro ao abrir o ficheiro.\n");
    return;
  }

  FILE *temp = fopen(FILE_NAME_TEMP, "w");
  if (temp == NULL) {
    printf("Erro ao criar o ficheiro temporário.\n");
    fclose(arq);
    return;
  }

  int id, idFunc;
  char nome[100], local[100], seccao[100], descricao[200];
  int encontrado = 0;

  while (fscanf(arq, "%d;%d;%99[^;];%99[^;];%99[^;];%199[^\n]\n", &id, &idFunc,
                nome, local, seccao, descricao) == 6) {
    if (id == optionId) {
      PostoTrabalho novoPosto;
      strcpy(novoPosto.nome, nome);
      strcpy(novoPosto.local, local);
      strcpy(novoPosto.seccao, seccao);
      strcpy(novoPosto.descricao, descricao);
      novoPosto.id = id;
      novoPosto.idFunc = idFunc;

      int optionSelected;
      do {
        printf("\n--- Menu de Actualização de Posto de Trabalho ---\n");
        printf("1. Alterar nome\n");
        printf("2. Alterar local\n");
        printf("3. Alterar secção\n");
        printf("4. Alterar descrição\n");
        printf("5. Alterar todos os campos\n");
        printf("0. Guardar alterações e sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &optionSelected);
        getchar(); // limpar \n

        switch (optionSelected) {
        case 1:
          printf("Novo nome: ");
          fgets(novoPosto.nome, sizeof(novoPosto.nome), stdin);
          strtok(novoPosto.nome, "\n");
          break;
        case 2:
          printf("Novo local: ");
          fgets(novoPosto.local, sizeof(novoPosto.local), stdin);
          strtok(novoPosto.local, "\n");
          break;
        case 3:
          printf("Nova secção: ");
          fgets(novoPosto.seccao, sizeof(novoPosto.seccao), stdin);
          strtok(novoPosto.seccao, "\n");
          break;
        case 4:
          printf("Nova descrição: ");
          fgets(novoPosto.descricao, sizeof(novoPosto.descricao), stdin);
          strtok(novoPosto.descricao, "\n");
          break;
        case 5:
          printf("Novo nome: ");
          fgets(novoPosto.nome, sizeof(novoPosto.nome), stdin);
          strtok(novoPosto.nome, "\n");

          printf("Novo local: ");
          fgets(novoPosto.local, sizeof(novoPosto.local), stdin);
          strtok(novoPosto.local, "\n");

          printf("Nova secção: ");
          fgets(novoPosto.seccao, sizeof(novoPosto.seccao), stdin);
          strtok(novoPosto.seccao, "\n");

          printf("Nova descrição: ");
          fgets(novoPosto.descricao, sizeof(novoPosto.descricao), stdin);
          strtok(novoPosto.descricao, "\n");
          break;
        case 0:
          printf("A guardar alterações...\n");
          break;
        default:
          printf("Opção inválida! Tente novamente.\n");
        }
      } while (optionSelected != 0);

      fprintf(temp, "%d;%d;%s;%s;%s;%s\n", id, idFunc, novoPosto.nome,
              novoPosto.local, novoPosto.seccao, novoPosto.descricao);
      encontrado = 1;
    } else {
      fprintf(temp, "%d;%d;%s;%s;%s;%s\n", id, idFunc, nome, local, seccao,
              descricao);
    }
  }

  fclose(arq);
  fclose(temp);

  if (encontrado) {
    remove(FILE_NAME);
    rename(FILE_NAME_TEMP, FILE_NAME);
    printf("Posto de trabalho com ID %d alterado com sucesso.\n", optionId);
  } else {
    remove(FILE_NAME_TEMP);
    printf("Posto de trabalho com ID %d não encontrado.\n", optionId);
  }
}

void ApagarPostoTrabalho() {
  printf("\n============ APAGAR POSTO DE TRABALHO =============\n");
  int optionId;
  printf("Digite o ID do posto de trabalho: ");
  scanf("%d", &optionId);

  FILE *arq = fopen(FILE_NAME, "r");
  if (arq == NULL) {
    printf("Erro ao abrir o ficheiro.\n");
    return;
  }

  FILE *temp = fopen(FILE_NAME_TEMP, "w");
  if (temp == NULL) {
    printf("Erro ao criar o ficheiro temporário.\n");
    fclose(arq);
    return;
  }

  int id, idFunc;
  char nome[100], local[100], seccao[100], descricao[200];
  int encontrado = 0;

  while (fscanf(arq, "%d;%d;%99[^;];%99[^;];%99[^;];%199[^\n]\n", &id, &idFunc,
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
    remove(FILE_NAME);
    rename(FILE_NAME_TEMP, FILE_NAME);
    printf("Posto de trabalho com ID %d apagado com sucesso.\n", optionId);
  } else {
    remove(FILE_NAME_TEMP);
    printf("Posto de trabalho com ID %d não encontrado.\n", optionId);
  }
}

/* void PesquisarPostoTrabalho() {
  printf("\n============ PESQUISAR POSTO DE TRABALHO =============\n");
  char termo[100];
  printf("Digite o nome, local ou parte da descrição para pesquisar: ");
  getchar(); // limpar \n pendente
  fgets(termo, sizeof(termo), stdin);
  strtok(termo, "\n"); // tirar o \n no final

  FILE *arq = fopen(FILE_NAME, "r");
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
} */

void PesquisarPostoTrabalho() {
  printf("\n============ PESQUISAR POSTO DE TRABALHO =============\n");
  printf("1. Pesquisar por ID\n");
  printf("2. Pesquisar por Nome (com * e ?)\n");
  printf("0. Voltar\n");

  int opcao;
  printf("\nEscolha uma opção: ");
  scanf("%d", &opcao);

  FILE *arq = fopen(FILE_NAME, "r");
  if (arq == NULL) {
    printf("Erro ao abrir o ficheiro.\n");
    return;
  }

  if (opcao == 1) {
    int optionId;
    printf("Digite o ID do posto de trabalho: ");
    scanf("%d", &optionId);

    int id, idFunc;
    char nome[100], local[100], seccao[100], descricao[100];
    int encontrado = 0;
    while (fscanf(arq, "%d;%d;%99[^;];%99[^;];%99[^;];%99[^\n]\n", &id, &idFunc,
                  nome, local, seccao, descricao) == 6) {
      printf("\nID: %d\nID Posto: %d\nNome: %s\nLocal: %s\nSeccao: "
             "%s\nDescrição: %s\n",
             id, idFunc, nome, local, seccao, descricao);
      encontrado = 1;
    }

    if (!encontrado) {
      printf("Posto com ID %d não encontrado.\n", optionId);
    }
  }

  else if (opcao == 2) {
    char padrao[100];
    char padraoLower[100];
    printf("Digite o padrão (use * e ?): ");
    scanf(" %[^\n]", padrao);
    strToLower(padraoLower, padrao);

    int id, idFunc, encontrados = 0;
    char nome[100], local[100], seccao[100], descricao[100];
    char nomeLower[100];

    printf("\n--- Postos de Trabalho Encontrados ---\n");
    while (fscanf(arq, "%d;%d;%99[^;];%99[^;];%99[^;];%99[^\n]\n", &id, &idFunc,
                  nome, local, seccao, descricao) == 6) {

      strToLower(nomeLower, nome);

      int corresponde = 0;

      if (opcao == 2) {
        corresponde = matchWildcard(padraoLower, nomeLower);
      } else if (opcao == 3) {
        corresponde = matchWildcard(padraoLower, nomeLower);
      }

      if (corresponde) {
        printf("\nID: %d\nID Funcionário: %d\nNome: %s\nLocal: %s\nSeccao: "
               "%s\nDescrição: %s\n",
               id, idFunc, nome, local, seccao, descricao);
        encontrados++;
      }
    }

    if (!encontrados) {
      printf("Nenhum posto de trabalho correspondente ao padrão.\n");
    }
  }

  else if (opcao == 0) {
    printf("A voltar ao Menu Principal...\n");
  }

  else {
    printf("Opção inválida! Tente novamente.\n");
  }

  fclose(arq);
}