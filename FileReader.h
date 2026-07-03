#ifndef FILEREADER_H
#define FILEREADER_H

#include <string>
#include <vector>
#include "Process.h"

class FileReader {
	public:
	// lee y crea un vector de objetos Process
	static std::vector<Process> readFile(const std::string& filename);

	private:
	// Buena practica para eliminar espacios en blanco, sugerido por IA
	static std::string trim(const std::string& s);
	// Separa la informacion por bloques
	static Process parseLine(const std::string& line);
};

#endif
