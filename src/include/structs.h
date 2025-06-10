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
  char dataAquisicao[MAX_STR];
  char tipo[MAX_STR];
  char designacao[MAX_STR];
  char condicao[MAX_STR]; // Novo, Utilizado
  char observacao[255];
} Componente;

typedef struct {
  int idComponente;
  int idSubComponente;
} ComponenteSubcomponente;

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
  int idFunc;
  float montante;
  char tipoOperacao[MAX_STR];
  char dataSaida[MAX_STR];
  char dataEntrada[MAX_STR];
  char dataPreviaChegada[MAX_STR];
  char dataOperacao[MAX_STR];
  char observacao[MAX_STR];
} Operacao;

#endif