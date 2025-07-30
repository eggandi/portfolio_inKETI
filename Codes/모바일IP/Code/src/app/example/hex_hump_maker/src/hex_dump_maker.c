
#include <stdio.h>
#include <stdlib.h>

void print_hex(FILE *output, size_t size) {
    unsigned char buf = 0x00;
    for(size_t  i = 0; i < size; i++)
    {
        fputc(buf, output);
        buf++;
    }
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <output_file> <file_size[byte]>\n", argv[0]);
        return 1;
    }
    FILE *output = fopen(argv[1], "wb+");
    if (!output) {
        perror("Error opening output file");
        fclose(output);
        return 1;
    }
    size_t file_size = atoi(argv[2]);

    print_hex(output, file_size);

    fclose(output);

    printf("Hex dump written to %s, its size is %d.\n", argv[1], atoi(argv[2]));
    return 0;
}
