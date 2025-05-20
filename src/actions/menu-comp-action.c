#include "../include/structs.h"
#include "../include/utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> // para lidar com a data de aquisição automatica

#define FILE_NAME "componentes.txt"

int obterProximoComponenteId() {

  FILE *arq = fopen(FILE_NAME, "r");
  if (arq == NULL)
    return 1;

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

void salvarComponente(Componente novoComponente) {
  FILE *arq = fopen(FILE_NAME, "a");
  if (arq == NULL) {
    printf("Erro ao abrir o ficheiro.\n");
    return;
  }

  int id = obterProximoComponenteId();
  fprintf(arq, "%d;%d;%d;%d;%d;%s;%s;%s;%s;%d;%s\n", id,
          novoComponente.idFornecedor, novoComponente.idFabricante,
          novoComponente.idPostoTrabalho, novoComponente.numSerie,
          novoComponente.tipo, novoComponente.designacao,
          novoComponente.condicao, novoComponente.observacao,
          novoComponente.garantia, novoComponente.dataAquisicao);

  fclose(arq);
}

void InserirComponente() {
  printf("\n============ INSERIR COMPONENTE =============\n");
  Componente novoComponente;
  getchar();
  do {
    printf("Designação: ");
    fgets(novoComponente.designacao, sizeof(novoComponente.designacao), stdin);
    strtok(novoComponente.designacao, "\n");
    if (strlen(novoComponente.designacao) == 0)
      printf("A designação é obrigatória!\n");
  } while (strlen(novoComponente.designacao) == 0);

  do {
    printf("Tipo: ");
    fgets(novoComponente.tipo, sizeof(novoComponente.tipo), stdin);
    strtok(novoComponente.tipo, "\n");
    if (strlen(novoComponente.tipo) == 0)
      printf("O tipo é obrigatório!\n");
  } while (strlen(novoComponente.tipo) == 0);

  int optionSelected;
  do {
    printf("\n--- Menu de Condição de Componentes ---\n");
    printf("1. Novo\n");
    printf("2. Utilizado\n");
    printf("Escolha uma opção: ");
    scanf("%d", &optionSelected);
    getchar(); // limpar \n

    if (optionSelected == 1)
      strcpy(novoComponente.condicao, "novo");
    else if (optionSelected == 2)
      strcpy(novoComponente.condicao, "utilizado");
    else
      printf("Opção inválida! Tente novamente.\n");

  } while (optionSelected != 1 && optionSelected != 2);

  printf("Número de Série: ");
  scanf("%d", &novoComponente.numSerie);
  getchar();

  printf("Garantia(em anos): ");
  scanf("%d", &novoComponente.garantia);
  getchar();

  // Obter data actual e guardar como string no campo dataAquisicao
  time_t agora;
  struct tm *dataHora;

  time(&agora);                 // Obtém o tempo actual
  dataHora = localtime(&agora); // Converte para struct tm local

  // Formatar a data como string no formato DD/MM/AAAA
  snprintf(novoComponente.dataAquisicao, sizeof(novoComponente.dataAquisicao),
           "%02d/%02d/%d", dataHora->tm_mday, dataHora->tm_mon + 1,
           dataHora->tm_year + 1900);

  int error = 0;
  do {
    printf("ID Fornecedor: ");
    scanf("%d", &novoComponente.idFornecedor);
    getchar();

    error = fornecedorAlreadyExists(novoComponente.idFornecedor);
    if (!error)
      printf("ID do Fornecedor não existe.\n");
  } while (!error);

  do {
    printf("ID Fabricante: ");
    scanf("%d", &novoComponente.idFabricante);
    getchar();

    error = fabricanteAlreadyExists(novoComponente.idFabricante);
    if (!error)
      printf("ID do Fabricante não existe.\n");
  } while (!error);

  do {
    printf("ID Posto Trabalho: ");
    scanf("%d", &novoComponente.idPostoTrabalho);
    getchar();

    error = postoAlreadyExists(novoComponente.idPostoTrabalho);
    if (!error)
      printf("ID do Posto de Trabalho não existe.\n");
  } while (!error);

  printf("Observação: ");
  fgets(novoComponente.observacao, sizeof(novoComponente.observacao), stdin);
  strtok(novoComponente.observacao, "\n");

  salvarComponente(novoComponente);
}

void ListarComponentes() {
  FILE *arq = fopen(FILE_NAME, "r");
  if (arq == NULL) {
    printf("Erro ao abrir o ficheiro.\n");
    return;
  }

  printf("\n============ LISTA DE COMPONENTES =============\n");

  int id, idFornecedor, idFabricante, idPostoTrabalho, numSerie, garantia;
  char tipo[100], designacao[100], condicao[100], observacao[255];
  char dataAquisicao[20];
  int encontrou = 0;

  while (fscanf(arq,
                "%d;%d;%d;%d;%d;%99[^;];%99[^;];%99[^;];%254[^;];%d;%20[^\n]\n",
                &id, &idFornecedor, &idFabricante, &idPostoTrabalho, &numSerie,
                tipo, designacao, condicao, observacao, &garantia,
                dataAquisicao) == 11) {
    printf("\nID: %d\nFornecedor: %d\nFabricante: %d\nPosto: %d\nNº Série: "
           "%d\nTipo: %s\nDesignação: %s\nCondição: %s\nObservação: "
           "%s\nGarantia: %d anos\nData de Aquisição: %s\n",
           id, idFornecedor, idFabricante, idPostoTrabalho, numSerie, tipo,
           designacao, condicao, observacao, garantia, dataAquisicao);

    encontrou = 1;
  }

  if (!encontrou) {
    printf("\nNenhum componente encontrado.\n");
  }

  fclose(arq);
}

void ApagarComponente() {
  printf("\n============ APAGAR COMPONENTE =============\n");
  int optionId;
  printf("Digite o id do componente: ");
  scanf("%d", &optionId);

  FILE *arq = fopen(FILE_NAME, "r");
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

  int id, idFornecedor, idFabricante, idPostoTrabalho, numSerie, garantia;
  char tipo[100], designacao[100], condicao[100], observacao[255],
      dataAquisicao[20];
  int encontrado = 0;

  while (fscanf(arq,
                "%d;%d;%d;%d;%d;%99[^;];%99[^;];%99[^;];%254[^;];%d;%19[^\n]\n",
                &id, &idFornecedor, &idFabricante, &idPostoTrabalho, &numSerie,
                tipo, designacao, condicao, observacao, &garantia,
                dataAquisicao) == 11) {
    if (id != optionId) {
      fprintf(temp, "%d;%d;%d;%d;%d;%s;%s;%s;%s;%d;%s\n", id, idFornecedor,
              idFabricante, idPostoTrabalho, numSerie, tipo, designacao,
              condicao, observacao, garantia, dataAquisicao);
      ;
    } else {
      encontrado = 1;
    }
  }

  fclose(arq);
  fclose(temp);

  if (encontrado) {
    remove(FILE_NAME);             // Apaga o antigo
    rename("temp.txt", FILE_NAME); // Renomeia o novo
    printf("Componente com ID %d apagado com sucesso.\n", optionId);
  } else {
    remove("temp.txt"); // Não encontrado, apaga temp
    printf("Componente com ID %d não encontrado.\n", optionId);
  }
}

void limparBuffer() {
  int c;
  while ((c = getchar()) != '\n' && c != EOF)
    ;
}

void AlterarComponente() {
  printf("\n============ ALTERAR COMPONENTE =============\n");
  int optionId;
  printf("Digite o ID do componente que deseja alterar: ");
  scanf("%d", &optionId);
  limparBuffer();

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

  int id, idFornecedor, idFabricante, idPostoTrabalho;
  char designacao[100], tipo[100], condicao[100], observacao[255];
  int numSerie, garantia;
  char dataAquisicao[20];
  int encontrado = 0;

  while (fscanf(arq,
                "%d;%d;%d;%d;%d;%99[^;];%99[^;];%99[^;];%254[^;];%d;%19[^\n]\n",
                &id, &idFornecedor, &idFabricante, &idPostoTrabalho, &numSerie,
                designacao, tipo, condicao, observacao, &garantia,
                dataAquisicao) == 11) {

    if (id == optionId) {
      Componente novoComponente;

      // Guardar dados antigos
      novoComponente.id = id;
      novoComponente.idFornecedor = idFornecedor;
      novoComponente.idFabricante = idFabricante;
      novoComponente.idPostoTrabalho = idPostoTrabalho;
      novoComponente.numSerie = numSerie;
      novoComponente.garantia = garantia;
      strcpy(novoComponente.designacao, designacao);
      strcpy(novoComponente.tipo, tipo);
      strcpy(novoComponente.condicao, condicao);
      strcpy(novoComponente.observacao, observacao);
      strcpy(novoComponente.dataAquisicao, dataAquisicao);

      int opcao;
      do {
        printf("\n--- MENU DE ALTERAÇÃO ---\n");
        printf("1. Designação\n");
        printf("2. Tipo\n");
        printf("3. Condição\n");
        printf("4. Observação\n");
        printf("5. Nº Série\n");
        printf("6. Garantia\n");
        printf("7. Alterar todos\n");
        printf("0. Guardar e sair\n");
        printf("Opção: ");
        scanf("%d", &opcao);
        limparBuffer();

        switch (opcao) {
        case 1:
          printf("Nova designação: ");
          fgets(novoComponente.designacao, sizeof(novoComponente.designacao),
                stdin);
          novoComponente.designacao[strcspn(novoComponente.designacao, "\n")] =
              '\0';
          break;
        case 2:
          printf("Novo tipo: ");
          fgets(novoComponente.tipo, sizeof(novoComponente.tipo), stdin);
          novoComponente.tipo[strcspn(novoComponente.tipo, "\n")] = '\0';
          break;
        case 3: {
          int optionSelected;
          do {
            printf("\n--- Menu de Condição de Componentes ---\n");
            printf("1. Novo\n");
            printf("2. Utilizado\n");
            printf("Escolha uma opção: ");
            scanf("%d", &optionSelected);
            getchar(); // limpar \n

            if (optionSelected == 1)
              strcpy(novoComponente.condicao, "novo");
            else if (optionSelected == 2)
              strcpy(novoComponente.condicao, "utilizado");
            else
              printf("Opção inválida! Tente novamente.\n");

          } while (optionSelected != 1 && optionSelected != 2);
          break;
        }
        case 4:
          printf("Nova observação: ");
          fgets(novoComponente.observacao, sizeof(novoComponente.observacao),
                stdin);
          novoComponente.observacao[strcspn(novoComponente.observacao, "\n")] =
              '\0';
          break;
        case 5:
          printf("Novo número de série: ");
          scanf("%d", &novoComponente.numSerie);
          limparBuffer();
          break;
        case 6:
          printf("Nova garantia (anos): ");
          scanf("%d", &novoComponente.garantia);
          limparBuffer();
          break;
        case 7:
          printf("Nova designação: ");
          fgets(novoComponente.designacao, sizeof(novoComponente.designacao),
                stdin);
          novoComponente.designacao[strcspn(novoComponente.designacao, "\n")] =
              '\0';

          printf("Novo tipo: ");
          fgets(novoComponente.tipo, sizeof(novoComponente.tipo), stdin);
          novoComponente.tipo[strcspn(novoComponente.tipo, "\n")] = '\0';

          int optionSelected;
          do {
            printf("\n--- Menu de Condição de Componentes ---\n");
            printf("1. Novo\n");
            printf("2. Utilizado\n");
            printf("Escolha uma opção: ");
            scanf("%d", &optionSelected);
            getchar(); // limpar \n

            if (optionSelected == 1)
              strcpy(novoComponente.condicao, "novo");
            else if (optionSelected == 2)
              strcpy(novoComponente.condicao, "utilizado");
            else
              printf("Opção inválida! Tente novamente.\n");

          } while (optionSelected != 1 && optionSelected != 2);
          break;

          printf("Nova observação: ");
          fgets(novoComponente.observacao, sizeof(novoComponente.observacao),
                stdin);
          novoComponente.observacao[strcspn(novoComponente.observacao, "\n")] =
              '\0';

          printf("Novo número de série: ");
          scanf("%d", &novoComponente.numSerie);
          limparBuffer();

          printf("Nova garantia (anos): ");
          scanf("%d", &novoComponente.garantia);
          limparBuffer();

          printf("Novo ID Fornecedor: ");
          scanf("%d", &novoComponente.idFornecedor);
          limparBuffer();

          printf("Novo ID Fabricante: ");
          scanf("%d", &novoComponente.idFabricante);
          limparBuffer();

          printf("Novo ID Posto de Trabalho: ");
          scanf("%d", &novoComponente.idPostoTrabalho);
          limparBuffer();
          break;
        case 0:
          printf("Alterações guardadas.\n");
          break;
        default:
          printf("Opção inválida.\n");
        }
      } while (opcao != 0);

      // Escreve componente alterado no ficheiro temporário
      fprintf(temp, "%d;%d;%d;%d;%d;%s;%s;%s;%s;%d;%s\n", novoComponente.id,
              novoComponente.idFornecedor, novoComponente.idFabricante,
              novoComponente.idPostoTrabalho, novoComponente.numSerie,
              novoComponente.designacao, novoComponente.tipo,
              novoComponente.condicao, novoComponente.observacao,
              novoComponente.garantia, novoComponente.dataAquisicao);

      encontrado = 1;
    } else {
      // Mantém componente não alterado
      fprintf(temp, "%d;%d;%d;%d;%d;%s;%s;%s;%s;%d;%s\n", id, idFornecedor,
              idFabricante, idPostoTrabalho, numSerie, designacao, tipo,
              condicao, observacao, garantia, dataAquisicao);
    }
  }

  fclose(arq);
  fclose(temp);

  if (encontrado) {
    remove(FILE_NAME);
    rename("temp.txt", FILE_NAME);
    printf("Componente atualizado com sucesso.\n");
  } else {
    remove("temp.txt");
    printf("Componente com ID %d não encontrado.\n", optionId);
  }
}

void PesquisarComponentes() {
  printf("\n============ PESQUISAR COMPONENTE =============\n");
  char termo[100];
  printf(
      "Digite o designacao, condição ou parte da observação para pesquisar: ");
  getchar(); // limpar \n pendente
  fgets(termo, sizeof(termo), stdin);
  strtok(termo, "\n"); // tirar o \n no final

  FILE *arq = fopen(FILE_NAME, "r");
  if (arq == NULL) {
    printf("Erro ao abrir o ficheiro.\n");
    return;
  }

  int id, idFornecedor, idFabricante, idPostoTrabalho, numSerie, garantia;
  char tipo[100], designacao[100], condicao[100], observacao[255],
      dataAquisicao[20];
  int encontrado = 0;

  while (fscanf(arq,
                "%d;%d;%d;%d;%d;%99[^;];%99[^;];%99[^;];%254[^;];%d;%19[^\n]\n",
                &id, &idFornecedor, &idFabricante, &idPostoTrabalho, &numSerie,
                tipo, designacao, condicao, observacao, &garantia,
                dataAquisicao) == 11) {
    if (strstr(designacao, termo) != NULL || strstr(condicao, termo) != NULL ||
        strstr(observacao, termo) != NULL) {
      printf("\nID: %d\n", id);
      printf("\nID Fabricante: %d\n", idFabricante);
      printf("\nID Fornecerdor: %d\n", idFornecedor);
      printf("\nID Posto de trabalho: %d\n", idPostoTrabalho);
      printf("Nome: %s\n", designacao);
      printf("Tipo: %s\n", tipo);
      printf("Condiçã: %s\n", condicao);
      printf("Observação: %s\n", observacao);
      encontrado = 1;
    }
  }

  fclose(arq);

  if (!encontrado) {
    printf("\nNenhum componente encontrado com o termo \"%s\".\n", termo);
  }
}