#include "../include/structs.h"
#include <ctype.h> // Para poder usar a funcao de verificacao se é um digito <isdigit()>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILE_NAME "empresas.txt"

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
      ; // limpar buffer
    return;
  }

  FILE *arq = fopen(FILE_NAME, "r");
  if (!arq) {
    printf("Erro ao abrir o ficheiro de empresas.\n");
    return;
  }

  FILE *temp = fopen("temp.txt", "w");
  if (!temp) {
    printf("Erro ao criar o ficheiro temporário.\n");
    fclose(arq);
    return;
  }

  int id;
  char nome[100], tipo[100], contacto[200];
  int encontrado = 0;

  // Copia tudo para temp, excepto a empresa a ser removida
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

  // Substitui o ficheiro original
  remove(FILE_NAME);
  rename("temp.txt", FILE_NAME);
  printf("Empresa com ID %d apagada com sucesso.\n", optionId);

  // Apaga operações associadas
  FILE *operacoes = fopen("operacoes.txt", "r");
  FILE *tempOperacoes = fopen("temp_operacoes.txt", "w");

  int operacao_encontrada = 0;

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
    operacao_encontrada = 1;
  } else {
    if (operacoes)
      fclose(operacoes);
    if (tempOperacoes)
      fclose(tempOperacoes);
  }

  // Apaga componentes associados
  FILE *components = fopen("componentes.txt", "r");
  FILE *tempComponents = fopen("temp_componentes.txt", "w");

  int component_encontrada = 0;

  if (operacoes && tempOperacoes) {
    int id, idFornecedor, idFabricante, idPostoTrabalho;
    char tipo[100], designacao[100], condicao[100], observacao[255];

    while (fscanf(arq, "%d;%d;%d;%d;%99[^;];%99[^;];%99[^;];%254[^\n]\n", &id,
                  &idFornecedor, &idFabricante, &idPostoTrabalho, tipo,
                  designacao, condicao, observacao) == 8) {
      if (id != optionId) {
        fprintf(temp, "%d;%d;%d;%d;%s;%s;%s;%s\n", id, idFornecedor,
                idFabricante, idPostoTrabalho, tipo, designacao, condicao,
                observacao);
      } else {
        component_encontrada = 1;
      }
    }

    fclose(operacoes);
    fclose(tempOperacoes);
    remove("componentes.txt");
    rename("temp_componentes.txt", "componentes.txt");
    printf("Componentes associados à empresa foram removidas.\n");
    operacao_encontrada = 1;
  } else {
    if (components)
      fclose(operacoes);
    if (tempComponents)
      fclose(tempOperacoes);
  }

  // Apaga postos de trabalhos associados
  FILE *postos = fopen("posto-de-trabalho.txt", "r");
  FILE *tempPostos = fopen("temp_posto-de-trabalho.txt", "w");

  int posto_encontrada = 0;

  if (postos && tempPostos) {
    int id, idFunc;
    char nome[100], local[100], seccao[100], descricao[200];

    while (fscanf(arq, "%d;%d;%99[^;];%99[^;];%99[^;];%199[^\n]\n", &id,
                  &idFunc, nome, local, seccao, descricao) == 6) {
      if (id != optionId) {
        fprintf(temp, "%d;%d;%s;%s;%s;%s\n", id, idFunc, nome, local, seccao,
                descricao);
      } else {
        posto_encontrada = 1;
      }
    }

    fclose(operacoes);
    fclose(tempOperacoes);
    remove("posto-de-trabalho.txt");
    rename("temp_posto-de-trabalho.txt", "posto-de-trabalho.txt");
    printf("Postos de trabalho associados à empresa foram removidas.\n");
    posto_encontrada = 1;
  } else {
    if (postos)
      fclose(operacoes);
    if (tempPostos)
      fclose(tempOperacoes);
  }

  if (operacao_encontrada || component_encontrada || posto_encontrada)
    printf("Referências a empresa foram removidas dos ficheiros "
           "relacionados.\n");
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
    rename("temp.txt", FILE_NAME);
    printf("Empresa com ID %d alterada com sucesso.\n", optionId);
  } else {
    remove("temp.txt");
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
