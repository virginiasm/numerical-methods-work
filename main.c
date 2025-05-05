// main.c
#include <stdio.h>
#include "t2.h"

int main() {
    Aeroportos aeros[MAX_AIRPORTS];
    Edge    edges[MAX_EDGES];
    int     airport_count, edge_count;

    const char *dir = "C:\\Users\\vivis\\Documents\\GitHub\\numerical-methods-work\\caso10.txt";
    if (!t2_CarregaDadosdoArquivo(dir, aeros, &airport_count, edges, &edge_count)) {
        return 1;
    }

    printf("***************************************************************************************************\n");
    printf("                                        AEROPORTOS                                                  \n");
    printf("***************************************************************************************************\n");

    double b[MAX_AIRPORTS];
    for (int i = 0; i < airport_count; i++) {
        b[i] = aeros[i].passengers;
    }

    double A[MAX_AIRPORTS][MAX_AIRPORTS];
    t2_ConstroiMatriz(A, aeros, airport_count, edges, edge_count);

    printf("\nMatriz:\n");
    t2_ImprimeMatriz(A, aeros, airport_count);

    t2_GaussJacobi(aeros, airport_count, edges, edge_count);

    // --- Resultados finais ---
    printf("\n***************************************************************************************************\n");
    printf("                                       RESULTADOS FINAIS                                            \n");
    printf("***************************************************************************************************\n");
    t2_ImprimeResultados(aeros, airport_count);

    return 0;
}
