#ifndef GRAPHQUEST_H
#define GRAPHQUEST_H

#include "tdas/list.h"
#include "tdas/graph.h"
#include <stdbool.h>


// Struct del item
typedef struct {
    char nombre[100];
    int valor;
    int peso;
} Item;

// Struct del escenario
typedef struct Escenario {
    int id;
    char nombre[100];
    char descripcion[256];
    List* items;  // Lista de Item*

    // Conexiones a las otras habitaciones
    struct Escenario* arriba;
    struct Escenario* abajo;
    struct Escenario* izquierda;
    struct Escenario* derecha;

    int es_final; //escenario final
} Escenario;

// Struct del jugador
typedef struct {
    List* inventario;      // Lista de Item*
    int peso_total;
    int puntaje_total;
    int tiempo_restante;
    Escenario* actual; //habitacion en la que esta ahora
} Jugador;

// crear jugador
Jugador* crear_jugador(Escenario* inicial);

// Carga el laberinto desde un archivo CSV y construye el grafo.
void cargarLaberinto(Grafo* grafo, const char* nombreArchivo);

// Muestra el estado actual del jugador y del escenario. ademas de que verifica si el jugador ha llegado al escenario final.
bool mostrar_escenarios(Jugador* jugador);

// Permite al jugador tomar items del escenario actual. (se puede volver a utilizar aun no puedes tomar varios objetos a la vez)
// (se actualizan los valores correspondientes)
void recoger_items(Jugador* jugador);

// Permite al jugador dejar items de su inventario. (se actualizan los valores correspondientes)
void descartar_items(Jugador* jugador);

// Mueve al jugador a una habitacion conectada.
// Calcula el tiempo necesario según el peso total del inventario.
// verifica si el jugador se queda sin tiempo.
bool avanzar_direccion(Jugador* jugador);

// Reinicia completamente la partida
void reiniciar_partida(Grafo** grafo, Jugador** jugador, const char* nombreArchivo);

// Muestra un mensaje final al llegar al escenario de salida.
// Incluye el resumen del inventario y puntaje final.
void mostrar_mensaje_salida(Jugador* jugador);


#endif
