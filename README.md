# Simulador de Planificador MLQ (Multilevel Queue)

Simulador orientado a objetos en C++ del algoritmo de planificación **MLQ (Multilevel Queue)**, desarrollado para el Parcial No. 1 del curso de Sistemas Operativos (Universidad del Valle).

El simulador organiza los procesos en 3 colas con prioridad fija entre ellas y despacha la CPU según la política de cada cola:

| Cola | Algoritmo | Quantum |
|------|-----------|---------|
| Q1   | Round Robin | 2 |
| Q2   | Round Robin | 3 |
| Q3   | STCF (Shortest Time to Completion First) | — |

La prioridad entre colas es fija: **Q1 > Q2 > Q3** (apropiativo entre niveles: si llega un proceso a una cola de mayor prioridad, desaloja inmediatamente al que se esté ejecutando en una cola inferior).

## Estructura del proyecto

```
.
├── data/                   # Archivos de entrada (.txt)
│   ├── mlq001.txt
│   └── mlq002.txt
├── output/                 # Archivos de salida generados por el programa
├── Process.h / Process.cpp     # Modela un proceso y sus métricas
├── Queue.h / Queue.cpp         # Modela una cola de planificación (RR o STCF)
├── Scheduler.h / Scheduler.cpp # Orquesta la simulación completa
├── FileReader.h / FileReader.cpp # Lee el .txt de entrada y crea los Process
├── FileWriter.h / FileWriter.cpp # Escribe el .txt de salida con las métricas
├── main.cpp                    # Punto de entrada: menú interactivo
└── README.md
```

## Compilación

Desde la raíz del proyecto (donde están todos los `.cpp`):

```bash
g++ -std=c++17 main.cpp Process.cpp FileReader.cpp Queue.cpp Scheduler.cpp FileWriter.cpp -o programa
```

> Requiere C++17 porque `main.cpp` usa `<filesystem>` para listar los archivos de `data/`.

## Ejecución

```bash
./programa
```

El programa:

1. Lista los archivos `.txt` encontrados en la carpeta `data/`.
2. Pide al usuario que elija uno escribiendo su número (valida la entrada; si es inválida, vuelve a preguntar).
3. Lee el archivo, corre la simulación completa y calcula las métricas de cada proceso.
4. Genera el archivo de salida en `output/<nombre_del_archivo>_output.txt`.

Ejemplo de uso:

```
Archivos disponibles en 'data':
  1. mlq001.txt
  2. mlq002.txt
Seleccione un archivo (1-2): 1
Archivo de salida generado en: output/mlq001_output.txt
```

No se imprime ningún rastro de la simulación en consola (ni el "Gantt" de tiempos ni las métricas): todo el resultado queda en el archivo de salida.

## Formato del archivo de entrada

```
# Archivo: mlq001.txt
# etiqueta; burst time (BT); arrival time (AT);Queue (Q);Priority(5>1)
A;6; 0; 1; 5
B;9; 0; 1; 4
C;10; 0; 2; 3
D;15; 0; 2; 3
E;8;0; 3; 2
```

Las líneas que empiezan con `#` se ignoran (comentarios). Cada línea de proceso tiene 5 campos separados por `;`:

| Campo | Tipo | Significado |
|-------|------|--------------|
| etiqueta | String | Identificador del proceso |
| BT | Float | Burst time: tiempo total de CPU que necesita |
| AT | Float | Arrival time: momento en que llega al sistema |
| Q | Integer | Cola a la que pertenece (1, 2 o 3) |
| Priority | Integer | Prioridad dentro de su cola (5 = más alta, 1 = más baja). No se usa actualmente porque ninguna de las 3 colas planifica por prioridad. |

## Formato del archivo de salida

```
# archivo: mlq001.txt
# etiqueta; BT; AT; Q; Pr; WT; CT; RT; TAT
A;6; 0; 1; 5; 4; 10; 0; 10
B;9; 0; 1; 4; 6; 15; 2; 15
C;10; 0; 2; 3; 24; 34; 15; 34
D;15; 0; 2; 3; 25; 40; 18; 40
E;8; 0; 3; 2; 40; 48; 40; 48
# WT=19.80; CT=29.40; RT=15; TAT=29.40;
```

Métricas calculadas por proceso:

- **WT** (Waiting Time): tiempo total que el proceso esperó en las colas.
- **CT** (Completion Time): instante en que el proceso terminó.
- **RT** (Response Time): tiempo entre que llegó y la primera vez que tocó CPU.
- **TAT** (Turnaround Time): tiempo total desde que llegó hasta que terminó (`CT - AT`).

La última línea del archivo muestra el promedio de cada métrica sobre todos los procesos.

## Diseño y supuestos de la simulación

- **Simulación por eventos** (no por "ticks" fijos de 1 en 1): en cada paso, el `Scheduler` calcula cuánto tiempo puede correr el proceso activo sin interrupción (lo que le falte, su quantum, o hasta la próxima llegada, lo que sea menor). Esto permite manejar tiempos con decimales sin perder precisión ni rendimiento.
- **Prioridad fija entre colas**: Q1 siempre desaloja a Q2 y Q3; Q2 siempre desaloja a Q3.
- **STCF es apropiativo dentro de su propia cola**: si llega un proceso más corto a Q3 mientras otro se ejecuta, lo interrumpe.
- **Al ser interrumpido** (se acaba el quantum o lo desaloja una cola de mayor prioridad), un proceso de una cola RR vuelve al final de su cola con el quantum reiniciado.
- El campo **Priority** del archivo de entrada se guarda en cada `Process` pero no participa en la planificación actual, porque ninguna de las 3 colas usa un algoritmo por prioridad.

## Arquitectura (paradigma orientado a objetos)

- **`Process`**: encapsula los datos de un proceso (BT, AT, Q, Priority) y las métricas que el `Scheduler` calcula sobre él (WT, CT, RT, TAT, tiempo restante).
- **`Queue`**: encapsula una cola de planificación con su propia política (Round Robin o STCF); decide internamente cuál de sus procesos debe ejecutarse a continuación.
- **`Scheduler`**: mantiene el reloj de la simulación, decide qué cola tiene prioridad, ejecuta la simulación completa y calcula los promedios finales.
- **`FileReader`**: convierte un `.txt` de entrada en un `vector<Process>`.
- **`FileWriter`**: convierte el `vector<Process>` con métricas calculadas y los promedios en el `.txt` de salida.

## Pruebas realizadas

Se validó el funcionamiento con al menos los siguientes archivos de entrada:

- `data/mlq001.txt` — 5 procesos, todos con `AT=0`, distribuidos en las 3 colas.
- `data/mlq002.txt` — 4 procesos con llegadas escalonadas (`AT` distintos), para verificar que la admisión de procesos y la preferencia entre colas funcionan correctamente cuando no todos llegan al mismo tiempo.
