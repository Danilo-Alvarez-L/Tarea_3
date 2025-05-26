#include "tdas/extra.h"
#include "tdas/graph.h"
#include "GraphQuest.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>


Jugador* crear_jugador(Escenario* inicial)
{
    Jugador* jugador = (Jugador*)malloc(sizeof(Jugador));
    if (!jugador)
    {
      perror("Error al asignar memoria para Jugador");
      exit(EXIT_FAILURE);
    }  
    jugador->inventario = list_create();
    jugador->peso_total = 0;
    jugador->puntaje_total = 0;
    jugador->tiempo_restante = 10; // lo podemos ajustar para el juego :3
    jugador->actual = inicial;
    return jugador;
}

void cargarLaberinto(Grafo* grafo, const char* nombreArchivo)
{
  FILE* archivo = fopen(nombreArchivo, "r");
  if (archivo == NULL)
  {
    perror("Error al abrir el archivo");
    exit(EXIT_FAILURE);
  }

  char** campos;

  // Leer encabezado
  campos = leer_linea_csv(archivo, ',');

  // Primer paso: crear todos los escenarios (habitaciones)
  while ((campos = leer_linea_csv(archivo, ',')) != NULL)
  {
    Escenario* escenario = (Escenario*)malloc(sizeof(Escenario));
    if (!escenario)
    {
      perror("Error al asignar memoria para Escenario");
      exit(EXIT_FAILURE);
    }
    escenario->id = atoi(campos[0]);
    strcpy(escenario->nombre, campos[1]);
    strcpy(escenario->descripcion, campos[2]);
    escenario->items = list_create();

    // inicializar conexiones
    escenario->arriba = escenario->abajo = escenario->izquierda = escenario->derecha = NULL;

    // este if-else lo hice con ayuda de chatgpt porque al inicio no se guardaba el escenario final y no sabia porque.
    char* campo_final = campos[8];
if (strcasecmp(campo_final, "1") == 0 ||
    strcasecmp(campo_final, "si") == 0 ||
    strcasecmp(campo_final, "sí") == 0 ||
    strcasecmp(campo_final, "yes") == 0 ||
    strcasecmp(campo_final, "true") == 0)
    {
      escenario->es_final = 1;
    }
    else
    {
      escenario->es_final = 0;
    }

    // declarar items con su nombre peso y puntos
    List* items_txt = split_string(campos[3], ";");
    for (char* item_str = list_first(items_txt); item_str != NULL; item_str = list_next(items_txt))
    {
      List* datos_item = split_string(item_str, ",");
      if (list_size(datos_item) >= 3)
      {
        Item* item = (Item*)malloc(sizeof(Item));
        strcpy(item->nombre, list_first(datos_item));
        item->valor = atoi(list_next(datos_item));
        item->peso = atoi(list_next(datos_item));
        list_pushBack(escenario->items, item);
      }
      list_clean(datos_item);
      free(datos_item);
    }
    list_clean(items_txt);
    free(items_txt);

    agregar_nodo(grafo, escenario);
  }

  // Segundo paso: volver a recorrer y conectar los escenarios
  rewind(archivo);              // Volvemos al inicio del archivo
  leer_linea_csv(archivo, ','); // Saltamos encabezado otra vez

  // Recorremos de nuevo el CSV para enlazar cada habitacion con sus vecinos
  while ((campos = leer_linea_csv(archivo, ',')) != NULL)
  {
    int id = atoi(campos[0]);
    Escenario* origen = (Escenario*)buscar_nodo(grafo, id);
    if (!origen)
    {
      continue;
    }

    int arriba = atoi(campos[4]);
    int abajo = atoi(campos[5]);
    int izquierda = atoi(campos[6]);
    int derecha = atoi(campos[7]);

    if (arriba != -1)
    {
      Escenario* destino = (Escenario*)buscar_nodo(grafo, arriba);
      conectar_nodos(origen, destino, "arriba");
    }
    if (abajo != -1)
    {
      Escenario* destino = (Escenario*)buscar_nodo(grafo, abajo);
      conectar_nodos(origen, destino, "abajo");
    }
    if (izquierda != -1)
    {
      Escenario* destino = (Escenario*)buscar_nodo(grafo, izquierda);
      conectar_nodos(origen, destino, "izquierda");
    }
    if (derecha != -1)
    {
      Escenario* destino = (Escenario*)buscar_nodo(grafo, derecha);
      conectar_nodos(origen, destino, "derecha");
    }
  }

  fclose(archivo);
}

bool mostrar_escenarios(Jugador* jugador)
{
    Escenario* escenario = jugador->actual;

    // Si es el escenario final, termina la partida
    if (jugador->actual->es_final)
    {
      mostrar_mensaje_salida(jugador); //mostrar mensaje de felicitaciones :3
      return true;
    }

    limpiarPantalla();
    puts("========================================");
    puts("           Estado Actual");
    puts("========================================");
    printf("Estas en: %s\n", escenario->nombre);
    printf("Descripcion: %s\n\n", escenario->descripcion);

    // lista de los items en la habitacion
    printf("Items disponibles en esta habitacion:\n");
    if (list_size(escenario->items) == 0)
    {
        printf(" - No hay items aqui. :(\n");
    } else
    {
        for (Item* item = list_first(escenario->items); item != NULL; item = list_next(escenario->items))
        {
            printf("  - %s (%d kg, %d pts)\n", item->nombre, item->peso, item->valor);
        }
    }

    // datos del jugador 
    printf("\n  Datos del Jugador:\n");
    printf("  Tu tiempo restante es: %d\n", jugador->tiempo_restante);
    printf("  Llevas de peso: %d kg.\n", jugador->peso_total);
    printf("  Tu puntaje actual es de: %d pts. Quizas puedas intentar sumar mas :3\n", jugador->puntaje_total);

    // inventario del jugador
    printf("\n  Tu Inventario:\n");

    if (list_size(jugador->inventario) == 0)
    {
      printf("  - Tienes el Inventario vacio. recuerda acumular puntos :3\n");
    }
    else
    {
        for (Item* it = list_first(jugador->inventario); it != NULL; it = list_next(jugador->inventario))
        {
          printf("  - %s (%d pts, %d kg)\n", it->nombre, it->valor, it->peso);
        }
        printf("  Buen loot :D\n");
    }

    // Habitaciones a las que puede ir desde donde esta
    printf("\nHabitaciones a las que puedes ir:\n");
    if (escenario->arriba)     printf("  - Arriba: %s\n", escenario->arriba->nombre);
    if (escenario->abajo)      printf("  - Abajo: %s\n", escenario->abajo->nombre);
    if (escenario->izquierda)  printf("  - Izquierda: %s\n", escenario->izquierda->nombre);
    if (escenario->derecha)    printf("  - Derecha: %s\n", escenario->derecha->nombre);
    printf("Elige sabiamente 0.0\n");
    return false;
}

void recoger_items(Jugador* jugador)
{
    Escenario* escenario = jugador->actual; // ir al escenario donde esta el jugador

    // verificar si hay items en esta habitacion o no
    if (list_size(escenario->items) == 0)
    {
        printf("No hay items aqui :( quizas en otra habitacion encuentres.\n");
        presioneTeclaParaContinuar();
        return;
    }

    printf("\nMira bien, puedes llevarte:\n");

    // si hay items muestra los disponibles y su numero para llevarlo
    int i = 1;
    for (Item* item = list_first(escenario->items); item != NULL; item = list_next(escenario->items))
    {
        printf("%d) %s (%d pts, %d kg)\n", i++, item->nombre, item->valor, item->peso);
    }

    printf("\nIngrese el numero del Item que quieres llevarte o usa '0' para cancelar: ");
    int opcion;
    scanf("%d", &opcion);
    if (opcion <= 0 || opcion > list_size(escenario->items))
    {
      return;
    }

    // Buscar el item en la opcion elegida del jugador
    i = 1;
    Item* item = list_first(escenario->items);
    while (i < opcion)
    {
        item = list_next(escenario->items);
        i++;
    }

    // Lo sacamos de la lista del escenario y lo pasamos al inventario
    list_popCurrent(escenario->items);
    list_pushBack(jugador->inventario, item);

    // Actualizar estado del jugador
    jugador->puntaje_total += item->valor;
    jugador->peso_total += item->peso;
    jugador->tiempo_restante -= 1;

    printf("Has tomado: %s. Buena decision :3\n", item->nombre);
    printf("Tiempo restante: %d. Ten cuidado de quedarte sin tiempo.\n", jugador->tiempo_restante);
    presioneTeclaParaContinuar();
}

void descartar_items(Jugador* jugador)
{
    // verificar si tiene items en el inventario
    if (list_size(jugador->inventario) == 0)
    {
        printf("Tu inventario esta vacio. solo descartarias atomos (._.)\n");
        presioneTeclaParaContinuar();
        return;
    }

    printf("\n Tienes en tu inventario:\n");
    int i = 1;
    for (Item* item = list_first(jugador->inventario); item != NULL; item = list_next(jugador->inventario))
    {
        printf("%d) %s (%d pts, %d kg)\n", i++, item->nombre, item->valor, item->peso);
    }

    printf("\nSeguro que quieres dejar algo?\n");
    printf("\nIngrese el numero del item que vas a descartar o usa '0' si te arrepientes: ");

    int opcion;
    scanf("%d", &opcion);
    if (opcion <= 0 || opcion > list_size(jugador->inventario))
    {
      return;
    }

    // Buscar el ítem a descartar
    i = 1;
    Item* item = list_first(jugador->inventario);
    while (i < opcion)
    {
        item = list_next(jugador->inventario);
        i++;
    }

    // Eliminar del inventario y actualizar estado (peso puntaje y tiempo)
    list_popCurrent(jugador->inventario);
    jugador->puntaje_total -= item->valor;
    jugador->peso_total -= item->peso;
    jugador->tiempo_restante -= 1;

    printf("Has dejado: %s. Espero no te arrepientas :3\n", item->nombre);
    printf("Ahora te mueves mas rapido, cuidado flash. ;)\n");
    printf("Tu tiempo restante: %d. No te quedes sin tiempo.\n", jugador->tiempo_restante);
    presioneTeclaParaContinuar();
    free(item); // liberar memoria del ítem
}

bool avanzar_direccion(Jugador* jugador)
{
    Escenario* escenario = jugador->actual;
    char dir;

    // muestra las opciones disponibles
    printf("\nPuedes ir a:\n");
    if (escenario->arriba)     printf("W) Arriba: %s\n", escenario->arriba->nombre);
    if (escenario->abajo)      printf("S) Abajo: %s\n", escenario->abajo->nombre);
    if (escenario->izquierda)  printf("A) Izquierda: %s\n", escenario->izquierda->nombre);
    if (escenario->derecha)    printf("D) Derecha: %s\n", escenario->derecha->nombre);

    printf("\nElige una habitacion (W/A/S/D o usa otra tecla para cancelar): ");
    scanf(" %c", &dir);

    // declara la variable destino para saber hacia que habitacion se mueve el jugador
    Escenario* destino = NULL;
    if ((dir == 'W' || dir == 'w') && escenario->arriba)
    {
      destino = escenario->arriba;
    }
    else if ((dir == 'S' || dir == 's') && escenario->abajo)
    {
      destino = escenario->abajo;
    }
    else if ((dir == 'A' || dir == 'a') && escenario->izquierda)
    {
      destino = escenario->izquierda;
    }
    else if ((dir == 'D' || dir == 'd') && escenario->derecha)
    {
      destino = escenario->derecha;
    }
    else
    {
        printf("Esa habitacion la inventaste. (0_0)\n");
        presioneTeclaParaContinuar();
        return false;
    }

    // Calcular el tiempo que toma avanzar
    int peso = jugador->peso_total;
    int costo_tiempo = (int)ceil(((double)peso + 1.0) / 10);

    jugador->tiempo_restante -= costo_tiempo;
    jugador->actual = destino;

    printf("\nTe moviste a: %s\n", destino->nombre);
    printf("Tiempo que usaste: %d\n", costo_tiempo);

    // si el jugador gano
    if (jugador->actual->es_final)
    {
      mostrar_mensaje_salida(jugador);
      return false;
    }
    
  // Si no es la salida y el tiempo se agoto
  if (jugador->tiempo_restante <= 0)
  {
    jugador->tiempo_restante = 0;
    printf("\nTe quedaste sin tiempo. Perdiste :( quizas la proxima sea mejor.\n");
    presioneTeclaParaContinuar();
    return true;
  }

return false;

}

void reiniciar_partida(Grafo** grafo, Jugador** jugador, const char* nombreArchivo)
{
  liberar_grafo(*grafo);
  *grafo = crear_grafo();
  cargarLaberinto(*grafo, nombreArchivo);
  *jugador = crear_jugador((Escenario*)buscar_nodo(*grafo, 1));
  printf("Partida reiniciada. Mucha Suerte :3\n");
  presioneTeclaParaContinuar();
}

void mostrar_mensaje_salida(Jugador* jugador)
{
  printf("\nLlegaste a la Salida. Felicidades\n");
  printf("\nTu inventario final fue:\n");

  if (list_size(jugador->inventario) == 0)
  {
    printf("  - No recogiste ningun item (0_0) quizas deberias volver a jugar.\n");
  }
  else
  {
    for (Item* item = list_first(jugador->inventario); item != NULL; item = list_next(jugador->inventario))
    {
      printf("  - %s (%d kg, %d pts)\n", item->nombre, item->peso, item->valor);
    }
    printf("Peso que llevaste: %d kg. Cuidado con tu espalda D:\n", jugador->peso_total);
  }

  printf("\nTu puntaje final fue: %d. Intenta superarlo :3\n\n", jugador->puntaje_total);
  presioneTeclaParaContinuar();
}