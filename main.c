#include <stdio.h>
#include <errno.h>
#include "xtra.h"


int main(int argc, char **argv) {

    if (argc < 2) {
        printf("No file specified\b");
        return 1;
    }

    FILE *file = fopen(argv[1], "r");
    if (file == NULL) {
        printf("Open failed, errno: %d\n", errno);
        return 1;
    }

    // Output the prologue
    printf(".global test\n");
    printf("test:\n");
    printf("\tpush %%rbp\n");
    printf("\tmov %%rsp, %%rbp\n");

    translate_instructions(file);

    // Output the epilogue
    printf("\tpop %%rbp\n");
    printf("\tret\n");

    fflush(stdout);
    fclose(file);

    return 0;
}
