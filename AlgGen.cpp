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