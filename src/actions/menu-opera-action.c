#include "../include//utils.h"
#include "../include/menu.h"
#include "../include/structs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> // para lidar com a data de aquisição automatica

#define FILE_NAME "./data/operacoes.txt"
#define FILE_NAME_TEMP "./data/temp.txt"

int obterProximaOperaId() {
  FILE *arq = fopen(FILE_NAME, "r");
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

void salvarOperacao(Operacao novaOperacao) {
  // veridicar se a pasta "data" existe, se não cria
  folderDataAlreadyExists();

  FILE *arq = fopen(FILE_NAME, "a");
  if (arq == NULL) {
    printf("Erro ao abrir o ficheiro.\n");
    return;
  }

  int numDoc = obterProximaOperaId();
  int numDocExterno = numDoc * 1024;
  fprintf(arq, "%d;%d;%d;%d;%d;%d;%f;%s;%s;%s;%s;%s;%s;\n", numDoc,
          numDocExterno, novaOperacao.idFunc, novaOperacao.idEmpresa,
          novaOperacao.idPosto, novaOperacao.idComponente,
          novaOperacao.montante, novaOperacao.observacao,
          novaOperacao.tipoOperacao, novaOperacao.dataOperacao,
          novaOperacao.dataEntrada, novaOperacao.dataSaida,
          novaOperacao.dataPreviaChegada);

  fclose(arq);
}

void InserirOperacao() {
  printf("\n============ INSERIR OPERACAO =============\n");
  Operacao novaOperacao;

  getchar();
  printf("Tipo de Operação: ");
  fgets(novaOperacao.tipoOperacao, sizeof(novaOperacao.tipoOperacao), stdin);
  strtok(novaOperacao.tipoOperacao, "\n");

  do {
    printf("Montante da Operação: ");
    scanf("%f", &novaOperacao.montante);
  } while (novaOperacao.montante < 0);

  getchar(); // limpar buffer do scanf
  printf("Observação: ");
  fgets(novaOperacao.observacao, sizeof(novaOperacao.observacao), stdin);
  strtok(novaOperacao.observacao, "\n");

  int error = 0;
  do {
    printf("ID do Funcionário: ");
    scanf("%d", &novaOperacao.idFunc);
    getchar();

    error = funcionarioAlreadyExists(novaOperacao.idFunc);
    if (!error) {
      printf("Id do Forncedor nao existe.\n");
    }
  } while (!error);

  error = 0;
  do {
    printf("ID da empresa: ");
    scanf("%d", &novaOperacao.idEmpresa);
    getchar();

    error = empresaAlreadyExists(novaOperacao.idEmpresa);
    if (!error) {
      printf("Id da empresa não existe.\n");
    }
  } while (!error);

  error = 0;
  do {
    printf("ID do posto de trabalho: ");
    scanf("%d", &novaOperacao.idPosto);
    getchar();

    error = postoAlreadyExists(novaOperacao.idPosto);
    if (!error) {
      printf("Id do posto de trabalho não existe.\n");
    }
  } while (!error);

  error = 0;
  do {
    printf("ID do componente: ");
    scanf("%d", &novaOperacao.idComponente);
    getchar();

    error = componenteAlreadyExists(novaOperacao.idComponente);
    if (!error) {
      printf("Id do posto de trabalho não existe.\n");
    }
  } while (!error);

  int qDiasDataPrev;
  do {
    printf("Quantidade de dias para a data prevista de entrega (em dias): ");
    scanf("%d", &qDiasDataPrev);
  } while (qDiasDataPrev < 0);

  time_t agora = time(NULL);
  struct tm dataHora = *localtime(&agora);

  snprintf(novaOperacao.dataOperacao, sizeof(novaOperacao.dataOperacao),
           "%04d-%02d-%02d", dataHora.tm_year + 1900, dataHora.tm_mon + 1,
           dataHora.tm_mday);

  dataHora.tm_mday += qDiasDataPrev;
  mktime(&dataHora); // normaliza a data

  snprintf(novaOperacao.dataPreviaChegada,
           sizeof(novaOperacao.dataPreviaChegada), "%02d/%02d/%04d",
           dataHora.tm_mday, dataHora.tm_mon + 1, dataHora.tm_year + 1900);

  strcpy(novaOperacao.dataEntrada, "null");
  strcpy(novaOperacao.dataSaida, "null");

  salvarOperacao(novaOperacao);
}

void ListarOperacoes() {
  FILE *arq = fopen(FILE_NAME, "r");
  if (arq == NULL) {
    printf("Erro ao abrir o ficheiro.\n");
    return;
  }

  printf("\n============ LISTA DE OPERACOES =============\n");

  int numDoc, numDocExterno, idFunc, idEmpresa, idPosto, idComponente;
  float montante;
  char observacao[100], tipoOperacao[50], dataOperacao[11], dataEntrada[11],
      dataSaida[11], dataPreviaChegada[11];

  int encontrou = 0;
  while (fscanf(arq,
                "%d;%d;%d;%d;%d;%d;%f;%99[^;];%49[^;];%10[^;];%10[^;];%10[^;];%"
                "10[^;];\n",
                &numDoc, &numDocExterno, &idFunc, &idEmpresa, &idPosto,
                &idComponente, &montante, observacao, tipoOperacao,
                dataOperacao, dataEntrada, dataSaida,
                dataPreviaChegada) == 13) {

    printf("\nNúmero do Documento: %d\n", numDoc);
    printf("Número do Documento Externo: %d\n", numDocExterno);
    printf("ID do Funcionário: %d\n", idFunc);
    printf("ID da Empresa: %d\n", idEmpresa);
    printf("ID do Posto de Trabalho: %d\n", idPosto);
    printf("ID do Componente: %d\n", idComponente);
    printf("Montante da Operação: %.2f\n", montante);
    printf("Observação: %s\n", observacao);
    printf("Tipo de Operação: %s\n", tipoOperacao);
    printf("Data da Operação: %s\n", dataOperacao);
    printf("Data de Entrada: %s\n", dataEntrada);
    printf("Data de Saída: %s\n", dataSaida);
    printf("Data Prevista de Chegada: %s\n", dataPreviaChegada);

    encontrou = 1;
  }

  if (!encontrou) {
    printf("\nNenhuma operação encontrada.\n");
  }

  fclose(arq);
}

void ApagarOperacao() {
  printf("\n============ APAGAR OPERACAO =============\n");
  int optionId;
  printf("Digite o nº do documento da operacao: ");
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

  int numDoc, numDocExterno, idFunc, idEmpresa, idPosto, idComponente;
  float montante;
  char observacao[100], tipoOperacao[50], dataOperacao[11], dataEntrada[11],
      dataSaida[11], dataPreviaChegada[11];
  int encontrado = 0;

  while (fscanf(arq,
                "%d;%d;%d;%d;%d;%d;%f;%99[^;];%49[^;];%10[^;];%10[^;];%10[^;];%"
                "10[^;];\n",
                &numDoc, &numDocExterno, &idFunc, &idEmpresa, &idPosto,
                &idComponente, &montante, observacao, tipoOperacao,
                dataOperacao, dataEntrada, dataSaida,
                dataPreviaChegada) == 13) {
    if (numDoc != optionId || numDocExterno != optionId) {
      fprintf(temp, "%d;%d;%d;%d;%d;%d;%f;%s;%s;%s;%s;%s;%s;\n", numDoc,
              numDocExterno, idFunc, idEmpresa, idPosto, idComponente, montante,
              observacao, tipoOperacao, dataOperacao, dataEntrada, dataSaida,
              dataPreviaChegada);
    } else {
      encontrado = 1;
    }
  }

  fclose(arq);
  fclose(temp);

  if (encontrado) {
    remove(FILE_NAME);
    rename(FILE_NAME_TEMP, FILE_NAME);
    printf("Operação com ID %d apagada com sucesso.\n", optionId);
  } else {
    remove(FILE_NAME_TEMP);
    printf("Operação com ID %d não encontrada.\n", optionId);
  }
}

void AlterarOperacao() {
  printf("\n============ ALTERAR OPERACAO =============\n");
  int optionId;
  printf("Digite o id da operacao que deseja alterar: ");
  scanf("%d", &optionId);
  getchar(); // limpar o \n que fica no buffer

  FILE *arq = fopen(FILE_NAME, "r");
  if (arq == NULL) {
    printf("Erro ao abrir o ficheiro.\n");
    return;
  }

  FILE *temp = fopen(FILE_NAME_TEMP, "w");
  if (temp == NULL) {
    printf("Erro ao criar ficheiro temporário.\n");
    fclose(arq);
    return;
  }

  int numDoc, numDocExterno, idFunc, idEmpresa, idPosto, idComponente,
      tipoOperacao;
  float montante;
  char observacao[100], dataOperacao[10], dataEntrada[10], dataSaida[10],
      dataPreviaChegada[10];
  int encontrado = 0;

  while (fscanf(arq, "%d;%d;%d;%d;%d;%d;%f;%s;%d;%s;%s;%s;%s;\n", &numDoc,
                &numDocExterno, &idFunc, &idEmpresa, &idPosto, &idComponente,
                &montante, observacao, &tipoOperacao, dataOperacao, dataEntrada,
                dataSaida, dataPreviaChegada) == 13) {
    if (numDoc == optionId) {
      Operacao novaOperacao;

      printf("Montante da Opercao: ");
      scanf("%f", &novaOperacao.montante);

      printf("Tipo de Opercao: ");
      fgets(novaOperacao.tipoOperacao, sizeof(novaOperacao.tipoOperacao),
            stdin);

      printf("Observacao: ");
      fgets(novaOperacao.observacao, sizeof(novaOperacao.observacao), stdin);
      strtok(novaOperacao.observacao, "\n");

      fprintf(arq, "%d;%d;%d;%d;%d;%d;%f;%s;%s;%s;%s;%s;%s;\n", numDoc,
              numDocExterno, idFunc, idEmpresa, idPosto, idComponente, montante,
              novaOperacao.observacao, novaOperacao.tipoOperacao, dataOperacao,
              novaOperacao.dataEntrada, novaOperacao.dataSaida,
              novaOperacao.dataPreviaChegada);
      encontrado = 1;
    } else {
      fprintf(temp, "%d;%d;%d;%d;%d;%d;%f;%s;%d;%s;%s;%s;%s;\n", numDoc,
              numDocExterno, idFunc, idEmpresa, idPosto, idComponente, montante,
              observacao, tipoOperacao, dataOperacao, dataEntrada, dataSaida,
              dataPreviaChegada);
    }
  }

  fclose(arq);
  fclose(temp);

  if (encontrado) {
    remove(FILE_NAME);
    rename(FILE_NAME_TEMP, FILE_NAME);
    printf("Funcionário com ID %d alterado com sucesso.\n", optionId);
  } else {
    remove(FILE_NAME_TEMP);
    printf("Funcionário com ID %d não encontrado.\n", optionId);
  }
}
