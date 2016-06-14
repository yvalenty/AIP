#include "stdafx.h"
#include <algorithm>
#include <iostream>
#include <vector>
#include <time.h>				
#include <math.h>	

#define D 22			//ilo?? bit?w dla jednego osobnika
#define MAX 100			//ilos? iteracji
#define COUNT 300		//ilo?? osobnik?w w jednej populacji
#define MUTPROB 0.01	//prawdopodobie?stwo mutacji
using namespace std;

//funkcja generacji liczb losowych
int gen_random() {
	return rand()%2;
}

//klasa osobnika
class Person
{
public:
	int dna[D];				//ci?g binarny
	double mark;			//wartos? przystosowania
	double probability;		//prawdopodobie?stwo wylosowania (dla selekcji metod? ruletki)
							//konstruktor domy?lny
	Person() {
		for (int i = 0; i < D; i++) {
			dna[i] = gen_random();
		}
	}
};


typedef vector<Person> vektor_osob;

//funkcja inicializuj?ca populacj?
void initialize(vektor_osob &population)
{
	for (int i = 0; i<COUNT; i++) {
		Person *osobnik;
		osobnik = new Person();
		population.push_back(*osobnik);
	}
}

//funkcja konwertacji liczby binarnej w dziesi?tkow?
double bin2dec(vektor_osob &population, int n) {
	double sum = 0.0;
	double x = 0.0;
	for (int i = D - 1; i >= 0; i--) {
		sum += population[n].dna[i] * pow(2, i);
	}
	x = -1 + (2 * sum) / (pow(2, 22) - 1);
	return x;
}

//funkcja oceny przystosowania
void fitness(vektor_osob &population)
{
	double x;
	for (int i = 0; i < COUNT; i++) {
		x = bin2dec(population, i);
		population[i].mark = sin(2 * x) + pow(cos(4 * x), 3);	//przystosowanie liczymy wstawiaj?c "wartos?" danego osobnika do podanego wzoru
	}
}

//funkcja licz?ca prawdopodobie?stwo wybrania danego osobnika 
double select_prob(vektor_osob &population) {
	double sum = 0;
	for (int i = 0; i < COUNT; i++) {
		population[i].mark = population[i].mark + 2.0;	/* tu dodajemy 2.0 dla tego ?e w niekt?rych przypadkach
														warto?? prystosowania jest ujemna co jest niedopuszczalne przy u?yciu metody ruletki.
														Wybra?em 2.0 dla tego, ?e to jest maksymalna warto?? ujemna, kt?r? mo?emy dosta? oceniaj?c warto?? przystosowania,
														wi?c niezale?nie od otzrzymanej warto??i na ko?cu zawsze b?dzie liczba wi?ksza od 0
														*/
		sum += abs(population[i].mark); //suma wszystkich warto??i przystosowa? dla danej populacji
	}
	for (int i = 0; i < COUNT; i++) {
		population[i].probability = population[i].mark / sum; //prawdopodobie?stwo wybrania danego osobnika 
	}
	return sum;
}

//warunek funkcji sortuj?cej
bool equal(Person left, Person right)
{
	return left.mark > right.mark ? true : false;
}

//funkcja sortuj?ca
void sort(vektor_osob &population)
{
	sort(population.begin(), population.end(), equal);
}

//funkcja zamiany wektor?w
void swap(vektor_osob *&population, vektor_osob *&buffer)
{
	vektor_osob *temp = population;
	population = buffer;
	buffer = temp;
}

//funkcja generacji liczb losowych typu double
double fRand(double fMin, double fMax)
{
	double f = (double)rand() / RAND_MAX;
	return fMin + f * (fMax - fMin);
}