#ifndef GRAPH_H
#define GRAPH_H

#include "list.h"



// structura del grafo
typedef struct {
    void** nodos;        // Arreglo de punteros
    int cantidad;       // Número actual de escenarios cargados
    int capacidad;      // Tamaño total del arreglo
} Grafo;


// Crea un grafo vacio
Grafo* crear_grafo();

// Agrega un nuevo nodo al grafo.
void agregar_nodo(Grafo* grafo, void* esc);

// Busca un escenario por su ID.
void* buscar_nodo(Grafo* grafo, int id);

// Conecta dos nodos
void conectar_nodos(void* origen, void* destino, const char* direccion);

// Libera toda la memoria asociada al grafo y sus escenarios.
void liberar_grafo(Grafo* grafo);

#endif
