#include "ListaIO.h"

int main(void){
    Lista* listaIO = criaListaIO();
    Processo* p1 = (Processo*)malloc(sizeof(Processo*));
    Processo* p2 = (Processo*)malloc(sizeof(Processo*));
    Processo* p3 = (Processo*)malloc(sizeof(Processo*));

    insere(listaIO, criaElemento(2, 5, 0, p1));
    imprimeLista(listaIO);
    insere(listaIO, criaElemento(3, 6, 1, p2));
    imprimeLista(listaIO);
    insere(listaIO, criaElemento(4, 7, 2, p3));
    imprimeLista(listaIO);

    for (int i = 0; i < 5; i++) insere(listaIO, criaElemento(2,5,3,p1));
    imprimeLista(listaIO);

    for (int i = 0; i < 5; i++) {
        removeElemento(listaIO);
        imprimeLista(listaIO); 
    }

    return 0;
}