#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct StructuraMasina {
	int id;
	int nrUsi;
	float pret;
	char* model;
	char* numeSofer;
	unsigned char serie;
};
typedef struct StructuraMasina Masina;

struct Nod {
	Masina info;
	struct Nod* urmator;
	struct Nod* precedent;
};
typedef struct Nod Nod;

struct Lista {
	Nod* prim;
	Nod* ultim;
};
typedef struct Lista Lista;

Masina citireMasinaDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";
	fgets(buffer, 100, file);
	char* aux;
	Masina m1;
	aux = strtok(buffer, sep);
	m1.id = atoi(aux);
	m1.nrUsi = atoi(strtok(NULL, sep));
	m1.pret = atof(strtok(NULL, sep));
	aux = strtok(NULL, sep);
	m1.model = (char*)malloc(strlen(aux) + 1);
	strcpy_s(m1.model, strlen(aux) + 1, aux);

	aux = strtok(NULL, sep);
	m1.numeSofer = (char*)malloc(strlen(aux) + 1);
	strcpy_s(m1.numeSofer, strlen(aux) + 1, aux);

	m1.serie = *strtok(NULL, sep);
	return m1;
}

void afisareMasina(Masina masina) {
	printf("Id: %d\n", masina.id);
	printf("Nr. usi : %d\n", masina.nrUsi);
	printf("Pret: %.2f\n", masina.pret);
	printf("Model: %s\n", masina.model);
	printf("Nume sofer: %s\n", masina.numeSofer);
	printf("Serie: %c\n\n", masina.serie);
}

void afisareListaMasiniInceput(Lista list) {
	Nod* p = list.prim;
	while (p)
	{
		afisareMasina(p->info);
		p = p->urmator;
	}
}

void afisareListaMasiniSfarsit(Lista list) {
	Nod* p = list.ultim;
	while (p)
	{
		afisareMasina(p->info);
		p = p->precedent;
	}
}

void adaugaMasinaInLista(Lista* list, Masina masinaNoua) {
	Nod* p = (Nod*)malloc(sizeof(Nod));
	p->info = masinaNoua;
	p->precedent = list->ultim;
	p->urmator = NULL;
	if (list->ultim)
		list->ultim->urmator = p;
	else
		list->prim = p;
	list->ultim = p;
}

void adaugaLaInceputInLista(Lista* list, Masina masinaNoua) {
	Nod* p = (Nod*)malloc(sizeof(Nod));
	p->info = masinaNoua;
	p->urmator = list->ultim;
	p->precedent = NULL;
	if (list->ultim)
	{
		list->prim->precedent = p;
	}
	else
	{
		list->ultim = p;
	}
	list->prim = p;
}

Lista citireLDMasiniDinFisier(const char* numeFisier) {
	FILE* f = fopen(numeFisier, "r");
	Lista list;
	list.prim = NULL;
	list.ultim = NULL;
	while (!feof(f))
	{
		//adaugaMasinaInLista(&list, citireMasinaDinFisier(f));
		adaugaLaInceputInLista(&list, citireMasinaDinFisier(f));
	}
	fclose(f);
	return list;
}

void dezalocareLDMasini(Lista* list) {
	Nod* p = list->prim;
	while (p->urmator != NULL)
	{
		free(p->info.numeSofer);
		free(p->info.model);
		p = p->urmator;
		free(p->precedent);
	}
	free(p->info.numeSofer);
	free(p->info.model);
	free(p);
	list->prim = NULL;
	list->ultim = NULL;
}

float calculeazaPretMediu(Lista lista) {
	Nod* aux = lista.prim;
	float pretMediu = 0;
	int sum = 0;
	int contor = 0;
	while (aux)
	{
		sum += aux->info.pret;
		aux = aux->urmator;
		contor = contor + 1;
	}

	return sum / contor;
}

void stergeMasinaDupaID(Lista* lista, int id) {
	Nod* p = lista->prim;
	while (p) {
		if (p->info.id == id) {
			if (p->precedent) {
				p->precedent->urmator = p->urmator;
			}
			else {
				lista->prim = p->urmator;
			}

			if (p->urmator) {
				p->urmator->precedent = p->precedent;
			}
			else {
				lista->ultim = p->precedent;
			}
			free(p->info.model);
			free(p->info.numeSofer);
			free(p);
			return;
		}
		p = p->urmator;
	}

}

char* getNumeSoferMasinaScumpa(Lista lista) {
	float pretMaxim = 0;
	char* numeSofer = NULL;
	Nod* aux = lista.prim;
	while (aux) {
		if (aux->info.pret > pretMaxim) {
			pretMaxim = aux->info.pret;
			if (numeSofer) {
				free(numeSofer);
			}
			numeSofer = (char*)malloc(strlen(aux->info.numeSofer) + 1);
			strcpy(numeSofer, aux->info.numeSofer);
		}
		aux = aux->urmator;
	}
	return numeSofer;
}

int main() {
	Lista lista;
	lista = citireLDMasiniDinFisier("masini.txt");
	//afisareListaMasiniInceput(lista);
	afisareListaMasiniSfarsit(lista);

	float pretMediu = calculeazaPretMediu(lista);
	printf("Pret mediu: %.2f\n", pretMediu);
	char* sofer = getNumeSoferMasinaScumpa(lista);
	printf("Soferul cu cea mai scumpa masina: %s", sofer);
	stergeMasinaDupaID(&lista, 9);
	//afisareListaMasiniSfarsit(lista);

	dezalocareLDMasini(&lista);
	return 0;
}