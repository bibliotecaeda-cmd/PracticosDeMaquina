#ifndef ABB_H_INCLUDED
#define ABB_H_INCLUDED
#include <malloc.h>
#include "Alumno.h"

typedef struct nodo
{
    Alumno alumno;
    struct nodo *hijoDer;
    struct nodo *hijoIzq;
} Nodo;

typedef struct Arbol
{
    Nodo* raiz;
    Nodo* cursor;
    Nodo* padre;
    int cant;
} Arbol;

void InitABB(Arbol *a)
{
    a->raiz=NULL;
    a->padre=NULL;
    a->cursor=NULL;
    a->cant=0;
}

int LocalizarABB(char codigo[], Arbol *ABB, float *celdas)
{
    ABB->cursor = ABB->raiz;
    ABB->padre = NULL;
    (*celdas)=0;

    while((ABB->cursor != NULL) && (strcmp(ABB->cursor->alumno.codigo, codigo) != 0))
    {
        ABB->padre = ABB->cursor;

        if(strcmp(ABB->cursor->alumno.codigo, codigo)>0 )
        {
            ABB->cursor = ABB->cursor->hijoIzq;
        }
        else
        {
            ABB->cursor = ABB->cursor->hijoDer;
        }
        (*celdas) += 1;

    }

    if(ABB->cursor != NULL)
    {
        (*celdas) += 1;
        return 0;               //localizacion exitosa
    }
    else
    {
        return 1;               //localizacion no exitosa
    }
}

int AltaABB(Alumno nuevo, Arbol* ABB, float *modificaciones)
{

    float celdas = 0;
    Nodo *aux= (Nodo*)malloc(sizeof(Nodo));
    if (aux == NULL)
    {
        return 2;               //fallo de asignacion de memoria
    }
    else
    {
        if(LocalizarABB(nuevo.codigo, ABB, &celdas) == 1)
        {
            aux->alumno = nuevo;
            aux->hijoDer = NULL;
            aux->hijoIzq = NULL;
            if(ABB->raiz == NULL)
            {
                ABB->raiz = aux;
                (*modificaciones)=0.5;
                return 0;           //alta exitosa
            }
            else if(strcmp(ABB->padre->alumno.codigo, aux->alumno.codigo) > 0 )
            {
                ABB->padre->hijoIzq = aux;
            }
            else
            {
                ABB->padre->hijoDer = aux;
            }
            (*modificaciones)= 0.5;
            return 0;               //alta exitosa
        }
        else
        {
            free((void*)aux);
            return 1;               //alta no exitosa
        }
    }
}

int BajaABB(Alumno eliminar,Arbol* ABB,float *modificaciones)
{
    float celdas = 0;
    Nodo *auxPadre, *auxCursor;

    if(LocalizarABB(eliminar.codigo,ABB,&celdas)==0 && strcmp(eliminar.codigo, ABB->cursor->alumno.codigo)==0 && strcmp(eliminar.nombre, ABB->cursor->alumno.nombre)==0 && strcmp(eliminar.correo, ABB->cursor->alumno.correo)==0 && strcmp(eliminar.condicion, ABB->cursor->alumno.condicion)==0 && eliminar.nota == ABB->cursor->alumno.nota)
    {
            //Caso 1: eliminar nodo sin hijos
            if((ABB->cursor->hijoIzq == NULL) && (ABB->cursor->hijoDer == NULL))
            {
                if(ABB->cursor == ABB->raiz)
                {

                    free((void*)(ABB->cursor));
                    ABB->raiz = NULL;
                    (*modificaciones) +=0.5;
                    return 0;
                }
                else
                {

                    if(ABB->padre->hijoIzq == ABB->cursor)   //borrar nodo de la izquierda del padre
                    {
                        ABB->padre->hijoIzq = NULL;
                        free((void*)(ABB->cursor));
                        (*modificaciones) +=0.5;
                        return 0;
                    }
                    else
                    {
                        ABB->padre->hijoDer = NULL;         //borrar nodo de la derecha del padre
                        free((void*)(ABB->cursor));
                        (*modificaciones) +=0.5;
                        return 0;
                    }
                }

            }
            //Caso 2: bajar nodo con hijo derecho
            if((ABB->cursor->hijoIzq == NULL) && (ABB->cursor->hijoDer != NULL))
            {
                if(ABB->cursor == ABB->raiz)
                {
                    ABB->raiz = ABB->cursor->hijoDer;
                    free((void*)(ABB->cursor));
                    (*modificaciones) +=0.5;
                    return 0;
                }
                else
                {

                    if(ABB->padre->hijoIzq ==  ABB->cursor)         //borrar nodo de la izquierda del padre
                    {
                        ABB->padre->hijoIzq = ABB->cursor->hijoDer;
                        free((void*)(ABB->cursor));
                        (*modificaciones) +=0.5;
                        return 0;
                    }
                    else
                    {
                        ABB->padre->hijoDer = ABB->cursor->hijoDer;  //borrar nodo de la derecha del padre
                        free((void*)(ABB->cursor));
                        (*modificaciones) +=0.5;
                        return 0;
                    }
                }

            }
            //Caso 2: bajar nodo con hijo izquierdo
            else
            {
                if((ABB->cursor->hijoIzq !=NULL) && (ABB->cursor->hijoDer == NULL))
                {
                    if(ABB->cursor == ABB->raiz)
                    {
                        ABB->raiz = ABB->cursor->hijoIzq;
                        free((void*)(ABB->cursor));
                        (*modificaciones) +=0.5;
                        return 0;
                    }
                    else
                    {

                        if(ABB->padre->hijoIzq ==  ABB->cursor)
                        {
                            ABB->padre->hijoIzq = ABB->cursor->hijoIzq;     //borrar nodo de la izquierda del padre
                            free((void*)(ABB->cursor));
                            (*modificaciones) +=0.5;
                            return 0;
                        }
                        else
                        {
                            ABB->padre->hijoDer = ABB->cursor->hijoIzq;     //borrar nodo de la derecha del padre
                            free((void*)(ABB->cursor));
                            (*modificaciones) +=0.5;
                            return 0;
                        }
                    }


                }

            }
            //caso 3: bajar nodo con 2 hijos
            if((ABB->cursor->hijoIzq != NULL) && (ABB->cursor->hijoDer != NULL))
            {

                auxCursor = ABB->cursor->hijoIzq;
                auxPadre = ABB->cursor;
                while(auxCursor->hijoDer != NULL)           //mayor de los menores
                {
                    auxPadre = auxCursor;
                    auxCursor = auxCursor->hijoDer;
                }
                ABB->cursor->alumno = auxCursor->alumno;
                (*modificaciones)+=1;                       //costos por copia de datos
                if(auxPadre->hijoDer == auxCursor)
                {
                    auxPadre->hijoDer = auxCursor->hijoIzq;
                }
                else                                        //caso eliminar raiz
                {
                    auxPadre->hijoIzq = auxCursor->hijoIzq;
                }
                (*modificaciones) +=0.5;
                free((void*)(auxCursor));
                return 0;
            }


    }
    else
    {
        return 1;       //baja no exitosa
    }


}

int EvocarABB(Arbol *ABB, char codigo[], Alumno *alumno, float *celdas)
{

    if(LocalizarABB(codigo, ABB, celdas)==0)
    {
        (*alumno) = ABB->cursor->alumno;             //PREGUNTAR-> le estoy asignando el dato? o la direccion?
        return 0;               //evocacion exitosa
    }
    return 1;                   //evocacion no exitosa
}

void Barrido(Nodo* nodo)
{
    if(nodo!=NULL)
    {
        Barrido(nodo->hijoIzq);
        Barrido(nodo->hijoDer);
        free(nodo);
    }
}

void BarridoPreorden(Nodo* nodo, int *i)
{

    if (nodo != NULL)
    {
        printf("Codigo: %s\nNombre: %s\nCorreo: %s\nNota: %d\nCondicion: %s\n", nodo->alumno.codigo, nodo->alumno.nombre, nodo->alumno.correo, nodo->alumno.nota, nodo->alumno.condicion);
        if(nodo->hijoIzq!=NULL)printf("Hijo izquierdo: %s\n", nodo->hijoIzq->alumno.codigo);
        else printf("Hijo izquierdo: No tiene\n");
        if(nodo->hijoDer!=NULL)printf("Hijo derecho: %s\n\n", nodo->hijoDer->alumno.codigo);
        else printf("Hijo derecho: No tiene\n\n");
        if(*i%3==0)
        {
            system("pause");
            printf("\n");
        }
        (*i)++;
        BarridoPreorden(nodo->hijoIzq, i);
        BarridoPreorden(nodo->hijoDer, i);
    }

}


#endif // ABB_H_INCLUDED

