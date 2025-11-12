//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT 1024

int main() {
    char input[MAX_INPUT];
    char output[MAX_INPUT];
    
    printf("Enter a string to process: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    char *src = input;
    char *dst = output;
    char *end = output + MAX_INPUT - 1;
    
    while (*src != '\0' && dst < end) {
        if (isalpha((unsigned char)*src)) {
            if (islower((unsigned char)*src)) {
                *dst = toupper((unsigned char)*src);
            } else {
                *dst = tolower((unsigned char)*src);
            }
            dst++;
        } else if (isdigit((unsigned char)*src)) {
            *dst = *src;
            dst++;
        }
        src++;
    }
    
    *dst = '\0';
    
    if (dst == output) {
        printf("No alphabetic or numeric characters found\n");
    } else {
        printf("Processed string: %s\n", output);
    }
    
    return 0;
}