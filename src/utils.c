#include "include/utils.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h> // Para mkdir
#include <time.h>
#include <unistd.h> // Para access()

#define FILE_NAME_OPERATIONS "./data/operacoes.txt"
#define FILE_NAME_COMPS "./data/componentes.txt"
#define FILE_NAME_POSTO "./data/posto-de-trabalho.txt"
#define FILE_NAME_EMPRESAS "./data/empresas.txt"
#define FILE_NAME_FUNC "./data/funcionario.txt"

int postoAlreadyExists(int idPosto) {
  FILE *arq = fopen(FILE_NAME_POSTO, "r");
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
  FILE *arq = fopen(FILE_NAME_EMPRESAS, "r");
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
  FILE *arq = fopen(FILE_NAME_EMPRESAS, "r");
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
  FILE *arq = fopen(FILE_NAME_FUNC, "r");
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
  FILE *arq = fopen(FILE_NAME_OPERATIONS, "r");

  // Se não existe, cria o ficheiro vazio e retorna 0 (não existe)
  if (arq == NULL) {
    arq = fopen(FILE_NAME_OPERATIONS, "w");
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
  FILE *arq = fopen(FILE_NAME_EMPRESAS, "r");
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
  FILE *arq = fopen(FILE_NAME_COMPS, "r");
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

void obterDataActual(char *buffer, int tamanho) {
  time_t agora = time(NULL);
  struct tm *data_hora = localtime(&agora);

  // Formata a data como string e guarda no buffer
  strftime(buffer, tamanho, "%Y-%m-%d", data_hora);
}

void folderDataAlreadyExists() {
  if (access("./data", F_OK) != 0) {
    // Cria a pasta se não existir
    if (mkdir("./data", 0755) != 0) {
      printf("Erro ao criar a pasta 'data'.\n");
      return;
    }
  }
}

// Utilistarios de  pesqusisa

/* Função que converte para minusculo - garantido a compatibilidade com
 * caracteres especiais
 *
 * @params while (*src) para percorrer a string até o terminador nulo.
 * tolower((unsigned char)*src) garante comportamento correto para caracteres
 * acentuados ou especiais.
 */

void strToLower(char *dest, const char *src) {
  int i = 0;
  while (src[i]) {
    dest[i] = tolower((unsigned char)src[i]);
    i++;
  }
  dest[i] = '\0';
}

int matchWildcard(const char *pattern, const char *text) {
  size_t pLen = strlen(pattern);
  size_t tLen = strlen(text);

  // Se o padrão contém '*'
  if (strchr(pattern, '*')) {
    const char *star = strchr(pattern, '*');
    int starIndex = star - pattern;

    if (pLen == 1)
      return 1; // só tem '*', então tudo combina

    if (starIndex == 0) {
      // Começa com '*': *abc
      const char *sub = pattern + 1;
      return strstr(text, sub) != NULL;
    } else if (starIndex == pLen - 1) {
      // Termina com '*': abc*
      return strncmp(text, pattern, pLen - 1) == 0;
    } else {
      // No meio: ab*cd
      char prefix[128], suffix[128];
      strncpy(prefix, pattern, starIndex);
      prefix[starIndex] = '\0';
      strcpy(suffix, pattern + starIndex + 1);

      return strncmp(text, prefix, strlen(prefix)) == 0 &&
             strstr(text + strlen(prefix), suffix) != NULL;
    }
  }

  // Se o padrão contém '?'
  if (strchr(pattern, '?')) {
    if (tLen != pLen)
      return 0;

    for (size_t i = 0; i < pLen; i++) {
      if (pattern[i] == '?')
        continue;
      if (pattern[i] != text[i])
        return 0;
    }
    return 1;
  }

  // Comparação exata se não houver '*' ou '?'
  return strcmp(pattern, text) == 0;
}
