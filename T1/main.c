#include <stdio.h>
#include <stdlib.h>
#include "converteutf832.h"

int main(void){
    FILE* entrada_utf8 = fopen("utf8_peq.txt", "rb");
    FILE* saida_utf32 = fopen("testeConverte", "wb");
    convUtf8p32(entrada_utf8, saida_utf32);

    FILE* entrada_utf32 = fopen("utf32_peq.txt", "rb");
    FILE* saida_utf8 = fopen("testeConverte2", "wb");

    convUtf32p8(entrada_utf32, saida_utf8);

    return 0;
}