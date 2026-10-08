#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <malloc.h>
#include <ctype.h>
#define MAX 2000


/*
Basándonos en la información obtenida, podemos sacar las siguientes conclusiones:

-La LSOBB muestra los costos promedios más altos para las operaciones de alta y
baja debido a los corrimientos necesarios para mantener el arreglo ordenado,
pero presenta los costos más bajos en la evocación gracias a la búsqueda binaria con biseccion.

-El ABB muestra un comportamiento sumamente eficiente y equilibrado, manteniendo
costos promedios mínimos para altas y bajas, y costos de evocación (tanto éxito como fracaso)
muy bajos, apenas superiores a los de la LSOBB.

-La LVO +inf tiene los costos promedios más bajos para operaciones de ingreso y
eliminación, pero el costo promedio más alto para operaciones de evocación al
requerir un recorrido secuencial sobre la estructura.

En base a estos puntos, la elección de la estructura de datos dependerá de las prioridades del sistema.
Si se prioriza la eficiencia solo en operaciones de altas y bajas, la LVO +inf sería la mejor opción.
Si se prioriza la eficiencia solo en operaciones de evocación, la LSOBB sería la mejor opción.
Si se quiere tener un equilibrio entre todas las operaciones, la mejor opción sería el ABB,
que mantiene costos de alta y baja prácticamente despreciables y costos de evocación
considerablemente menores que los de la LVO +inf, compitiendo directamente con la LSOBB.

 ________________________________________________________________________
| Operacion / Metrica       |   LVO +inf   |    LSOBB     |     ABB      |
|===========================|==============|==============|==============|
| Alta                      |              |              |              |
|   Cantidad                |       2746   |       2745   |       2746   |
|   Costo Maximo            |       1.00   |    1932.00   |       0.50   |
|   Costo Promedio          |       1.00   |     413.07   |       0.50   |
|---------------------------|--------------|--------------|--------------|
| Baja                      |              |              |              |
|   Cantidad                |       2001   |       2000   |       2001   |
|   Costo Maximo            |       0.50   |    1893.00   |       1.50   |
|   Costo Promedio          |       0.50   |     502.61   |       1.02   |
|---------------------------|--------------|--------------|--------------|
| Evocar exitoso            |              |              |              |
|   Cantidad                |       2175   |       2174   |       2175   |
|   Costo Maximo            |    1994.00   |      11.00   |      22.00   |
|   Costo Promedio          |     566.80   |      10.14   |      11.84   |
|---------------------------|--------------|--------------|--------------|
| Evocar fracaso            |              |              |              |
|   Cantidad                |       2051   |       2052   |       2051   |
|   Costo Maximo            |    1206.00   |      11.00   |      21.00   |
|   Costo Promedio          |     463.92   |       9.89   |      12.04   |
|___________________________|______________|______________|______________|

*/

///Estructuras y funciones

typedef struct{
    int dni;
    char nombreyapellido[50];
    char domicilio[80];
    int codigopostal;
    int nmesa;
    int circuito;
}persona;

int compararPersonas(persona persona1, persona persona2){
    if(persona1.dni == persona2.dni && (strcmp(persona1.nombreyapellido,persona2.nombreyapellido) == 0) &&
       (strcmp(persona1.domicilio,persona2.domicilio) == 0) && persona1.codigopostal == persona2.codigopostal &&
    persona1.nmesa == persona2.nmesa && persona1.circuito == persona2.circuito){
        return 0; ///EXITO MISMA PERSONA
    }
    else{
        return 1; ///FRACASO PERSONA DISTINTA
    }
}

///LVO

typedef struct Nodo{
    persona personas;
    struct Nodo *siguiente;
}Nodo;

typedef struct{
    Nodo *acceso;
    Nodo *cursor;
    Nodo *cursorAux;
    int cantidad;
}lvo;

void inicializarEstructuraLVO(lvo *lvo){

    lvo->acceso = (Nodo*)malloc(sizeof(Nodo)); ///Pido memoria para la Marca
    if (lvo->acceso != NULL) {
        lvo->acceso->personas.dni = 999999999;
        lvo->acceso->siguiente = NULL;
    }
    lvo->cursor = lvo->acceso;
    lvo->cursorAux = NULL;
    lvo->cantidad = 0;
}

int localizarLVO(int dnibuscado, lvo *lvo, float *costo){
    lvo->cursor = lvo->acceso;
    lvo->cursorAux = lvo->acceso;
    *costo = 0.0;
    while(lvo->cursor->personas.dni < dnibuscado){
        *costo = *costo + 1; ///Costo por nodo consultado
        lvo->cursorAux = lvo->cursor;
        lvo->cursor = lvo->cursor->siguiente;
    }
    *costo = *costo + 1; ///Suma costo cuando sale
    if(lvo->cursor->personas.dni == dnibuscado){
        return 0; ///Exito
    }
    else{
        return 1; ///Fracaso
    }
}

int altaLVO(persona alta, lvo *lvo, float *costo){
    float costolocalizar = 0;
    int estado;
    *costo = 0.0;
    Nodo *posibleAlta = (Nodo*)malloc(sizeof(Nodo));
    if (posibleAlta == NULL){
        return 1; ///No puedo dar de alta
    }
    estado = localizarLVO(alta.dni, lvo, &costolocalizar);
    if(estado == 0){
        free(posibleAlta); ///Libero memoria pq no voy a dar de alta
        return 1; ///Nupla encontrada
    }else{
        posibleAlta->personas = alta; ///Guardo el dato a insertar
        if(lvo->cursor == lvo->acceso){ ///insercion entre el acceso y el primer elemento
            posibleAlta->siguiente=lvo->cursor;
            lvo->acceso=posibleAlta;
        }else{///insercion entre 2 nodos o insercion al final de la lista
            posibleAlta->siguiente=lvo->cursor;
            lvo->cursorAux->siguiente=posibleAlta;
        }
        *costo = *costo + 1;  ///0.5 por cada puntero q muevo
        lvo->cantidad = lvo->cantidad + 1;
        return 0; ///Exito
    }
}

int bajaLVO(persona baja, lvo *lvo, float *costo){
    float costolocalizar = 0;
    int estado;
    *costo = 0.0;
    estado = localizarLVO(baja.dni, lvo, &costolocalizar);
    if(estado == 1){
        return 1; ///FRACASO No esta el elemento
    }else{
        if (compararPersonas(baja,lvo->cursor->personas) != 0){
            return 1; ///NO ES LA MISMA PERSONA
        }
         if(lvo->cursor == lvo->acceso){ ///Borrar al Inicio
            lvo->acceso = lvo->cursor->siguiente;
            free(lvo->cursor);
        }else{ ///Borrar cuando el cursor no apunta al inicio
            lvo->cursorAux->siguiente = lvo->cursor->siguiente;
            free(lvo->cursor);
        }
    *costo = *costo + 0.5; ///0.5 de costo por corrimiento de nodo
    lvo->cantidad = lvo->cantidad - 1;
    return 0; ///EXITO
    }
}

int evocarLVO(int dnibuscar, persona *encontrada, lvo *lvo, float *costo){
    int estado;
    estado = localizarLVO(dnibuscar, lvo, costo);
    if(estado == 1){
        return 1; ///FRACASO No lo encontro
    }else{
        encontrada->dni = lvo->cursor->personas.dni;
        strcpy(encontrada->nombreyapellido, lvo->cursor->personas.nombreyapellido);
        strcpy(encontrada->domicilio, lvo->cursor->personas.domicilio);
        encontrada->codigopostal = lvo->cursor->personas.codigopostal;
        encontrada->nmesa = lvo->cursor->personas.nmesa;
        encontrada->circuito = lvo->cursor->personas.circuito;
        return 0; ///EXITO
    }
}

void mostrarLVO(lvo lvo){
    int i=0;
    lvo.cursor=lvo.acceso;
    while(lvo.cursor->personas.dni < 999999999){
        printf("\n//////////////////////////////");
        printf(" POSICION DEL ENVIO: %d //////////////////////////////\n\n",i);
        printf("Dni: %d\n", lvo.cursor->personas.dni);
        printf("Nombre y Apellido: %s\n", lvo.cursor->personas.nombreyapellido);
        printf("Domicilio: %s\n", lvo.cursor->personas.domicilio);
        printf("Codigo postal: %d\n", lvo.cursor->personas.codigopostal);
        printf("Numero de Mesa: %d\n", lvo.cursor->personas.nmesa);
        printf("Circuito: %d\n", lvo.cursor->personas.circuito);
        printf("\n");
        lvo.cursor= lvo.cursor->siguiente;
        i++;
    }
}

///LSOBB

typedef struct{
    persona personas[MAX];
    int cantidad;
}lso;

void inicializarEstructuraLSO(lso *lso){
    lso->cantidad = 0;
}

int localizarLSO(int dnibuscado, lso lso, int *posicion, float *costo){

    if (lso.cantidad == 0) { ///Lista vacia, cargo en la primera posicion
        *posicion = 0;
        *costo = 0.0;
        return 1; ///FRACASO NUPLA NO ENCONTRADA
    }
    int vector_marca[lso.cantidad];
    for(int i = 0; i < lso.cantidad; i++){
        vector_marca[i] = 0;
    }
    int li = 0;
    int ls = lso.cantidad - 1;
    int testigo;
    *costo = 0.0;
    testigo = floor((li + ls)/2);
    while(li < ls){
        if (vector_marca[testigo] != 1){
            *costo = *costo + 1;
        }
        vector_marca[testigo] = 1;
        if(dnibuscado <= lso.personas[testigo].dni){
            ls = testigo;
        }
        else{
            li = testigo + 1;
        }
        testigo = floor((li + ls)/2);
    }
    if (vector_marca[testigo] != 1){
            *costo = *costo + 1;
        }
    *posicion = testigo;
    if(lso.personas[testigo].dni == dnibuscado){
        return 0; ///EXITO
    }
    else{
        if (dnibuscado > lso.personas[testigo].dni) { ///CASO ESPECIAL INSERTAR DESPUES DEL ULTIMO ELEMENTO
            *posicion = *posicion + 1;
        }
        return 1; ///FRACASO NUPLA NO ENCONTRADA
    }
}

int altaLSO(persona alta, lso *lso, float *costo){
    float costolocalizar = 0;
    int posicion = 0;
    int i;
    int estado;
    *costo = 0.0;
    if(lso->cantidad == MAX){
        return 1; ///FRACASO NO TENGO MAS ESPACIO
    }
    estado = localizarLSO(alta.dni, *lso, &posicion, &costolocalizar);
    if(estado == 0){
        return 1; ///FRACASO NUPLA ENCONTRADA
    }else{
        for(i = lso->cantidad - 1; posicion <= i; i--){
            lso->personas[i + 1] = lso->personas[i];
            *costo = *costo + 1;
        }
        lso->personas[posicion] = alta;
        lso->cantidad = lso->cantidad + 1;
        return 0; ///EXITO
    }
}

int bajaLSO(persona baja, lso *lso, float *costo){
    float costolocalizar = 0;
    int posicion = 0;
    int i;
    int estado;
    *costo = 0.0;
    estado = localizarLSO(baja.dni, *lso, &posicion, &costolocalizar);
    if(estado != 0){
        return 1; ///FRACASO NUPLA NO ENCONTRADA
    }else{
        if (compararPersonas(baja,lso->personas[posicion]) != 0){
            return 1; ///NO ES LA MISMA PERSONA
        }
        for(i = posicion; i < lso->cantidad - 1; i++){
            lso->personas[i] = lso->personas[i + 1];
            *costo = *costo + 1;
        }
        lso->cantidad = lso->cantidad - 1;
        return 0; ///EXITO
    }
}

int evocarLSO(int dniBuscar, persona *encontrada, lso lso, float *costo){
    int posicion = 0;
    int estado;
    estado = localizarLSO(dniBuscar, lso, &posicion, costo);
    if(estado != 0){
        return 1; ///FRACASO NUPLA NO ENCONTRADA
    }else{
        encontrada->dni = lso.personas[posicion].dni;
        strcpy(encontrada->nombreyapellido, lso.personas[posicion].nombreyapellido);
        strcpy(encontrada->domicilio, lso.personas[posicion].domicilio);
        encontrada->codigopostal = lso.personas[posicion].codigopostal;
        encontrada->nmesa = lso.personas[posicion].nmesa;
        encontrada->circuito = lso.personas[posicion].circuito;
        return 0; ///EXITO
    }
}

void mostrarLSO(lso lso){
    int i;
    for (i = 0; i < lso.cantidad; i++){
        printf("\n//////////////////////////////");
        printf(" POSICION DEL ENVIO: %d //////////////////////////////\n\n",i);
        printf("Dni: %d\n", lso.personas[i].dni);
        printf("Nombre y Apellido: %s\n", lso.personas[i].nombreyapellido);
        printf("Domicilio: %s\n", lso.personas[i].domicilio);
        printf("Codigo postal: %d\n", lso.personas[i].codigopostal);
        printf("Numero de Mesa: %d\n", lso.personas[i].nmesa);
        printf("Circuito: %d\n", lso.personas[i].circuito);
    }
}

///ABB

typedef struct Nodoabb{
    persona personas;
    struct Nodoabb *hijoDerecha;
    struct Nodoabb *hijoIzquierda;
} Nodoabb;

typedef struct arbol{
    Nodoabb* raiz;
    Nodoabb* cursor;
    Nodoabb* padre;
    int cantidad;
} arbol;

void inicializarEstructuraABB(arbol *abb){
    abb->raiz=NULL;
    abb->padre=NULL;
    abb->cursor=NULL;
    abb->cantidad=0;
}

int localizarABB(int dnibuscado, arbol *abb, float *costo){
    abb->cursor = abb->raiz;
    abb->padre = NULL;
    *costo = 0.0;

    while((abb->cursor != NULL) && dnibuscado != abb->cursor->personas.dni ){
        abb->padre = abb->cursor;

        if(dnibuscado < abb->cursor->personas.dni ){
            abb->cursor = abb->cursor->hijoIzquierda;
        }else{
            abb->cursor = abb->cursor->hijoDerecha;
        }
        *costo = *costo + 1;
    }

    if(abb->cursor != NULL){
        *costo = *costo + 1;  ///Sumo 1 cuando encontre el elemento
        return 0;///EXITO
    }else{
        return 1;///FRACASO
    }
}

int altaABB(persona alta, arbol* abb, float *costo){
    float costolocalizar = 0;
    int estado;
    *costo = 0.0;
    Nodoabb *posibleAlta = (Nodoabb*)malloc(sizeof(Nodoabb));
    if (posibleAlta == NULL){
        return 1; ///No puedo dar de alta
    }
    estado = localizarABB(alta.dni, abb, &costolocalizar);
    if(estado == 0){
        free(posibleAlta); ///Libero memoria pq no voy a dar de alta
        return 1; ///FRACASO NUPLA ENCONTRADA
    }else{
        posibleAlta->personas = alta; ///Guardo el dato a insertar
        posibleAlta->hijoDerecha = NULL;
        posibleAlta->hijoIzquierda = NULL;
        abb->cantidad = abb->cantidad + 1;
        if (abb->raiz == NULL){
                abb->raiz = posibleAlta;
                *costo = *costo + 0.5;

                return 0;  ///EXITO INSERCION EN RAIZ
        }
        if(alta.dni < abb->padre->personas.dni )
        {
            abb->padre->hijoIzquierda = posibleAlta;
        }
        else
        {
            abb->padre->hijoDerecha = posibleAlta;
        }
        *costo = *costo + 0.5;
        return 0; ///EXITO INSERCION A DERECHA O IZQUIERDA
    }
}

int bajaABB(persona baja,arbol *abb,float *costo){
    float costolocalizar = 0;
    int estado;
    Nodoabb *auxiliarPadre, *auxiliarCursor;
    *costo = 0.0;
    estado = localizarABB(baja.dni, abb, &costolocalizar);
    if(estado == 1){
        return 1; ///FRACASO NUPLA NO ENCONTRADA
    }else{
        if (compararPersonas(baja,abb->cursor->personas) != 0){
            return 1; ///NO ES LA MISMA PERSONA
        }
        abb->cantidad = abb->cantidad - 1;
        if((abb->cursor->hijoIzquierda == NULL) && (abb->cursor->hijoDerecha == NULL)){ ///Caso 1
            if(abb->cursor == abb->raiz){
                    free(abb->cursor);
                    abb->raiz = NULL;
                    (*costo) +=0.5;
                    return 0; ///EXITO
            }
            else{
                if(abb->padre->hijoIzquierda == abb->cursor){  ///borrar nodo de la izquierda del padre
                        abb->padre->hijoIzquierda = NULL;
                        free(abb->cursor);
                        (*costo) +=0.5;
                        return 0;///EXITO
                }
                else{
                        abb->padre->hijoDerecha = NULL;         ///borrar nodo de la derecha del padre
                        free(abb->cursor);
                        (*costo) +=0.5;
                        return 0;///EXITO
                    }
            }

        }
        if((abb->cursor->hijoIzquierda == NULL) && (abb->cursor->hijoDerecha != NULL)){///Caso 2
            if(abb->cursor == abb->raiz){
                abb->raiz = abb->cursor->hijoDerecha;
                free(abb->cursor);
                (*costo) +=0.5;
                return 0;///EXITO
            }
            else{
                if(abb->padre->hijoIzquierda ==  abb->cursor){         //borrar nodo de la izquierda del padre
                    abb->padre->hijoIzquierda = abb->cursor->hijoDerecha;
                    free(abb->cursor);
                    (*costo) +=0.5;
                    return 0;///EXITO
                }
                else{
                    abb->padre->hijoDerecha = abb->cursor->hijoDerecha;  //borrar nodo de la derecha del padre
                    free(abb->cursor);
                    (*costo) +=0.5;
                    return 0;///EXITO
                }
            }
        }
        else{///Caso 2.1
            if((abb->cursor->hijoIzquierda !=NULL) && (abb->cursor->hijoDerecha == NULL)){
                if(abb->cursor == abb->raiz){
                    abb->raiz = abb->cursor->hijoIzquierda;
                    free(abb->cursor);
                    (*costo) +=0.5;
                    return 0;///EXITO
                }
                else{
                    if(abb->padre->hijoIzquierda ==  abb->cursor){
                        abb->padre->hijoIzquierda = abb->cursor->hijoIzquierda;     //borrar nodo de la izquierda del padre
                        free(abb->cursor);
                        (*costo) +=0.5;
                        return 0;///EXITO
                    }
                    else{
                        abb->padre->hijoDerecha = abb->cursor->hijoIzquierda;     //borrar nodo de la derecha del padre
                        free(abb->cursor);
                        (*costo) +=0.5;
                        return 0;///EXITO
                        }
                }
            }
        }
        if((abb->cursor->hijoIzquierda != NULL) && (abb->cursor->hijoDerecha != NULL)){///Caso 3
            auxiliarCursor = abb->cursor->hijoDerecha;
            auxiliarPadre = abb->cursor;
            while(auxiliarCursor->hijoIzquierda != NULL){
                auxiliarPadre = auxiliarCursor;
                auxiliarCursor = auxiliarCursor->hijoIzquierda;
            }
            abb->cursor->personas = auxiliarCursor->personas;
            (*costo)+=1;                       //costos por copia de datos

            // Desvinculación de punteros:
            if(auxiliarPadre->hijoIzquierda == auxiliarCursor){
                auxiliarPadre->hijoIzquierda = auxiliarCursor->hijoDerecha;
            }
            else{
                auxiliarPadre->hijoDerecha = auxiliarCursor->hijoDerecha;
            }
            (*costo) +=0.5;
            free(auxiliarCursor);
            return 0;///EXITO
        }
    }
}

int evocarABB(int dnibuscar, persona *encontrada, arbol *abb, float *costo){
    int estado;
    estado = localizarABB(dnibuscar, abb, costo);
    if(estado == 1){
        return 1; ///FRACASO NUPLA NO ENCONTRADA
    }else{
        encontrada->dni = abb->cursor->personas.dni;
        strcpy(encontrada->nombreyapellido, abb->cursor->personas.nombreyapellido);
        strcpy(encontrada->domicilio, abb->cursor->personas.domicilio);
        encontrada->codigopostal = abb->cursor->personas.codigopostal;
        encontrada->nmesa = abb->cursor->personas.nmesa;
        encontrada->circuito = abb->cursor->personas.circuito;
        return 0; ///EXITO
    }
}

void barridoNULL(Nodoabb *Nodoabb){
    if(Nodoabb!=NULL)
    {
        barridoNULL(Nodoabb->hijoIzquierda);
        barridoNULL(Nodoabb->hijoDerecha);
        free(Nodoabb);
    }
}

void barridoPreorden(Nodoabb* Nodoabb, int *i){

    if (Nodoabb != NULL)
    {
        printf("Dni: %d\nNombre y Apellido: %s\nDomicilio: %s\nCodigo Postal: %d\nN mesa: %d\nCircuito: %d\n", Nodoabb->personas.dni, Nodoabb->personas.nombreyapellido, Nodoabb->personas.domicilio, Nodoabb->personas.codigopostal, Nodoabb->personas.nmesa,Nodoabb->personas.circuito);
        if(Nodoabb->hijoIzquierda!=NULL){
            printf("Hijo izquierdo: %d\n", Nodoabb->hijoIzquierda->personas.dni);
        }
        else{
            printf("Hijo izquierdo: No tiene\n");
        }
        if(Nodoabb->hijoDerecha!=NULL){
            printf("Hijo derecho: %d\n\n", Nodoabb->hijoDerecha->personas.dni);
        }
        else{
            printf("Hijo derecho: No tiene\n\n");
        }
        if(*i%3==0)
        {
            system("pause");
            printf("\n");
        }
        (*i)++;
        barridoPreorden(Nodoabb->hijoIzquierda, i);
        barridoPreorden(Nodoabb->hijoDerecha, i);
    }

}

///COSTOS

typedef struct{
    float MaximoAltas;
    float AcumuladorAltas;
    float MedioAltas;
    int CantidadAltas;

    float MaximoBajas;
    float AcumuladorBajas;
    float MedioBajas;
    int CantidadBajas;

    float MaximoEvocarExitoso;
    float AcumuladorEvocarExitoso;
    float MedioEvocarExitoso;
    int CantidadEvocarExitoso;

    float MaximoEvocarFracaso;
    float AcumuladorEvocarFracaso;
    float MedioEvocarFracaso;
    int CantidadEvocarFracaso;
}costos;

typedef struct {
    costos lso;
    costos lvo;
    costos abb;
}TablaCostos;

void resetTabla(TablaCostos *tabla){

/// LISTA VINCULADA ORDENADA
    tabla->lvo.MaximoAltas = 0.0;
    tabla->lvo.AcumuladorAltas = 0.0;
    tabla->lvo.MedioAltas = 0.0;
    tabla->lvo.CantidadAltas = 0;

    tabla->lvo.MaximoBajas = 0.0;
    tabla->lvo.AcumuladorBajas = 0.0;
    tabla->lvo.MedioBajas = 0.0;
    tabla->lvo.CantidadBajas = 0;

    tabla->lvo.MaximoEvocarExitoso = 0.0;
    tabla->lvo.AcumuladorEvocarExitoso = 0.0;
    tabla->lvo.MedioEvocarExitoso = 0.0;
    tabla->lvo.CantidadEvocarExitoso = 0;

    tabla->lvo.MaximoEvocarFracaso = 0.0;
    tabla->lvo.AcumuladorEvocarFracaso = 0.0;
    tabla->lvo.MedioEvocarFracaso = 0.0;
    tabla->lvo.CantidadEvocarFracaso = 0;

/// LISTA SECUENCIAL ORDENADA
    tabla->lso.MaximoAltas = 0.0;
    tabla->lso.AcumuladorAltas = 0.0;
    tabla->lso.MedioAltas = 0.0;
    tabla->lso.CantidadAltas = 0;

    tabla->lso.MaximoBajas = 0.0;
    tabla->lso.AcumuladorBajas = 0.0;
    tabla->lso.MedioBajas = 0.0;
    tabla->lso.CantidadBajas = 0;

    tabla->lso.MaximoEvocarExitoso = 0.0;
    tabla->lso.AcumuladorEvocarExitoso = 0.0;
    tabla->lso.MedioEvocarExitoso = 0.0;
    tabla->lso.CantidadEvocarExitoso = 0;

    tabla->lso.MaximoEvocarFracaso = 0.0;
    tabla->lso.AcumuladorEvocarFracaso = 0.0;
    tabla->lso.MedioEvocarFracaso = 0.0;
    tabla->lso.CantidadEvocarFracaso = 0;

/// ARBOL BINARIO DE BUSQUEDA
    tabla->abb.MaximoAltas = 0.0;
    tabla->abb.AcumuladorAltas = 0.0;
    tabla->abb.MedioAltas = 0.0;
    tabla->abb.CantidadAltas = 0;

    tabla->abb.MaximoBajas = 0.0;
    tabla->abb.AcumuladorBajas = 0.0;
    tabla->abb.MedioBajas = 0.0;
    tabla->abb.CantidadBajas = 0;

    tabla->abb.MaximoEvocarExitoso = 0.0;
    tabla->abb.AcumuladorEvocarExitoso = 0.0;
    tabla->abb.MedioEvocarExitoso = 0.0;
    tabla->abb.CantidadEvocarExitoso = 0;

    tabla->abb.MaximoEvocarFracaso = 0.0;
    tabla->abb.AcumuladorEvocarFracaso = 0.0;
    tabla->abb.MedioEvocarFracaso = 0.0;
    tabla->abb.CantidadEvocarFracaso = 0;
}

void printTabla(TablaCostos tabla) {
    printf("\n\n"
           " ________________________________________________________________________\n"
           "| Operacion / Metrica       |   LVO +inf   |    LSOBB     |     ABB      |\n"
           "|===========================|==============|==============|==============|\n"
           "| Alta                      |              |              |              |\n"
           "|   Cantidad                |   %8d   |   %8d   |   %8d   |\n"
           "|   Costo Maximo            |   %8.2f   |   %8.2f   |   %8.2f   |\n"
           "|   Costo Promedio          |   %8.2f   |   %8.2f   |   %8.2f   |\n"
           "|---------------------------|--------------|--------------|--------------|\n"
           "| Baja                      |              |              |              |\n"
           "|   Cantidad                |   %8d   |   %8d   |   %8d   |\n"
           "|   Costo Maximo            |   %8.2f   |   %8.2f   |   %8.2f   |\n"
           "|   Costo Promedio          |   %8.2f   |   %8.2f   |   %8.2f   |\n"
           "|---------------------------|--------------|--------------|--------------|\n"
           "| Evocar exitoso            |              |              |              |\n"
           "|   Cantidad                |   %8d   |   %8d   |   %8d   |\n"
           "|   Costo Maximo            |   %8.2f   |   %8.2f   |   %8.2f   |\n"
           "|   Costo Promedio          |   %8.2f   |   %8.2f   |   %8.2f   |\n"
           "|---------------------------|--------------|--------------|--------------|\n"
           "| Evocar fracaso            |              |              |              |\n"
           "|   Cantidad                |   %8d   |   %8d   |   %8d   |\n"
           "|   Costo Maximo            |   %8.2f   |   %8.2f   |   %8.2f   |\n"
           "|   Costo Promedio          |   %8.2f   |   %8.2f   |   %8.2f   |\n"
           "|___________________________|______________|______________|______________|\n\n",
           // Alta
           tabla.lvo.CantidadAltas,   tabla.lso.CantidadAltas,   tabla.abb.CantidadAltas,
           tabla.lvo.MaximoAltas,    tabla.lso.MaximoAltas,    tabla.abb.MaximoAltas,
           tabla.lvo.MedioAltas,  tabla.lso.MedioAltas,  tabla.abb.MedioAltas,

           // Baja
           tabla.lvo.CantidadBajas,   tabla.lso.CantidadBajas,   tabla.abb.CantidadBajas,
           tabla.lvo.MaximoBajas,    tabla.lso.MaximoBajas,    tabla.abb.MaximoBajas,
           tabla.lvo.MedioBajas,  tabla.lso.MedioBajas,  tabla.abb.MedioBajas,

           // Evocar exitoso
           tabla.lvo.CantidadEvocarExitoso, tabla.lso.CantidadEvocarExitoso, tabla.abb.CantidadEvocarExitoso,
           tabla.lvo.MaximoEvocarExitoso,  tabla.lso.MaximoEvocarExitoso,  tabla.abb.MaximoEvocarExitoso,
           tabla.lvo.MedioEvocarExitoso,tabla.lso.MedioEvocarExitoso,tabla.abb.MedioEvocarExitoso,

           // Evocar fracaso
           tabla.lvo.CantidadEvocarFracaso, tabla.lso.CantidadEvocarFracaso, tabla.abb.CantidadEvocarFracaso,
           tabla.lvo.MaximoEvocarFracaso,  tabla.lso.MaximoEvocarFracaso,  tabla.abb.MaximoEvocarFracaso,
           tabla.lvo.MedioEvocarFracaso,tabla.lso.MedioEvocarFracaso,tabla.abb.MedioEvocarFracaso);
}

void resetear(TablaCostos *tabla, lso *lso, lvo *lvo, arbol *abb){
    inicializarEstructuraLVO(lvo);
    inicializarEstructuraLSO(lso);
    inicializarEstructuraABB(abb);
    resetTabla(tabla);
}

int leerArchivo(TablaCostos *tabla, lso *lso,lvo *lvo, arbol *abb) {
    FILE *fp;
    int operacion, i, estado;
    persona aux, auxS;
    float costo;
    if ((fp = fopen("Operaciones_Padron.txt","r")) == NULL)
        return 0;

    while (fscanf(fp, "%d", &operacion) == 1) {
        if (operacion == 1 || operacion == 2) {
            fscanf(fp, " %d", &aux.dni);
            fscanf(fp, " %[^\n]", aux.nombreyapellido);
            for(i = 0; aux.nombreyapellido[i] != '\0'; i++){
                aux.nombreyapellido[i] = toupper((unsigned char)aux.nombreyapellido[i]);
            }
            fscanf(fp, " %[^\n]", aux.domicilio);
            for(i = 0; aux.domicilio[i] != '\0'; i++){
                aux.domicilio[i] = toupper((unsigned char)aux.domicilio[i]);
            }
            fscanf(fp, " %d", &aux.codigopostal);
            fscanf(fp, " %d", &aux.nmesa);
            fscanf(fp, " %d", &aux.circuito);

            if (operacion == 1) {
                ///LVO
                costo = 0.0;
                if(altaLVO(aux, lvo, &costo) == 0){
                    tabla->lvo.AcumuladorAltas = tabla->lvo.AcumuladorAltas + costo;
                    tabla->lvo.CantidadAltas++;
                    if (tabla->lvo.MaximoAltas < costo){
                        tabla->lvo.MaximoAltas = costo;
                    }
                }
                ///LSO
                costo = 0.0;
                if(altaLSO(aux, lso, &costo) == 0){
                    tabla->lso.AcumuladorAltas = tabla->lso.AcumuladorAltas + costo;
                    tabla->lso.CantidadAltas++;
                    if (tabla->lso.MaximoAltas < costo){
                        tabla->lso.MaximoAltas = costo;
                    }
                }
                ///ABB
                costo = 0.0;
                if(altaABB(aux, abb, &costo) == 0){
                    tabla->abb.AcumuladorAltas = tabla->abb.AcumuladorAltas + costo;
                    tabla->abb.CantidadAltas++;
                    if (tabla->abb.MaximoAltas < costo){
                        tabla->abb.MaximoAltas = costo;
                    }
                }
            } else if (operacion == 2) {
                ///LVO
                if (lvo->cantidad > 0){
                    costo = 0.0;
                    if ((bajaLVO(aux, lvo, &costo)) == 0){
                        tabla->lvo.CantidadBajas++;
                        tabla->lvo.AcumuladorBajas = tabla->lvo.AcumuladorBajas + costo;
                        if (tabla->lvo.MaximoBajas < costo){
                            tabla->lvo.MaximoBajas = costo;
                        }
                    }
                }
                ///LSO
                if (lso->cantidad > 0){
                    costo = 0.0;
                    if ((bajaLSO(aux, lso, &costo)) == 0){
                        tabla->lso.CantidadBajas++;
                        tabla->lso.AcumuladorBajas = tabla->lso.AcumuladorBajas + costo;
                        if (tabla->lso.MaximoBajas < costo){
                            tabla->lso.MaximoBajas = costo;
                        }
                    }
                }
                ///ABB
                if (abb->cantidad > 0){
                        costo = 0.0;
                        if ((bajaABB(aux, abb, &costo)) == 0){
                            tabla->abb.CantidadBajas++;
                            tabla->abb.AcumuladorBajas = tabla->abb.AcumuladorBajas + costo;
                            if (tabla->abb.MaximoBajas < costo){
                                tabla->abb.MaximoBajas = costo;
                            }
                        }
                }
            }
        } else if (operacion == 3) { ///Si no hay elementos debo llamar al evocar?
            fscanf(fp, " %d", &aux.dni);
            ///LVO
            if (lvo->cantidad > 0 && aux.dni<999999999){
                costo = 0.0;
                estado = evocarLVO(aux.dni, &auxS, lvo, &costo);
                if (estado == 0){
                    tabla->lvo.CantidadEvocarExitoso++;
                    tabla->lvo.AcumuladorEvocarExitoso = tabla->lvo.AcumuladorEvocarExitoso + costo;
                    if (tabla->lvo.MaximoEvocarExitoso < costo){
                        tabla->lvo.MaximoEvocarExitoso = costo;
                    }
                } else {
                    tabla->lvo.CantidadEvocarFracaso++;
                    tabla->lvo.AcumuladorEvocarFracaso = tabla->lvo.AcumuladorEvocarFracaso + costo;
                    if (tabla->lvo.MaximoEvocarFracaso < costo){
                        tabla->lvo.MaximoEvocarFracaso = costo;
                    }
                }
            }
            ///LSO
            if (lso->cantidad > 0){
                costo = 0.0;
                estado = evocarLSO(aux.dni, &auxS, *lso, &costo);
                if (estado == 0){
                    tabla->lso.CantidadEvocarExitoso++;
                    tabla->lso.AcumuladorEvocarExitoso = tabla->lso.AcumuladorEvocarExitoso + costo;
                    if (tabla->lso.MaximoEvocarExitoso < costo){
                        tabla->lso.MaximoEvocarExitoso = costo;
                    }
                } else {
                    tabla->lso.CantidadEvocarFracaso++;
                    tabla->lso.AcumuladorEvocarFracaso = tabla->lso.AcumuladorEvocarFracaso + costo;
                    if (tabla->lso.MaximoEvocarFracaso < costo){
                        tabla->lso.MaximoEvocarFracaso = costo;
                    }
                }
            }
            ///ABB
            if (abb->cantidad > 0){
                costo = 0.0;
                estado = evocarABB(aux.dni, &auxS, abb, &costo);
                if (estado == 0){
                    tabla->abb.CantidadEvocarExitoso++;
                    tabla->abb.AcumuladorEvocarExitoso = tabla->abb.AcumuladorEvocarExitoso + costo;
                    if (tabla->abb.MaximoEvocarExitoso < costo){
                        tabla->abb.MaximoEvocarExitoso = costo;
                    }
                } else {
                    tabla->abb.CantidadEvocarFracaso++;
                    tabla->abb.AcumuladorEvocarFracaso = tabla->abb.AcumuladorEvocarFracaso + costo;
                    if (tabla->abb.MaximoEvocarFracaso < costo){
                        tabla->abb.MaximoEvocarFracaso = costo;
                    }
                }
            }
        }
    }
    fclose(fp);

    tabla->lvo.MedioAltas = (tabla->lvo.CantidadAltas != 0) ? tabla->lvo.AcumuladorAltas / tabla->lvo.CantidadAltas : 0;
    tabla->lvo.MedioBajas = (tabla->lvo.CantidadBajas != 0) ? tabla->lvo.AcumuladorBajas / tabla->lvo.CantidadBajas : 0;
    tabla->lvo.MedioEvocarExitoso = (tabla->lvo.CantidadEvocarExitoso != 0) ? tabla->lvo.AcumuladorEvocarExitoso / tabla->lvo.CantidadEvocarExitoso : 0;
    tabla->lvo.MedioEvocarFracaso = (tabla->lvo.CantidadEvocarFracaso != 0) ? tabla->lvo.AcumuladorEvocarFracaso / tabla->lvo.CantidadEvocarFracaso : 0;

    tabla->lso.MedioAltas = (tabla->lso.CantidadAltas != 0) ? tabla->lso.AcumuladorAltas / tabla->lso.CantidadAltas : 0;
    tabla->lso.MedioBajas = (tabla->lso.CantidadBajas != 0) ? tabla->lso.AcumuladorBajas / tabla->lso.CantidadBajas : 0;
    tabla->lso.MedioEvocarExitoso = (tabla->lso.CantidadEvocarExitoso != 0) ? tabla->lso.AcumuladorEvocarExitoso / tabla->lso.CantidadEvocarExitoso : 0;
    tabla->lso.MedioEvocarFracaso = (tabla->lso.CantidadEvocarFracaso != 0) ? tabla->lso.AcumuladorEvocarFracaso / tabla->lso.CantidadEvocarFracaso : 0;

    tabla->abb.MedioAltas = (tabla->abb.CantidadAltas != 0) ? tabla->abb.AcumuladorAltas / tabla->abb.CantidadAltas : 0;
    tabla->abb.MedioBajas = (tabla->abb.CantidadBajas != 0) ? tabla->abb.AcumuladorBajas / tabla->abb.CantidadBajas : 0;
    tabla->abb.MedioEvocarExitoso = (tabla->abb.CantidadEvocarExitoso != 0) ? tabla->abb.AcumuladorEvocarExitoso / tabla->abb.CantidadEvocarExitoso : 0;
    tabla->abb.MedioEvocarFracaso = (tabla->abb.CantidadEvocarFracaso != 0) ? tabla->abb.AcumuladorEvocarFracaso / tabla->abb.CantidadEvocarFracaso : 0;

    return 1;
}

int main(){
    int opcion,i;
    lso lso;
    lvo lvo;
    arbol abb;
    TablaCostos tabla;
    resetear(&tabla, &lso, &lvo, &abb);
    do{
        printf("|--------------------------------------------------------|\n");
        printf("|1->Comparacion de Estructuras (LVO, LSOBB, ABB)         |\n");
        printf("|2->Mostrar Lista Viculada Ordenada con terminacion      |\n");
        printf("|3->Mostrar Lista Secuencial Ordenada con Biseccion      |\n");
        printf("|4->Mostrar Arbol Binario de Busqueda                    |\n");
        printf("|5->Salir                                                |\n");
        printf("|--------------------------------------------------------|\n");
        printf("|Seleccione una opcion: ");
        scanf("%d", &opcion);
        printf("|________________________________________________________|\n");
        printf("\n\n");
        system("pause");
        system("cls");
        switch(opcion){
            case 1:
                barridoNULL(abb.raiz);
                abb.raiz = NULL;
                resetear(&tabla, &lso, &lvo,&abb);
                leerArchivo(&tabla, &lso, &lvo, &abb);
                printTabla(tabla);
                printf("\n\n");
                system("pause");
                system("cls");
                break;
            case 2:
                if(lvo.cantidad == 0){
                    printf("Lista Viculada Ordenada con Marca +infinito VACIA \n \n");
                }
                mostrarLVO(lvo);
                system("pause");
                system("cls");
                break;
            case 3:
                if(lso.cantidad == 0){
                    printf("Lista Secuencial Ordenada con Busqueda por Biseccion VACIA \n \n");
                }
                mostrarLSO(lso);
                system("pause");
                system("cls");
                break;
            case 4:
                if(abb.raiz == NULL){
                    printf("Arbol VACIO \n \n");
                }
                i=1;
                barridoPreorden(abb.raiz,&i);
                system("pause");
                system("cls");
                break;
        }

    } while(opcion != 5);

    barridoNULL(abb.raiz); ///lIMPIAR EL ARBOL AL FINALIZAR EL PROGRAMA
    return 0;
}
