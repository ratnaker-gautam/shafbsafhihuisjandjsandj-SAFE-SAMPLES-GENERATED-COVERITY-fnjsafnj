//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024

int main(void) {
    char input[MAX_INPUT_LEN];
    char *p, *q;
    size_t len;
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    len = strlen(input);
    if (len == 0 || len >= MAX_INPUT_LEN) {
        fprintf(stderr, "Invalid input length\n");
        return EXIT_FAILURE;
    }
    
    if (input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    
    if (len == 0) {
        fprintf(stderr, "Empty input\n");
        return EXIT_FAILURE;
    }
    
    p = input;
    while (*p && isspace((unsigned char)*p)) {
        p++;
    }
    
    if (*p == '\0') {
        fprintf(stderr, "Input contains only whitespace\n");
        return EXIT_FAILURE;
    }
    
    q = input + len - 1;
    while (q > p && isspace((unsigned char)*q)) {
        q--;
    }
    
    *(q + 1) = '\0';
    
    p = input;
    while (*p) {
        if (isalpha((unsigned char)*p)) {
            if (islower((unsigned char)*p)) {
                *p = toupper((unsigned char)*p);
            } else {
                *p = tolower((unsigned char)*p);
            }
        }
        p++;
    }
    
    p = input;
    q = input;
    while (*p) {
        if (!isspace((unsigned char)*p) || (q > input && !isspace((unsigned char)*(q - 1)))) {
            *q = *p;
            q++;
        }
        p++;
    }
    *q = '\0';
    
    len = strlen(input);
    if (len > 0) {
        p = input;
        q = input + len - 1;
        while (p < q) {
            char temp = *p;
            *p = *q;
            *q = temp;
            p++;
            q--;
        }
    }
    
    printf("Processed: %s\n", input);
    
    return EXIT_SUCCESS;
}