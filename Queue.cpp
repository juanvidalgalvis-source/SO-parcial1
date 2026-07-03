// En su mayoria, este codigo lo escribio la IA porque no sabia mucho que hacer

#include "Queue.h"
#include <cmath>

// Constructor
Queue::Queue(int queueNumber, SchedulingAlgorithm algorithm, double quantum)

// Inicializacion
: queueNumber(queueNumber), algorithm(algorithm), quantum(quantum) {}

// Agrega un Proceso al final de la cola
void Queue::addProcess(Process* p) {readyQueue.push_back(p);}

// Verifica si la cola esta vacia
bool Queue::isEmpty() const {return readyQueue.empty();}

// Muestra cantidad de procesos
int Queue::size() const {return static_cast<int>(readyQueue.size());}

// Getters
int Queue::getQueueNumber() const {return queueNumber;}
SchedulingAlgorithm Queue::getAlgorithm() const {return algorithm;}
double Queue::getQuantum() const {return quantum;}

// Lee el siguiente proceso
Process* Queue::peekNext() const {
	if (readyQueue.empty()) return nullptr; // Si no hay proceso, devuelve un puntero nulo

	if (algorithm == SchedulingAlgorithm::ROUND_ROBIN) {return readyQueue.front();} // Se verifica cola con RR devuelve el primer proceso de la cola

    	// STCF: el que tenga menor tiempo restante.
    	// Empate -> el que llego primero (arrival time menor).
    	Process* best = readyQueue.front(); // Primer proceso se toma como el mejor

	// Recorre todos los procesos de la cola y  busca el mejor (menor tiempo restante)
	for (Process* p : readyQueue) {
        	if (p->getRemainingTime() < best->getRemainingTime() - 1e-9) {
            		best = p; // Si lo encuentra, lo  declara el mejor proceso
        	// Si no, resuelve empates y elige quien llego primero
		} else if (std::abs(p->getRemainingTime() - best->getRemainingTime()) < 1e-9 && p->getArrivalTime() < best->getArrivalTime()) {
            		best = p;}
    	}
    	return best;
}

// La eliminacion de un proceso
void Queue::removeProcess(Process* p) {
    	for (auto it = readyQueue.begin(); it != readyQueue.end(); ++it) {
       		if (*it == p) {
            		readyQueue.erase(it);
            		return;
        	}
    	}
}

// Manda al final de la cola, los procesos que interviene
void Queue::rotateToBack(Process* p) {
    	removeProcess(p);
    	readyQueue.push_back(p);}
