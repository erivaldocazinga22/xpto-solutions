#include "include/menu.h"
#include "include/utils.h"
#include <stdio.h>

int main() {
  /* Funcao pricipal
   *
   * Menu principal da aplicacao
   * Do While - para garantir que a aplicacao nao seja encerrada em o user
   * pedir(digitar 0)
   */
  int opcao;

  do {
    clear_screen();
    printf("\n=== Sistema de Gestão de Equipamentos - XPTO Solutions ===\n");
    printf("1. Gerir Componentes\n");
    printf("2. Gerir Postos de Trabalho\n");
    printf("3. Gerir Funcionários\n");
    printf("4. Gerir Empresas\n");
    printf("5. Gerir Operações\n");
    printf("6. Consultas Especiais\n");
    /* printf("7. Estatísticas\n"); */
    printf("0. Sair\n");
    printf("Escolha uma opção: ");
    scanf("%d", &opcao);

    switch (opcao) {
    case 1:
      menuComponentes();
      break;
    case 2:
      menuPostosTrabalho();
      break;
    case 3:
      menuFuncionarios();
      break;
    case 4:
      menuEmpresas();
      break;
    case 5:
      menuOperacoes();
      break;
    case 6:
      menuConsultas();
      break;
    /* case 7:
      printf("Visualizar estatísticas\n");
      break; */
    case 0:
      printf("A terminar o programa...\n");
      break;
    default:
      printf("Opção inválida! Tente novamente.\n");
    }

  } while (opcao != 0);

  return 0;
}