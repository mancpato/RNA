#ifndef RNA_H_INCLUDED
#define RNA_H_INCLUDED

/// Neuronas por capa
#define N 2     /// entradas x[i]
#define H 4     /// capa oculta
#define K 4     /// salidas

#define SI 1
#define NO 0

struct RNA {
    /** Neuronas por capa */
    int ni;  // entradas
    int nh;  // ocultas
    int nk;  // salidas
    /** Pesos de cada capa */
    double wh[N][H];   // ni*nh
    double wy[H][K];   // nh*nk
    /** Ruido de cada capa */
    double bh[H];     // nh
    double by[K];     // nk
    /** Activación por capa */
    double fh[H];     // nh
    double fy[K];     // nk

    double Alpha;   // Tasa de aprendizaje
};

struct RNA *CrearRNA(int n, int h, int k, double TasaDeAprendizaje);
void IniciarPesos(struct RNA *R);
double *Clasificar(double x[], struct RNA *R);
double Entrenar(double *x, double *out, struct RNA *R);

double f(double x);
double df(double x);

double PesoInicial();
double Ruido(double);

#endif // RNA_H_INCLUDED
