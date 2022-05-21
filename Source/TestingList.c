#include "ListaIO.h"

int main(void){
    Lista* listaIO = criaLista();
    Processo* p1 = (Processo*)malloc(sizeof(Processo*));
    Processo* p2 = (Processo*)malloc(sizeof(Processo*));
    Processo* p3 = (Processo*)malloc(sizeof(Processo*));

    ListaElemento* e1 = criaElemento(2, 5, 0, p1);
    ListaElemento* e2 = criaElemento(3, 6, 1, p2);
    ListaElemento* e3 = criaElemento(4, 7, 2, p3); 
    insere(listaIO, e1);
    imprimeLista(listaIO);
    insere(listaIO, e2);
    imprimeLista(listaIO);
    insere(listaIO, e3);
    imprimeLista(listaIO);

    /*
    ListaElemento* e = criaElemento(2,5,3,p1);
    for (int i = 0; i < 5; i++) insere(listaIO, e);
    imprimeLista(listaIO);
    */
    removeElemento(listaIO, e2);
    imprimeLista(listaIO);
    removeElemento(listaIO, e3);
    imprimeLista(listaIO);
    removeElemento(listaIO, e1);
    imprimeLista(listaIO);

    /*
    for (int i = 0; i < 5; i++){
        removeElemento(listaIO, e);
        imprimeLista(listaIO);
    }
    */

    return 0;
}