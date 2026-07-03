#include "FileWriter.h"
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cmath>
#include <stdexcept>

std::string FileWriter::formatNumber(double value) {
    std::ostringstream oss;
    if (std::abs(value - std::round(value)) < 1e-9) {
        // Es un numero entero (ej: 6.0 -> "6"), igual que en el archivo de entrada
        oss << static_cast<long long>(std::round(value));
    } else {
        oss << std::fixed << std::setprecision(2) << value;
    }
    return oss.str();
}

// Mismo orden de campos que tenia Process::print(), pero en formato de linea .txt
// en vez de formato "ID: .. | BT: .." para consola
std::string FileWriter::buildProcessLine(const Process& p) {
    std::ostringstream line;
    line << p.getId() << ";"
         << formatNumber(p.getBurstTime()) << "; "
         << formatNumber(p.getArrivalTime()) << "; "
         << p.getQueueNumber() << "; "
         << p.getPriority() << "; "
         << formatNumber(p.getWaitingTime()) << "; "
         << formatNumber(p.getCompletionTime()) << "; "
         << formatNumber(p.getResponseTime()) << "; "
         << formatNumber(p.getTurnaroundTime());
    return line.str();
}

void FileWriter::writeFile(const std::string& outputFilename,
                            const std::vector<Process>& processes,
                            const AverageMetrics& averages,
                            const std::string& originalFilename) {
    std::ofstream file(outputFilename);
    if (!file.is_open()) {
        throw std::runtime_error("No se pudo crear el archivo de salida: " + outputFilename);
    }

    file << "# archivo: " << originalFilename << "\n";
    file << "# etiqueta; BT; AT; Q; Pr; WT; CT; RT; TAT\n";

    for (const Process& p : processes) {
        file << buildProcessLine(p) << "\n";
    }

    file << "# WT=" << formatNumber(averages.avgWaitingTime)
         << "; CT=" << formatNumber(averages.avgCompletionTime)
         << "; RT=" << formatNumber(averages.avgResponseTime)
         << "; TAT=" << formatNumber(averages.avgTurnaroundTime) << ";\n";

    file.close();
}
