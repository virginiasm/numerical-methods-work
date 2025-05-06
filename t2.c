/**
 * @file t2.c
 * @brief Trabalho desenvolvido para a disciplina Métodos Numéricos, o qual simula a chegada
 * de passageiros entre aeroportos usando o método de Gauss-Jacobi.
 * 
 * Este programa lê os dados de aerosportos e conexões dos arquivos disponibilizado para a simulação
 * Monta a" matrizdo sistema linear e aplica o método 
 * 
 * @author Virgínia Soares Muller
 * @date 06/05/2025
 * 
 */
#include "t2.h"

/**
 * @brief Procura o índice de um aeroporto dado seu nome.
 * @param aeros - vetor aeroportos
 * @param total - quantidade de aeroportos
 * @param nome - Nome do aeroporto procurado
 * @return idx do aero no vetor ou -1 para não encontrado
 */
int t2_EncontraAeroporto(Aeroportos aeros[], int total, const char *nome) {
    for (int idx = 0; idx < total; idx++) {
        if (strcmp(aeros[idx].nome, nome) == 0) {
            return idx;
        }
    }
    return -1;
}

/**
 * @brief Função para carregar os dados do arquivo para os vetores de aeroportos e rotas.
 * @param filename - arquivo de entrada
 * @param aeros - vetor de aeroportos
 * @param quantiAeros - ponteiro para a quantidade de aeroportos
 * @param rotas - vetor de rotas
 * @param quantiConexoes - ponteiro para a quantidade de conexões
 * @return retorna 1 para ok e 0 se ocorrer erro ao abrir arquivo
 */
int t2_CarregaDadosdoArquivo(const char *filename, Aeroportos aeros[], int *quantiAeros, Rota rotas[], int *quantiConexoes){
    FILE *file = fopen(filename, "r");
    char linha[100];
    *quantiAeros = *quantiConexoes = 0;

    if (!file) { 
        perror("Erro ao abrir arquivo"); 
        return 0; 
    }

    while (fgets(linha, sizeof(linha), file)) {
        char *token, *tokens[3];
        int   num = 0;
        token = strtok(linha, " \n");
        while (token && num < 3) {
            tokens[num++] = token;
            token = strtok(NULL, " \n");
        }

        if (num == 2) {
            char *cnome = tokens[0];
            int   pPassageiros  = atoi(tokens[1]);
            if (t2_EncontraAeroporto(aeros, *quantiAeros, cnome) < 0) {
                strcpy(aeros[*quantiAeros].nome, cnome);
                aeros[*quantiAeros].passageiros = pPassageiros;
                aeros[*quantiAeros].novosPassageiros = pPassageiros;
                (*quantiAeros)++;
            }
        }
        else if (num == 3) {
            char  *origem = tokens[0];
            char  *destino   = tokens[1];
            double pPercentual  = atof(tokens[2]) / 100.0;
            int   iDestino = t2_EncontraAeroporto(aeros, *quantiAeros, origem);
            int   iOrigem   = t2_EncontraAeroporto(aeros, *quantiAeros, destino);
            if (iDestino >= 0 && iOrigem >= 0) {
                rotas[(*quantiConexoes)++] = (Rota){iDestino, iOrigem, pPercentual};
            }
        }
    }

    fclose(file);
    return 1;
}

/**
 * @brief Constroi a matriz de coeficientes do sistema utlizado Gauss-Jacobi
 * @param matriz - matriz desenvolvida
 * @param quantiAeros - quantidade de aeroportos
 * @param rotas - vetor de rotas
 * @param quantiConexoes - quanti de rotas
 */
void vt2_ConstroiMatriz(double matriz[MAX_AEROPORTOS][MAX_AEROPORTOS], Aeroportos aeros[], int quantiAeros, Rota rotas[], int quantiConexoes){
    for (int i = 0; i < quantiAeros; i++) /* Inicializei a matriz em 0.0*/
        for (int j = 0; j < quantiAeros; j++)
            matriz[i][j] = 0.0;

    /* na diagonal, adiciono os valores de chegada de cada aeroporto
    EXEMPLO: AWI, SBF, NOE, OVH, EFI.
            AWI |   SBF |  NOE  |  OVH  |  EFI
    AWI |  249  |       |       |       |
    SBF |       |  609  |       |       |
    NOE |       |       |  407  |       |
    OVH |       |       |       |   656 |
    EFI |       |       |       |       | 443
    */
    for (int i = 0; i < quantiAeros; i++){
        matriz[i][i] = aeros[i].passageiros;
    }
    /* Adiciona as percentagens nas posições
    EXEMPLO:
           AWI  |   SBF |  NOE  |  OVH  |  EFI
    AWI |  249  |  0.4  |  0.35 |  0.0  |  0.95
    SBF |   0.0 |  609  |  0.45 |  0.64 |  0.0
    NOE |   0.0 |  0.0  |  407  |  0.0  |  0.0
    OVH |   0.0 |  0.45 |  0.08 |  656  |  0.0
    EFI | 0.05  |  0.12 |  0.05 |  0.0 v|  443
    
    */
   for (int k = 0; k < quantiConexoes; k++) {
    int origem = rotas[k].origem;
    int destino   = rotas[k].destino;
    matriz[destino][origem] = rotas[k].percentual;
}

}

/**
 * @brief Imprime a matriz de coeficientes
 * @param matriz - matriz desenvolvida
 * @param aeros - vetor de aeroportos
 * @param quantiAeros - quantidade de aeroportos
 */
void vt2_ImprimeMatriz(double matriz[MAX_AEROPORTOS][MAX_AEROPORTOS], Aeroportos aeros[], int quantiAeros){
    printf("       ");
    for (int j = 0; j < quantiAeros; j++) {
        printf("%6s ", aeros[j].nome);
    }
    printf("\n");
    printf("----------------------------------------------------------------\n");

    // linhas
    for (int i = 0; i < quantiAeros; i++) {
        printf("%4s |", aeros[i].nome);
        for (int j = 0; j < quantiAeros; j++) {
            printf(" %6.2f", matriz[i][j]);
        }
        printf("\n");
    }
    printf("-----------------------------------------------------------------\n");
}

/**
 * @brief Executa Método de Gauss-Jacobi 
 * @param aeros - vetor de aeroportos
 * @param quantiAeros - quantidade de aeroportos
 * @param rotas - vetor de totas
 * @param quantiConexoes - quantidade de conexões 
 */

void vt2_GaussJacobi(Aeroportos aeros[], int quantiAeros, Rota rotas[], int quantiConexoes){
    double vetorChegadas[MAX_AEROPORTOS];
    int iter = 0;
    for (int i = 0; i < quantiAeros; i++) {
        vetorChegadas[i] = aeros[i].passageiros;
    }
    while (1) {
        int changed = 0;

        for (int i = 0; i < quantiAeros; i++) {
            aeros[i].novosPassageiros = vetorChegadas[i];
        }
        for (int k = 0; k < quantiConexoes; k++) {
            int from = rotas[k].origem;
            int to   = rotas[k].destino;
            aeros[to].novosPassageiros += aeros[from].passageiros * rotas[k].percentual;
        }
        for (int i = 0; i < quantiAeros; i++) { /* Verifica convergêcia antes de atualizar */
            double erro = fabs(aeros[i].novosPassageiros - aeros[i].passageiros);
            if (erro > TOLERANCIA) {
                changed = 1;
            }
        }                                                                                      
        for (int i = 0; i < quantiAeros; i++) {
            aeros[i].passageiros = aeros[i].novosPassageiros;
        }
        if (!changed || ++iter > ITERACAO) {
            break;
        }
    }
}


/**
 * @brief Encontra o aeroporto com menor e maior número de pessoas
 * @param aeros - vetor de aeroportos
 * @param quantiAeros - quantidade de aeroportos
 * @param min - ponteiro para o valor minimo
 * @param max - ponteiro para o valor maximo
 * @param minNome - ponteiro para o nome do aeroporto com menor numero de pessoas
 * @param maxNome - ponteiro para o nome do aeroporto com maior numero de pessoas
 */

void vt2_EncontraMinMax(Aeroportos aeros[], int quantiAeros, double *min, char **minNome, double *max, char **maxNome){
    *min = *max = aeros[0].passageiros;
    *minNome = *maxNome = aeros[0].nome;
    for (int i = 1; i < quantiAeros; i++) {
        if (aeros[i].passageiros < *min) {
            *min = aeros[i].passageiros;
            *minNome = aeros[i].nome;
        }
        if (aeros[i].passageiros > *max) {
            *max = aeros[i].passageiros;
            *maxNome = aeros[i].nome;
        }
    }
}

/**
 * @brief Imprime os resultados finais da simulação
 * @param aeros - vetor de aeroportos
 * @param quantiAeros - quantidade de aeroportos
 */
void vt2_ImprimeResultados(Aeroportos aeros[], int quantiAeros) {
    double min, max;
    char *minNome, *maxNome;
    vt2_EncontraMinMax(aeros, quantiAeros, &min, &minNome, &max, &maxNome);

    printf("\nRESULTADO:\n");
    for (int i = 0; i < quantiAeros; i++) {
        printf(" %s = %.8lf\n", aeros[i].nome, aeros[i].passageiros);
      //  printf(" %s = %.6e\n", aeros[i].name, aeros[i].passengers);
    }
    printf("Menor Populacao: %s  - %.8lf - \n", minNome, min);
    printf("Maior Populacao: %s  - %.8lf - \n", maxNome, max);
}
