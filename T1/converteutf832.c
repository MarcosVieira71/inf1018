/* Julia Gomes Zibordi 2320934 3WA */
/* Marcos Paulo Marinho Vieira 2320466 3WA*/

#include <stdlib.h>
#include <stdio.h>
#include "converteutf832.h"

int verificaTamanhoCaractereUtf8(int byteInicial);
int preencheVetorDeBytes(unsigned char* utf8Bytes, int qtdBytes, int byteInicial, FILE* arquivo_entrada);
int escreveCaractereUtf32(unsigned char* utf8Bytes, int qtdBytes, FILE* arquivo_saida);
int escreveBom(FILE* arquivo_saida);
int verificaLittleEndian(unsigned char* bom);
unsigned int obtemCodigoUnicode(unsigned char* utf32Bytes, int elittleEndian);
int verificaTamanhoCaractereUtf32(unsigned int codigoUnicode);
int escreveCaractereUtf8(unsigned int codigoUnicode, int qtdBytes, FILE* arquivo_saida);

int convUtf8p32(FILE* arquivo_entrada, FILE* arquivo_saida) {
    if (!escreveBom(arquivo_saida)) {
        fprintf(stderr, "Erro na escrita do BOM\n");
        return -1;
    }

    // byteInicial é definido como inteiro, pois a função fgetc retorna um inteiro e deve ser possível representar EOF (-1) 

    int byteInicial = 0;

    // Lê um caractere UTF-8 do arquivo de entrada por vez e escreve seu correspondente em UTF-32 no arquivo de saída

    while (1) {
        byteInicial = fgetc(arquivo_entrada);
        if (byteInicial == EOF) break;

        int qtdBytesCaractere = verificaTamanhoCaractereUtf8(byteInicial);

        if(!qtdBytesCaractere) {
            fprintf(stderr, "Caractere UTF-8 inválido.\n");
            return -1;
        }

        // O vetor utf8Bytes guarda cada byte do caractere UTF-8 lido e possui a quantidade correta de bytes para armazenar o caractere UTF-8

        unsigned char* utf8Bytes = (unsigned char*)malloc(sizeof(unsigned char) * qtdBytesCaractere);
        
        if (!utf8Bytes) {
            fprintf(stderr, "Erro na alocação de memória\n");
            return -1;
        }

        int leitura = preencheVetorDeBytes(utf8Bytes, qtdBytesCaractere, byteInicial, arquivo_entrada);
        if (!leitura) {
            fprintf(stderr, "Erro na leitura do arquivo UTF-8\n");
            free(utf8Bytes);  
            return -1;
        }

        int escrita = escreveCaractereUtf32(utf8Bytes, qtdBytesCaractere, arquivo_saida);
        if (!escrita) {
            fprintf(stderr, "Erro na escrita do arquivo UTF-32\n");
            free(utf8Bytes); 
            return -1;
        }

        free(utf8Bytes);
    }
    return 0; 
}

int convUtf32p8(FILE* arquivo_entrada, FILE* arquivo_saida) {
    unsigned char bom[4];

    if(fread(bom, sizeof(unsigned char), 4, arquivo_entrada) < 4) {
        fprintf(stderr, "BOM inválido ou arquivo incompleto\n");
        return -1;
    }
    
    int eLittleEndian = verificaLittleEndian(bom);

    if(eLittleEndian == -1) {
        fprintf(stderr, "BOM inválido\n");
        return -1;
    }

    // Lê um caractere UTF-32 do arquivo de entrada por vez e escreve seu correspondente em UTF-8 no arquivo de saída

    while(1) {
        unsigned char utf32Bytes[4];

        size_t qtdBytesLidos = fread(utf32Bytes, sizeof(unsigned char), 4, arquivo_entrada);

        if(qtdBytesLidos < 4) {
            if(feof(arquivo_entrada)) break;
            else {
                fprintf(stderr,"Erro na leitura do arquivo UTF-32\n");
                return -1;
            }
        }
        
        // Considera a ordem dos bytes (little ou big-endian) para obter o código Unicode correto

        unsigned int codigoUnicode = obtemCodigoUnicode(utf32Bytes, eLittleEndian);

        int qtdBytesCaractere = verificaTamanhoCaractereUtf32(codigoUnicode);

        if(!qtdBytesCaractere) {
            fprintf(stderr, "Caractere UTF-32 inválido.\n");
            return -1;
        }

        if(!escreveCaractereUtf8(codigoUnicode, qtdBytesCaractere, arquivo_saida)) {
            fprintf(stderr, "Erro na escrita do arquivo UTF-8.\n");
            return -1;
        }
        
    }

    return 0;
}

int escreveBom(FILE* arquivo_saida) {
    unsigned char bom[] = {0xFF, 0xFE, 0x00, 0x00};
    if (fwrite(bom, sizeof(unsigned char), 4, arquivo_saida) < 4) return 0;
    return 1;
}

int escreveCaractereUtf32(unsigned char* utf8Bytes, int qtdBytes, FILE* arquivo_saida) {
    unsigned int utf32Char = 0;

    // Máscaras para extrair os bits relevantes de cada byte UTF-8, dependendo da sua quantidade de bytes

    int mask[] = {0x7F, 0x1F, 0x0F, 0x07};

    // Extrai os bits relevantes do primeiro byte, ignorando o prefixo de tamanho

    utf32Char = utf8Bytes[0] & mask[qtdBytes - 1];

    // Processa os bytes restantes, deslocando os bits e combinando com os 6 bits relevantes de cada byte UTF-8 

    for (int i = 1; i < qtdBytes; i++) {
        utf32Char = (utf32Char << 6) | (utf8Bytes[i] & 0x3F);
    }

    if (fwrite(&utf32Char, sizeof(utf32Char), 1, arquivo_saida) != 1) return 0;
    return 1;
}

// Verifica a quantidade de bytes usados para o caractere UTF-8 lido baseado no seu primeiro byte

int verificaTamanhoCaractereUtf8(int byteInicial) {

    if (byteInicial < 0x80) {
        return 1;
    } else if ((byteInicial & 0xE0) == 0xC0) {
        return 2;
    } else if ((byteInicial & 0xF0) == 0xE0) {
        return 3;
    } else if ((byteInicial & 0xF8) == 0xF0) {
        return 4;
    }
    return 0;
}

int preencheVetorDeBytes(unsigned char* utf8Bytes, int qtdBytes, int byteInicial, FILE* arquivo_entrada) {
    utf8Bytes[0] = (unsigned char) byteInicial;
    for (int i = 1; i < qtdBytes; i++) {
        int byteTemp = fgetc(arquivo_entrada);
        if (byteTemp == EOF) return 0;

        utf8Bytes[i] = (unsigned char) byteTemp;
    }
    return 1;
}


int verificaLittleEndian(unsigned char* bom) {
    if(bom[0] == 0x00 && bom[1] == 0x00 && bom[2] == 0xFE && bom[3] == 0xFF) return 0;
    else if(bom[0] == 0xFF && bom[1] == 0xFE && bom[2] == 0x00 && bom[3] == 0x00) return 1;
    else return -1; 
}

// Combina os bytes dos caracteres corretamente de acordo com a ordem em que foram gravados

unsigned int obtemCodigoUnicode(unsigned char* utf32Bytes, int elittleEndian) {
    if(!elittleEndian) return (unsigned int) utf32Bytes[0] << 24 | utf32Bytes[1] << 16 | utf32Bytes[2] << 8 | utf32Bytes[3];
    else return (unsigned int) utf32Bytes[3] << 24 | utf32Bytes[2] << 16 | utf32Bytes[1] << 8 | utf32Bytes[0];
}

int verificaTamanhoCaractereUtf32(unsigned int codigoUnicode) {
    if(codigoUnicode > 0x10ffff) {
        return 0;
    }
    else if(codigoUnicode < 0x80) return 1;
    else if(codigoUnicode < 0x800) return 2;
    else if(codigoUnicode < 0x10000) return 3;
    else return 4;

}

int escreveCaractereUtf8(unsigned int codigoUnicode, int qtdBytes, FILE* arquivo_saida) {
    unsigned char utf8Bytes[4];
    if (qtdBytes == 1) utf8Bytes[0] = (unsigned char) codigoUnicode;
    else {

        for(int i = qtdBytes-1; i > 0; i--){

            // Adiciona o prefixo 10 seguido dos 6 bits menos significativos 

            utf8Bytes[i] = 0x80 | (codigoUnicode & 0x3f);

            // "Remove" 6 bits que já foram processados

            codigoUnicode >>= 6;
        }

        // 0xff << (8 - qtdBytes) resulta no prefixo que indica a quantidade de bytes

        utf8Bytes[0] = (unsigned char)((0xff << (8 - qtdBytes)) | codigoUnicode);
    }
    if (fwrite(utf8Bytes, sizeof(unsigned char), qtdBytes, arquivo_saida) < qtdBytes) return 0;
    return 1;
}
