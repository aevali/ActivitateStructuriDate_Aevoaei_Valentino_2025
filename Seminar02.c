#include <stdio.h>
#include <stdlib.h>

struct Masina {
	int id;
	int nrLocuri;
	char* marca;
	float capacitateC;
	char normaPoluare;
};

struct Masina initializare(int id, int nrLocuri, const char* marca, float capacitateC, char normaPoluare) {
	struct Masina m;
	m.id = id;
	m.nrLocuri = nrLocuri;
	m.capacitateC = capacitateC;
	m.normaPoluare = normaPoluare;
	if (marca == NULL)
		m.marca = NULL;
	else
	{
		m.marca = (char*)malloc(strlen(marca) + 1);
		strcpy_s(m.marca, strlen(marca) + 1, marca);
	}
	return m;
}

void afisare(struct Masina m) {
	printf("\n Id masina: %d\n Nr locuri: %d\n Capacitate: %.2f\n Marca: %s\n Euro: %c\n", m.id, m.nrLocuri, m.capacitateC, m.marca, m.normaPoluare);
}

void modificaNrLocuri(struct Masina* m, int nrNou) {
	if (nrNou > 0)
		m->nrLocuri = nrNou;
}

int main()
{

}