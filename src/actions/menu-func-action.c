#include "../include/structs.h"
#include "../include/utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILE_NAME "./data/funcionario.txt"
#define FILE_NAME_TEMP "./data/temp.txt"
#define FILE_NAME_POSTO "./data/posto-de-trabalho.txt"
#define FILE_NAME_TEMP_POSTO "./data/temp_postos.txt"
#define FILE_NAME_OPERACAO "./data/operacoes.txt"
#define FILE_NAME_TEMP_OPERACAO "./data/temp_operacoes.txt"

/*
 * int obterProximoFuncId() - Função que verifica o ultimo id e incrementa
 *
 */
int obterProximoFuncId() {
  FILE *arq = fopen(FILE_NAME, "r"); // Ler o arquivo
  if (arq == NULL)
    return 1;

  int ultimoId = 0;
  char linha[512];
  while (fgets(linha, sizeof(linha), arq) != NULL) {
    int idTemp;
    if (sscanf(linha, "%d;", &idTemp) == 1) {
      if (idTemp > ultimoId)
        ultimoId = idTemp;
    }
  }

  fclose(arq);
  return ultimoId + 1;
}

/*
 * int salvarFuncionario(Funcionario novoFuncionario) - Função que precisiste os
 * dados
 *
 */

void salvarFuncionario(Funcionario novoFuncionario) {
  // Verifica se a pasta "data" existe
  folderDataAlreadyExists();

  FILE *arq = fopen(FILE_NAME, "a");
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
  FILE *arq = fopen(FILE_NAME, "r");
  if (!arq) {
    printf("Erro ao abrir o ficheiro.\n");
    return;
  }

  int id, encontrou = 0;
  char nome[100], funcao[100], descricao[200];

  printf("\n============ LISTA DE FUNCIONÁRIOS =============\n");

  while (fscanf(arq, "%d;%99[^;];%99[^;];%199[^\n]\n", &id, nome, funcao,
                descricao) == 4) {
    printf("\nID: %d\nNome: %s\nFunção: %s\nDescrição: %s\n", id, nome, funcao,
           descricao);
    encontrou = 1;
  }

  if (!encontrou)
    printf("\nNenhum funcionário encontrado.\n");

  fclose(arq);
}

void ApagarFuncionario() {
  printf("\n============ APAGAR FUNCIONÁRIO =============\n");
  int optionId;
  printf("Digite o id do funcionario: ");
  scanf("%d", &optionId);

  // Abrir o ficheiro original de funcionários
  FILE *arq = fopen(FILE_NAME, "r");
  if (arq == NULL) {
    printf("Erro ao abrir o ficheiro de funcionários.\n");
    return;
  }

  FILE *temp = fopen(FILE_NAME_TEMP, "w");
  if (temp == NULL) {
    printf("Erro ao criar o ficheiro temporário.\n");
    fclose(arq);
    return;
  }

  int id;
  char nome[100], funcao[100], descricao[200];
  int encontrado = 0;

  // Ler e filtrar funcionários que não possuem o ID selecionado <optionId>
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

  if (!encontrado) {
    remove(FILE_NAME_TEMP);
    printf("Funcionário com ID %d não encontrado.\n", optionId);
    return;
  }

  // Substituir ficheiro original
  remove(FILE_NAME);
  rename(FILE_NAME_TEMP, FILE_NAME);

  // === Remover de posto-de-trabalho.txt ===
  FILE *postos = fopen(FILE_NAME_POSTO, "r");
  FILE *tempPostos = fopen(FILE_NAME_TEMP_POSTO, "w");
  if (postos && tempPostos) {
    int idPosto, idFuncionario;
    char cargo[100], local[100], area[100], descPosto[200];

    while (fscanf(postos, "%d;%d;%99[^;];%99[^;];%99[^;];%199[^\n]\n", &idPosto,
                  &idFuncionario, cargo, local, area, descPosto) == 6) {
      if (idFuncionario != optionId) {
        fprintf(tempPostos, "%d;%d;%s;%s;%s;%s\n", idPosto, idFuncionario,
                cargo, local, area, descPosto);
      }
    }
    fclose(postos);
    fclose(tempPostos);
    remove(FILE_NAME_POSTO);
    rename(FILE_NAME_TEMP_POSTO, FILE_NAME_POSTO);
  } else {
    if (postos)
      fclose(postos);
    if (tempPostos)
      fclose(tempPostos);
  }

  // === Remover de operacoes.txt ===
  FILE *operacoes = fopen(FILE_NAME_OPERACAO, "r");
  FILE *tempOperacoes = fopen(FILE_NAME_TEMP_OPERACAO, "w");

  int operacao_encontrada =
      0; // varialvel para controlar se existe opecao com o id do funcionario

  if (operacoes && tempOperacoes) {
    int idOperacao, idFuncionario;
    char tipoOperacao[100], descricaoOperacao[200];

    while (fscanf(operacoes, "%d;%d;%99[^;];%199[^\n]\n", &idOperacao,
                  &idFuncionario, tipoOperacao, descricaoOperacao) == 4) {
      if (idFuncionario != optionId) {
        fprintf(tempOperacoes, "%d;%d;%s;%s\n", idOperacao, idFuncionario,
                tipoOperacao, descricaoOperacao);
      }
    }
    fclose(operacoes);
    fclose(tempOperacoes);
    remove(FILE_NAME_OPERACAO);
    rename(FILE_NAME_TEMP_OPERACAO, FILE_NAME_OPERACAO);
    operacao_encontrada = 1;
  } else {
    if (operacoes)
      fclose(operacoes);
    if (tempOperacoes)
      fclose(tempOperacoes);
  }

  printf("Funcionário com ID %d apagado com sucesso.\n", optionId);
  if (operacao_encontrada) {
    printf("[INFO]: Referências ao funcionário foram removidas dos ficheiros "
           "relacionados.\n");
  }
}

void AlterarFuncionario() {
  printf("\n============ ALTERAR FUNCIONÁRIO =============\n");
  int optionId;
  printf("Digite o id do funcionário que deseja alterar: ");
  scanf("%d", &optionId);
  getchar(); // limpar \n do buffer

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

  int id;
  char nome[100], funcao[100], descricao[200];
  int encontrado = 0;

  while (fscanf(arq, "%d;%99[^;];%99[^;];%199[^\n]\n", &id, nome, funcao,
                descricao) == 4) {
    if (id == optionId) {
      Funcionario novoFuncionario;
      strcpy(novoFuncionario.nome, nome);
      strcpy(novoFuncionario.funcao, funcao);
      strcpy(novoFuncionario.descricao, descricao);

      int optionSelected;
      do {
        printf("\n--- Menu de Actualização de Funcionários ---\n");
        printf("1. Alterar nome\n");
        printf("2. Alterar função\n");
        printf("3. Alterar descrição\n");
        printf("4. Alterar todos os campos\n");
        printf("0. Guardar alterações e sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &optionSelected);
        getchar(); // limpar \n

        switch (optionSelected) {
        case 1:
          printf("Novo nome: ");
          fgets(novoFuncionario.nome, sizeof(novoFuncionario.nome), stdin);
          strtok(novoFuncionario.nome, "\n");
          break;
        case 2:
          printf("Nova função: ");
          fgets(novoFuncionario.funcao, sizeof(novoFuncionario.funcao), stdin);
          strtok(novoFuncionario.funcao, "\n");
          break;
        case 3:
          printf("Nova descrição: ");
          fgets(novoFuncionario.descricao, sizeof(novoFuncionario.descricao),
                stdin);
          strtok(novoFuncionario.descricao, "\n");
          break;
        case 4:
          printf("Novo nome: ");
          fgets(novoFuncionario.nome, sizeof(novoFuncionario.nome), stdin);
          strtok(novoFuncionario.nome, "\n");

          printf("Nova função: ");
          fgets(novoFuncionario.funcao, sizeof(novoFuncionario.funcao), stdin);
          strtok(novoFuncionario.funcao, "\n");

          printf("Nova descrição: ");
          fgets(novoFuncionario.descricao, sizeof(novoFuncionario.descricao),
                stdin);
          strtok(novoFuncionario.descricao, "\n");
          break;
        case 0:
          printf("A guardar alterações...\n");
          break;
        default:
          printf("Opção inválida! Tente novamente.\n");
        }
      } while (optionSelected != 0);

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
    remove(FILE_NAME);
    rename(FILE_NAME_TEMP, FILE_NAME);
    printf("Funcionário com ID %d alterado com sucesso.\n", optionId);
  } else {
    remove(FILE_NAME_TEMP);
    printf("Funcionário com ID %d não encontrado.\n", optionId);
  }
}

/* Formas de uso dos wildcards
 * "Jo*" — encontra “João”, “Josefa”, “Jorge”
 * "???a" — encontra nomes com 4 letras terminando em "a"
 * "*tecnico*" — encontra qualquer campo com a palavra "tecnico"
 *
 */
void PesquisarFuncionario() {
  printf("\n============ PESQUISAR FUNCIONÁRIO =============\n");
  printf("1. Pesquisar por ID\n");
  printf("2. Pesquisar por Nome (com * e ?)\n");
  printf("3. Pesquisar por qualquer campo (Nome, Função ou Descrição)\n");
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
    printf("Digite o ID do funcionário: ");
    scanf("%d", &optionId);

    int id, encontrado = 0;
    char nome[100], funcao[100], descricao[200];

    while (fscanf(arq, "%d;%99[^;];%99[^;];%199[^\n]\n", &id, nome, funcao,
                  descricao) == 4) {
      if (id == optionId) {
        printf("\n--- Funcionário Encontrado ---\n");
        printf("ID: %d\n", id);
        printf("Nome: %s\n", nome);
        printf("Função: %s\n", funcao);
        printf("Descrição: %s\n", descricao);
        encontrado = 1;
        break;
      }
    }

    if (!encontrado) {
      printf("Funcionário com ID %d não encontrado.\n", optionId);
    }
  }

  else if (opcao == 2 || opcao == 3) {
    char padrao[100];
    char padraoLower[100];
    printf("Digite o padrão (use * e ?): ");
    scanf(" %[^\n]", padrao);
    strToLower(padraoLower, padrao);

    int id, encontrados = 0;
    char nome[100], funcao[100], descricao[200];
    char nomeLower[100], funcaoLower[100], descricaoLower[200];

    printf("\n--- Funcionários Encontrados ---\n");
    while (fscanf(arq, "%d;%99[^;];%99[^;];%199[^\n]\n", &id, nome, funcao,
                  descricao) == 4) {

      strToLower(nomeLower, nome);
      strToLower(funcaoLower, funcao);
      strToLower(descricaoLower, descricao);

      int corresponde = 0;

      if (opcao == 2) {
        corresponde = matchWildcard(padraoLower, nomeLower);
      } else if (opcao == 3) {
        corresponde = matchWildcard(padraoLower, nomeLower) ||
                      matchWildcard(padraoLower, funcaoLower) ||
                      matchWildcard(padraoLower, descricaoLower);
      }

      if (corresponde) {
        printf("ID: %d\n", id);
        printf("Nome: %s\n", nome);
        printf("Função: %s\n", funcao);
        printf("Descrição: %s\n\n", descricao);
        encontrados++;
      }
    }

    if (!encontrados) {
      printf("Nenhum funcionário correspondente ao padrão.\n");
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
