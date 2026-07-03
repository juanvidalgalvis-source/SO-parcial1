#ifndef QUEUE_H
#define QUEUE_H

#include <deque> // Decicion tomada por el STCF, con la IA
#include "Process.h"

// Algoritmos de planificacion, en esto me ayudo a pensar la IA, como si fuese una "maquina de estados"
enum class SchedulingAlgorithm {
    	ROUND_ROBIN,
    	STCF};

class Queue {
private:
    	int queueNumber;
    	SchedulingAlgorithm algorithm;
    	double quantum;
    	std::deque<Process*> readyQueue;  // Un Deque facilita la planificacion del STCF y se puede usar en general

public:
    	// quantum se ignora si el algoritmo no es ROUND_ROBIN
    	Queue(int queueNumber, SchedulingAlgorithm algorithm, double quantum = 0.0);

    	void addProcess(Process* p); // Agregar proceso
    	bool isEmpty() const;
    	int size() const;

    	int getQueueNumber() const;
    	SchedulingAlgorithm getAlgorithm() const;
    	double getQuantum() const;

    	Process* peekNext() const; // Indica cual proceso se ejecuta en cpu segun el algoritmo

    	void removeProcess(Process* p); // Remueve un proceso cuando este termina

    	void rotateToBack(Process* p); // Para RR, devuelve procesos al final de la cola
};

#endif
