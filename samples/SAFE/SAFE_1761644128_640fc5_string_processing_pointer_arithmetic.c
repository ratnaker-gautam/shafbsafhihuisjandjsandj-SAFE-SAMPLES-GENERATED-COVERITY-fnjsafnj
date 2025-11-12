//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024

int main() {
    char input[MAX_INPUT_LEN];
    char output[MAX_INPUT_LEN];
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len == 0 || (len == 1 && input[0] == '\n')) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    if (len >= sizeof(input)) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    char *src = input;
    char *dst = output;
    char *end = input + len;
    
    while (src < end) {
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
        } else if (isspace((unsigned char)*src)) {
            *dst = ' ';
            dst++;
        }
        src++;
    }
    
    *dst = '\0';
    
    if (dst - output >= sizeof(output)) {
        fprintf(stderr, "Output buffer overflow\n");
        return 1;
    }
    
    if (printf("%s\n", output) < 0) {
        fprintf(stderr, "Error writing output\n");
        return 1;
    }
    
    return 0;
}