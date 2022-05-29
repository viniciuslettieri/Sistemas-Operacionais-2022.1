#include <stdlib.h>
#include <time.h>

#include "Util.h"

int geraNumero(int minimo, int maximo) {
    if (minimo > maximo)
        return -1;
    else
        return rand() % maximo + minimo;
}

void setaAleatorio() { 
    srand(time(0)); 
}