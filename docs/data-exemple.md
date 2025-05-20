Sim, **é possível obter a data actual em C** usando a biblioteca `<time.h>`. Abaixo está um exemplo simples que mostra como obter e imprimir a data e hora actuais:

---

### ✅ Exemplo de código em C para obter a data e hora actual:

```c
#include <stdio.h>
#include <time.h>

int main() {
    time_t agora;
    struct tm *dataHora;

    time(&agora); // Obtém o tempo actual em segundos desde 1970
    dataHora = localtime(&agora); // Converte para data/hora local

    printf("Data actual: %02d/%02d/%d\n", 
           dataHora->tm_mday,
           dataHora->tm_mon + 1,  // meses vão de 0 a 11
           dataHora->tm_year + 1900); // anos desde 1900

    printf("Hora actual: %02d:%02d:%02d\n",
           dataHora->tm_hour,
           dataHora->tm_min,
           dataHora->tm_sec);

    return 0;
}
```

---

### 📌 Observações:

* `time_t` armazena o tempo em segundos desde 01/01/1970 (Unix epoch).
* `localtime()` converte esse valor para uma estrutura `tm` com dados como dia, mês, ano, hora, etc.
* É necessário **adicionar 1900 ao ano** e **1 ao mês**, porque a estrutura `tm` trabalha com valores baseados em 0.

---

Se quiseres guardar essa data num ficheiro ou utilizá-la para logs, também é possível formatar com `strftime`. Posso mostrar isso também se precisares.
