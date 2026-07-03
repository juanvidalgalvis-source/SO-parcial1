#ifndef FILEWRITER_H
#define FILEWRITER_H

#include <string>
#include <vector>
#include "Process.h"
#include "Scheduler.h" // aqui vive AverageMetrics

class FileWriter {
public:
    // Escribe el archivo de salida con el formato pedido en el enunciado:
    //   # archivo: <nombre del archivo de entrada>
    //   # etiqueta; BT; AT; Q; Pr; WT; CT; RT; TAT
    //   A;6; 0; 1; 5; 3; 9; 0; 9
    //   ...
    //   # WT=..; CT=..; RT=..; TAT=..;
    //
    // outputFilename: ruta donde se va a crear el archivo de salida
    // processes: vector ya con las metricas calculadas (viene de Scheduler::getProcesses())
    // averages: promedios calculados (viene de Scheduler::calculateAverages())
    // originalFilename: nombre del archivo de entrada, solo para el encabezado
    static void writeFile(const std::string& outputFilename,
                           const std::vector<Process>& processes,
                           const AverageMetrics& averages,
                           const std::string& originalFilename);

private:
    // Convierte un double a texto: sin decimales si es un numero entero,
    // con 2 decimales si no lo es (para que se vea igual que en el archivo de entrada)
    static std::string formatNumber(double value);

    // Arma la linea de un proceso individual con el orden de campos requerido
    static std::string buildProcessLine(const Process& p);
};

#endif
