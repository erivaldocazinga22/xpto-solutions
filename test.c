#include <stdio.h>
#include <time.h>

int main() {
  time_t agora;
  struct tm *dataHora;

  time(&agora);                 // Obtém o tempo actual em segundos desde 1970
  dataHora = localtime(&agora); // Converte para data/hora local

  printf("Data actual: %02d/%02d/%d\n", dataHora->tm_mday,
         dataHora->tm_mon + 1,      // meses vão de 0 a 11
         dataHora->tm_year + 1900); // anos desde 1900

  printf("Hora actual: %02d:%02d:%02d\n", dataHora->tm_hour, dataHora->tm_min,
         dataHora->tm_sec);

  return 0;
}