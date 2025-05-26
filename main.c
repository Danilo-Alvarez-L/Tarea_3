#include <stdio.h>
#include <stdbool.h>
#include "tdas/extra.h"
#include "tdas/graph.h"
#include "GraphQuest.h"

char archivoUsado[100];

void mostrarMenuPrincipal()
{
    limpiarPantalla();
    puts("========================================");
    puts("           GraphQuest           ");
    puts("========================================");
    puts("1) Cargar Laberinto desde Archivo CSV");
    puts("2) Iniciar Partida");
    puts("3) Salir");
}

void mostrarMenuJuego()
{
    puts("========================================");
    puts("           Opciones          ");
    puts("========================================");
    puts("1) Recoger Item(s)");
    puts("2) Descartar Item(s)");
    puts("3) Avanzar en una Direccion");
    puts("4) Reiniciar Partida");
    puts("5) Salir de la partida");
}

int main()
{
    char opcion;
    Grafo* grafo = NULL;
    Jugador* jugador = NULL;

    do {
        mostrarMenuPrincipal();
        printf("Seleccione una opcion: ");
        scanf(" %c", &opcion);

        switch (opcion)
        {
            // carga el laberinto del archivo csv
            case '1':
                printf("Ingrese el nombre del archivo CSV: ");
                scanf("%s", archivoUsado);
                grafo = crear_grafo();
                cargarLaberinto(grafo, archivoUsado);
                jugador = crear_jugador((Escenario*)buscar_nodo(grafo, 1));
                printf("Laberinto cargado exitosamente!\n");
                presioneTeclaParaContinuar();
                break;
            // verifica si hay un laberinto y jugador cargado
            case '2':
                if (grafo == NULL || jugador == NULL) {
                    printf("Debes cargar el laberinto primero.\n");
                    presioneTeclaParaContinuar();
                    break;
                }

                // Entramos al ciclo de juego
                char opcionJuego;
                do {
                    // Si el jugador gana, se reinicia la partida automáticamente
                    bool jugador_gano = mostrar_escenarios(jugador);
                    if (jugador_gano)
                    {
                        printf("\nEnter para jugar de nuevo\n");
                        presioneTeclaParaContinuar();
                        reiniciar_partida(&grafo, &jugador, archivoUsado);
                        continue;
                    }
                    
                    mostrarMenuJuego();
                    printf("Seleccione una accion: ");
                    scanf(" %c", &opcionJuego);

                    switch (opcionJuego)
                    {
                        case '1':
                            recoger_items(jugador);
                            break;
                        case '2':
                            descartar_items(jugador);
                            break;
                        case '3':
                        {
                            bool perdio = avanzar_direccion(jugador);
                            if (perdio)
                            {
                                // Si el jugador se queda sin tiempo, reiniciar partida
                                reiniciar_partida(&grafo, &jugador, archivoUsado);
                                continue;
                            }
                            break;
                        }
                        case '4':
                            reiniciar_partida(&grafo, &jugador, archivoUsado);
                            break;
                        case '5':
                            printf("Saliendo de la partida..\n");
                            break;
                        default:
                            printf("Opcion invalida. Intente nuevamente.\n");
                            presioneTeclaParaContinuar();
                    }

                } while (opcionJuego != '5');

                break;

            case '3':
                printf("Gracias por jugar GraphQuest <3 Nos vemos.\n");
                break;

            default:
                printf("Opcion no encontrada. Por favor, intente de nuevo.\n");
                presioneTeclaParaContinuar();
        }

    } while (opcion != '3');

    //libera memoria del grafo
    liberar_grafo(grafo);
    return 0;
}
