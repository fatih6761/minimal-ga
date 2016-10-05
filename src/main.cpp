/* 
 * File:   main.cpp
 * Author: Fatih
 *
 * Created on 24 Kasım 2013 Pazar, 01:07
 */

#include <iostream>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <cstdint>
#include <cassert>
#include <algorithm>

#define POP_SIZE        (100)
#define MAX_GENR        (100000)
#define MUT_CHANCE      (10)

#define GENE_TYPE       double
#define GENE_SIZE	(sizeof(GENE_TYPE))
#define GENE_MASK       (GENE_SIZE/2)

using namespace std;

typedef struct {
    GENE_TYPE gene;
    double fitness;
} individual; 

individual population[POP_SIZE];
GENE_TYPE mutRate = MUT_CHANCE;

double fitnessFunction(GENE_TYPE x) {
    if (x > 1e-6)
	return x * x * log(x);
    return 0.; // limit of above function around 0
}

void recombine(GENE_TYPE p1, GENE_TYPE p2, GENE_TYPE *c1, GENE_TYPE *c2) {
    assert(sizeof(uint64_t) == sizeof(GENE_TYPE));
    int cpoint = rand() % (GENE_SIZE * 8) + 1;
    uint64_t mask = (1UL << cpoint) - 1;
    
    *(uint64_t *)(c1) = *(uint64_t *)(&p1) & (mask);
    *(uint64_t *)(c1) |= *(uint64_t *)(&p2) & (mask);
    *(uint64_t *)(c2) = *(uint64_t *)(&p2) & (mask);
    *(uint64_t *)(c2) |= *(uint64_t *)(&p1) & (mask);
}

void mutate(GENE_TYPE * c1, GENE_TYPE * c2) {
    *c1 += (rand() % 100 - 50) * MUT_CHANCE / 50.;
    *c2 += (rand() % 100 - 50) * MUT_CHANCE / 50.;
}

bool ValueCmp(individual const & a, individual const & b) {
    return a.fitness < b.fitness;
}

/*
 * MinimalGA uygulamasi ana fonksiyonu
 */
int main(int argc, char** argv) {
    int i, j;
    GENE_TYPE parent1, parent2;
    GENE_TYPE child1, child2;

    srand(time(0));
    
    cout << "Basit Genetik Algoritma Uygulamasi" << endl;
    cout << "Uyumluluk fonksiyonu : Some exponential stuff" << endl;
    cout << "Populasyon : " << POP_SIZE << endl;
    cout << "En fazla nesil : " << MAX_GENR << endl;
    cout << "Mutasyon olasiligi : " << MUT_CHANCE << endl;
    
    for (i = 0; i < POP_SIZE; i++) {
        population[i].gene = (rand() % 100 - 50) / 50.;
        population[i].fitness = fitnessFunction(population[i].gene);
    }
    sort(population, population + POP_SIZE, ValueCmp);
    
    i = 0;
    while( i < MAX_GENR) {         
        for (j = 2; j < POP_SIZE; j += 2) {
            parent1 = population[j].gene;
            parent2 = population[j + 1].gene;
            if (j < rand()%POP_SIZE) recombine(parent1, parent2, &child1, &child2);
            mutate(&child1, &child2);
            population[j].gene = child1;
            population[j + 1].gene = child2;
        }
        
        for(j = 0; j < POP_SIZE; j++)
            population[j].fitness = fitnessFunction(population[j].gene);
        
        sort(population, population + POP_SIZE, ValueCmp);
        i++;

	mutRate *= 0.9999;

        if(abs(population[0].fitness) < 1e-6 || abs(population[1].fitness) < 1e-6)
            break;
    }
    

    int minIndex = (population[0].fitness > population[1].fitness) ? 1 : 0;
    cout << "En uygun gen : " << population[minIndex].gene << endl;
    cout << "En uygun sonuc : " << population[minIndex].fitness << endl;
    
    cin.get();
    
    return 0;
}

