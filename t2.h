/**
 * @file t2.h
 * @brief Estuturas, definições e protótipos para execução do trabalho.
 * Utiliza o método de Gauss-Jacobi para resolver o problema proposto.abort
 * 
 * @author Virgínia Soares Müller
 * @date 06/05/2025
 */

#ifndef T2_H
#define T2_H

#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdio.h>

#define MAX_AEROPORTOS    100
#define MAX_NOME          3
#define MAX_CONEXOES      20000
#define TOLERANCIA        1e-6
#define ITERACAO          50

typedef struct {
    char   nome[MAX_NOME];
    double passageiros;
    double novosPassageiros;
} Aeroportos;

typedef struct {
    int    origem;
    int    destino;
    double percentual;
} Rota;

int t2_EncontraAeroporto(Aeroportos aeros[], int total, const char *nome);
int t2_CarregaDadosdoArquivo(const char *filename, Aeroportos aeros[], int *quantiAeros, Rota rotas[], int *quantiConexoes);
void vt2_ConstroiMatriz(double matriz[MAX_AEROPORTOS][MAX_AEROPORTOS], Aeroportos aeros[], int quantiAeros, Rota rotas[], int quantiConexoes);
void vt2_ImprimeMatriz(double matriz[MAX_AEROPORTOS][MAX_AEROPORTOS], Aeroportos aeros[], int quantiAeros);
void vt2_GaussJacobi(Aeroportos aeros[], int quantiAeros, Rota rotas[], int quantiConexoes);
void vt2_EncontraMinMax(Aeroportos aeros[], int quantiAeros, double *min, char **minNome, double *max, char **maxNome);
void vt2_ImprimeResultados(Aeroportos aeros[], int quantiAeros);

#endif 
