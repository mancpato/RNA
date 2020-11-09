/**
    RNA

    Estructura de la RNA y las funciones asociadas:

        CrearRNA        Se solicita el espacio de memoria para la RNA
        IniciarPesos    Se dan los valores iniciales a los pesos
        Clasificar      Determina la pertenencia a las clases de la entrada x[]
        Entrenar        Dada una entrada y la clase correcta, ajusta los pesos
        PesoInicial     Regresa un número para ser asignado a cada pesos
        Ruido           Un valor aleatorio entre 0 y 1
        f               Función de activación
        df              Derivada de la función de activación

    Miguel Angel NOrzagaray Cosío
    UABCS/DSC
*/

#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include "RNA.h"

/** \brief      Crea la RNA con todos sus parámetros iniciales
 *
 * \param int n             Cantidad de variables de entrada
 * \param int h             Cantidad de neuronas en la capa oculta
 * \param int k             Cantidad de clases en la salida
 * \param TasaDeAprendizaje     <-- eso mero
 * \param R struct RNA*     La RNA creada
 * \return struct RNA *     Apuntador a la RNA creada
 *
 */struct RNA *CrearRNA(int n, int h, int k, double TasaDeAprendizaje)
{
    struct RNA *R;

    if ( n!=N || h!=H || k!=K ) {
        puts("Topología incorrecta");
        return NULL;
    }

    if ( !(R = (struct RNA *)malloc(sizeof(struct RNA))) )
        return NULL;

    R->ni = n;
    R->nh = h;
    R->nk = k;
    R->Alpha = TasaDeAprendizaje;
    IniciarPesos(R);
    return R;
}

/** \brief
 *
 * \param R struct RNA*
 * \return void
 *
 */
void IniciarPesos(struct RNA *R)
{
    int i,j;

    for ( i=0; i<R->ni ; i++)
        for ( j=0 ; j<R->nh ; j++ )
            R->wh[i][j] = PesoInicial();
    for ( i=0; i<R->nh ; i++)
        for ( j=0 ; j<R->nk ; j++ )
            R->wy[i][j] = PesoInicial();

    for ( i=0; i<R->nh ; i++)
        R->bh[i] = PesoInicial();
    for ( i=0; i<R->nk ; i++)
        R->by[i] = PesoInicial();
}

/** \brief Genera el vector de pertenencia de un dato x[] a las K clases
 *
 * \param x[] double    Conjunto de N variables de la entrada por clasificar
 * \param R struct RNA* La RNA
 * \return double*      El vector de pertenencias
 *
 */
double *Clasificar(double x[], struct RNA *R)
{
    int j, k;
    double Sum;

    double *y = (double *)malloc(R->nk*sizeof(double));
    if ( !y )
        return NULL;

    for ( j=0 ; j < R->nh ; j++ ) {
        Sum = R->bh[j];
        for ( k=0 ; k < R->ni ; k++ )
            Sum += x[k]*R->wh[k][j];
        R->fh[j] = f(Sum);
    }

    for ( j=0 ; j < R->nk ; j++ ) {
        Sum = R->by[j];
        for (int k=0; k < R->nh ; k++)
            Sum += R->fh[k]*R->wy[k][j];
        R->fy[j] = f(Sum);
    }
    for ( j=0; j < R->nk ; j++ )
        y[j] = R->fy[j];

    return y;
}

/** \brief Entrenar Ajusta los pesos considerando la diferencia entre
    la salida estimada y[K] y la salida correcta out[K] para la
    entrada x[N].
 *
 * \param x double*     Conjunto de N variables de la entrada por clasificar
 * \param out double*   Clasificación correcta de x en las K clases
 * \param R struct RNA* La RNA
 *
 * \return double       El error cometido al clasificar x, antes de la
 *                      corrección de pesos. Solo por control, pues no se requiere
 *						para el parendizaje.
 */
double Entrenar(double *x, double *out, struct RNA *R)
{
    int j,k;
    double *y;
    double dy[K], dh[H];
    double Error, SError;

    y = Clasificar(x, R);

    SError = 0;
    // Cambio en la capa de salida
    for ( j=0 ; j<R->nk ; j++) {
        Error = out[j] - R->fy[j];
        dy[j] = Error*df(R->fy[j]);
        SError += Error*Error;
    }
    // Cambio en la capa oculta
    for ( j=0 ; j<R->nh ; j++) {
        Error = 0;
        for( k=0 ; k<R->nk ; k++ )
            Error += dy[k]*R->wy[j][k];
        dh[j] = Error*df(R->fh[j]);
    }

    // Actualización de pesos wy
    for (int j=0; j<R->nk; j++) {
        R->by[j] += dy[j]*R->Alpha;
        for (int k=0; k<R->nh ; k++)
            R->wy[k][j] += R->fh[k]*dy[j]*R->Alpha;
    }
    // Actualización de pesos wh
    for ( j=0 ; j<R->nh ; j++) {
        R->bh[j] += dh[j]*R->Alpha;
        for ( k=0; k<R->ni ; k++) {
            R->wh[k][j] += x[k]*dh[j]*R->Alpha;
        }
    }
    free(y);
    return sqrt(SError);
}

double PesoInicial()
{
    //return 0.5;	// Muy mala, el aprendizaje suele no converger
    return Ruido(1);/**< Pesos iniciales serán aleatorios en [0,1) */
    //return 1-Ruido(2);/**< Pesos iniciales aleatorios en (-1,1) */
}

double Ruido(double p)
{
    return p*(double)rand()/RAND_MAX;
}

double f(double x)
{
    return 1 / (1 + exp(-x));
}

double df(double x)
{
    return f(x) * (1 - f(x));
}

/* Fin de archivo */
