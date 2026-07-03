#include <iostream>
#include <vector>
#include "Process.h"
#include "FileReader.h"

int main(int argc, char* argv[]) {

	std::string filename = "data/mlq001.txt";
	if (argc > 1) {
		filename = argv[1];}

	std::vector<Process> processes = FileReader::readFile(filename);

	// ESTO LO PUEDO ELLIMINAR A FUTURO
	std::cout << "Se leyeron " << processes.size() << " procesos: " << std::endl;
	std::cout << "_____________________" << std::endl;

	for (const Process& p : processes) {p.print();}

	return 0;
}
