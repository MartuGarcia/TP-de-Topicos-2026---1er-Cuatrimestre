#ifndef FUNCIONES_H_INCLUDED
#define FUNCIONES_H_INCLUDED

//estructura para punto 1)
typedef struct
{
    int region;
    long cantRegistros;
    long sumaWHOG;
    long sumaWPER;
}Region;

//estructuras para punto 2)
typedef struct
{
    int pos_id;
    int pos_whog;
    int pos_wper;
    int pos_region;
    int pos_sexo;
    int pos_edad;
    int pos_tp_ocupacion;
    int pos_tp_trabajo;
    int pos_tp_tnr;
    int pos_tipo_hogar; //la agrego para el punto 3) y reutilizar la funcion de ObtenerPosicion
} PosicionesCampos;

typedef struct
{
    long id;
    long whog;
    long wper;
    int region;
    int sexo;
    int edad;
    int tp_ocupacion;
    int tp_trabajo;
    int tp_tnr;
    int tipo_hogar;
    char grupoEdad[50];
}Registros;

///vectores y memeoria dinamica
void* crearVector(size_t capInicial,size_t tamElem);
void destruirVector(void **vec);
int redimensionarVector(void **vec, size_t *cap, size_t tamElem); //redimenciono si es necesario
///matrices y memoria dinamica
void** crearMatriz(int filas, int columnas, size_t tam);
void inicializarMatriz(void **matriz, int filas, int columnas);
void destruirMatriz(void** matriz, int filas);


///funciones de string.h y stdlib.h
char* miStrcpy(char* destino, const char* origen);
char *miStrtok(char *s1, const char *s2);
int miStrcmp(const char *s1, const char *s2);
char* miStrchr(const char *cadena, int caracter);
int miAtoi(const char *numPtr);
double miAtof(const char *numPtr);
void* miMemcpy(void *destino, const void *origen, size_t tam);

/// Punto 1)
void inicializarRegion(Region *vecRegion);
void leerArchivoPunto1(const char *nombreArchivo);
int trozarLineaPunto1(char *linea,int *region,long *whog, long *wper);
void calculoPonderadoPorRegion(Region *vec, int region, long whog, long wper);
void mostrarPunto1(Region *vec);

/// Punto 2)
void leerArchivoPunto2(const char *nombreArchivo);
void sacarComillasCampos(char *str);
void obtenerPosicionesCampos(char *linea, PosicionesCampos *pos);
int trozarLineaPunto2(char *linea, Registros *reg, PosicionesCampos *pos);
void calcularGrupoEdad(int edad,char *grupoEdad);
int insertarElementoPunto2(void **vec, size_t *ce, size_t *cap, void *elem, size_t tamElem);
void mostrarPunto2(void *vec, size_t ce);

/// Punto 3)
//este punto trabaja con matrices dinamicas genericas
//reutilice funciones como obtenerPosicionesCampos y sacarComillasCampos para poder trozar correctamente
int trozarLineaPunto3(char *linea, Registros *reg, PosicionesCampos *pos);
void leerArchivoPunto3(const char *nombreArchivo);
void mostrarPunto3(void **mat, int filas, int columnas);


#endif // FUNCIONES_H_INCLUDED
