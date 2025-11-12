//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT 1024

void reverse_string(char *str) {
    if (str == NULL) return;
    
    char *start = str;
    char *end = str + strlen(str) - 1;
    
    while (start < end) {
        char temp = *start;
        *start = *end;
        *end = temp;
        start++;
        end--;
    }
}

void remove_non_alpha(char *str) {
    if (str == NULL) return;
    
    char *read = str;
    char *write = str;
    
    while (*read != '\0') {
        if (isalpha((unsigned char)*read)) {
            *write = *read;
            write++;
        }
        read++;
    }
    *write = '\0';
}

void to_uppercase(char *str) {
    if (str == NULL) return;
    
    char *ptr = str;
    while (*ptr != '\0') {
        *ptr = toupper((unsigned char)*ptr);
        ptr++;
    }
}

int main(void) {
    char input[MAX_INPUT];
    char processed[MAX_INPUT];
    
    printf("Enter a string (max %d characters): ", MAX_INPUT - 1);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (strlen(input) == 0) {
        fprintf(stderr, "Empty input string\n");
        return 1;
    }
    
    if (strlen(input) >= MAX_INPUT - 1) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    strcpy(processed, input);
    
    printf("Original: %s\n", processed);
    
    remove_non_alpha(processed);
    printf("Alpha only: %s\n", processed);
    
    to_uppercase(processed);
    printf("Uppercase: %s\n", processed);
    
    reverse_string(processed);
    printf("Reversed: %s\n", processed);
    
    return 0;
}