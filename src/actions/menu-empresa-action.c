#include "../include/structs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int obterProximoEmpresaId() {
  FILE *arq = fopen("empresas.txt", "r");
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

void salvarEmpresa(Empresa novaEmpresa) {
  FILE *arq = fopen("empresas.txt", "a");
  if (arq == NULL) {
    printf("Erro ao abrir o ficheiro.\n");
    return;
  }

  int id = obterProximoEmpresaId();
  fprintf(arq, "%d;%s;%s;%s\n", id, novaEmpresa.nome, novaEmpresa.contacto,
          novaEmpresa.tipo);

  fclose(arq);
}

void InserirEmpresa() {
  printf("\n============ INSERIR EMPRESA =============\n");
  Empresa novaEmpresa;

  getchar();

  printf("Nome: ");
  fgets(novaEmpresa.nome, sizeof(novaEmpresa.nome), stdin);
  strtok(novaEmpresa.nome, "\n");

  printf("Tipo: ");
  fgets(novaEmpresa.tipo, sizeof(novaEmpresa.tipo), stdin);
  strtok(novaEmpresa.tipo, "\n");

  printf("Contancto: ");
  fgets(novaEmpresa.contacto, sizeof(novaEmpresa.contacto), stdin);
  strtok(novaEmpresa.contacto, "\n");

  salvarEmpresa(novaEmpresa);
}

void ListarEmpresas() {
  FILE *arq = fopen("empresas.txt", "r");
  if (arq == NULL) {
    printf("Erro ao abrir o ficheiro.\n");
    return;
  }

  printf("\n============ LISTA DE EMPRESAS =============\n");

  int id;
  char nome[100], tipo[100], contacto[200];

  while (fscanf(arq, "%d;%99[^;];%99[^;];%199[^\n]\n", &id, nome, tipo,
                contacto) == 4) {
    printf("\nID: %d\n", id);
    printf("Nome: %s\n", nome);
    printf("Tipo: %s\n", tipo);
    printf("Contacto: %s\n", contacto);
  }

  fclose(arq);
}

void ApagarEmpresa() {
  printf("\n============ APAGAR EMPRESA =============\n");
  int optionId;
  printf("Digite o id da Empresa: ");
  scanf("%d", &optionId);

  FILE *arq = fopen("empresas.txt", "r");
  if (arq == NULL) {
    printf("Erro ao abrir o ficheiro de empresas.\n");
    return;
  }

  FILE *temp = fopen("temp.txt", "w");
  if (temp == NULL) {
    printf("Erro ao criar o ficheiro temporário.\n");
    fclose(arq);
    return;
  }

  int id;
  char nome[100], tipo[100], contacto[200];
  int encontrado = 0;

  // Lê todas as empresas, e escreve no ficheiro temporário apenas as que não
  // forem a ser apagadas
  while (fscanf(arq, "%d;%99[^;];%99[^;];%199[^\n]\n", &id, nome, tipo,
                contacto) == 4) {
    if (id != optionId) {
      fprintf(temp, "%d;%s;%s;%s\n", id, nome, tipo, contacto);
    } else {
      encontrado = 1;
    }
  }

  fclose(arq);
  fclose(temp);

  if (!encontrado) {
    remove("temp.txt");
    printf("Empresa com ID %d não encontrada.\n", optionId);
    return;
  }

  // Substituir ficheiro de empresas
  remove("empresas.txt");
  rename("temp.txt", "empresas.txt");
  printf("Empresa com ID %d apagada com sucesso.\n", optionId);

  // Agora apagar referências a essa empresa em operacoes.txt
  FILE *operacoes = fopen("operacoes.txt", "r");
  FILE *tempOperacoes = fopen("temp_operacoes.txt", "w");
  if (operacoes && tempOperacoes) {
    int idOperacao, idFuncionario, idEmpresa;
    char tipoOperacao[100], descricao[200];

    while (fscanf(operacoes, "%d;%d;%d;%99[^;];%199[^\n]\n", &idOperacao,
                  &idFuncionario, &idEmpresa, tipoOperacao, descricao) == 5) {
      if (idEmpresa != optionId) {
        fprintf(tempOperacoes, "%d;%d;%d;%s;%s\n", idOperacao, idFuncionario,
                idEmpresa, tipoOperacao, descricao);
      }
    }

    fclose(operacoes);
    fclose(tempOperacoes);
    remove("operacoes.txt");
    rename("temp_operacoes.txt", "operacoes.txt");
    printf("Operações associadas à empresa foram removidas.\n");
  } else {
    if (operacoes)
      fclose(operacoes);
    if (tempOperacoes)
      fclose(tempOperacoes);
    printf("Aviso: Não foi possível atualizar o ficheiro de operações.\n");
  }
}

void AlterarEmpresa() {
  printf("\n============ ALTERAR EMPRESA =============\n");
  int optionId;
  printf("Digite o id da empresa que deseja alterar: ");
  scanf("%d", &optionId);
  getchar(); // limpar o \n que fica no buffer

  FILE *arq = fopen("empresas.txt", "r");
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
  char nome[100], tipo[100], contacto[200];
  int encontrado = 0;

  while (fscanf(arq, "%d;%99[^;];%99[^;];%199[^\n]\n", &id, nome, tipo,
                contacto) == 4) {
    if (id == optionId) {
      Empresa novoEmpresa;

      printf("\nNovo nome: ");
      fgets(novoEmpresa.nome, sizeof(novoEmpresa.nome), stdin);
      strtok(novoEmpresa.nome, "\n"); // remove \n

      printf("Novo Tipo: ");
      fgets(novoEmpresa.tipo, sizeof(novoEmpresa.tipo), stdin);
      strtok(novoEmpresa.tipo, "\n");

      printf("Novo contacto: ");
      fgets(novoEmpresa.contacto, sizeof(novoEmpresa.contacto), stdin);
      strtok(novoEmpresa.contacto, "\n");

      fprintf(temp, "%d;%s;%s;%s\n", id, novoEmpresa.nome, novoEmpresa.tipo,
              novoEmpresa.contacto);
      encontrado = 1;
    } else {
      fprintf(temp, "%d;%s;%s;%s\n", id, nome, tipo, contacto);
    }
  }

  fclose(arq);
  fclose(temp);

  if (encontrado) {
    remove("empresas.txt");
    rename("temp.txt", "empresas.txt");
    printf("Funcionário com ID %d alterado com sucesso.\n", optionId);
  } else {
    remove("temp.txt");
    printf("Funcionário com ID %d não encontrado.\n", optionId);
  }
}

void PesquisarEmpresa() {
  printf("\n============ PESQUISAR EMPRESA =============\n");
  char termo[100];
  printf("Digite o nome, tipo ou parte do contacto para pesquisar: ");
  getchar(); // limpar \n pendente
  fgets(termo, sizeof(termo), stdin);
  strtok(termo, "\n"); // tirar o \n no final

  FILE *arq = fopen("empresas.txt", "r");
  if (arq == NULL) {
    printf("Erro ao abrir o ficheiro.\n");
    return;
  }

  int id;
  char nome[100], tipo[100], contacto[200];
  int encontrado = 0;

  while (fscanf(arq, "%d;%99[^;];%99[^;];%199[^\n]\n", &id, nome, tipo,
                contacto) == 4) {
    if (strstr(nome, termo) != NULL || strstr(tipo, termo) != NULL ||
        strstr(contacto, termo) != NULL) {
      printf("\nID: %d\n", id);
      printf("Nome: %s\n", nome);
      printf("Tipo: %s\n", tipo);
      printf("Contacto: %s\n", contacto);
      encontrado = 1;
    }
  }

  fclose(arq);

  if (!encontrado) {
    printf("\nNenhuma empresa encontrado com o termo \"%s\".\n", termo);
  }
}
