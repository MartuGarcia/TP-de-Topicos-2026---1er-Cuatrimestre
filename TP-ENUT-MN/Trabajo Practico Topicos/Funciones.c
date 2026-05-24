#include <stdio.h>
#include <stdlib.h>
#include "Prototipos.h"

#define MAXLINEA 1000
#define REGIONES 6

//variable global
const char* nombresRegiones[] =
{
    "GBA", "PAMPEANA", "NOROESTE",
    "NORESTE", "CUYO", "PATAGONIA"
};

void mostrarPunto1(Region *vec)
{
    Region *p = vec;
    Region *fin = vec + REGIONES;

    const char **nombre = nombresRegiones;

    printf("\n===============================================================================================\n");
printf("%-8s %-12s %-18s %-18s %-15s\n",
       "REGION", "REGISTROS", "HOGARES(WHOG)", "PERSONAS(WPER)", "NOMBRE");
printf("===============================================================================================\n");

while(p < fin)
{
    printf("%-8d %-12ld %-18.2f %-18.2f %-15s\n",
           p->region,
           p->cantRegistros,
           p->sumaWHOG,
           p->sumaWPER,
           *nombre);

    p++;
    nombre++;
}

    printf("=================================================================================\n");
}

void inicializarRegion(Region *vecRegion)
{
    Region *p = vecRegion;
    Region *fin = vecRegion + REGIONES;
    int codigo = 1;

    while(p < fin)
    {
        p->region = codigo++;
        p->cantRegistros = 0;
        p->sumaWHOG = 0;
        p->sumaWPER = 0;

        p++;
    }
}

void leerArchivo(const char *nombreArchivo)
{
    FILE *fp = fopen(nombreArchivo, "rt");
    if(!fp)
    {
        printf("Error al abrir archivo (enut2021_base.csv) \n");
        return;
    }
    char linea[MAXLINEA];
    int region;
    double whog, wper;

    Region regiones[REGIONES];
    inicializarRegion(regiones);

    fgets(linea, MAXLINEA, fp);

    while(fgets(linea, MAXLINEA, fp))
    {
        if(trozarLinea(linea, &region, &whog, &wper))
        {
            //por cada linea que trozo, llamo a esta funcion para realizar la suma
            calculoPonderadoPorRegion(regiones, region, whog, wper);
        }
    }

    fclose(fp);

    mostrarPunto1(regiones);
}

int trozarLinea(char *linea,int *region,double *whog, double *wper)
{
    char* actualLinea = linea;
    int numCampo = 0;

    *region =0;
    *whog =0;
    *wper =0;

    actualLinea = miStrtok(linea," \t\n");

    while(actualLinea != NULL)
    {
        if(!(*actualLinea == 'N' && *(actualLinea+1) == 'A'))
        {
            if(numCampo ==1)
            {
                *whog = miAtof(actualLinea);
            }
            else if (numCampo == 2)
            {
                *wper = miAtof(actualLinea);
            }
            else if (numCampo == 3)
            {
                *region = miAtoi(actualLinea);
            }
        }
        actualLinea = miStrtok(NULL," \t\n");
        numCampo++;
    }
    return (*region >= 1 && *region <= 6);
}

void calculoPonderadoPorRegion(Region *vec, int region, double whog, double wper)
{
    Region *r = vec + (region - 1); //me posiciono en la region correcta ya que va de [0 1 2 3 4 5]

    r->cantRegistros++;
    r->sumaWHOG += whog;
    r->sumaWPER += wper;
}

//funciones de string.h y stdlib.h

char *miStrtok(char *s1, const char *s2)
{
    static char *siguiente; //sirve para conservar su valor cuando termine la funcion
    char *inicio;

    if(s1 != NULL)
    {
        siguiente = s1;
    }

    if(siguiente == NULL)
    {
        return NULL;
    }

    while(*siguiente && miStrchr(s2,*siguiente) != NULL)
    {
        siguiente++;
    }

    if(*siguiente == '\0')
    {
        return NULL;
    }
    //guarda el inicio
    inicio = siguiente;

    while(*siguiente && miStrchr(s2,*siguiente) == NULL)
    {
        siguiente++;
    }

    if(*siguiente)
    {
        *siguiente = '\0';
        siguiente++;
    }

    return inicio;
}

char* miStrchr(const char *cadena, int caracter)
{
    const char* cad = cadena; //puntero que recorre la cadena
    while (*cad)//mientras no llegue al final de la cadena, al barra cero
    {
        if (*cad == caracter)//si el caracter es igual a lo que apunta el puntero
        {
            return (char *)cad; //retorna el puntero al primer carácter encontrado
        }
        cad++;
    }

    if(caracter == '\0')
        return (char *)cad;

    return NULL; //retorna NULL si no se encuentra el carácter
}

int miAtoi(const char *numPtr)
{
    int signo = 1;
    int resultado = 0;

    while(*numPtr == ' ' || *numPtr == '\t')
        numPtr++;

    if(*numPtr == '-')
    {
        signo = -1;
        numPtr++;
    }
    else if(*numPtr == '+')
    {
        numPtr++;
    }

    while(*numPtr >= '0' && *numPtr <= '9')
    {
        resultado = resultado * 10 + (*numPtr - '0');
        numPtr++;
    }

    return resultado * signo;
}

double miAtof(const char *numPtr)
{
    double resultado = 0.0;
    double divisor = 1.0;
    int signo = 1;

    while(*numPtr == ' ' || *numPtr == '\t')
        numPtr++;

    if(*numPtr == '-')
    {
        signo = -1;
        numPtr++;
    }
    else if(*numPtr == '+')
    {
        numPtr++;
    }

    while(*numPtr >= '0' && *numPtr <= '9')
    {
        resultado = resultado * 10 + (*numPtr - '0');
        numPtr++;
    }

    if(*numPtr == '.')
    {
        numPtr++;

        while(*numPtr >= '0' && *numPtr <= '9')
        {
            resultado = resultado * 10 + (*numPtr - '0');
            divisor *= 10;
            numPtr++;
        }
    }

    return signo * (resultado / divisor);
}
