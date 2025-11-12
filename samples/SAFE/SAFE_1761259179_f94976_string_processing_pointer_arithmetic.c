//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_INPUT 1024

int is_valid_input(const char *str) {
    if (str == NULL) return 0;
    size_t len = strlen(str);
    if (len == 0 || len >= MAX_INPUT) return 0;
    return 1;
}

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

void count_characters(const char *str, int *letters, int *digits, int *others) {
    if (str == NULL || letters == NULL || digits == NULL || others == NULL) return;
    *letters = 0;
    *digits = 0;
    *others = 0;
    const char *ptr = str;
    while (*ptr != '\0') {
        if (isalpha((unsigned char)*ptr)) {
            (*letters)++;
        } else if (isdigit((unsigned char)*ptr)) {
            (*digits)++;
        } else {
            (*others)++;
        }
        ptr++;
    }
}

int main(void) {
    char input[MAX_INPUT + 1];
    
    printf("Enter a string (max %d characters): ", MAX_INPUT);
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (!is_valid_input(input)) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    char processed[MAX_INPUT + 1];
    strncpy(processed, input, sizeof(processed) - 1);
    processed[sizeof(processed) - 1] = '\0';
    
    printf("Original: %s\n", processed);
    
    reverse_string(processed);
    printf("Reversed: %s\n", processed);
    
    remove_non_alpha(processed);
    printf("Alpha only: %s\n", processed);
    
    int letters, digits, others;
    count_characters(input, &letters, &digits, &others);
    printf("Character counts in original:\n");
    printf("  Letters: %d\n", letters);
    printf("  Digits: %d\n", digits);
    printf("  Others: %d\n", others);
    
    return 0;
}