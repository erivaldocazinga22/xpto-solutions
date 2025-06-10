#include "../include/structs.h"
#include "../include/utils.h"
#include <ctype.h> // Para poder usar a funcao de verificacao se é um digito <isdigit()>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILE_NAME "./data/empresas.txt"
#define FILE_NAME_TEMP "./data/temp.txt"

int obterProximoEmpresaId() {
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

void salvarEmpresa(Empresa novaEmpresa) {
  // verificar se a pasta 'data' exists, se não cria
  folderDataAlreadyExists();

  FILE *arq = fopen(FILE_NAME, "a");
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

  getchar(); // limpar buffer

  printf("Nome: ");
  fgets(novaEmpresa.nome, sizeof(novaEmpresa.nome), stdin);
  strtok(novaEmpresa.nome, "\n"); // remover \n

  int optionSelected = -1;

  // Pedir tipo de empresa até o utilizador inserir uma opção válida
  while (optionSelected < 1 || optionSelected > 4) {
    printf("\n------ Menu tipo de empresa ------\n");
    printf("1. Fornecedor\n");
    printf("2. Fabricante\n");
    printf("3. Reciclagem\n");
    printf("4. Agência\n");

    printf("Escolha um tipo (1-4): ");
    scanf("%d", &optionSelected);
    getchar(); // limpar \n

    switch (optionSelected) {
    case 1:
      strcpy(novaEmpresa.tipo, "Fornecedor");
      break;
    case 2:
      strcpy(novaEmpresa.tipo, "Fabricante");
      break;
    case 3:
      strcpy(novaEmpresa.tipo, "Reciclagem");
      break;
    case 4:
      strcpy(novaEmpresa.tipo, "Agência");
      break;
    default:
      printf("Opção inválida! Tente novamente.\n");
      break;
    }
  }

  int contactoValido = 0;
  while (!contactoValido) {
    printf("Contacto (9 dígitos): ");
    fgets(novaEmpresa.contacto, sizeof(novaEmpresa.contacto), stdin);
    strtok(novaEmpresa.contacto, "\n");

    if (strlen(novaEmpresa.contacto) != 9) {
      printf("Contacto inválido! Deve conter exatamente 9 dígitos.\n");
      continue;
    }

    contactoValido = 1;
    for (int i = 0; i < 9; i++) {
      if (!isdigit(novaEmpresa.contacto[i])) {
        printf("Contacto inválido! Deve conter apenas números.\n");
        contactoValido = 0;
        break;
      }
    }
  }

  salvarEmpresa(novaEmpresa);
}

void ListarEmpresas() {
  FILE *arq = fopen(FILE_NAME, "r");
  if (!arq) {
    printf("Erro ao abrir o ficheiro de empresas.\n");
    return;
  }

  printf("\n============ LISTA DE EMPRESAS =============\n");

  int id;
  char nome[100], tipo[100], contacto[200];
  int encontrou = 0;

  while (fscanf(arq, "%d;%99[^;];%99[^;];%199[^\n]\n", &id, nome, tipo,
                contacto) == 4) {
    printf("\nID: %d\n", id);
    printf("Nome: %s\n", nome);
    printf("Tipo: %s\n", tipo);
    printf("Contacto: %s\n", contacto);
    encontrou = 1;
  }

  if (!encontrou) {
    printf("\nNenhuma empresa registada no sistema.\n");
  }

  fclose(arq);
}

void ApagarEmpresa() {
  printf("\n============ APAGAR EMPRESA =============\n");

  int optionId;
  printf("Digite o ID da empresa: ");
  if (scanf("%d", &optionId) != 1) {
    printf("Entrada inválida. ID deve ser numérico.\n");
    while (getchar() != '\n')
      ;
    return;
  }

  FILE *arq = fopen(FILE_NAME, "r");
  if (!arq) {
    printf("Erro ao abrir o ficheiro de empresas.\n");
    return;
  }

  FILE *temp = fopen(FILE_NAME_TEMP, "w");
  if (!temp) {
    fclose(arq);
    printf("Erro ao criar ficheiro temporário.\n");
    return;
  }

  int id, encontrado = 0;
  char nome[100], tipo[100], contacto[200];

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
    remove(FILE_NAME_TEMP);
    printf("Empresa com ID %d não encontrada.\n", optionId);
    return;
  }

  remove(FILE_NAME);
  rename(FILE_NAME_TEMP, FILE_NAME);
  printf("Empresa com ID %d apagada com sucesso.\n", optionId);

  // Remover operações
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
  }

  // Remover componentes
  FILE *components = fopen("componentes.txt", "r");
  FILE *tempComponents = fopen("temp_componentes.txt", "w");
  if (components && tempComponents) {
    int idComp, idFornecedor, idFabricante, idPostoTrabalho;
    char tipo[100], designacao[100], condicao[100], observacao[255];
    while (fscanf(components, "%d;%d;%d;%d;%99[^;];%99[^;];%99[^;];%254[^\n]\n",
                  &idComp, &idFornecedor, &idFabricante, &idPostoTrabalho, tipo,
                  designacao, condicao, observacao) == 8) {
      if (idFornecedor != optionId && idFabricante != optionId) {
        fprintf(tempComponents, "%d;%d;%d;%d;%s;%s;%s;%s\n", idComp,
                idFornecedor, idFabricante, idPostoTrabalho, tipo, designacao,
                condicao, observacao);
      }
    }
    fclose(components);
    fclose(tempComponents);
    remove("componentes.txt");
    rename("temp_componentes.txt", "componentes.txt");
    printf("Componentes associados à empresa foram removidos.\n");
  } else {
    if (components)
      fclose(components);
    if (tempComponents)
      fclose(tempComponents);
  }

  // Remover postos de trabalho
  FILE *postos = fopen("posto-de-trabalho.txt", "r");
  FILE *tempPostos = fopen("temp_posto-de-trabalho.txt", "w");
  if (postos && tempPostos) {
    int idPosto, idFunc;
    char nome[100], local[100], seccao[100], descricao[200];
    while (fscanf(postos, "%d;%d;%99[^;];%99[^;];%99[^;];%199[^\n]\n", &idPosto,
                  &idFunc, nome, local, seccao, descricao) == 6) {
      if (idFunc != optionId) {
        fprintf(tempPostos, "%d;%d;%s;%s;%s;%s\n", idPosto, idFunc, nome, local,
                seccao, descricao);
      }
    }
    fclose(postos);
    fclose(tempPostos);
    remove("posto-de-trabalho.txt");
    rename("temp_posto-de-trabalho.txt", "posto-de-trabalho.txt");
    printf("Postos de trabalho associados à empresa foram removidos.\n");
  } else {
    if (postos)
      fclose(postos);
    if (tempPostos)
      fclose(tempPostos);
  }

  printf("Remoção concluída.\n");
}

void AlterarEmpresa() {
  printf("\n============ ALTERAR EMPRESA =============\n");
  int optionId;
  printf("Digite o id da empresa que deseja alterar: ");
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

  int id;
  char nome[100], tipo[100], contacto[200];
  int encontrado = 0;

  while (fscanf(arq, "%d;%99[^;];%99[^;];%199[^\n]\n", &id, nome, tipo,
                contacto) == 4) {
    if (id == optionId) {
      encontrado = 1;

      int opcao;
      char novoNome[100], novoTipo[100], novoContacto[200];
      strcpy(novoNome, nome);
      strcpy(novoTipo, tipo);
      strcpy(novoContacto, contacto);

      do {
        printf("\n--- O que deseja alterar? ---\n");
        printf("1. Alterar Nome\n");
        printf("2. Alterar Tipo\n");
        printf("3. Alterar Contacto\n");
        printf("4. Alterar Todos os campos\n");
        printf("0. Confirmar e guardar alterações\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);
        getchar(); // limpar \n

        switch (opcao) {
        case 1:
          printf("Novo nome: ");
          fgets(novoNome, sizeof(novoNome), stdin);
          strtok(novoNome, "\n");
          break;
        case 2: {
          int optionSelected = -1;

          // Pedir tipo de empresa até o utilizador inserir uma opção válida
          while (optionSelected < 1 || optionSelected > 4) {
            printf("\n------ Menu tipo de empresa ------\n");
            printf("1. Fornecedor\n");
            printf("2. Fabricante\n");
            printf("3. Reciclagem\n");
            printf("4. Agência\n");

            printf("Escolha um tipo (1-4): ");
            scanf("%d", &optionSelected);
            getchar(); // limpar \n

            switch (optionSelected) {
            case 1:
              strcpy(novoTipo, "Fornecedor");
              break;
            case 2:
              strcpy(novoTipo, "Fabricante");
              break;
            case 3:
              strcpy(novoTipo, "Reciclagem");
              break;
            case 4:
              strcpy(novoTipo, "Agência");
              break;
            default:
              printf("Opção inválida! Tente novamente.\n");
              break;
            }
          }
        } break;
        case 3:
          printf("Novo contacto: ");
          fgets(novoContacto, sizeof(novoContacto), stdin);
          strtok(novoContacto, "\n");
          break;
        case 4:
          printf("Novo nome: ");
          fgets(novoNome, sizeof(novoNome), stdin);
          strtok(novoNome, "\n");

          printf("Novo tipo: ");
          fgets(novoTipo, sizeof(novoTipo), stdin);
          strtok(novoTipo, "\n");

          printf("Novo contacto: ");
          fgets(novoContacto, sizeof(novoContacto), stdin);
          strtok(novoContacto, "\n");
          break;
        case 0:
          printf("A guardar alterações...\n");
          break;
        default:
          printf("Opção inválida! Tente novamente.\n");
        }
      } while (opcao != 0);

      fprintf(temp, "%d;%s;%s;%s\n", id, novoNome, novoTipo, novoContacto);
    } else {
      fprintf(temp, "%d;%s;%s;%s\n", id, nome, tipo, contacto);
    }
  }

  fclose(arq);
  fclose(temp);

  if (encontrado) {
    remove(FILE_NAME);
    rename(FILE_NAME_TEMP, FILE_NAME);
    printf("Empresa com ID %d alterada com sucesso.\n", optionId);
  } else {
    remove(FILE_NAME_TEMP);
    printf("Empresa com ID %d não encontrada.\n", optionId);
  }
}

void PesquisarEmpresa() {
  printf("\n============ PESQUISAR EMPRESA =============\n");
  char termo[100];
  printf("Digite o nome, tipo ou parte do contacto para pesquisar: ");
  getchar(); // limpar \n pendente
  fgets(termo, sizeof(termo), stdin);
  strtok(termo, "\n"); // tirar o \n no final

  FILE *arq = fopen(FILE_NAME, "r");
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
