#include "lista.h"
#include <stdio.h>
#include <stdlib.h>

/**
*Implementación de una lista simplemente enlazada con posición indirecta.
*   @author Crisafulli Sofia, Fredes Simón y Rodríguez Leonardo.
*
*/


/**
 Inicializa una lista vacía.
 Una referencia a la lista creada es referenciada en *L.
 Finaliza indicando LST_ERROR_MEMORIA si no es posible reservar memoria correspondientemente.
**/
void crear_lista(tLista * l){
    (*l) = (tPosicion) malloc(sizeof(struct celda));
    if((*l)==NULL)
        exit(LST_ERROR_MEMORIA);
    (*l)->elemento = NULL;
    (*l)->siguiente = NULL;
}

/**
 Inserta el elemento E, en la posición P, en L.
 Con L = A,B,C,D y la posición P direccionando C, luego:
 L' = A,B,E,C,D.
 Finaliza indicando LST_ERROR_MEMORIA si no es posible reservar memoria correspondientemente.
**/
void l_insertar(tLista l, tPosicion p, tElemento e){
    struct celda *celda = malloc(sizeof(struct celda)); // Se reserva espacio para el nodo en el cual se guardara el elemento.
    if((celda)==NULL)
        exit(LST_ERROR_MEMORIA);
    celda->elemento = e;
    celda->siguiente = p->siguiente;                     // El siguiente de la celda es el siguiente a la posicion al estar trabajando con posicion indirecta.
    p->siguiente = celda;
}

/**
 Elimina la celda P de L.
 El elemento almacenado en la posición P es eliminado mediante la función fEliminar.
 Finaliza indicando LST_POSICION_INVALIDA si P es fin(L).
**/
void l_eliminar(tLista l, tPosicion p, void (*fEliminar)(tElemento)){
    struct celda *nodo;
    if(p->siguiente == NULL)                    //En caso de que la posicion == fin.
        exit(LST_POSICION_INVALIDA);
    fEliminar((p->siguiente)->elemento);        //Se elimina el elemento con la funcion pasada por parametro.
    nodo = p->siguiente;                         //Se apunta al nodo para asi despues eliminarlo.
    p->siguiente = p->siguiente->siguiente;     //El nodo anterior apunta al nodo siguiente al actual.
    free(nodo);                                  //Se libera la celda de la memoria.
}

/**
 Destruye la lista L, elimininando cada una de sus celdas.
 Los elementos almacenados en las celdas son eliminados mediante la función fEliminar.
**/
void l_destruir(tLista * l, void (*fEliminar)(tElemento)){
    struct celda *nodo;
    while((*l)->siguiente != NULL){
        nodo = (*l)->siguiente;
        fEliminar(nodo->elemento);               //Se elimina el elemento con la funcion pasada por parametro.
        (*l)->siguiente = nodo->siguiente;
        free(nodo);                              //Se libera la celda de la memoria.
    }
    free(*l);                                  //Por ultimo se elimina el espacio en memoria reservado para la lista.
    (*l)=NULL;
}

 /**
 Recupera y retorna el elemento en la posición P.
 Finaliza indicando LST_POSICION_INVALIDA si P es fin(L).
**/
tElemento l_recuperar(tLista l, tPosicion p){
    if(p->siguiente == NULL)
        exit(LST_POSICION_INVALIDA);
    return p->siguiente->elemento;
}

/**
 Recupera y retorna la primera posición de L.
 Si L es vacía, primera(L) = ultima(L) = fin(L).
**/
tPosicion l_primera(tLista l){
    return l;                                           //La primera posicion es a la que apunta la lista.
}

/**
 Recupera y retorna la posición siguiente a P en L.
 Finaliza indicando LST_NO_EXISTE_SIGUIENTE si P es fin(L).
**/
tPosicion l_siguiente(tLista l, tPosicion p){
        if(p->siguiente == NULL)
            exit(LST_NO_EXISTE_SIGUIENTE);
        return p->siguiente;
}

/**
 Recupera y retorna la posición anterior a P en L.
 Finaliza indicando LST_NO_EXISTE_ANTERIOR si P es primera(L).
**/
tPosicion l_anterior(tLista l, tPosicion p){
    tPosicion nodo = l;

    if(p == l_primera(l))
        exit(LST_NO_EXISTE_ANTERIOR);
    while (nodo->siguiente != p) {                      //Recorremos hasta estar parado en el nodo anterior.
        nodo = nodo->siguiente;
    }
    return nodo;
}
/**
 Recupera y retorna la última posición de L.
 Si L es vacía, primera(L) = ultima(L) = fin(L).
**/
tPosicion l_ultima(tLista l){
    tPosicion nodo = l;
    if(l->siguiente == NULL)
        return l;
    while ((nodo->siguiente)->siguiente != NULL) {      //Mientras que existan siguientas avanzo hasta la ultima posicion.
        nodo = nodo->siguiente;
    }
    return nodo;
}

/**
 Recupera y retorna la posición fin de L.
 Si L es vacía, primera(L) = ultima(L) = fin(L).
**/
tPosicion l_fin(tLista l){
    tPosicion nodo = l;

    while (nodo->siguiente != NULL) {
        nodo = nodo->siguiente;
    }
    return nodo;
}

/**
*Retorna la longitud de la lista.
*/
int l_longitud(tLista l){
    if(l->siguiente == NULL) {
        return 0;
    }
    return 1 + l_longitud(l->siguiente);
}
