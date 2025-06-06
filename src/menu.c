#include "include/menu.h"
#include <stdio.h>
#include<stdlib.h>

void menuComponentes() {
  int opcao;

  do {
    printf("\n--- Gestão de Componentes ---\n");
    printf("1. Inserir Componente\n");
    printf("2. Alterar Componente\n");
    printf("3. Apagar Componente\n");
    printf("4. Listar Componentes\n");
    printf("5. Troca de posto de trabalho - Componente\n");
    printf("6. Pesquisar Componente\n");
    printf("0. Voltar ao Menu Principal\n");
    printf("Escolha uma opção: ");
    scanf("%d", &opcao);

    switch (opcao) {
    case 1:
      InserirComponente();
      break;
    case 2:
      AlterarComponente();
      break;
    case 3:
      ApagarComponente();
      break;
    case 4:
      ListarComponentes();
      break;
    case 5:
      TrocaDePostoDeTrabalho();
      break;
    case 6:
      PesquisarComponentes();
      break;
    case 0:
      printf("A voltar ao Menu Principal...\n");
      break;
    default:
      printf("Opção inválida! Tente novamente.\n");
    }
  } while (opcao != 0);
}

void menuFuncionarios() {
  int opcao;
  do {
    printf("\n--- Gestão de Funcionários ---\n");
    printf("1. Inserir Funcionário\n");
    printf("2. Alterar Funcionário\n");
    printf("3. Apagar Funcionário\n");
    printf("4. Listar Funcionários\n");
    printf("5. Pesquisar Funcionário\n");
    printf("0. Voltar ao Menu Principal\n");
    printf("Escolha uma opção: ");
    scanf("%d", &opcao);
    
    switch (opcao) {
    case 1:
      InserirFuncionario();
      break;
    case 2:
      AlterarFuncionario();
      break;
    case 3:
      ApagarFuncionario();
      break;
    case 4:
      ListarFuncionarios();
      break;
    case 5:
      PesquisarFuncionario();
      break;
    case 0:
      printf("A voltar ao Menu Principal...\n");
      break;
    default:
      printf("Opção inválida! Tente novamente.\n");
    }
  } while (opcao != 0);
}

void menuPostosTrabalho() {
  int opcao;

  do {
    printf("\n--- Gestão de Postos de Trabalho ---\n");
    printf("1. Inserir Posto de Trabalho\n");
    printf("2. Alterar Posto de Trabalho\n");
    printf("3. Apagar Posto de Trabalho\n");
    printf("4. Listar Postos de Trabalho\n");
    printf("5. Pesquisar Posto de Trabalho\n");
    printf("0. Voltar ao Menu Principal\n");
    printf("Escolha uma opção: ");
    scanf("%d", &opcao);

    switch (opcao) {
    case 1:
      InserirPostoTrabalho();
      break;
    case 2:
      AlterarPostoTrabalho();
      break;
    case 3:
      ApagarPostoTrabalho();
      break;
    case 4:
      ListarPostosTrabalho();
      break;
    case 5:
      PesquisarPostoTrabalho();
      break;
    case 0:
      printf("A voltar ao Menu Principal...\n");
      break;
    default:
      printf("Opção inválida! Tente novamente.\n");
    }
  } while (opcao != 0);
}

void menuEmpresas() {
  int opcao;

  do {
    printf("\n--- Gestão de Empresas ---\n");
    printf("1. Inserir Empresa\n");
    printf("2. Alterar Empresa\n");
    printf("3. Apagar Empresa\n");
    printf("4. Listar Empresas\n");
    printf("5. Pesquisar Empresa\n");
    printf("0. Voltar ao Menu Principal\n");
    printf("Escolha uma opção: ");
    scanf("%d", &opcao);

    switch (opcao) {
    case 1:
      InserirEmpresa();
      break;
    case 2:
      AlterarEmpresa();
      break;
    case 3:
      ApagarEmpresa();
      break;
    case 4:
      ListarEmpresas();
      break;
    case 5:
      PesquisarEmpresa();
      break;
    case 0:
      printf("A voltar ao Menu Principal...\n");
      break;
    default:
      printf("Opção inválida! Tente novamente.\n");
    }
  } while (opcao != 0);
}

void menuOperacoes() {
  int opcao;

  do {
    printf("\n--- Gestão de Operações ---\n");
    printf("1. Inserir Operação\n");
    printf("2. Alterar Operação\n");
    printf("3. Apagar Operação\n");
    printf("4. Listar Operações\n");
    printf("5. Pesquisar Operação\n");
    printf("0. Voltar ao Menu Principal\n");
    printf("Escolha uma opção: ");
    scanf("%d", &opcao);

    switch (opcao) {
    case 1:
      InserirOperacao();
      break;
    case 2:
      printf("Alterar Operação\n");
      break;
    case 3:
      ApagarOperacao();
      break;
    case 4:
      ListarOperacoes();
      break;
    case 5:
      printf("Pesquisar Operação\n");
      break;
    case 0:
      printf("A voltar ao Menu Principal...\n");
      break;
    default:
      printf("Opção inválida! Tente novamente.\n");
    }
  } while (opcao != 0);
}

void menuConsultas() {
  int opcao;

  do {
    printf("\n--- Consultas Especiais ---\n");
    printf("1. Componentes com data prevista ultrapassada\n");
    printf("2. Componentes por Posto de Trabalho\n");
    printf("3. Consultar Operações por Documento\n");
    printf("4. Pesquisar Empresas\n");
    printf("5. Pesquisar Componentes\n");
    printf("0. Voltar ao Menu Principal\n");
    printf("Escolha uma opção: ");
    scanf("%d", &opcao);

    switch (opcao) {
    case 1:
      printf("Listar Componentes Atrasados\n");
      break;
    case 2:
      printf("Listar Componentes por Posto\n");
      break;
    case 3:
      printf("Consultar Operações por Documento\n");
      break;
    case 4:
      printf("Pesquisar Empresas\n");
      break;
    case 5:
      printf("Pesquisar Componentes\n");
      break;
    case 0:
      printf("A voltar ao Menu Principal...\n");
      break;
    default:
      printf("Opção inválida! Tente novamente.\n");
    }
  } while (opcao != 0);
}