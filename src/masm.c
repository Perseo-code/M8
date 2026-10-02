// MASM (M8 Assembler)
/*
A program made to compile assembly using a custom architecture called M8
*/
#include <string.h>
#include <stdio.h>
#include "main.h"



/*int main(int argc, char* argv[]) {
    if (argc < 3) {
        printf("Usage: masm <input> <output>\n");
        return 1;
    }
    //printf("input: %s\n", argv[1]);
    FILE* filename = fopen(argv[1], "r");
    if (filename == NULL) {
        printf("Failed to open file %s", argv[1]);
        return 1;
    }

    fseek(filename, 0, SEEK_END);
    long filesize = ftell(filename);
    rewind(filename);
    char* buffer = malloc(filesize + 1);
    if (!buffer) {
        fclose(filename);
        return 2;
    }
    
    size_t read = fread(buffer, 1, filesize, filename);
    buffer[read] = '\0';
    fclose(filename);
    FILE* output = fopen(argv[2], "wb");
    if (output == NULL) {
        printf("Failed to create output file\n");
        return 1;
    }
    //printf("filesize = %ld\n", filesize);
    //printf("read = %zu\n", read);
    //printf("buffer = \"%s\"\n", buffer);
    lexer(buffer);

    //printf("token_count = %u\n", token_count);

    //for (uint32_t i = 0; i < token_count; i++) {
    //    printf("Token %u: type=%d, literal=\"%s\"\n",
    //        i,
    //        list[i].type,
    //        list[i].literal);
    
    while (parser_position < token_count) {
        ParsingError error;

        ParsedIns parsed = parse(&error);
        if (error != OKAY) {
            printf("Error: %d \n", error);
            return 1;
        }

        if (parsed.ptype == NON)
            break;
        Encoded encoded = encoder(parsed);

        fwrite(encoded.data, 1, encoded.size, output);
    }
    fclose(output);
    return 0;
}*/

int compiler(char* buffer, FILE* output) {
    //printf("filesize = %ld\n", filesize);
    //printf("read = %zu\n", read);
    //printf("buffer = \"%s\"\n", buffer);
    lexer(buffer);

    //printf("token_count = %u\n", token_count);

    //for (uint32_t i = 0; i < token_count; i++) {
    //    printf("Token %u: type=%d, literal=\"%s\"\n",
    //        i,
    //        list[i].type,
    //        list[i].literal);
    Assembler* assm = init_assembler();
    while (parser_position < token_count) {
        ParsingError error;

        ParsedIns parsed = parse(&error);
        if (error != OKAY) {
            return error;
        }

        if (parsed.ptype == NON)
            break;
        Encoded encoded = encoder(parsed, assm);

        fwrite(encoded.data, 1, encoded.size, output);
    }
    fclose(output);
    return OKAY;
}