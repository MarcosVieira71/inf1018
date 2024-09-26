#include "converteutf832.h"
#include <stdio.h>
#include <stdlib.h>
  
int escreveBom(FILE* arquivo_saida){
  unsigned char bom[] = {0xFF, 0xFE, 0x00, 0x00};
  if(fwrite(bom, sizeof(unsigned char), 4, arquivo_saida) < 4) return 0;
  return 1; 
}

int convUtf8p32(FILE *arquivo_entrada, FILE *arquivo_saida){
    if(!escreveBom(arquivo_saida)){
      printf(stderr, "Falhou na escrita do BOM\n");
      return -1;
    }

    int c;
    unsigned int utf32chr;
    while(1){
      c = fgetc(arquivo_entrada);
      if(c < 0x80){
        utf32chr = c;
      }
    
    }

}

 

int convUtf8p32(FILE *arquivo_entrada, FILE *arquivo_saida){

}