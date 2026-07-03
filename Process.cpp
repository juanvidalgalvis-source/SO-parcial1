#include "Process.h"

// Constructor
Process::Process(const std::string& id, double burstTime, double arrivalTime, int queueNumber, int priority)
// Inicializacion - StartTime comienza en -1 dado que no es un tiempo valido, la idea es que el scheluder lo modifique apenas llegue un verdadero proceso
: id(id), burstTime(burstTime), arrivalTime(arrivalTime), queueNumber(queueNumber), priority(priority), remainingTime (burstTime), startTime(-1.0), started(false), waitingTime(0.0), completionTime(0.0), responseTime(0.0), turnaroundTime(0.0) {}

// Getters
std::string Process::getId() const {return id;}
double Process::getBurstTime() const {return burstTime;}
double Process::getArrivalTime() const {return arrivalTime;}
int Process::getQueueNumber() const {return queueNumber;}
int Process::getPriority() const {return priority;}
double Process::getRemainingTime() const {return remainingTime;}
double Process::getStartTime() const {return  startTime;}
double Process::getWaitingTime() const {return waitingTime;}
double Process::getCompletionTime() const {return completionTime;}
double Process::getResponseTime() const {return responseTime;}
double Process::getTurnaroundTime() const {return turnaroundTime;}
bool Process::isStarted() const {return started;}

// Setters
void Process::setStartTime(double t) {startTime = t;}
void Process::setCompletionTime(double t) {completionTime = t;}
void Process::setWaitingTime(double t) {waitingTime = t;}
void Process::setResponseTime(double t) {responseTime = t;}
void Process::setTurnaroundTime(double t) {turnaroundTime = t;}
void Process::setStarted(bool s) {started = s;}

	// Decrece, se resta tiempo
void Process::decreaseRemainingTime(double t) {
	remainingTime = remainingTime - t;
	if (remainingTime < 0) remainingTime = 0;}
