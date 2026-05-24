#ifndef FUNCIONES_H_INCLUDED
#define FUNCIONES_H_INCLUDED

//estructura
typedef struct
{
    int region;
    long cantRegistros;
    double sumaWHOG;
    double sumaWPER;
}Region;

//funciones de string.h y stdlib.h
char *miStrtok(char *s1, const char *s2);
char* miStrchr(const char *cadena, int caracter);
int miAtoi(const char *numPtr);
double miAtof(const char *numPtr);

// Punto 1)
void inicializarRegion(Region *vecRegion);
void leerArchivo(const char *nombreArchivo);
int trozarLinea(char *linea,int *region,double *whog, double *wper);
void calculoPonderadoPorRegion(Region *vec, int region, double whog, double wper);
void mostrarPunto1(Region *vec);
#endif // FUNCIONES_H_INCLUDED
