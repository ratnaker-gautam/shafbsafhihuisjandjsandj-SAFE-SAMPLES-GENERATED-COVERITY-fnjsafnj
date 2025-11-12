//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT 1024

int main() {
    char input[MAX_INPUT];
    char output[MAX_INPUT];
    char *src, *dst;
    size_t len;
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    len = strlen(input);
    if (len == 0 || len >= MAX_INPUT) {
        fprintf(stderr, "Invalid input length\n");
        return 1;
    }
    
    src = input;
    dst = output;
    
    while (*src != '\0' && (size_t)(dst - output) < sizeof(output) - 1) {
        if (isalpha((unsigned char)*src)) {
            if (isupper((unsigned char)*src)) {
                *dst = tolower((unsigned char)*src);
            } else {
                *dst = toupper((unsigned char)*src);
            }
            dst++;
        } else if (isdigit((unsigned char)*src)) {
            *dst = *src;
            dst++;
        }
        src++;
    }
    
    if ((size_t)(dst - output) >= sizeof(output)) {
        fprintf(stderr, "Output buffer overflow\n");
        return 1;
    }
    
    *dst = '\0';
    
    if (strlen(output) == 0) {
        fprintf(stderr, "No valid characters processed\n");
        return 1;
    }
    
    printf("Processed: %s\n", output);
    return 0;
}