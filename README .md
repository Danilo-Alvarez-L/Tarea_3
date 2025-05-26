# GraphQuest – Juego de Laberinto en Consola

## Descripción

**GraphQuest** es un juego de consola en C que te permite **recorrer un laberinto** representado como un **grafo**. Cada nodo del grafo es un **escenario** (habitación) con:

- Una descripción textual.  
- Conexiones a habitaciones vecinas (arriba, abajo, izquierda, derecha).  
- Ítems con sus datos (nombre, peso en kg, valor en pts).  

El objetivo es **llegar al escenario de salida** (ID 16) **antes de que se acabe el tiempo**, maximizando tu puntaje al recoger ítems. Las reglas que sigue son:

- Cada acción de **recoger** o **descartar** ítem resta **1 unidad de tiempo**.  
- Moverse consume tiempo según tu **peso total**:  
  $$T = \left\lceil\frac{\text{peso\_total} + 1}{10}\right\rceil$$  
- Si llegas al escenario final, ves tu puntaje e inventario; si el tiempo llega a 0, pierdes.

Se implementa modularmente usando **TDAs** en `tdas/graph.h`–`tdas/graph.c`, `tdas/list.h`–`tdas/list.c` y `tdas/extra.h`–`tdas/extra.c`.

---

## Cómo compilar y ejecutar

### Requisitos previos:

- Tener instalado [Visual Studio Code](https://code.visualstudio.com/).
- Instalar la extensión en [Visual Studio Code] **C/C++** (Microsoft).
- Tener instalado un compilador de C (como **gcc**)(https://platzi.com/tutoriales/1189-algoritmos-2017/2765-compilando-cc-desde-visual-studio-code-windows-10/). Si estás en Windows (), se recomienda instalar [MinGW](https://www.mingw-w64.org/) o utilizar el entorno [WSL](https://learn.microsoft.com/en-us/windows/wsl/).

- Estructura de carpetas:
  ```
  .
  ├── main.c
  ├── GraphQuest.c
  ├── GraphQuest.h
  ├── tdas/
  │   ├── graph.c
  │   ├── graph.h
  │   ├── list.c
  │   ├── list.h
  │   ├── extra.c
  │   └── extra.h
  └── data/
      └── graphquest.csv
  ```

### Pasos para compilar y ejecutar:

1. **Descarga y descomprime el archivo `.zip`** en una carpeta de tu elección.
2. **Abre el proyecto en Visual Studio Code**.
    - Inicia Visual Studio Code.
    - Selecciona `Archivo > Abrir carpeta...` y elige la carpeta donde descomprimiste el proyecto.
3. **Compila el código**.
    - Abre el archivo principal (`main.c`).
    - Abre la terminal integrada (`Terminal > Nueva terminal`).
    - En la terminal, compila el programa con el siguiente comando (ajusta el nombre si el archivo principal tiene otro nombre):

   ```
   gcc -I. -Itdas main.c GraphQuest.c tdas/graph.c tdas/list.c tdas/extra.c -o graphquest
   ```

4. **Ejecuta la aplicación:**
   - Una vez compilado, puedes ejecutar la aplicación con:

   ```bash
   ./graphquest
   ```


### **Paso para usar el dataset con el que viene(opcional)**

- seleccionando la opcion 1. Cuando se te pregunte por el CSV, introduce la ruta:

```
data/graphquest.csv
```
Nota: "data/" es necesario porque el archivo se encuentra en la carpeta "data".  
Si tu archivo está en otra ubicación o en la raíz del proyecto, solo debes escribir el nombre del archivo, por ejemplo:

"nombre_del_archivo".csv

---

## Funcionalidades implementadas

1. **Cargar Laberinto desde CSV**  
   - Lee cada línea del archivo y crea los escenarios con sus ítems.  
   - Vuelve a leer el CSV para conectar los nodos según sus IDs.  

2. **Iniciar Partida**  
   - Comienza en el escenario ID 1 (“Entrada principal”).  
   - Muestra estado: descripción, ítems, tiempo, inventario y direcciones posibles.

3. **Recoger Ítem(s)**  
   - Seleccionas un ítem disponible y pasa a tu inventario.  
   - Resta 1 de tiempo, suma peso y puntaje.

4. **Descartar Ítem(s)**  
   - Retiras un ítem del inventario.  
   - Resta 1 de tiempo, resta peso y puntaje.

5. **Avanzar en una Dirección**  
   - Eliges W/A/S/D para moverte.  
   - Resta tiempo según la fórmula.  
   - Detecta victoria (llegas al ID 16) o derrota (tiempo ≤ 0).

6. **Reiniciar Partida**  
   - Recarga el laberinto y reinicia tiempo e inventario.

7. **Salir del Juego**  
   - Libera toda la memoria y cierra la aplicación.

---

## Funcionalidades NO implementadas

- **Modo multijugador colaborativo** (opcional).

---

### Problemas conocidos:
- En una ejecucion de prueba me dejo moverme con el tiempo 0 justo en la camara secreta (ID 15), llegando al final y ganando el juego en el tiempo 0. Esta caso es excepcional porque despues intentando replicarlo pero no me ocurria este error y no se porque paso la verdad.

## A mejorar

- A veces hay que presionar **demasiados Enter** para avanzar el programa, especialmente al llegar a la salida.  
- Mejorar validación de entrada para evitar lecturas inválidas.  
- Liberar completamente todos los recursos (Item*, estructuras internas) al salir.
- Poder recoger/descartar varios items a la vez ya que por el momento solo se puede de uno en uno.
- Si ingresas una direccion de archivo invalida tienes que volver a iniciar programa.
---

## Ejemplo de uso

```bash
$ ./graphquest
========================================
           GraphQuest
========================================
1) Cargar Laberinto desde Archivo CSV
2) Iniciar Partida
3) Salir
Seleccione una opcion: 1
Ingrese el nombre del archivo CSV: data/graphquest.csv
Laberinto cargado exitosamente!
Presione una tecla para continuar...

# Ahora iniciar partida
Seleccione una opcion: 2
========================================
           Estado Actual
========================================
Estas en: Entrada principal
Descripcion: Una puerta rechinante abre paso...
Items disponibles:
 - No hay items aqui.
Datos del Jugador:
  Tiempo restante: 10
  Peso total:     0 kg
  Puntaje:        0 pts
Inventario:
 - Vacío
Habitaciones disponibles:
  - Abajo: Cocina
Elige una acción:
========================================
           Opciones
========================================
1) Recoger Ítem(s)
2) Descartar Ítem(s)
3) Avanzar en una Dirección
4) Reiniciar Partida
5) Salir de la partida
Seleccione una accion: 3
Elige una habitacion (W/A/S/D): S
Te moviste a: Cocina
Tiempo que usaste: 1
Presione una tecla para continuar...
```


## Contribución individual
- Danilo Álvarez Lara