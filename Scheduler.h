#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <vector>
#include "Process.h"
#include "Queue.h"

// Metricas promedio del vector de onjetos Process
struct AverageMetrics {
    	double avgWaitingTime;
    	double avgCompletionTime;
    	double avgResponseTime;
    	double avgTurnaroundTime;
};

class Scheduler {
private:
    	std::vector<Process> processes; // TODOS los procesos (dueño de la memoria real)
    	Queue queue1; // Q1: Round Robin, quantum = 2
    	Queue queue2; // Q2: Round Robin, quantum = 3
    	Queue queue3; // Q3: STCF
    	double currentTime;

    	// --- Helpers internos de la simulacion ---
    	void admitArrivals(std::vector<bool>& admitted);
    	double getNextArrivalTime(const std::vector<bool>& admitted) const;
    	Queue* getActiveQueue(); // cola de mayor prioridad que no este vacia (Q1>Q2>Q3)

public:
    	explicit Scheduler(std::vector<Process> processVector);

    	// Corre la simulacion completa hasta que todos los procesos terminen
    	void run();

    	// Devuelve el vector de procesos ya con sus metricas calculadas
    	std::vector<Process>& getProcesses();

    	// Calcula los promedios de WT, CT, RT y TAT sobre todos los procesos
    	AverageMetrics calculateAverages() const;
};

#endif
