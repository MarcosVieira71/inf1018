#include <stdlib.h>
#include <stdio.h>
#include "converteutf832.h"

int convUtf8p32(FILE* arquivo_entrada, FILE* arquivo_saida);
int verificaTamanhoCaractere(int chr);
int* geraVetorDeBytes(int numBytes);
int preencheVetorDeBytes(int* bytes, int numBytes, int cInicial, FILE* arquivo);
int escreveCaractereUtf32(int* bytes, int numBytes, FILE* arquivo);
int escreveBom(FILE* arquivo_saida);
int eLittleEndian(unsigned char* bom);
unsigned int pegaCodigo(unsigned char* chars, int littleEndian);

int convUtf8p32(FILE* arquivo_entrada, FILE* arquivo_saida) {
    if (!escreveBom(arquivo_saida)) {
        fprintf(stderr, "Erro na escrita do BOM\n");
        return -1;
    }

    int cInicial = 0;
    while (1) {
        cInicial = fgetc(arquivo_entrada);
        if (cInicial == EOF) break;

        int numBytesCaractere = verificaTamanhoCaractere(cInicial);
        int* bytes = geraVetorDeBytes(numBytesCaractere);

        if (!bytes) {
            fprintf(stderr, "Erro na alocação de memória\n");
            return -1;
        }

        int leitura = preencheVetorDeBytes(bytes, numBytesCaractere, cInicial, arquivo_entrada);
        if (!leitura) {
            fprintf(stderr, "Erro na leitura do arquivo UTF-8\n");
            free(bytes);  
            return -1;
        }

        int escrita = escreveCaractereUtf32(bytes, numBytesCaractere, arquivo_saida);
        if (!escrita) {
            fprintf(stderr, "Erro na escrita do arquivo UTF-32\n");
            free(bytes); 
            return -1;
        }

        free(bytes);
    }
    return 1; 
}

int convUtf32p8(FILE* arquivo_entrada, FILE* arquivo_saida) {
    unsigned char bom[4];
    size_t bytes = fread(bom, sizeof(unsigned char), 4, arquivo_entrada);

    if(bytes < 4 || eLittleEndian(bom) == -1) {
        fprintf(stderr, "BOM inválido.\n");
        return -1;
    }

    int littleEndian = eLittleEndian(bom) ? 1 : 0;

    while(1) {
        unsigned char chars[4];

        size_t numChars = fread(chars, sizeof(unsigned char), 4, arquivo_entrada);

        if(numChars < 4) {
            if(feof(arquivo_entrada)) break;
            else {
                fprintf(stderr,"Erro na leitura do arquivo utf-32. \n");
                return -1;
            }
        }

        unsigned int codigo = pegaCodigo(chars, littleEndian);
        
    }

    

    return 0;
}

int escreveBom(FILE* arquivo_saida) {
    unsigned char bom[] = {0xFF, 0xFE, 0x00, 0x00};
    if (fwrite(bom, sizeof(unsigned char), 4, arquivo_saida) < 4) return 0;
    return 1;
}

int escreveCaractereUtf32(int* bytes, int numBytes, FILE* arquivo_saida) {
    unsigned int utf32chr = 0;
    int mask[] = {0x7F, 0x1F, 0x0F, 0x07};

    utf32chr = bytes[0] & mask[numBytes - 1];

    for (int i = 1; i < numBytes; i++) {
        utf32chr = (utf32chr << 6) | (bytes[i] & 0x3F);
    }

    if (fwrite(&utf32chr, sizeof(utf32chr), 1, arquivo_saida) != 1) return 0;
    return 1;
}

int verificaTamanhoCaractere(int chr) {
    if (chr < 0x80) {
        return 1;
    } else if ((chr & 0xE0) == 0xC0) {
        return 2;
    } else if ((chr & 0xF0) == 0xE0) {
        return 3;
    } else if ((chr & 0xF8) == 0xF0) {
        return 4;
    }
    return 0;
}

int preencheVetorDeBytes(int* bytes, int numBytes, int cInicial, FILE* arquivo_entrada) {
    bytes[0] = cInicial;
    for (int i = 1; i < numBytes; i++) {
        int cTemp = fgetc(arquivo_entrada);
        if (cTemp == EOF) return 0;

        bytes[i] = cTemp;
    }
    return 1;
}

int* geraVetorDeBytes(int numBytes) {
    return (int*)malloc(sizeof(int) * numBytes);
}

int eLittleEndian(unsigned char* bom) {
    if(bom[0] == 0x00 && bom[1] == 0x00 && bom[2] == 0xFE && bom[3] == 0xFF) return 0;
    else if(bom[0] == 0xFF && bom[1] == 0xFE && bom[2] == 0x00 && bom[3] == 0x00) return 1;
    else return -1; 
}

unsigned int pegaCodigo(unsigned char* chars, int littleEndian) {
    if(!littleEndian) return chars[0] << 24 | chars[1] << 16 | chars[2] << 8 | chars[3];
    else return chars[3] << 24 | chars[2] << 16 | chars[1] << 8 | chars[0];
}
