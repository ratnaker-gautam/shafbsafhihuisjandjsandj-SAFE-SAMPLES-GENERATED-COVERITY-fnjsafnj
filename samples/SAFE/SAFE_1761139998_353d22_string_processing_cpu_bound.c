//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdint.h>

#define MAX_INPUT_LEN 1024

int is_valid_input(const char *str) {
    if (str == NULL) return 0;
    size_t len = strlen(str);
    if (len == 0 || len >= MAX_INPUT_LEN) return 0;
    for (size_t i = 0; i < len; i++) {
        if (!isprint((unsigned char)str[i])) return 0;
    }
    return 1;
}

void process_string(char *str) {
    if (str == NULL) return;
    size_t len = strlen(str);
    
    for (int iteration = 0; iteration < 1000; iteration++) {
        for (size_t i = 0; i < len; i++) {
            if (isalpha((unsigned char)str[i])) {
                if (islower((unsigned char)str[i])) {
                    str[i] = (char)toupper((unsigned char)str[i]);
                } else {
                    str[i] = (char)tolower((unsigned char)str[i]);
                }
            }
        }
        
        for (size_t i = 0; i < len / 2; i++) {
            char temp = str[i];
            str[i] = str[len - 1 - i];
            str[len - 1 - i] = temp;
        }
        
        for (size_t i = 0; i < len; i++) {
            if (isdigit((unsigned char)str[i])) {
                str[i] = '0' + ((str[i] - '0' + 1) % 10);
            }
        }
    }
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    
    printf("Enter a string to process: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    
    if (!is_valid_input(input)) {
        fprintf(stderr, "Invalid input: must be printable characters, 1-%d characters\n", MAX_INPUT_LEN - 1);
        return 1;
    }
    
    char *working_copy = malloc(len + 1);
    if (working_copy == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    
    strcpy(working_copy, input);
    
    process_string(working_copy);
    
    printf("Original: %s\n", input);
    printf("Processed: %s\n", working_copy);
    
    free(working_copy);
    return 0;
}