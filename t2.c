/*
NOME: VIRGINIA SOARES MULLER
DATA: 06/05/2025

*/

#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

#define N 10
#define TOL 1e-6
#define MAX_ITER 100

const char* variaveis[N] = {"SFL", "QBN", "JUQ", "QPZ", "XWK", "EXC", "YSY", "EGK", "UWF", "LUB"};

int indice_var(const char* nome) {
    for (int i = 0; i < N; i++) {
        if (strcmp(variaveis[i], nome) == 0)
            return i;
    }
    return -1;
}

void gauss_jacobi(double A[N][N], double b[N], double x[N]) {
    double x_old[N];
    int iter = 0;
    double erro;

    for (int i = 0; i < N; i++) x[i] = 0;

    do {
        for (int i = 0; i < N; i++) x_old[i] = x[i];

        for (int i = 0; i < N; i++) {
            double soma = b[i];
            for (int j = 0; j < N; j++) {
                if (j != i) soma -= A[i][j] * x_old[j];
            }
            x[i] = soma / A[i][i];
        }

        erro = 0;
        for (int i = 0; i < N; i++) {
            erro += fabs(x[i] - x_old[i]);
        }

        iter++;
    } while (erro > TOL && iter < MAX_ITER);
}

int main(){
    FILE *file = fopen("C:\\Users\\vivis\\Documents\\GitHub\\numerical-methods-work\\caso10.txt", "r");
    if(file == NULL){
        perror("Erro ao abrir o arquivo.");
        return -1;
    }

    char aeroportos[1000][4];
    double A[N][N], b[N], x[N];
    char linha[100], var1[10], var2[10];
    int idx1, idx2;
    double valor;

      // Inicializa A e b
      for (int i = 0; i < N; i++) {
        b[i] = 0;
        for (int j = 0; j < N; j++)
            A[i][j] = 0;
    }

      // Lê linha por linha
      while (fgets(linha, sizeof(linha), file)) {
        int count = sscanf(linha, "%s %s %lf", var1, var2, &valor);
        if (count == 2) {
            idx1 = indice_var(var1);
            b[idx1] = atof(var2);
        } else if (count == 3) {
            idx1 = indice_var(var1);
            idx2 = indice_var(var2);
            A[idx1][idx2] = valor;
        }
    }

    fclose(file);

      // Impressão da matriz A e vetor b
      printf("Matriz A (coeficientes):\n");
      for (int i = 0; i < N; i++) {
          for (int j = 0; j < N; j++) {
              printf("%6.2f ", A[i][j]);
          }
          printf("\n");
      }
  
      printf("\nVetor b (termos independentes):\n");
      for (int i = 0; i < N; i++) {
          printf("%s = %.2f\n", variaveis[i], b[i]);
      }
      printf("\n");

      gauss_jacobi(A, b, x);

      printf("Soluções:\n");
      for (int i = 0; i < N; i++) {
          printf("%s = %.6f\n", variaveis[i], x[i]);
      }
  
      return 0;
}

