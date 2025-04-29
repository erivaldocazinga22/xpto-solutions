#include "../include//utils.h"
#include "../include/menu.h"
#include "../include/structs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int obterProximaOperaId() {
  FILE *arq = fopen("opercoes.txt", "r");
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

void salvarOperacao(Operacao novaOpercao) {
  FILE *arq = fopen("opercoes.txt", "a");
  if (arq == NULL) {
    printf("Erro ao abrir o ficheiro.\n");
    return;
  }

  int numDoc = obterProximaOperaId();
  int numDocExterno = numDoc * 1000;
  fprintf(arq, "%d;%d;%d;%d;%d;%d;%f;%s;%d;%s;%s;%s;%s;\n", numDoc,
          numDocExterno, novaOpercao.idFunc, novaOpercao.idEmpresa,
          novaOpercao.idPosto, novaOpercao.idComponente, novaOpercao.montante,
          novaOpercao.observacoa, novaOpercao.tipoOperacao,
          novaOpercao.dataOperacao, novaOpercao.dataEntrada,
          novaOpercao.dataSaida, novaOpercao.dataPreviaChegada);

  fclose(arq);
}

void InserirOperacao() {
  printf("\n============ INSERIR OPERCACAO =============\n");
  Operacao novaOperacao;
  int error = 0;

  getchar();

  do {
    printf("ID Operacoe: ");
    scanf("%d", &novaOperacao.idFunc);
    getchar();

    error = operacaoAlreadyExists(novaOperacao.idFunc);
    if (!error) {
      printf("Id do Operacoe nao existe.\n");
    }
  } while (!error);

  error = 0;
  do {
    printf("ID Empresa: ");
    scanf("%d", &novaOperacao.idEmpresa);
    getchar();

    error = operacaoAlreadyExists(novaOperacao.idEmpresa);
    if (!error) {
      printf("Id do Empresa nao existe.\n");
    }
  } while (!error);

  error = 0;
  do {
    printf("ID Posto: ");
    scanf("%d", &novaOperacao.idPosto);
    getchar();

    error = operacaoAlreadyExists(novaOperacao.idPosto);
    if (!error) {
      printf("Id do Posto nao existe.\n");
    }
  } while (!error);

  error = 0;
  do {
    printf("ID Componente: ");
    scanf("%d", &novaOperacao.idComponente);
    getchar();

    error = operacaoAlreadyExists(novaOperacao.idComponente);
    if (!error) {
      printf("Id do Componente nao existe.\n");
    }
  } while (!error);

  printf("Montante da Opercao: ");
  scanf("%f", &novaOperacao.montante);

  printf("Tipo de Opercao: ");
  scanf("%d", &novaOperacao.tipoOperacao);

  printf("Observacao: ");
  fgets(novaOperacao.observacoa, sizeof(novaOperacao.observacoa), stdin);
  strtok(novaOperacao.observacoa, "\n");

  printf("Data da Opercao(agora): ");
  fgets(novaOperacao.dataOperacao, sizeof(novaOperacao.dataOperacao), stdin);
  strtok(novaOperacao.dataOperacao, "\n");

  printf("Data Previa de chegada(agora): ");
  fgets(novaOperacao.dataPreviaChegada, sizeof(novaOperacao.dataPreviaChegada),
        stdin);
  strtok(novaOperacao.dataPreviaChegada, "\n");

  salvarOperacao(novaOperacao);
}

void ListarOperacoes() {
  FILE *arq = fopen("opercoes.txt", "r");
  if (arq == NULL) {
    printf("Erro ao abrir o ficheiro.\n");
    return;
  }

  printf("\n============ LISTA DE OPERCOES =============\n");

  int numDoc, numDocExterno, idFunc, idEmpresa, idPosto, idComponente,
      tipoOperacao;
  float montante;
  char observacoa[100], dataOperacao[10], dataEntrada[10], dataSaida[10],
      dataPreviaChegada[10];

  while (fscanf(arq, "%d;%d;%d;%d;%d;%d;%f;%s;%d;%s;%s;%s;%s;\n", &numDoc,
                &numDocExterno, &idFunc, &idEmpresa, &idPosto, &idComponente,
                &montante, observacoa, &tipoOperacao, dataOperacao, dataEntrada,
                dataSaida, dataPreviaChegada) == 13) {
    printf("\nNumero Documento: %d\n", numDoc);
    printf("\nNumero Documento Externo: %d\n", numDocExterno);
    printf("\nID do Funcionario: %d\n", idFunc);
    printf("\nID da Empresa: %d\n", idEmpresa);
    printf("\nID da Posto: %d\n", idEmpresa);
    printf("\nID da Componente: %d\n", idEmpresa);
    printf("Montante da operacao: %f\n", montante);
    printf("Função: %s\n", observacoa);
    printf("Descrição: %d\n", tipoOperacao);
    printf("Descrição: %s\n", dataOperacao);
    printf("Descrição: %s\n", dataEntrada);
    printf("Descrição: %s\n", dataSaida);
    printf("Descrição: %s\n", dataPreviaChegada);
  }

  fclose(arq);
}

void ApagarOperacao() {
  printf("\n============ APAGAR OPERACOES =============\n");
  int optionId;
  printf("Digite o id da opercao: ");
  scanf("%d", &optionId);

  FILE *arq = fopen("operacoes.txt", "r");
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

  int numDoc, numDocExterno, idFunc, idEmpresa, idPosto, idComponente,
      tipoOperacao;
  float montante;
  char observacoa[100], dataOperacao[10], dataEntrada[10], dataSaida[10],
      dataPreviaChegada[10];
  int encontrado = 0;

  while (fscanf(arq, "%d;%d;%d;%d;%d;%d;%f;%s;%d;%s;%s;%s;%s;\n", &numDoc,
                &numDocExterno, &idFunc, &idEmpresa, &idPosto, &idComponente,
                &montante, observacoa, &tipoOperacao, dataOperacao, dataEntrada,
                dataSaida, dataPreviaChegada) == 13) {
    if (numDoc != optionId || numDocExterno != optionId) {
      fprintf(temp, "%d;%d;%d;%d;%d;%d;%f;%s;%d;%s;%s;%s;%s;\n", numDoc,
              numDocExterno, idFunc, idEmpresa, idPosto, idComponente, montante,
              observacoa, tipoOperacao, dataOperacao, dataEntrada, dataSaida,
              dataPreviaChegada);
    } else {
      encontrado = 1;
    }
  }

  fclose(arq);
  fclose(temp);

  if (encontrado) {
    remove("operacoes.txt");             // Apaga o antigo
    rename("temp.txt", "operacoes.txt"); // Renomeia o novo
    printf("Funcionário com ID %d apagado com sucesso.\n", optionId);
  } else {
    remove("temp.txt"); // Não encontrado, apaga temp
    printf("Funcionário com ID %d não encontrado.\n", optionId);
  }
}

void AlterarOperacao() {
  printf("\n============ ALTERAR OPERACAO =============\n");
  int optionId;
  printf("Digite o id da operacao que deseja alterar: ");
  scanf("%d", &optionId);
  getchar(); // limpar o \n que fica no buffer

  FILE *arq = fopen("operacoes.txt", "r");
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

  int numDoc, numDocExterno, idFunc, idEmpresa, idPosto, idComponente,
      tipoOperacao;
  float montante;
  char observacoa[100], dataOperacao[10], dataEntrada[10], dataSaida[10],
      dataPreviaChegada[10];
  int encontrado = 0;

  while (fscanf(arq, "%d;%d;%d;%d;%d;%d;%f;%s;%d;%s;%s;%s;%s;\n", &numDoc,
                &numDocExterno, &idFunc, &idEmpresa, &idPosto, &idComponente,
                &montante, observacoa, &tipoOperacao, dataOperacao, dataEntrada,
                dataSaida, dataPreviaChegada) == 13) {
    if (numDoc == optionId) {
      Operacao novaOperacao;

      printf("Montante da Opercao: ");
      scanf("%f", &novaOperacao.montante);

      printf("Tipo de Opercao: ");
      scanf("%d", &novaOperacao.tipoOperacao);

      printf("Observacao: ");
      fgets(novaOperacao.observacoa, sizeof(novaOperacao.observacoa), stdin);
      strtok(novaOperacao.observacoa, "\n");

      fprintf(arq, "%d;%d;%d;%d;%d;%d;%f;%s;%d;%s;%s;%s;%s;\n", numDoc,
              numDocExterno, idFunc, idEmpresa, idPosto, idComponente, montante,
              novaOperacao.observacoa, novaOperacao.tipoOperacao, dataOperacao,
              novaOperacao.dataEntrada, novaOperacao.dataSaida,
              novaOperacao.dataPreviaChegada);
      encontrado = 1;
    } else {
      fprintf(temp, "%d;%d;%d;%d;%d;%d;%f;%s;%d;%s;%s;%s;%s;\n", numDoc,
              numDocExterno, idFunc, idEmpresa, idPosto, idComponente, montante,
              observacoa, tipoOperacao, dataOperacao, dataEntrada, dataSaida,
              dataPreviaChegada);
    }
  }

  fclose(arq);
  fclose(temp);

  if (encontrado) {
    remove("operacoes.txt");
    rename("temp.txt", "operacoes.txt");
    printf("Funcionário com ID %d alterado com sucesso.\n", optionId);
  } else {
    remove("temp.txt");
    printf("Funcionário com ID %d não encontrado.\n", optionId);
  }
}

void PesquisarOperacao() {
  printf("\n============ PESQUISAR OPERACAO =============\n");
  char termo[100];
  printf("Digite o nome, função ou parte da descrição para pesquisar: ");
  getchar(); // limpar \n pendente
  fgets(termo, sizeof(termo), stdin);
  strtok(termo, "\n"); // tirar o \n no final

  FILE *arq = fopen("operacoes.txt", "r");
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