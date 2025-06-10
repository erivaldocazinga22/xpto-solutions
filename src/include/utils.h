#ifndef UTILS_H
#define UTILS_H

int postoAlreadyExists(int idPosto);
int fornecedorAlreadyExists(int idForncedor);
int fabricanteAlreadyExists(int idFabricante);
int funcionarioAlreadyExists(int idFunc);
int operacaoAlreadyExists(int idOpera);
int empresaAlreadyExists(int idEmpresa);
int componenteAlreadyExists(int idComponente);

void clear_screen();
void obterDataActual(char *buffer, int tamanho);

void folderDataAlreadyExists();

// utilitarios de pesquisa
void strToLower(char *dest, const char *src);
int matchWildcard(const char *pattern, const char *text);
#endif