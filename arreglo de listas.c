#include "string.h"
#include <stdlib.h>
#include <stdio.h>
int const dim = 10;

typedef struct { 
   int nota; 
   int legajo;
   char nombreApe[40]; 
   char materia[40]; 
   int idMateria;
} registroArchivo; 

typedef struct { 
   int nota; 
   int legajo;
   char nombreApe[40];
} notaAlumno;

typedef struct _nodo { 
   notaAlumno dato; 
   struct _nodo *siguiente; 
} nodo;

typedef struct { 
    int idMateria;
    char materia[40]; 
    nodo * listaDeNotas; 
} celda; 

///PROTOTIPADO DE FUNCIONES PARA LISTA
nodo* inicLista();
nodo* crearNodo(int nota, char nombreApe[]);
nodo* insertarNodo(nodo* lista, nodo* nuevoNodo);

///PROTOTIPADO DE FUNCION PARA CARGAR EL ARCHIVO
registroArchivo cargarArchivo();
void mostrarArchivo();

void pasarArchivoToADL(celda arreglo[], int validos);

void mostrarArregloDeListas(celda arreglo[], int validos);

void pasarDeADLToArchivoDeAprobados(celda arreglo[], int validos);

void mostrarArchivoAprobados();

///PROTOTIPADO DE FUNCIONES PARA ESTRUCTURAS COMPUESTAS
int ingresarAlumno(celda arreglo[], int dimension);
int alta(celda arreglo[], int nota, int legajo, char nombreApe[], char materia[],int idmateria, int validos);
int buscarPosMateria(celda arreglo[], char materia[], int validos);
int agregarMateria(celda arreglo[], char materia[], int validos);

int main(){
int opc, validos;
celda arreglo[dim];
char eleccion = 's';
while(eleccion=='s'){
cargarArchivo();
printf("desea cargar otro alumno al archivo: ");
fflush(stdin);
scanf(" %c", &eleccion);
}
printf("ARCHIVO DE ALUMNOS\n");
mostrarArchivo();

validos = ingresarAlumno(arreglo, 10);

printf("ARREGLO DE ALUMNOS CARGADO CON EL ARCHIVO\n");
pasarArchivoToADL(arreglo, validos);


pasarDeADLToArchivoDeAprobados(arreglo, validos);
printf("ARCHIVO CON LOS ALUMNOS APROBADOS\n");
mostrarArchivoAprobados();




return 0;
}


///------------------FUNCIONES PARA EL MANEJO DE LISTAS-----------------

nodo* inicLista(){
    return NULL;
}

nodo* crearNodo(int nota, char nombreApe[]){
    nodo* aux = (nodo*)malloc(sizeof(nodo));
    aux->dato.nota = nota;
    strcpy(aux->dato.nombreApe, nombreApe);
    aux->siguiente = NULL;

    return aux;
}

nodo* insertarNodo(nodo* lista, nodo* nuevoNodo){
    if(lista==NULL){
        lista = nuevoNodo;
    }else{
        nuevoNodo->siguiente = lista;
        lista = nuevoNodo;
    }
    return lista;
}



///------------------FUNCIONES PARA EL MANEJO DE ARCHIVOS-----------------

registroArchivo cargarArchivo(){
    FILE *archi;
    registroArchivo aux;
    archi = fopen("miarchivo.bin", "ab");

    if(archi!=NULL){
        printf("Ingrese la nota del alumno: ");
        fflush(stdin);
        scanf("%i", &aux.nota);
        printf("Ingrese el legajo del alumno: ");
        fflush(stdin);
        scanf("%i", &aux.legajo);
        printf("Ingrese el nombre del alumno: ");
        fflush(stdin);
        gets(aux.nombreApe);
        printf("Ingrese la materia: ");
        fflush(stdin);
        gets(aux.materia);
        printf("Ingrese el id de la materia: ");
        fflush(stdin);
        scanf("%i", &aux.idMateria);
        fwrite(&aux,sizeof(registroArchivo), 1, archi);

        fclose(archi);
    }
    return aux;
}

void mostrarArchivo(){
    FILE *archi;
    registroArchivo aux;
    archi = fopen("miarchivo.bin", "rb");

    if(archi!=NULL){
        while(fread(&aux,sizeof(registroArchivo),1,archi)==1){
            puts("------------------------------\n");
            printf("nota: %i\n", aux.nota);
            printf("legajo: %i\n", aux.legajo);
            printf("nombre: %s\n", aux.nombreApe);
            printf("materia: %s\n", aux.materia);
            printf("idMateria: %i\n", aux.idMateria);
            puts("------------------------------");
        }
        fclose(archi);
    }
}


///funciones para el uso de estructuras compuestas

int ingresarAlumno(celda arreglo[], int dimension){
int nota;
int legajo;
char nombreApe[20];
char materia[20];
int idMateria;
int validos = 0;
char eleccion = 's';

while(eleccion == 's' && validos<dimension){
printf("ingrese la nota del alumno: ");
fflush(stdin);
scanf("%d", &nota);
printf("ingrese el legajo del alumno: ");
fflush(stdin);
scanf("%d", &legajo);
printf("ingrese el nombre y apellido del alumno: ");
fflush(stdin);
gets(nombreApe);
printf("ingrese la materia: ");
fflush(stdin);
gets(materia);
printf("ingrese el id de la materia: ");
fflush(stdin);
scanf("%i", &idMateria);
validos = alta(arreglo,nota,legajo,nombreApe,materia,idMateria,validos);
printf("desea cargar otro alumno: ");
fflush(stdin);
scanf("%c",&eleccion);
}

return validos;
}

int alta(celda arreglo[], int nota, int legajo, char nombreApe[], char materia[],int idmateria, int validos) {
    nodo* aux = crearNodo(nota, nombreApe);
    int pos = buscarPosMateria(arreglo, materia, validos);
    if(pos==-1){
        validos = agregarMateria(arreglo, materia, validos);
        pos = validos -1;
    }
    arreglo[pos].listaDeNotas = insertarNodo(arreglo[pos].listaDeNotas, aux);

    return validos;
}

int buscarPosMateria(celda arreglo[], char materia[], int validos) {
    int rta = -1;
    int i = 0;
    while((i<validos) && (rta==-1)){
        if(strcmp(arreglo[i].materia,materia)==0){
            rta = i;
        }
        i++;
    }
    return rta;
}

int agregarMateria(celda arreglo[], char materia[], int validos){
strcpy(arreglo[validos].materia,materia);
///copio la materia en el arreglo
arreglo[validos].listaDeNotas=inicLista();
///inicializo la lista de esa materia
validos++;
///incremento la cantidad de materias

return validos; ///devuelvo la cantidad de materias en el arreglo
}

void pasarArchivoToADL(celda arreglo[], int validos) {
    FILE *archi;
    registroArchivo aux;
    
    archi = fopen("miarchivo.bin", "rb");

    if (archi != NULL) {
        int i = 0;
        while (fread(&aux, sizeof(registroArchivo), 1, archi) == 1 && i < validos) {
            nodo* nuevoNodo = (nodo*)malloc(sizeof(nodo));
            nuevoNodo->dato.nota = aux.nota;
            nuevoNodo->dato.legajo = aux.legajo;
            strcpy(nuevoNodo->dato.nombreApe, aux.nombreApe);
            nuevoNodo->siguiente = arreglo[i].listaDeNotas;
            arreglo[i].listaDeNotas = nuevoNodo;
            arreglo[i].idMateria = aux.idMateria;
            strcpy(arreglo[i].materia, aux.materia);
            i++;
        }

        fclose(archi);
        mostrarArregloDeListas(arreglo, validos);
    }
}




void mostrarArregloDeListas(celda arreglo[], int validos) {
    int i;
    for (i=0; i < validos; i++) {
        printf("------------------------------\n");
        printf("Materia: %s\n", arreglo[i].materia);
        printf("ID de Materia: %i\n", arreglo[i].idMateria);
        printf("Notas de Alumnos:\n");

        nodo* actual = arreglo[i].listaDeNotas;
        while (actual != NULL) {
            printf("  - Nota: %i, Legajo: %i, Nombre: %s\n", actual->dato.nota, actual->dato.legajo, actual->dato.nombreApe);
            actual = actual->siguiente;
        }

        printf("------------------------------\n");
    }
}


void pasarDeADLToArchivoDeAprobados(celda arreglo[], int validos) {
    FILE *archi;
    registroArchivo aux;

    archi = fopen("aprobados.bin", "wb");
    
    if (archi != NULL) {
        for (int i = 0; i < validos; i++) {
            nodo* actual = arreglo[i].listaDeNotas;

            while (actual != NULL) {
                if (actual->dato.nota >= 6) {
                    aux.nota = actual->dato.nota;
                    aux.legajo = actual->dato.legajo;
                    strcpy(aux.nombreApe, actual->dato.nombreApe);
                    strcpy(aux.materia, arreglo[i].materia);
                    aux.idMateria = arreglo[i].idMateria;
                    fwrite(&aux, sizeof(registroArchivo), 1, archi);
                }

                actual = actual->siguiente;
            }
        }

        fclose(archi);
    }
}


void mostrarArchivoAprobados(){
    FILE *archi;
    registroArchivo aux;
    archi = fopen("aprobados.bin", "rb");

    if(archi!=NULL){
        while (fread(&aux,sizeof(registroArchivo),1,archi)==1){
            printf("------------------------------\n");
                        printf("nota: %i\n", aux.nota);
                        printf("legajo: %i\n", aux.legajo);
                        printf("nombre: %s\n", aux.nombreApe);
                        printf("materia: %s\n", aux.materia);
                        printf("idMateria: %i\n", aux.idMateria);
                        puts("------------------------------");
        }
        {
       
        }
        fclose(archi);
    }
}