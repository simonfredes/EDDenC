
#include <stdio.h>
#include <stdlib.h>
#include "mapeo.h"


/**
*Implementación de un mapeo con hash abierto.
*   @author Crisafulli Sofia, Fredes Simón y Rodríguez Leonardo.
*
*/


void funcion_NO_hace_nada(tElemento e);
void funcion_NO_hace_nada(tElemento e){

}

void (*fEliminarClave)(void *);
void (*fEliminarValor)(void *);

/*
Elimina una entrada del mapeo.
*/
void fEliminarEntrada(tElemento e){
     tEntrada entrada = (tEntrada) e;
     fEliminarClave(entrada->clave);
     fEliminarValor(entrada->valor);
     free(entrada);
}
/*
Devuelve el mayor entre dos números pasados por parámetro.
*/
int MAX(int a,int b){
    int mayor;
    if(a>b)
        mayor=a;
    else
        mayor=b;
    return mayor;
}

/**
 Inicializa un mapeo vacío, con capacidad inicial igual al MAX(10, CI).
 Una referencia al mapeo creado es referenciada en *M.
 A todo efecto, el valor hash para las claves será computado mediante la función fHash.
 A todo efecto, la comparación de claves se realizará mediante la función fComparacion.
 Finaliza indicando MAP_ERROR_MEMORIA si no es posible reservar memoria correspondientemente.
**/
void crear_mapeo(tMapeo * m, int ci, int (*fHash)(void *), int (*fComparacion)(void *, void *)){ //FUNCIONA
    int tamanioInicial= MAX(10, ci);
    (*m) = (tMapeo) malloc(sizeof(struct mapeo)); //Reservamos espacio en memoria para el mapeo.
    if((*m)==NULL)
        exit(MAP_ERROR_MEMORIA);
    (*m)->longitud_tabla = tamanioInicial;
    (*m)->cantidad_elementos = 0;
    (*m)->hash_code = fHash;
    (*m)->comparador = fComparacion;

    (*m)->tabla_hash = (tLista*) malloc(tamanioInicial*sizeof(tLista));//Reservamos espacio en memoria para cada una de las listas de los buckets del mapeo.

    for(int i=0;i<tamanioInicial;i++){//Creamos cada lista para cada uno de los buckets.
        crear_lista(&((*m)->tabla_hash[i]));
    }

}


/**
 Recupera el valor correspondiente a la entrada con clave C en M, si esta existe.
 Retorna el valor correspondiente, o NULL en caso contrario.
**/
tValor m_recuperar(tMapeo m, tClave c){
    int funcionHash = (m->hash_code(c))%(m->longitud_tabla);
    int esta = 0;
    tValor valor=NULL;
    tLista l=m->tabla_hash[funcionHash];//Guardamos la lista en la cual deberia estar la clave.
    tEntrada entrada;
    tPosicion cursor;
    tPosicion fin;
    cursor=l_primera(l);
    fin=l_fin(l);
    while(cursor!=fin && !esta){//Recorremos la lista buscando la entrada con igual clave.
        entrada=(tEntrada) l_recuperar(l,cursor);//Guardamos la entrada en la que esta apuntando el cursor.
         if(m->comparador(c,entrada->clave)==0){
            valor=entrada->valor;
            esta=1;

        }
        else{
            cursor=l_siguiente(l,cursor);
        }
    }
    return valor;
}


/**
 Elimina la entrada con clave C en M, si esta existe.
 La clave y el valor de la entrada son eliminados mediante las funciones fEliminarC y fEliminarV.
**/
void m_eliminar(tMapeo m, tClave c, void (*fEliminarC)(void *), void (*fEliminarV)(void *)){ //ERROR 111
        int funcionHash = (m->hash_code(c))% (m->longitud_tabla);
        int encontre = 0;

        tEntrada entrada;
        tPosicion cursor;
        tPosicion fin;
        tLista lista = m->tabla_hash[funcionHash];//Guardo la lista en donde debería estar la clave.
        cursor=l_primera(lista);
        fin=l_fin(lista);

        while(cursor != fin && !encontre){
          entrada= (tEntrada) l_recuperar(lista,cursor);

          if(m->comparador(c,entrada->clave) == 0){ //Comparamos la clave c y la clave de la lista en la que estoy buscando.
                  encontre = 1;
          }
          else
            cursor= l_siguiente(lista,cursor);

        }
        if (encontre){//si la clave se encuentra en el mapeo.
             fEliminarClave= fEliminarC;//elimino la clave.
             fEliminarValor= fEliminarV;//elimino el valor.
             l_eliminar(lista, cursor, &fEliminarEntrada);//elimino la entrada del mapeo.
             m->cantidad_elementos= m->cantidad_elementos -1;
        }
   }


/**
 Destruye el mapeo M, elimininando cada una de sus entradas.
 Las claves y valores almacenados en las entradas son eliminados mediante las funciones fEliminarC y fEliminarV.
**/
void m_destruir(tMapeo * m, void (*fEliminarC)(void *), void (*fEliminarV)(void *)){
   int cant = 0;
   tLista lista;
   fEliminarClave = fEliminarC;
   fEliminarValor = fEliminarV;
   while(cant < ((*m)->longitud_tabla)){
      lista = ((*m)->tabla_hash[cant]);//Ubicamos cada lista del mapeo.
      l_destruir(&lista, &fEliminarEntrada);//Destruimos todas las listas del arreglo de buckets.
      cant = cant+1;
   }
    (*m)->cantidad_elementos = 0;
    free(((*m))->tabla_hash);
    (*m)->tabla_hash = NULL;//Eliminamos lo a lo que apunta m.
    free((*m));   //Eliminamos lo apuntado por m.
    (*m) = NULL;
}



void redimensionar(tMapeo m){
    int nuevo_size = (2*(m)->longitud_tabla); //nuevo tamaño del arreglo
    int bucket;
    tEntrada entrada;
    tClave clave;
    tPosicion posicion_actual_lista, posicion_fin_lista;
    tLista lista_actual;
    tLista * lista =  malloc(sizeof(tLista)*m->longitud_tabla*2);//reservo espacio en emoria para la lista

    for(int i=0; i<nuevo_size; i++)
    {
        crear_lista(&(lista[i])); //para cada bucket del nuevo arreglo creo una lista
    }

    for(int i=0; i<(m)->longitud_tabla; i++) //Recorre la tabla existente.
    {
        lista_actual = (m)->tabla_hash[i];
        posicion_actual_lista = l_primera(lista_actual);
        posicion_fin_lista = l_fin(lista_actual);
        while(posicion_actual_lista!=posicion_fin_lista)
        {
            entrada = l_recuperar(lista_actual, posicion_actual_lista);
            clave = entrada->clave;
            bucket = (m)->hash_code(clave) % nuevo_size;
            l_insertar(lista[bucket], l_primera(lista[bucket]), entrada); //Almacena en la nueva tabla los datos anteriores.
            posicion_actual_lista = l_siguiente(lista_actual, posicion_actual_lista);
        }
         l_destruir(&((m)->tabla_hash[i]),&funcion_NO_hace_nada);
    }
    free(m->tabla_hash);
    (m)->tabla_hash = 	lista;
    (m)->longitud_tabla = nuevo_size;
}




/**
 Inserta una entrada con clave C y valor V, en M.
 Si una entrada con clave C y valor Vi ya existe en M, modifica Vi por V.
 Retorna NULL si la clave C no existía en M, o Vi en caso contrario.
 Finaliza indicando MAP_ERROR_MEMORIA si no es posible reservar memoria correspondientemente.
**/
tValor m_insertar(tMapeo m, tClave c, tValor v){
   int fh= ((m->hash_code(c))%(m->longitud_tabla));
   int longitud = (m-> longitud_tabla);
   int cantElem = (m-> cantidad_elementos);
   tLista lista = m->tabla_hash[fh];
   tValor valor= NULL;
   tEntrada entrada;
   tPosicion cursor;
   tPosicion fin;
   int esta=0;
   cursor=l_primera(lista);
   fin=l_fin(lista);
   while(cursor != fin && !esta){//Recorro buscando la entrada con clave igual a la clave pasada por parametro.
     entrada = (tEntrada) l_recuperar(lista,cursor);
     if(m->comparador(c,entrada->clave)==0){
        valor = entrada->valor;
        entrada->valor=v;
        esta = 1;
     }
     else
       cursor=l_siguiente(lista,cursor);
    }

     if (!esta){
        if((cantElem / longitud) >= (0.75)){
       	 redimensionar(m);
        }
        tEntrada entradaNueva = (tEntrada) malloc(sizeof(struct entrada));//Reservamos espacio en memoria para una entrada.
        entradaNueva-> clave = c;
        entradaNueva->valor = v;
        l_insertar(lista,cursor, entradaNueva);
        cantElem+= 1;
        m->cantidad_elementos = cantElem;
    }
    return valor;
}


