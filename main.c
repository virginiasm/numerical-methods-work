#include <stdio.h>
#include <string.h>
#include "t2.h"

int main(int argc, char *argv[]) {
    Aeroportos aeros[MAX_AIRPORTS];
    Edge    edges[MAX_EDGES];
    int     airport_count, edge_count;
    double A[MAX_AIRPORTS][MAX_AIRPORTS];

    if (argc != 2) {
        fprintf(stderr, "Uso: %s <arquivo>\n", argv[0]);
        return 1;
    }
    const char *file = argv[1];
    if (!t2_CarregaDadosdoArquivo(file, aeros, &airport_count, edges, &edge_count)) {
        return 1;
    }

    printf("***************************************************************************************************\n");
    printf("                                        AEROPORTOS                                                 \n");
    printf("***************************************************************************************************\n");

    t2_ConstroiMatriz(A, aeros, airport_count, edges, edge_count);

    printf("\nMatriz:\n");
    t2_ImprimeMatriz(A, aeros, airport_count);

    // Gauss–Jacobi
    t2_GaussJacobi(aeros, airport_count, edges, edge_count);

    printf("***************************************************************************************************\n");
    printf("                                       RESULTADOS FINAIS                                           \n");
    printf("***************************************************************************************************\n");
    t2_ImprimeResultados(aeros, airport_count);

    return 0;
}
