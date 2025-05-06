#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "t2.h"

/**
 * Procura o índice de um aeroporto dado seu código.
 */
int t2_EncontraAeroporto(Aeroportos aeros[], int total, const char *nome) {
    for (int idx = 0; idx < total; idx++) {
        if (strcmp(aeros[idx].name, nome) == 0) {
            return idx;
        }
    }
    return -1;
}

int t2_CarregaDadosdoArquivo(const char *filename, Aeroportos aeros[], int *airport_count, Edge edges[], int *edge_count){
    FILE *file = fopen(filename, "r");
    if (!file) { perror("Erro ao abrir arquivo"); return 0; }

    char line[100];
    *airport_count = *edge_count = 0;

    while (fgets(line, sizeof(line), file)) {
        char *token, *tokens[3];
        int   num = 0;
        token = strtok(line, " \n");
        while (token && num < 3) {
            tokens[num++] = token;
            token = strtok(NULL, " \n");
        }

        if (num == 2) {
            // AEROPORTO PASSAGEIROS
            char *nm = tokens[0];
            int   p  = atoi(tokens[1]);
            if (t2_EncontraAeroporto(aeros, *airport_count, nm) < 0) {
                strcpy(aeros[*airport_count].name, nm);
                aeros[*airport_count].passengers     = p;
                aeros[*airport_count].new_passengers = p;
                (*airport_count)++;
            }
        }
        else if (num == 3) {
            char  *from = tokens[0];
            char  *to   = tokens[1];
            double pct  = atof(tokens[2]) / 100.0;
            int   i_from = t2_EncontraAeroporto(aeros, *airport_count, from);
            int   i_to   = t2_EncontraAeroporto(aeros, *airport_count, to);
            if (i_from >= 0 && i_to >= 0) {
                edges[(*edge_count)++] = (Edge){i_from, i_to, pct};
            }
        }
    }

    fclose(file);
    return 1;
}

/**
 * Constrói a matriz aumentada A (n×n) tal que:
 *   A[i][i] = b[i]   (chegadas diretas)
 *   A[i][j] = -P_{ij} para i != j
 */
void t2_ConstroiMatriz(double A[MAX_AIRPORTS][MAX_AIRPORTS], Aeroportos aeros[], int n, Edge edges[], int m){

    for (int i = 0; i < n; i++) /* Inicializei a matriz em 0.0*/
        for (int j = 0; j < n; j++)
            A[i][j] = 0.0;

    /* na diagonal, adiciono os valores de chegada de cada aeroporto
    EXEMPLO: AWI, SBF, NOE, OVH, EFI.
            AWI |   SBF |  NOE  |  OVH  |  EFI
    AWI |  249  |       |       |       |
    SBF |       |  609  |       |       |
    NOE |       |       |  407  |       |
    OVH |       |       |       |   656 |
    EFI |       |       |       |       | 443
    */
    for (int i = 0; i < n; i++) 
        A[i][i] = aeros[i].passengers;

    /* Adiciona as percentagens nas posições
    EXEMPLO:
           AWI  |   SBF |  NOE  |  OVH  |  EFI
    AWI |  249  |  0.4  |  0.35 |  0.0  |  0.95
    SBF |   0.0 |  609  |  0.45 |  0.64 |  0.0
    NOE |   0.0 |  0.0  |  407  |  0.0  |  0.0
    OVH |   0.0 |  0.45 |  0.08 |  656  |  0.0
    EFI | 0.05  |  0.12 |  0.05 |  0.0 v|  443
    
    */
   for (int k = 0; k < m; k++) {
    int from = edges[k].from;
    int to   = edges[k].to;
    A[to][from] = edges[k].percent;
}

}

/**
 * Imprime a matriz  A[i][j].
 */
void t2_ImprimeMatriz(double A[MAX_AIRPORTS][MAX_AIRPORTS], Aeroportos aeros[], int n){
    printf("       ");
    for (int j = 0; j < n; j++) {
        printf("%6s ", aeros[j].name);
    }
    printf("\n");
    printf("-----------------------------------------------------------\n");

    // linhas
    for (int i = 0; i < n; i++) {
        printf("%4s |", aeros[i].name);
        for (int j = 0; j < n; j++) {
            printf(" %6.2f", A[i][j]);
        }
        printf("\n");
    }
    printf("-----------------------------------------------------------\n");
}

void t2_GaussJacobi(Aeroportos aeros[], int n, Edge edges[], int m){
    double b[MAX_AIRPORTS];
    int iter = 0;
    for (int i = 0; i < n; i++) {
        b[i] = aeros[i].passengers;
    }
    while (1) {
        int changed = 0;

        // new_passengers = b
        for (int i = 0; i < n; i++) {
            aeros[i].new_passengers = b[i];
        }
        // new_passengers += P * x^(k)
        for (int k = 0; k < m; k++) {
            int from = edges[k].from;
            int to   = edges[k].to;
            aeros[to].new_passengers +=
                aeros[from].passengers * edges[k].percent;
        }
        // checa convergência antes de atualizar
        for (int i = 0; i < n; i++) {
            double erro = fabs(aeros[i].new_passengers - aeros[i].passengers);
            if (erro > TOLERANCIA) {
                changed = 1;
            }
        }                                                                                      
        for (int i = 0; i < n; i++) {
            aeros[i].passengers = aeros[i].new_passengers;
        }
        if (!changed || ++iter > ITERACAO) {
            break;
        }
    }
}

void t2_EncontraMinMax(Aeroportos aeros[], int n,
                      double *min, char **min_name,
                      double *max, char **max_name)
{
    *min = *max = aeros[0].passengers;
    *min_name = *max_name = aeros[0].name;
    for (int i = 1; i < n; i++) {
        if (aeros[i].passengers < *min) {
            *min = aeros[i].passengers;
            *min_name = aeros[i].name;
        }
        if (aeros[i].passengers > *max) {
            *max = aeros[i].passengers;
            *max_name = aeros[i].name;
        }
    }
}

void t2_ImprimeResultados(Aeroportos aeros[], int n) {
    double min, max;
    char *min_name, *max_name;
    t2_EncontraMinMax(aeros, n, &min, &min_name, &max, &max_name);

    printf("\nRESULTADOS FINAIS:\n");
    for (int i = 0; i < n; i++) {
        printf(" %s = %.8lf\n", aeros[i].name, aeros[i].passengers);
      //  printf(" %s = %.6e\n", aeros[i].name, aeros[i].passengers);
    }
    printf("Menor: %s (%.8lf)\n", min_name, min);
    printf("Maior: %s (%.8lf)\n", max_name, max);
}
