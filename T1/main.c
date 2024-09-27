#include <stdio.h>
#include <stdlib.h>
#include "converteutf832.h"

int main(void){
    FILE* entrada = fopen("utf8_peq.txt", "rb");
    FILE* saida = fopen("testeConverte", "wb");
    convUtf8p32(entrada, saida);
    return 0;
}