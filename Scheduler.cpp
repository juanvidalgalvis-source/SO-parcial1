#include "Scheduler.h"
#include <iostream> // usado solo para advertencias
#include <limits>
#include <algorithm>

// Constructor
Scheduler::Scheduler(std::vector<Process> processVector)
// Inicializacion
: processes(std::move(processVector)),
	queue1(1, SchedulingAlgorithm::ROUND_ROBIN, 2.0),
      	queue2(2, SchedulingAlgorithm::ROUND_ROBIN, 3.0),
      	queue3(3, SchedulingAlgorithm::STCF, 0.0),
      	currentTime(0.0) {} // Tiempo en 0

// Revisa los procesos de la cola // ESTO LO IMPLEMENTO LA IA
void Scheduler::admitArrivals(std::vector<bool>& admitted) {
    	for (size_t i = 0; i < processes.size(); ++i) {
        	if (!admitted[i] && processes[i].getArrivalTime() <= currentTime + 1e-9) {
            	admitted[i] = true;
            	Process* p = &processes[i];
            	switch (p->getQueueNumber()) {
                	case 1: queue1.addProcess(p); break;
                	case 2: queue2.addProcess(p); break;
                	case 3: queue3.addProcess(p); break;
                	default:
                    	std::cerr << "Advertencia: proceso " << p->getId() << " tiene una cola invalida (" << p->getQueueNumber() << "), se ignora." << std::endl;
            		}
        	}
    	}
}

// Busca quien es el proximo en llegar // ESTO LO IMPLEMENTO LA IA
double Scheduler::getNextArrivalTime(const std::vector<bool>& admitted) const {
    	double next = std::numeric_limits<double>::infinity();
    	for (size_t i = 0; i < processes.size(); ++i) {
        	if (!admitted[i]) {
            		next = std::min(next, processes[i].getArrivalTime());
        	}
    	}
    	return next;}

// Busca la Queue que esté activa
Queue* Scheduler::getActiveQueue() {
    	// Prioridad fija entre colas: Q1 > Q2 > Q3
    	if (!queue1.isEmpty()) return &queue1;
    	if (!queue2.isEmpty()) return &queue2;
    	if (!queue3.isEmpty()) return &queue3;
    	return nullptr;}


// SIMULACION -----------------------------------------------------------------------------------
void Scheduler::run() {
    	size_t n = processes.size(); // Guarda el tammaño del vector de objetos Process
    	if (n == 0) return; // Si no hay proceso, termina inmediatamente

    	std::vector<bool> admitted(n, false); // Se crea un vector de admitidos en cola
    	size_t finishedCount = 0; // Cuenta procesos terminados
    	currentTime = 0.0; // Inicializa el reloj de la simulacion

    	while (finishedCount < n) { // Ejecuta la simulacion hasta que todos los objetos terminen
        	admitArrivals(admitted); // Agrega procesos admitidos
        	Queue* active = getActiveQueue(); //Obtiene la Queue activa/prioritaria

        	if (!active) { // No hay ningun proceso listo todavia, busca cuando llegara el siguiente proceso
            		double nextArrival = getNextArrivalTime(admitted);
            		if (nextArrival == std::numeric_limits<double>::infinity()) {break;} // Comprueba si no quedan más procesos y finaliza la simulacion 
            	currentTime = nextArrival; // Avanza el reloj hasta que el siguiente llegue
            	continue; 
       	}

        Process* p = active->peekNext(); // Obtiene el proceso que debe de ejecutar la cpu

        if (!p->isStarted()) { // Comprueba si es la primera vez usando la cpu
            	p->setStartTime(currentTime); // Se guarda el instante donde comenzo
            	p->setResponseTime(currentTime - p->getArrivalTime()); // Se calcula el responseTime
            	p->setStarted(true);} // Marca proceso iniciado

        	double quantumLimit = (active->getAlgorithm() == SchedulingAlgorithm::ROUND_ROBIN) // Cuanto tiempo como maximo puede ejecutarse
                                   ? active->getQuantum()  // Mira si la cola es RR y usa el quantum
                                   : std::numeric_limits<double>::infinity(); // Si no cumple lo primero, entonces es STCF y no tiene limite de tiempo

        	double nextArrivalGap = getNextArrivalTime(admitted) - currentTime; // Cuanto falta para que llegue el siguiente proceso
        if (nextArrivalGap < 0) nextArrivalGap = 0; // No existen tiempos negativos, es por si ha un error //Preguntar si esta de más

        	double runDuration = std::min({p->getRemainingTime(), quantumLimit, nextArrivalGap}); // Se decide cuanto tiempo se ejecutara el proceso (tiempo restante, limite de tiempo, siguiente en arrivar)

		// -------------------------------------
        	//double start = currentTime; // Se guarda ese instante inicial
	       	// ---------------------------------------

		currentTime += runDuration; // Avanza el reloj del 
        	p->decreaseRemainingTime(runDuration); // Decrece el tiempo restante del proceso

        if (p->getRemainingTime() <= 1e-9) { // Comprueba si el proceso termino
            	// Guarda todas las variables modificadas
            	p->setCompletionTime(currentTime);
            	p->setTurnaroundTime(currentTime - p->getArrivalTime());
            	p->setWaitingTime(p->getTurnaroundTime() - p->getBurstTime());
            	active->removeProcess(p); // Se elimina el proceso de la cola
            	finishedCount++; // Incrementa el numero de procesos terminados
        } else {
            	// Se comprueba si la Queue es RR y envia el proceso al final de la Queue
            	if (active->getAlgorithm() == SchedulingAlgorithm::ROUND_ROBIN) {active->rotateToBack(p);}
        }
    }
}

// Se devuelve el vector de procesos
std::vector<Process>& Scheduler::getProcesses() {return processes;}

// Se calculan los promedios
AverageMetrics Scheduler::calculateAverages() const {
    	AverageMetrics avg{0.0, 0.0, 0.0, 0.0}; // Todos los promedios en 0
    	if (processes.empty()) return avg; // Si no hay procesos, devuelva todos los promedios en 0

    	double sumWT = 0.0, sumCT = 0.0, sumRT = 0.0, sumTAT = 0.0; // Acumuladores para la suma de cada metrica
    	for (const Process& p : processes) { // Recorre los procesos y acumula todas los valores
        	sumWT += p.getWaitingTime();
        	sumCT += p.getCompletionTime();
        	sumRT += p.getResponseTime();
        	sumTAT += p.getTurnaroundTime();
    	}

    	double n = static_cast<double>(processes.size()); // Convierte la cantidad de procesos a decimal y se calculan los promedios
    	avg.avgWaitingTime = sumWT / n;
    	avg.avgCompletionTime = sumCT / n;
    	avg.avgResponseTime = sumRT / n;
    	avg.avgTurnaroundTime = sumTAT / n;
    	return avg;
}
