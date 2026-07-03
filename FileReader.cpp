#include "FileReader.h"
#include <iostream>
#include <fstream> // permite leer archivos
#include <sstream> // permite "flujo de texto" buscar
#include <stdexcept>  // permite lanzar excepciones, muestra buenas practicas

// Buenas practicas, fue sugerido por IA
std::string FileReader::trim(const std::string& s) {
	size_t start = s.find_first_not_of(" \t\r\n");
	if(start == std::string::npos) return "";
	size_t end = s.find_last_not_of(" \t\r\n");
	return s.substr(start, end - start + 1);}

Process FileReader::parseLine(const std::string& line) {
	std::vector<std::string> fields;
	std::stringstream ss(line);
	std::string field;

	// Lineas para asegurarse de que  el campo creado tenga 5 campos
	while (std::getline(ss, field, ';')) {
		fields.push_back(trim(field));}
	// y si no es de 5, lanza un error
	if (fields.size() !=5) {
		throw std::runtime_error("Linea con formato invalido:" + line);}

	std::string id = fields[0];
	double burstTime = std::stod(fields[1]);
	double arrivalTime = std::stod(fields[2]);
	int queueNumber = std::stoi(fields[3]);
	int priority = std::stoi(fields[4]);

	return Process(id, burstTime, arrivalTime, queueNumber, priority);
}

// Esta parte es implementada por IA
std::vector<Process> FileReader::readFile(const std::string& filename) {
	std::ifstream file(filename);
	if (!file.is_open()) {
		throw std::runtime_error("No se pudo abrir el archivo" + filename);}

	// Se crea el vector de procesos
	std::vector<Process> processes;
	std::string line;

	while (std::getline(file, line)) {
		std::string trimmedLine = trim(line);

		// Esto hace que se ignoren todos los comentarios del txt, #, y las lineas vacias
		if (trimmedLine.empty() || trimmedLine[0] == '#') {
		continue;}

		try {
			processes.push_back(parseLine(trimmedLine));
		} catch (const std::exception& e) {
			std::cerr << "Advertencia: se omitio una linea." << e.what() << std::endl;
		}

	}

	file.close();
	return processes;

}
