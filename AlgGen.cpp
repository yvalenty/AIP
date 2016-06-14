#include "stdafx.h"
#include <algorithm>
#include <iostream>
#include <vector>
#include <time.h>				
#include <math.h>	

#define D 22			//ilo?? bit?w dla jednego osobnika
#define MAX 100			//ilos?? iteracji
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
	double mark;			//wartos?? przystosowania
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

//funkcja konwertacji liczby binarnej w dziesi?tn?
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
		population[i].mark = sin(2 * x) + pow(cos(4 * x), 3);	//przystosowanie liczymy wstawiaj?c "wartos??" danego osobnika do podanego wzoru
	}
}

//funkcja licz?ca prawdopodobie?stwo wybrania danego osobnika 
double select_prob(vektor_osob &population) {
	double sum = 0;
	for (int i = 0; i < COUNT; i++) {
		population[i].mark = population[i].mark + 2.0;	/* tu dodajemy 2.0 dla tego ?e w niekt?rych przypadkach
														warto?? prystosowania jest ujemna co jest niedopuszczalne przy u?yciu metody ruletki.
														Wybra?em 2.0 dla tego, ?e to jest maksymalna warto?? ujemna, kt?r? mo?emy dosta? oceniaj?c warto?? przystosowania,
														wi?c niezale?nie od otzrzymanej warto??i na ko?cu zawsze b?dzie liczba w?ksza od 0
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

//selekcja metod? ruletki
void rulette(vektor_osob &population, vektor_osob &buffer)
{
	double sum = select_prob(population);
	for (int k = 0; k < COUNT; k++) {
		double sel = fRand(0, sum);
		double tempsum = 0;
		for (int i = 0; i < COUNT; i++) {
			tempsum += population[i].probability;
			if (tempsum > sel) {
				buffer[k] = population[i];
				break;
			}
		}
	}
}

//mutacja
void mutate(Person &osoba)
{
	int divider = rand() % D;
	int mutagen = gen_random();
	osoba.dna[divider] = mutagen;
}


//krzyzowanie
void merge(vektor_osob &population, vektor_osob &buffer)
{
	double mutation = MUTPROB;
	int divider, i1, i2;
	rulette(population, buffer);
	int temp[D];
	for (int i = 1; i<COUNT; i++) {
		i1 = rand() % (COUNT / 2);//wybieram osoby do krzy?owania
		i2 = rand() % (COUNT / 2);
		divider = rand() % D;//wybieram punkt krzy?owania
		int k = 0;
		for (int i = 0; i < D; i++) {
			temp[i] = buffer[i1].dna[i];
		}
		for (int j = divider; j < D; j++) {
				buffer[i1].dna[j] = population[i2].dna[j];
				buffer[i2].dna[j] = temp[j];
			}
		if (rand() < mutation) mutate(buffer[i]);
		i++;
		
	}
}

//wypisanie wynik?w
inline void show(vektor_osob &o, int i)
{
	cout << i << "\t";
	for (int k = 0; k < D; k++) {
		cout << o[i-1].dna[k];
	}
	cout << "\t";
	cout << o[i - 1].mark;
	cout << endl;
}

//zachowanie najbardziej pasuj?cej znalezionej warto?ci
void save_max(vektor_osob &population, double &ff, double &fv) {
	if (population[0].mark>ff) {
		ff = population[0].mark;
		fv = bin2dec(population, 0);
	}
}

int main()
{
	srand(unsigned(time(NULL)));
	vektor_osob population_1;
	vektor_osob *population, *buffer;
	double findfitness = 0;//ocena przystosowania najbardziej pasuj?cej znalezionej warto?ci
	double findvalue = 0;//najbardziej pasuj?ca znaleziona warto?c
	initialize(population_1);
	population = &population_1;
	buffer = &population_1;

	for (int i = 0; i < MAX; i++) {
		fitness(*population);
		sort(*population);
		save_max(*population, findfitness, findvalue);
		show(*population, 1);
		merge(*population, *buffer);
		swap(population, buffer);
	}
	cout << "Znaleziony maximum wynosi: " << findfitness << endl;
	cout << "W punkcie: " << findvalue << endl;
	system("PAUSE");
	return 0;
}