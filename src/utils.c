#include "include/utils.h"
#include <stdio.h>

int postoAlreadyExists(int idPosto) {
  FILE *arq = fopen("posto-de-trabalho.txt", "r");
  if (arq == NULL) {
    printf("Erro ao abrir o ficheiro.\n");
    return 0;
  }

  int id, idFunc, encontrado = 0;
  char nome[100], local[100], seccao[100], descricao[100];

  while (fscanf(arq, "%d;%d;%99[^;];%99[^;];%99[^;];%99[^\n]\n", &id, &idFunc,
                nome, local, seccao, descricao) == 6) {
    if (id == idPosto) {
      encontrado++;
    }
  }

  fclose(arq);
  return encontrado;
}

int fabricanteAlreadyExists(int idFabricante) {
  FILE *arq = fopen("empresas.txt", "r");
  if (arq == NULL) {
    printf("Erro ao abrir o ficheiro.\n");
    return 0;
  }

  int id;
  char nome[100], tipo[100], contacto[200];
  int encontrado = 0;

  while (fscanf(arq, "%d;%99[^;];%99[^;];%199[^\n]\n", &id, nome, tipo,
                contacto) == 4) {
    if (id == idFabricante) {
      encontrado++;
    }
  }
  fclose(arq);
  return encontrado;
}

int fornecedorAlreadyExists(int idForncedor) {
  FILE *arq = fopen("empresas.txt", "r");
  if (arq == NULL) {
    printf("Erro ao abrir o ficheiro.\n");
    return 0;
  }

  int id;
  char nome[100], tipo[100], contacto[200];
  int encontrado = 0;

  while (fscanf(arq, "%d;%99[^;];%99[^;];%199[^\n]\n", &id, nome, tipo,
                contacto) == 4) {
    if (id == idForncedor) {
      /* verificar o tipo se e forncedor */
      encontrado++;
    }
  }
  fclose(arq);
  return encontrado;
}

int funcionarioAlreadyExists(int idFunc) {
  FILE *arq = fopen("funcionario.txt", "r");
  if (arq == NULL) {
    printf("Erro ao abrir o ficheiro.\n");
    return 0;
  }

  int id, encontrado = 0;
  char nome[100], funcao[100], descricao[200];

  while (fscanf(arq, "%d;%99[^;];%99[^;];%199[^\n]\n", &id, nome, funcao,
                descricao) == 4) {
    if (id == idFunc) {
      encontrado++;
    }
  }
  fclose(arq);
  return encontrado;
};

int operacaoAlreadyExists(int idOpera) {
  FILE *arq = fopen("opercoes.txt", "r");
  if (arq == NULL) {
    printf("Erro ao abrir o ficheiro.\n");
    return 0;
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
    if (numDoc == idOpera) {
      encontrado++;
    }
  }
  fclose(arq);
  return encontrado;
}