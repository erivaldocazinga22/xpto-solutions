#ifndef STRUCTS_H
#define STRUCTS_H

#define MAX_STR 100
typedef struct {
  int id;
  char nome[MAX_STR];
  char funcao[MAX_STR];
  char descricao[MAX_STR];
} Funcionario;

typedef struct {
  int id;
  char nome[MAX_STR];
  char tipo[MAX_STR];
  char contacto[MAX_STR];
} Empresa;

typedef struct {
  int id;
  int idFunc;
  char nome[MAX_STR];
  char local[MAX_STR];
  char seccao[MAX_STR];
  char descricao[MAX_STR];

} PostoTrabalho;

typedef struct {
  int id;
  int idFornecedor;
  int idFabricante;
  int idPostoTrabalho;
  int numSerie;
  int garantia;
  char dataAquisicao[20];
  char tipo[MAX_STR];
  char designacao[MAX_STR];
  char condicao[MAX_STR]; // Novo, Utilizado
  char observacao[255];
} Componente;

/* identificação, designação, número de série, data de
aquisição, garantia, identificação do fornecedor, identificação do fabricante,
tipo, condição(Novo, Utilizado), identificação do posto de trabalho e
observações */
typedef struct {
  int numDoc;
  int numDocExterno;
  int idPosto;
  int idComponente;
  int idEmpresa;
  int tipoOperacao;
  char dataSaida[10];
  char dataEntrada[10];
  char dataPreviaChegada[10];
  int idFunc;
  char dataOperacao[10];
  float montante;
  char observacoa[MAX_STR];
} Operacao;

/* incluindo o número do documento interno, número do documento
externo, identificação do posto de trabalho, identificação do componente,
identificação da empresa, tipo de operação, data de saída, data de chegada, data
prevista de chegada, identificação do funcionário responsável, data da operação,
montante, observações; */
#endif