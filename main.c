/**
 * @file main.c
 * @brief Programa que executa a simulação.
 * 
 * 
 * - Leitura do arquivo de entrada a qual contém os dados dos aeroportos e conexões
 * - Construção da matriz do sistema linear
 * - Aplicação do método de gauss-jacobi
 * - Imprime os resultados fiansi da simulação
 * 
 * O programa deve ser executado com o caminnho do arquivo de entrada como argumento:
 * Exemplo de uso: ./gauss-jacobi cas010.txt
 * 
 * @author Virgínia Soares Müller
 * @date 06/05/2025
 * 
 */

 #include "main.h"

int main(int argc, char *argv[]) {
    Aeroportos aeros[MAX_AEROPORTOS];
    Rota    rotas[MAX_CONEXOES];
    int     quantiAeros, quantiConexao;
    double matriz[MAX_AEROPORTOS][MAX_AEROPORTOS];

    if (argc != 2) {
        fprintf(stderr, "Uso: %s <arquivo>\n", argv[0]);
        return 1;
    }
    const char *file = argv[1];
    if (!t2_CarregaDadosdoArquivo(file, aeros, &quantiAeros, rotas, &quantiConexao)) {
        return 1;
    }

    printf("***************************************************************************************************\n");
    printf("                                        AEROPORTOS                                                 \n");
    printf("***************************************************************************************************\n");

    vt2_ConstroiMatriz(matriz, aeros, quantiAeros, rotas, quantiConexao);

   // printf("\nMatriz:\n");
   // vt2_ImprimeMatriz(matriz, aeros, quantiAeros);

    /* Gauss–Jacobi */
    vt2_GaussJacobi(aeros, quantiAeros, rotas, quantiConexao);

    printf("***************************************************************************************************\n");
    printf("                                       RESULTADOS FINAIS                                           \n");
    printf("***************************************************************************************************\n");
    vt2_ImprimeResultados(aeros, quantiAeros);

    return 0;
}
