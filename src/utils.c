#include "include/utils.h"
#include <stdio.h>
#include <stdlib.h>

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

  // Se não existe, cria o ficheiro vazio e retorna 0 (não existe)
  if (arq == NULL) {
    arq = fopen("opercoes.txt", "w");
    if (arq == NULL) {
      printf("Erro ao criar o ficheiro.\n");
      return 0;
    }
    fclose(arq);
    return 0;
  }

  int numDoc, numDocExterno, idFunc, idEmpresa, idPosto, idComponente,
      tipoOperacao;
  float montante;
  char observacoa[100], dataOperacao[10], dataEntrada[10], dataSaida[10],
      dataPreviaChegada[10];

  int encontrado = 0;
  while (
      fscanf(arq,
             "%d;%d;%d;%d;%d;%d;%f;%99[^;];%d;%9[^;];%9[^;];%9[^;];%9[^;];\n",
             &numDoc, &numDocExterno, &idFunc, &idEmpresa, &idPosto,
             &idComponente, &montante, observacoa, &tipoOperacao, dataOperacao,
             dataEntrada, dataSaida, dataPreviaChegada) == 13) {
    if (numDoc == idOpera) {
      encontrado++;
    }
  }

  fclose(arq);
  return encontrado;
}

int empresaAlreadyExists(int idEmpresa) {
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
    if (id == idEmpresa) {
      encontrado++;
    }
  }
  fclose(arq);
  return encontrado;
};

int componenteAlreadyExists(int idComponente) {
  FILE *arq = fopen("componentes.txt", "r");
  if (arq == NULL) {
    printf("Erro ao abrir o ficheiro.\n");
    return 0;
  }

  int id, idFornecedor, idFabricante, idPostoTrabalho, numSerie, garantia;
  char tipo[100], designacao[100], condicao[100], observacao[255];
  char dataAquisicao[20];
  int encontrado = 0;

  while (fscanf(arq,
                "%d;%d;%d;%d;%d;%99[^;];%99[^;];%99[^;];%254[^;];%d;%20[^\n]\n",
                &id, &idFornecedor, &idFabricante, &idPostoTrabalho, &numSerie,
                tipo, designacao, condicao, observacao, &garantia,
                dataAquisicao) == 11) {
    if (id == idComponente) {
      encontrado++;
    }
  }
  fclose(arq);
  return encontrado;
};

void clear_screen() {
#ifdef _WIN32
    system("cls");
#else
    printf("\033[2J\033[H");
    fflush(stdout);
#endif
}