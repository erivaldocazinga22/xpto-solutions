#include"../include/menu.h"
#include"../include/utils.h"
#include<stdio.h>
#include<string.h>
#include<time.h>

#define FILE_NAME_OPERATIONS "operacoes.txt"
#define FILE_NAME_COMPS "componentes.txt"

void listarComponentesAtrasados() {
    FILE *arqOps = fopen(FILE_NAME_OPERATIONS, "r");
    if (arqOps == NULL) {
        printf("Erro ao abrir o ficheiro de operações.\n");
        return;
    }

    // Obter data de hoje
    char dataHoje[11];
    obterDataActual(dataHoje, sizeof(dataHoje));

    int encontrou = 0;

    int numDoc, numDocExterno, idFunc, idEmpresa, idPosto, idComponente;
    float montante;
    char observacao[100], tipoOperacao[50], dataOperacao[11], dataEntrada[11],
         dataSaida[11], dataPreviaChegada[11];

    while (fscanf(arqOps, "%d;%d;%d;%d;%d;%d;%f;%99[^;];%49[^;];%10[^;];%10[^;];%10[^;];%10[^\n]\n",
        &numDoc, &numDocExterno, &idFunc, &idEmpresa, &idPosto, &idComponente,
        &montante, observacao, tipoOperacao, dataOperacao,
        dataEntrada, dataSaida, dataPreviaChegada) == 13) {

        if (dataEntrada == "null" && (strcmp(dataHoje, dataPreviaChegada) > 0)) {
            // Está atrasado — buscar componente no ficheiro de componentes
            FILE *arqComp = fopen(FILE_NAME_COMPS, "r");
            if (arqComp == NULL) {
                printf("Erro ao abrir o ficheiro de componentes.\n");
                fclose(arqOps);
                return;
            }

            int id, idFornecedor, idFabricante, idPostoTrabalho, numSerie, garantia;
            char tipo[100], designacao[100], condicao[100], obsComp[255];
            char dataAquisicao[20];

            while (fscanf(arqComp,
                        "%d;%d;%d;%d;%d;%99[^;];%99[^;];%99[^;];%254[^;];%d;%20[^\n]\n",
                        &id, &idFornecedor, &idFabricante, &idPostoTrabalho,
                        &numSerie, tipo, designacao, condicao, obsComp,
                        &garantia, dataAquisicao) == 11) {

                if (id == idComponente) {
                    printf("\n=========== COMPONENTE ATRASADO ===========\n");
                    // Mostrar informações do componente
                    printf("\n[Componente]\nID: %d\nFornecedor: %d\nFabricante: %d\nID do Posto: %d\n"
                           "Nº Série: %d\nTipo: %s\nDesignação: %s\nCondição: %s\nObservação: %s\n"
                           "Garantia: %d anos\nData de Aquisição: %s\nDocumento Nº: %d\nTipo: %s\nMontante: %.2f kz\n"
                           "Data da Operação: %s\n"
                           "Data Prevista de Chegada: %s\n",
                           id, idFornecedor, idFabricante, idPostoTrabalho,
                           numSerie, tipo, designacao, condicao, obsComp,
                           garantia, dataAquisicao,numDoc, tipoOperacao, montante,
                           dataOperacao,
                           dataPreviaChegada);

                    printf("============================================\n");

                    encontrou = 1;
                    break;
                }
            }

            fclose(arqComp);
        }
    }

    if (!encontrou) {
        printf("Nenhum componente atrasado encontrado.\n");
    }

    fclose(arqOps);
}

void listarComponentesPorPosto() {
    FILE *arq = fopen(FILE_NAME_OPERATIONS, "r");
    if (arq == NULL) {
        printf("Erro ao abrir o ficheiro.\n");
        return;
    }

    int OptionidPostoTrabalho, error = 0;
    do {
        printf("ID Posto Trabalho: ");
        scanf("%d", &OptionidPostoTrabalho);
        getchar();

        error = postoAlreadyExists(OptionidPostoTrabalho);
        if (!error)
            printf("ID do Posto de Trabalho não existe.\n");
    } while (!error);
    
    int id, idFornecedor, idFabricante, idPostoTrabalho, numSerie, garantia;
    char tipo[100], designacao[100], condicao[100], observacao[255];
    char dataAquisicao[20];
    int encontrou = 0;

    while (fscanf(arq,
                "%d;%d;%d;%d;%d;%99[^;];%99[^;];%99[^;];%254[^;];%d;%20[^\n]\n",
                &id, &idFornecedor, &idFabricante, &idPostoTrabalho, &numSerie,
                tipo, designacao, condicao, observacao, &garantia,
                dataAquisicao) == 11) {
                   if (idPostoTrabalho == OptionidPostoTrabalho) {
                    printf("\nID: %d\nFornecedor: %d\nFabricante: %d\nPosto: %d\nNº Série: "
           "%d\nTipo: %s\nDesignação: %s\nCondição: %s\nObservação: "
           "%s\nGarantia: %d anos\nData de Aquisição: %s\n",
           id, idFornecedor, idFabricante, idPostoTrabalho, numSerie, tipo,
           designacao, condicao, observacao, garantia, dataAquisicao);
                    encontrou = 1;
                   }           
    }

    if (!encontrou) {
        printf("\nNenhum componente neste posto encontrado.\n");
    }

    fclose(arq);

}

void consultarOperacaoPorDoc() {}
