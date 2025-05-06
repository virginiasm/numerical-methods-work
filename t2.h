#ifndef T2_H
#define T2_H

#define MAX_AIRPORTS    100
#define MAX_NAME        3
#define MAX_EDGES       500
#define TOLERANCIA      1e-6
#define ITERACAO        1000

typedef struct {
    char   name[MAX_NAME];
    double passengers;
    double new_passengers;
} Aeroportos;

typedef struct {
    int    from;
    int    to;
    double percent;
} Edge;

int t2_EncontraAeroporto(Aeroportos aeros[], int total, const char *nome);
int t2_CarregaDadosdoArquivo(const char *filename, Aeroportos aeros[], int *airport_count, Edge edges[], int *edge_count);
void t2_ConstroiMatriz(double A[MAX_AIRPORTS][MAX_AIRPORTS], Aeroportos aeros[], int n, Edge edges[], int m);
void t2_ImprimeMatriz(double A[MAX_AIRPORTS][MAX_AIRPORTS], Aeroportos aeros[], int n);
void t2_GaussJacobi(Aeroportos aeros[], int airport_count, Edge edges[], int edge_count);
void t2_EncontraMinMax(Aeroportos aeros[], int airport_count, double *min, char **min_name, double *max, char **max_name);
void t2_ImprimeResultados(Aeroportos aeros[], int airport_count);

#endif 
