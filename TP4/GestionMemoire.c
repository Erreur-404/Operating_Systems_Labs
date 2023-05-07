#include "./libs/lib.h"

const int PAGE_OFFSET_NUMBER_OF_BITS = 10;

unsigned int calculerNumeroDePage(unsigned long adresse) {
    return adresse >> PAGE_OFFSET_NUMBER_OF_BITS;
}

unsigned long calculerDeplacementDansLaPage(unsigned long adresse) {
	unsigned long mask = 1024 - 1;
	return adresse & mask;
}

unsigned long calculerAdresseComplete(unsigned int numeroDePage, unsigned long deplacementDansLaPage) {
	return (numeroDePage << PAGE_OFFSET_NUMBER_OF_BITS) + deplacementDansLaPage;
}

void rechercherTLB(struct RequeteMemoire* req, struct SystemeMemoire* mem) {
	unsigned int numeroPage = calculerNumeroDePage(req->adresseVirtuelle);
	for (int i = 0; i < TAILLE_TLB; i++) {
		if (mem->tlb->numeroPage[i] == numeroPage && mem->tlb->entreeValide[i]) {
			unsigned long deplacement = calculerDeplacementDansLaPage(req->adresseVirtuelle);
			req->adressePhysique = calculerAdresseComplete(mem->tlb->numeroCadre[i], deplacement);
			req->estDansTLB = 1;
			mem->tlb->dernierAcces[i] = req->date;
			return;
		}
	}
	req->adressePhysique = 0;
	req->estDansTLB = 0;
}

void rechercherTableDesPages(struct RequeteMemoire* req, struct SystemeMemoire* mem) {
	unsigned int numeroPage = calculerNumeroDePage(req->adresseVirtuelle);
	if (mem->tp->entreeValide[numeroPage]) {
		unsigned long deplacement = calculerDeplacementDansLaPage(req->adresseVirtuelle);
		req->adressePhysique = calculerAdresseComplete(mem->tp->numeroCadre[numeroPage], deplacement);
		req->estDansTablePages = 1;
	} else {
		req->adressePhysique = 0;
		req->estDansTablePages = 0;
	}
}

void ajouterDansMemoire(struct RequeteMemoire* req, struct SystemeMemoire* mem) {
	for (int i = 0; i < TAILLE_MEMOIRE; i++) {
		if (mem->memoire->utilisee[i] == 0) {
			mem->memoire->numeroPage[i] = calculerNumeroDePage(req->adresseVirtuelle);
			mem->memoire->dateCreation[i] = req->date;
			mem->memoire->dernierAcces[i] = req->date;
			mem->memoire->utilisee[i] = 1;
			req->adressePhysique = calculerAdresseComplete(i, calculerDeplacementDansLaPage(req->adresseVirtuelle));
			break;
		}
	}
}

void mettreAJourTLB(struct RequeteMemoire* req, struct SystemeMemoire* mem) {
	int indexEntreeARemplacer = 0;
	for (int i = 0; i < TAILLE_TLB; i++) {
		if (!mem->tlb->entreeValide[i]) {
			indexEntreeARemplacer = i;
			break;
		}
		else if (mem->tlb->dateCreation[i] < mem->tlb->dateCreation[indexEntreeARemplacer]) {
			indexEntreeARemplacer = i;
		}
	}
	mem->tlb->numeroPage[indexEntreeARemplacer] = calculerNumeroDePage(req->adresseVirtuelle);
	mem->tlb->numeroCadre[indexEntreeARemplacer] = calculerNumeroDePage(req->adressePhysique);
	mem->tlb->entreeValide[indexEntreeARemplacer] = 1;
	mem->tlb->dernierAcces[indexEntreeARemplacer] = req->date;
	mem->tlb->dateCreation[indexEntreeARemplacer] = req->date;
}

// NE PAS MODIFIER
int main() {
    evaluate(
		&calculerNumeroDePage, 
		&calculerDeplacementDansLaPage, 
		&calculerAdresseComplete, 
        &rechercherTLB, 
		&rechercherTableDesPages,
		&mettreAJourTLB,
		&ajouterDansMemoire
    );
    return 0;
}
