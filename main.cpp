#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <limits>
#include <filesystem>
#include "Process.h"
#include "FileReader.h"
#include "Scheduler.h"
#include "FileWriter.h"

namespace fs = std::filesystem;

// Se hace una lista de los archivos
static std::vector<std::string> listTxtFiles(const std::string& directory) {
	std::vector<std::string> files;

	if (!fs::exists(directory) || !fs::is_directory(directory)) { return files;}

	for (const auto& entry : fs::directory_iterator(directory)) {
		if (entry.is_regular_file() && entry.path().extension() == ".txt") {
			files.push_back(entry.path().filename().string());}
	}

	std::sort(files.begin(), files.end());
	return files;

}

// Le pide al usuario que elija un numero de la lista mostrada; repite si el valor es invalido
static int askUserToChoose(int totalOptions) {
	int choice = 0;

	while (true) {
		std::cout << "Seleccione un archivo (1-" << totalOptions << "): ";
		std::cin >> choice;

		if (std::cin.fail() || choice < 1 || choice > totalOptions) {
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			std::cout << "Opcion invalida, intente de nuevo." << std::endl;
			continue;
		}
		break;
	}
	return choice;
}

int main(int argc, char* argv[]) {

	const std::string dataDir = "data";
	const std::string outputDir = "output";

	std::vector<std::string> txtFiles = listTxtFiles(dataDir);

	if (txtFiles.empty()) {
		std::cerr << "No se encontraron archivos .txt en '" << dataDir << "'." << std::endl;
		return 1;
	}

	std::cout << "Archivos disponibles en '" << dataDir << "':" << std::endl;
	for (size_t i = 0; i < txtFiles.size(); ++i) {
		std::cout << "  " << (i + 1) << ". " << txtFiles[i] << std::endl;
	}

	int choice = askUserToChoose(static_cast<int>(txtFiles.size()));
	std::string inputFilename = txtFiles[choice - 1];
	std::string inputPath = dataDir + "/" + inputFilename;

	// Lectura de un archivo
	std::vector<Process> processes = FileReader::readFile(inputPath);

	//SIMULACION
	Scheduler scheduler(processes);
	scheduler.run();

	AverageMetrics averages = scheduler.calculateAverages();

	// Escritura del archivo de salida
	fs::create_directories(outputDir);
	std::string outputFilename = outputDir + "/" + fs::path(inputFilename).stem().string() + "_output.txt";

	FileWriter::writeFile(outputFilename, scheduler.getProcesses(), averages, inputFilename);

	std::cout << "Archivo de salida generado en: " << outputFilename << std::endl;

	return 0;
}

