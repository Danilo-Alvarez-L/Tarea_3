#include "graph.h"
#include "GraphQuest.h"
#include <stdlib.h>
#include <string.h>

Grafo* crear_grafo()
{
    Grafo* g = (Grafo*)malloc(sizeof(Grafo));
    g->capacidad = 100;
    g->cantidad = 0;
    g->nodos = (void**)malloc(sizeof(void*) * g->capacidad);
    return g;
}


void agregar_nodo(Grafo* grafo, void* esc)
{
    if (grafo->cantidad >= grafo->capacidad)
    {
        grafo->capacidad *= 2;
        grafo->nodos = (void**)realloc(grafo->nodos, sizeof(void*) * grafo->capacidad);
    }
    grafo->nodos[grafo->cantidad++] = esc;
}


void* buscar_nodo(Grafo* grafo, int id)
{
    for (int i = 0; i < grafo->cantidad; i++)
    {
        Escenario* escenario = (Escenario*)grafo->nodos[i];  // CAST NECESARIO
        if (escenario->id == id)
            return escenario;
    }
    return NULL;
}


void conectar_nodos(void* origen_ptr, void* destino_ptr, const char* direccion)
{
    Escenario* origen = (Escenario*)origen_ptr;
    Escenario* destino = (Escenario*)destino_ptr;

    if (strcmp(direccion, "arriba") == 0)
        origen->arriba = destino;
    else if (strcmp(direccion, "abajo") == 0)
        origen->abajo = destino;
    else if (strcmp(direccion, "izquierda") == 0)
        origen->izquierda = destino;
    else if (strcmp(direccion, "derecha") == 0)
        origen->derecha = destino;
}


// Liberar memoria del grafo (no se encarga de liberar items)
void liberar_grafo(Grafo* grafo)
{
    for (int i = 0; i < grafo->cantidad; i++)
    {
        Escenario* escenario = (Escenario*)grafo->nodos[i];
        // Liberar cada Item* antes de limpiar la lista
        for (Item* item = list_first(escenario->items); item != NULL; item = list_next(escenario->items))
        {
            free(item);
        }
        list_clean(escenario->items);
        free(escenario->items);
        free(escenario);
    }
    free(grafo->nodos);
    free(grafo);
}