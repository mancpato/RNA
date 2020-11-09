/**
    RNA

    Clasificdor de puntos en el plano, por lo que las variables de
    entrada son N=2, x0 y x1.

    Se define la cantidad de clases como K, dando sus centroides en
    el arreglo Clases[K][N].

    El diseño es muy básico. Solo emplea la función sigmoide.

    Miguel Angel NOrzagaray Cosío
    UABCS/DSC
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "RNA.h"

int main()
{
    time_t t;
    int i,j,k;
    int NumVar = N;
    int NumClases = K;
    int Refinando = NO;

/***** Controles del experimento: *********

    Otros controles posibles son:
    - Cantidad H de neuronas ocultas en RNA.h
    - Pesos iniciales en la función PesoInicial de RNA.c
*/
    int Epocas = 20000;
    int MostrarAvance = SI;
    int MostrarPesos = NO;
    int CasosDePrueba = 2;
    double Alfa = .1;
    double Clase[4][2] = {{1,4}, {4,1}, {3,3}, {-1,-1}};
/***** Fin de los controles **************/

    double x[N], out[K];
    double Error[K], ErrorTotal;
    double *Tipo;
    struct RNA *R;
    FILE *fR;

    puts("Programa de ejemplo de RNA - Clasificador de puntos en el plano");
    puts("UABCS/DSC\n");

    srand((unsigned) time(&t));

    R = CrearRNA(N,H,K,Alfa);
    if ( !R ) {
        puts("¡Ups!");
        return 0;
    }
    printf("Red creada con Tasa de Aprendizaje:\tAlfa=%.3g\n",R->Alpha);
    fR = fopen("PesosRed.txt","wt");

    printf("Iniciando el entrenamiento. Epocas = %d\n",Epocas);
    for ( i=0 ; i<Epocas ; i++ ) {
        if ( MostrarAvance )
            printf("%5d:\t\t",i+1);
        ErrorTotal = 0;
        for ( j=0 ; j<NumClases ; j++ ) {
            x[0] = Clase[j][0] + (Epocas%2?-1:1)*Ruido(.25);
            x[1] = Clase[j][1] + (Epocas%2?-1:1)*Ruido(.25);
            memset(out,0.0,NumClases*sizeof(double));
            out[j] = 1;
            Error[j] = Entrenar(x,out,R);
            ErrorTotal += Error[j];
            if ( MostrarAvance )
                printf("Clase %d = %8.4f%%,\t", j, 100*Error[j]);
        }
        if ( 100*ErrorTotal < 5  &&  !Refinando ) {
            R->Alpha /= 10;
            printf("\nTasa de aprendizaje cambia a %f\n",R->Alpha);
            Refinando = SI;
            i = 0;
        }
        if ( MostrarAvance )
            printf("\r");
    }
    if ( MostrarAvance )
        printf("\n");
	puts("Aprendizaje terminado");

    if ( MostrarPesos ) {
        puts("\nPesos:\nW(h,n)");
        for ( j=0 ; j<R->nh ; j++ ) {
            for ( i=0 ; i<R->ni ; i++ )
                printf("%4.2f\t",R->wh[i][j]);
            putchar('\n');
        }
        for ( j=0 ; j<R->nh ; j++ )
            printf("b[%2d]=%4.2f\t",j,R->bh[j]);
        puts("\nW(k,h)");
        for ( j=0 ; j<R->nk ; j++ ) {
            for ( i=0 ; i<R->nh ; i++ )
                printf("%4.2f\t",R->wy[i][j]);
        putchar('\n');
        }
        for ( j=0 ; j<R->nk ; j++ )
            printf("b[%2d]=%4.2f\t",j,R->by[j]);
        putchar('\n');
    }
    printf("\nError final=%f%%\n", ErrorTotal);

    printf("Grabando red entrenada ... ");
    fprintf(fR,"%d\t%d\t%d\n",N,H,K);
    for ( j=0 ; j<R->nh ; j++ ) {
        for( i=0 ; i<R->ni ; i++ )
            fprintf(fR,"%f\t",R->wh[i][j]);
        fprintf(fR,"\n");
    }
    for ( j=0 ; j<R->nh ; j++ )
        fprintf(fR,"%f\t",R->bh[j]);
    fprintf(fR,"\n");
    for ( j=0 ; j<R->nk ; j++ ) {
        for ( i=0 ; i<R->nh ; i++ )
            fprintf(fR,"%f\t",R->wy[i][j]);
        fprintf(fR,"\n");
    }
    for ( j=0 ; j<R->nk ; j++ )
        fprintf(fR,"%f\t",R->by[j]);
    fprintf(fR,"\n\nError final=%f%%\n", ErrorTotal);
    fprintf(fR,"Epocas=%d\tTasa de Aprendizaje=%f\n",Epocas,Alfa);
    fprintf(fR,"UABCS/DSC/RNA/manc " __DATE__ __TIME__);

    puts(" Estructura y pesos grabados en 'PesosRed.txt'");
    puts("Si el aprendizaje fue adecuado, copie el archivo con otro nombre.");
    fclose(fR);

    puts("\nClasificando ejemplos:");
    for ( j=0 ; j<NumClases ; j++ )
        Error[j] = 0;
    for ( i=0 ; i<CasosDePrueba ; i++ ) {
        printf("Caso %d\n",i+1);
        for ( j=0 ; j<NumClases ; j++ ) {
            x[0] = Clase[j][0] + Ruido(.5) - Ruido(.5);
            x[1] = Clase[j][1] + Ruido(.5) - Ruido(.5);
            Tipo = Clasificar(x, R);
            Error[j] += fabs(1-Tipo[j]);
            printf(" [ ");
            for ( k=0 ; k<NumVar ; k++ )
                printf("%6.2f", x[k]);
            printf(" ]-> ");
            for ( k=0 ; k<NumClases ; k++ ) {
                printf("%6.4f ", Tipo[k]);
            }
            printf("\n");
        }
    }
    printf("\nError promedio por clase:");
    for ( j=0 ; j<NumClases ; j++ ) {
        Error[j] /= CasosDePrueba;
        printf("\t\t%.4g%%", 100*Error[j]);
    }
    return 0;
}

/* Fin de archivo */
