#include "../include/structs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int obterProximoFuncId() {
  FILE *arq = fopen("funcionario.txt", "r");
  if (arq == NULL) {
    return 1;
  }

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

void salvarFuncionario(Funcionario novoFuncionario) {
  FILE *arq = fopen("funcionario.txt", "a");
  if (arq == NULL) {
    printf("Erro ao abrir o ficheiro.\n");
    return;
  }

  int id = obterProximoFuncId();
  fprintf(arq, "%d;%s;%s;%s\n", id, novoFuncionario.nome,
          novoFuncionario.funcao, novoFuncionario.descricao);

  fclose(arq);
}

void InserirFuncionario() {
  printf("\n============ INSERIR FUNCIONARIO =============\n");
  Funcionario novoFuncionario;

  getchar();

  printf("Nome: ");
  fgets(novoFuncionario.nome, sizeof(novoFuncionario.nome), stdin);
  strtok(novoFuncionario.nome, "\n");

  printf("Funcao: ");
  fgets(novoFuncionario.funcao, sizeof(novoFuncionario.funcao), stdin);
  strtok(novoFuncionario.funcao, "\n");

  printf("Descricao: ");
  fgets(novoFuncionario.descricao, sizeof(novoFuncionario.descricao), stdin);
  strtok(novoFuncionario.descricao, "\n");

  salvarFuncionario(novoFuncionario);
}

void ListarFuncionarios() {
  FILE *arq = fopen("funcionario.txt", "r");
  if (arq == NULL) {
    printf("Erro ao abrir o ficheiro.\n");
    return;
  }

  printf("\n============ LISTA DE FUNCIONÁRIOS =============\n");

  int id;
  char nome[100], funcao[100], descricao[200];

  while (fscanf(arq, "%d;%99[^;];%99[^;];%199[^\n]\n", &id, nome, funcao,
                descricao) == 4) {
    printf("\nID: %d\n", id);
    printf("Nome: %s\n", nome);
    printf("Função: %s\n", funcao);
    printf("Descrição: %s\n", descricao);
  }

  fclose(arq);
}

void ApagarFuncionario() {
  printf("\n============ APAGAR FUNCIONÁRIO =============\n");
  int optionId;
  printf("Digite o id do funcionario: ");
  scanf("%d", &optionId);

  FILE *arq = fopen("funcionario.txt", "r");
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

  int id;
  char nome[100], funcao[100], descricao[200];
  int encontrado = 0;

  while (fscanf(arq, "%d;%99[^;];%99[^;];%199[^\n]\n", &id, nome, funcao,
                descricao) == 4) {
    if (id != optionId) {
      fprintf(temp, "%d;%s;%s;%s\n", id, nome, funcao, descricao);
    } else {
      encontrado = 1;
    }
  }

  fclose(arq);
  fclose(temp);

  if (encontrado) {
    remove("funcionario.txt");             // Apaga o antigo
    rename("temp.txt", "funcionario.txt"); // Renomeia o novo
    printf("Funcionário com ID %d apagado com sucesso.\n", optionId);
  } else {
    remove("temp.txt"); // Não encontrado, apaga temp
    printf("Funcionário com ID %d não encontrado.\n", optionId);
  }
}

void AlterarFuncionario() {
  printf("\n============ ALTERAR FUNCIONÁRIO =============\n");
  int optionId;
  printf("Digite o id do funcionário que deseja alterar: ");
  scanf("%d", &optionId);
  getchar(); // limpar o \n que fica no buffer

  FILE *arq = fopen("funcionario.txt", "r");
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

  int id;
  char nome[100], funcao[100], descricao[200];
  int encontrado = 0;

  while (fscanf(arq, "%d;%99[^;];%99[^;];%199[^\n]\n", &id, nome, funcao,
                descricao) == 4) {
    if (id == optionId) {
      Funcionario novoFuncionario;

      printf("\nNovo nome: ");
      fgets(novoFuncionario.nome, sizeof(novoFuncionario.nome), stdin);
      strtok(novoFuncionario.nome, "\n"); // remove \n

      printf("Nova função: ");
      fgets(novoFuncionario.funcao, sizeof(novoFuncionario.funcao), stdin);
      strtok(novoFuncionario.funcao, "\n");

      printf("Nova descrição: ");
      fgets(novoFuncionario.descricao, sizeof(novoFuncionario.descricao),
            stdin);
      strtok(novoFuncionario.descricao, "\n");

      fprintf(temp, "%d;%s;%s;%s\n", id, novoFuncionario.nome,
              novoFuncionario.funcao, novoFuncionario.descricao);
      encontrado = 1;
    } else {
      fprintf(temp, "%d;%s;%s;%s\n", id, nome, funcao, descricao);
    }
  }

  fclose(arq);
  fclose(temp);

  if (encontrado) {
    remove("funcionario.txt");
    rename("temp.txt", "funcionario.txt");
    printf("Funcionário com ID %d alterado com sucesso.\n", optionId);
  } else {
    remove("temp.txt");
    printf("Funcionário com ID %d não encontrado.\n", optionId);
  }
}

void PesquisarFuncionario() {
  printf("\n============ PESQUISAR FUNCIONÁRIO =============\n");
  char termo[100];
  printf("Digite o nome, função ou parte da descrição para pesquisar: ");
  getchar(); // limpar \n pendente
  fgets(termo, sizeof(termo), stdin);
  strtok(termo, "\n"); // tirar o \n no final

  FILE *arq = fopen("funcionario.txt", "r");
  if (arq == NULL) {
    printf("Erro ao abrir o ficheiro.\n");
    return;
  }

  int id;
  char nome[100], funcao[100], descricao[200];
  int encontrado = 0;

  while (fscanf(arq, "%d;%99[^;];%99[^;];%199[^\n]\n", &id, nome, funcao,
                descricao) == 4) {
    if (strstr(nome, termo) != NULL || strstr(funcao, termo) != NULL ||
        strstr(descricao, termo) != NULL) {
      printf("\nID: %d\n", id);
      printf("Nome: %s\n", nome);
      printf("Função: %s\n", funcao);
      printf("Descrição: %s\n", descricao);
      encontrado = 1;
    }
  }

  fclose(arq);

  if (!encontrado) {
    printf("\nNenhum funcionário encontrado com o termo \"%s\".\n", termo);
  }
}
