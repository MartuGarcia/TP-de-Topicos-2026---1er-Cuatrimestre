#include <stdio.h>
#include <stdlib.h>
#include "Prototipos.h"

#define MAXLINEA 5000
#define REGIONES 6

#define REDIMENCION 50
#define CAPINI 100

//variable global
const char* nombresRegiones[] =
{
    "GBA", "PAMPEANA", "NOROESTE",
    "NORESTE", "CUYO", "PATAGONIA"
};

///vectores y memoria dinamica
void* crearVector(size_t capInicial,size_t tamElem)
{
    void *vec = malloc (capInicial*tamElem);

    if(!vec)
    {
        printf("Error al crear el vector dinamico\n");
        return NULL;
    }
   return vec;
}
void destruirVector(void **vec)
{
    free(*vec);
    *vec = NULL;
}
int redimensionarVector(void **vec, size_t *cap, size_t tamElem)
{
    size_t capNueva = *cap + REDIMENCION;
    void* nuevo = realloc(*vec,capNueva*tamElem);

    if(!nuevo)
    {
        printf("Error al redimencionar el vector\n");
        return 0;
    }
    *vec = nuevo;
    *cap = capNueva;
    return 1;
}

///Punto 1)
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
    printf("%-8d %-12ld %-18ld %-18ld %-15s\n",
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

void leerArchivoPunto1(const char *nombreArchivo)
{
    FILE *fp = fopen(nombreArchivo, "rt");
    if(!fp)
    {
        printf("Error al abrir archivo (enut2021_base.csv) \n");
        return;
    }
    char linea[MAXLINEA];
    int region;
    long whog, wper;

    //Region regiones[REGIONES];
    //lo cambie para que se cree el vector por medio de la funcion y memeoria dinamica
    //por si en un futuro las regiones crecen y se necesita mas espacio
    Region *regiones = crearVector(REGIONES, sizeof(Region));
    if(!regiones)
    {
        fclose(fp);
        return;
    }

    inicializarRegion(regiones);

    fgets(linea, MAXLINEA, fp);

    while(fgets(linea, MAXLINEA, fp))
    {
        if(trozarLineaPunto1(linea, &region, &whog, &wper))
        {
            //por cada linea que trozo, llamo a esta funcion para realizar la suma
            calculoPonderadoPorRegion(regiones, region, whog, wper);
        }
    }

    fclose(fp);

    mostrarPunto1(regiones);
    destruirVector((void**)&regiones);
}

int trozarLineaPunto1(char *linea,int *region,long *whog, long *wper)
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
                *whog = miAtoi(actualLinea);
            }
            else if (numCampo == 2)
            {
                *wper = miAtoi(actualLinea);
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

void calculoPonderadoPorRegion(Region *vec, int region, long whog, long wper)
{
    Region *r = vec + (region - 1); //me posiciono en la region correcta ya que va de [0 1 2 3 4 5]

    r->cantRegistros++;
    r->sumaWHOG += whog;
    r->sumaWPER += wper;
}

///funciones de string.h y stdlib.h

void* miMemcpy(void* destino, const void* origen, size_t tam)
{
    // convierto a char* para copiar byte a byte
    char *d = (char*)destino;
    const char *o = (const char*)origen;

    // copio de adelante hacia atras
    while(tam)
    {
        *d = *o;
        d++;
        o++;
        tam--;
    }

    return destino;
}

char* miStrcpy(char* destino, const char* origen)
{
    char *dest = destino; //puntero que recorre el destino
    const char *orig = origen; //puntero que recorre el origen

    while(*orig)
    {
        *dest = *orig; //copia el caracter actual del origen al destino
        dest++;
        orig++;
    }
    *dest = '\0'; //agrega el barra cero al final del destino

    return destino; //retorna el puntero al inicio del destino
}

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
int miStrcmp(const char *s1, const char *s2)
{
    while ((*s1==*s2) && *s1 && *s2)
    {
        s1++;
        s2++;
    }
    return (unsigned char)*s1 - (unsigned char)*s2;
    //retorna la diferencia entre los caracteres no iguales o 0 si son iguales
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


///Punto 2)
void mostrarPunto2(void *vec, size_t ce)
{
    Registros *p = (Registros*)vec;
    Registros *fin = p + ce;
    //como son muchos registros los primeros no los muestra
    //asi que por eso puse un contador para ver si los primeros datos los mostraba bien
    //int cont =0;

    printf("\n==============================================================================================================================\n");
    printf("%-12s %-12s %-12s %-8s %-8s %-8s %-12s %-12s %-12s %-15s\n",
           "ID", "WHOG", "WPER", "REGION", "SEXO", "EDAD",
           "TP_OC", "TP_TR", "TP_TNR", "GRUPO_EDAD");
    printf("================================================================================================================================\n");

    while(p < fin /*&& cont <20*/)
    {
        printf("%-12ld %-12ld %-12ld %-8d %-8d %-8d %-12d %-12d %-12d %-15s\n",
               p->id,
               p->whog,
               p->wper,
               p->region,
               p->sexo,
               p->edad,
               p->tp_ocupacion,
               p->tp_trabajo,
               p->tp_tnr,
               p->grupoEdad);
               //cont++;
        p++;
    }

    printf("=======================================================================================================================\n");
}
void leerArchivoPunto2(const char *nombreArchivo)
{
    FILE *fp = fopen(nombreArchivo,"rt");
    if(!fp)
    {
        printf("Error al abrir archivo\n");
        return;
    }
    size_t ce = 0; //inicialmente hay 0 elementos en el vector de estructuras
    size_t cap = CAPINI;
    char linea[MAXLINEA];
    Registros reg;
    PosicionesCampos pos;

    void *vec = crearVector(CAPINI,sizeof(Registros)); //el tamaño de los elementos tiene que ser el de la estructura
    if(!vec)
    {
        fclose(fp);
        printf("Error al crear el vector dinamico, cierro el archivo");
        return;
    }

    fgets(linea,sizeof(linea),fp);
    obtenerPosicionesCampos(linea,&pos);
    while(fgets(linea,sizeof(linea),fp))
    {
        if(trozarLineaPunto2(linea,&reg,&pos))
        {
            calcularGrupoEdad(reg.edad,reg.grupoEdad);
            if(!insertarElementoPunto2(&vec, &ce, &cap, &reg, sizeof(Registros)))
            {
                destruirVector(&vec);
                fclose(fp);
                return;
            }
        }
    }
    fclose(fp);
    mostrarPunto2(vec,ce);
    destruirVector(&vec);
}
//los campos no estan todos ordenados entonces hay que buscarlos
void obtenerPosicionesCampos(char *linea, PosicionesCampos *pos)
{
    char *campo;
    int numCampo = 0;
    pos->pos_id = -1;
    pos->pos_whog = -1;
    pos->pos_wper = -1;
    pos->pos_region = -1;
    pos->pos_sexo = -1;
    pos->pos_edad = -1;
    pos->pos_tp_ocupacion = -1;
    pos->pos_tp_trabajo = -1;
    pos->pos_tp_tnr = -1;

    campo = miStrtok(linea, " \t\n");

    while(campo != NULL)
    {
        sacarComillasCampos(campo);
        if(miStrcmp(campo, "ID") == 0)
            pos->pos_id = numCampo;
        else if(miStrcmp(campo, "WHOG") == 0)
            pos->pos_whog = numCampo;
        else if(miStrcmp(campo, "WPER") == 0)
            pos->pos_wper = numCampo;
        else if(miStrcmp(campo, "REGION") == 0)
            pos->pos_region = numCampo;
        else if(miStrcmp(campo, "SEXO_SEL") == 0)
            pos->pos_sexo = numCampo;
        else if(miStrcmp(campo, "EDAD_SEL") == 0)
            pos->pos_edad = numCampo;
        else if(miStrcmp(campo, "TP_GRANGRUPO_OCUPACIONYAUTOCONSUMO") == 0)
            pos->pos_tp_ocupacion = numCampo;
        else if(miStrcmp(campo, "TP_GRANGRUPO_TRABAJOTOTAL") == 0)
            pos->pos_tp_trabajo = numCampo;
        else if(miStrcmp(campo, "TP_GRANGRUPO_TNR") == 0)
            pos->pos_tp_tnr = numCampo;

        campo = miStrtok(NULL, " \t\n");
        numCampo++;
    }
}
void sacarComillasCampos(char *str)
{
    char *inicio = str;

    if(*inicio == '"')
    {
        char *src = inicio + 1;
        char *dst = inicio;

        while(*src)
            *dst++ = *src++;

        *dst = '\0';
    }

    char *fin = str;
    while(*fin)
        fin++;

    while(fin > str && (*(fin-1) == '"' || *(fin-1) == '\r' || *(fin-1) == '\n'))
    {
        *(fin-1) = '\0';
        fin--;
    }
}
int trozarLineaPunto2(char *linea, Registros *reg, PosicionesCampos *pos)
{
    char *campo;//recorre el campo
    int numCampo =0;   //contador de posicion de campo
    //inicializo los campos
    reg->id = 0;
    reg->whog = 0;
    reg->wper = 0;
    reg->region = 0;
    reg->sexo = 0;
    reg->edad = 0;
    reg->tp_ocupacion = 0;
    reg->tp_trabajo = 0;
    reg->tp_tnr = 0;
    reg->grupoEdad[0] = '\0';


    campo = miStrtok(linea, " \t\n");

    while(campo != NULL)
    {
        sacarComillasCampos(campo);
        if(miStrcmp(campo,"NA")!=0)
        {
            if(numCampo == pos->pos_id)
                reg->id = miAtoi(campo);

            else if(numCampo == pos->pos_whog)
                reg->whog = miAtoi(campo);

            else if(numCampo == pos->pos_wper)
                reg->wper = miAtoi(campo);

            else if(numCampo == pos->pos_region)
                reg->region = miAtoi(campo);

            else if(numCampo == pos->pos_sexo)
                reg->sexo = miAtoi(campo);

            else if(numCampo == pos->pos_edad)
                reg->edad = miAtoi(campo);

            else if(numCampo == pos->pos_tp_ocupacion)
                reg->tp_ocupacion = miAtoi(campo);

            else if(numCampo == pos->pos_tp_trabajo)
                reg->tp_trabajo = miAtoi(campo);

            else if(numCampo == pos->pos_tp_tnr)
                reg->tp_tnr = miAtoi(campo);
        }

        campo = miStrtok(NULL, " \t\n");
        numCampo++;
    }
    //printf("REGION LEIDA: %d\n", reg->region);

    return (reg->region >= 1 && reg->region <= 6);
}
void calcularGrupoEdad(int edad,char *grupoEdad)
{
    if(edad >= 14 && edad <= 29)
        miStrcpy(grupoEdad, "14 a 29 anios");
    else if(edad >= 30 && edad <= 64)
        miStrcpy(grupoEdad, "30 a 64 anios");
    else if(edad >= 65)
        miStrcpy(grupoEdad, "65 anios y mas");

}

int insertarElementoPunto2(void **vec, size_t *ce, size_t *cap, void *elem, size_t tamElem)
{
    if(*ce >= *cap)
    {
        if(!redimensionarVector(vec, cap, tamElem))
        {
            printf("Error al redimencionar el tamanio del vector");
            return 0;
        }
    }

    //inserccion generica con char* por eso hay que multiplicar por el tamaño del elemento
    char *destino = (char*)(*vec) + (*ce * tamElem);

    miMemcpy(destino, elem, tamElem);

    (*ce)++;

    return 1;
}

