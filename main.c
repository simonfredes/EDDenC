#include <stdio.h>
#include <stdlib.h>
//@simonfredes
int cantidadElementos=0;

typedef struct celda{
    int elemento;
    struct celda * siguiente;
    struct celda * anterior;
} * tLista;

typedef struct nodo{
 int elemento;
 struct nodo * hijoDerecho;
 struct nodo * hijoIzquierdo;
} * tArbolBinario;

/*
Caso base1: Si el arbol esta vacio, entonces, creo raiz e inserto ese elemento
Caso base2: Si el nodo no tiene hijos:
                Si el elemento es mayor al nodo padre, inserto a la derecha
                Si el elemento es menor al nodo padre, inserto a la izquierda

Caso recursivo: Si el Arbol no es vacio, entonces, insertarElemento será:
                Si el elemento es mayor que el nodo padre, mi nuevo arbol será el nodo derecho
                Si el elemento es menor que el nodo padre, mi nuevo arbol será el nodo izquierdo.

*/
void insertarElementoArbol( tArbolBinario * arbol, int elemento){

    struct nodo * nuevo = malloc (sizeof(struct nodo));
    nuevo->elemento= elemento;
    nuevo->hijoDerecho= NULL;
    nuevo->hijoIzquierdo= NULL;

    //Caso base1
   if ( (*arbol) == NULL){
        (*arbol) = nuevo;
        printf("inserte en raiz el numero %d \n", elemento);
        cantidadElementos= cantidadElementos+1;
        return;
   }

   if (elemento < (*arbol) -> elemento){
       if ((*arbol)->hijoIzquierdo == NULL){ //si el hijo izquierdo es nulo, inserto ahi.
         (*arbol)->hijoIzquierdo=nuevo;
          printf("inserte a la izquierda el numero %d \n", elemento);
          cantidadElementos= cantidadElementos+1;


       }
       else{ //si tiene un hijo izquierdo, será mi nuevo sub arbol
            insertarElementoArbol( & ((*arbol)-> hijoIzquierdo), elemento);
       }
   }
   else{
      if ((*arbol)->hijoDerecho == NULL){ //si el hijo derecho es nulo, inserto ahi.
         (*arbol)->hijoDerecho=nuevo;
         printf("inserte a la derecha el numero %d \n", elemento);
         cantidadElementos= cantidadElementos+1;

       }
        else{ //si tiene un hijo derecho, será mi nuevo sub arbol
            insertarElementoArbol( & ((*arbol)-> hijoDerecho), elemento);
       }
   }

}
/*
  Caso base: si el arbol es vacio, no retornamos nada.

  Caso recursivo: Si el arbol no es vacio, entonces.
                    -Si tiene hijo izquierdo, imprimimos hijo izquierdo, preorden con hijo izquierdo y preorden con hijo derecho

*/
/*
int * preorden(tArbolBinario * arbol, int * arreglo , int posicion){
    int elemento=0;
    if ((*arbol) == NULL){
        return;
    }
    else{
            elemento= (*arbol)->elemento;
            printf("El valor del nodo es %d \n", elemento);
            arreglo[posicion]= elemento;
            posicion=posicion+1;
            preorden(&(*arbol)->hijoIzquierdo, arreglo, posicion);
            preorden((&(*arbol)->hijoDerecho),arreglo,posicion);

    }
}*/

int * preorden(tArbolBinario  arbol, int * arreglo , int posicion){
    int elemento=0;
    if ((arbol) == NULL){
        return;
    }
    else{
            elemento= (arbol)->elemento;
            printf("El valor del nodo es %d \n", elemento);
            preorden( arbol->hijoIzquierdo, arreglo, posicion);
            preorden( arbol->hijoDerecho,arreglo,posicion);

    }
}

/*
Caso base: Si el arbol es vacio, profundidad será 0

Caso recursivo: Si el arbol no es vacio, profundidad será:
                 1+ el maximo de altura de sus hijos

*/
int max(int num1, int num2) {
   int result;
   if (num1 > num2)
      result = num1;
   else
      result = num2;
   return result; //Ejercicios aprenderaprogramar.com
}

int profundidad(tArbolBinario arbol,int retorno){

    if (arbol==NULL){
        retorno=0;
    }
    else{
        retorno=1+ max(profundidad(arbol->hijoDerecho,retorno), profundidad(arbol->hijoIzquierdo,retorno));
    }

    return retorno;
}





int longitud (tLista  * l){
    if ( (*l) == NULL ){
        return 0;
    }
    else{
        return 1 + longitud( & (*l) -> siguiente);
    }
}

void insertar(int numero, tLista * l ){
    struct celda * nuevo = malloc(sizeof(struct celda));
    nuevo->elemento= numero;
    nuevo->anterior=NULL;
    nuevo->siguiente=NULL;
    int termine=0;
    tLista posicion= (*l);

    if ( (*l)==NULL ){
        *l = malloc(sizeof(struct celda));
        (*l)-> siguiente =NULL;
        (*l)-> anterior= NULL;
        (*l)-> elemento=numero;
    }
    else{
      while (posicion->siguiente!=NULL){
        posicion=posicion->siguiente;
      }
      posicion->siguiente=nuevo;
      nuevo->anterior=posicion;
    }
}

int destruir (tLista * lista){
    int retorno=0;
    if ( (*lista)==NULL ){
        return retorno;
    }
    else{

        if ( (*lista)->siguiente!=NULL){
            //*lista= & ((*lista)-> siguiente) ;
             printf("estoy en %d \n", (*lista) ->elemento);
            retorno = retorno + destruir(& ( (*lista) ->siguiente) ) ;

        }

            printf("estoy en el elemento %d \n", (*lista)->elemento);
            (*lista)->elemento=NULL;
            (*lista)->anterior=NULL;
            retorno= retorno+ sizeof(struct celda);
            free (*lista);
            *lista=NULL;




    }

    return retorno;
}

int pertenece(int n, tLista * l){

    if ((*l)==NULL){
        return 0;
    }
    else{
        if ((*l)->elemento == n){
            return 1;
        }
        else
           return (pertenece(n, &( (*l)->siguiente)) );
    }
}

int main(int argc, char **args){
    tArbolBinario arbol=NULL;
    insertarElementoArbol(&arbol, 10);
    insertarElementoArbol(&arbol, 5);
    insertarElementoArbol(&arbol, 15);
    insertarElementoArbol(&arbol, 3);
    insertarElementoArbol(&arbol, 8);
    insertarElementoArbol(&arbol, 9);
    insertarElementoArbol(&arbol, 7);
    int arreglo[15];
    int indice=0;



    printf("la cantidad de elementos del arbol binario son: %d \n",cantidadElementos);

    preorden(arbol, &arreglo,0);
    printf("la altura del arbol es %d \n", profundidad(arbol,0));
}

    /*
    printf("hola, el tamanio de la celda es de %d \n", sizeof(struct celda));
    int sizeLista=0;
    int espacioLiberado=0;
    int perteneceElem=0;
    tLista lista=NULL;
   // insertar(1,&lista);
    //insertar(2,&lista);
   // insertar(3,&lista);
    //espacioLiberado=destruir(&lista);
    perteneceElem= pertenece(3, &lista);

    sizeLista=longitud(&lista);
    printf("lista tiene %d \n", sizeLista);
    //printf("el espacio liberado fue de %d \n",espacioLiberado );
    printf("el elemento pertenece %d",perteneceElem );
    */


