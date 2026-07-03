#ifndef PROCESS_H
#define PROCESS_H

#include <string>

class Process {
	private:
	// Datos de cada proceso
	std::string id;
	double burstTime;
	double arrivalTime;
	int queueNumber;
	int priority;
	// Metricas para calculos
	double remainingTime;
	double startTime;
	bool started;
	// Aqui vienen las metricas nuevas
	double waitingTime;
	double completionTime;
	double responseTime;
	double turnaroundTime;

	public:
	// Constructor
	Process(const std::string& id, double burstTime, double arrivalTime, int queueNumber, int priority);

	// Getters
	std::string getId() const;
	double getBurstTime() const;
	double getArrivalTime() const;
	int getQueueNumber() const;
	int getPriority() const;
	double getRemainingTime() const;
	double getStartTime() const;
	bool isStarted() const;
	double getWaitingTime() const;
	double getCompletionTime() const;
	double getResponseTime() const;
	double getTurnaroundTime() const;

	// Setters
	void setStartTime(double t);
	void setWaitingTime(double t);
	void setCompletionTime(double t);
	void setResponseTime(double t);
	void setTurnaroundTime(double t);
	void setStarted(bool s);
	void decreaseRemainingTime(double t);


};

#endif

