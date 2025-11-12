//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT 1024

int validate_input(const char *input) {
    if (input == NULL) return 0;
    size_t len = strlen(input);
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

void count_character_types(const char *str, int *vowels, int *consonants) {
    if (str == NULL || vowels == NULL || consonants == NULL) return;
    *vowels = 0;
    *consonants = 0;
    const char *ptr = str;
    while (*ptr != '\0') {
        char c = tolower((unsigned char)*ptr);
        if (isalpha((unsigned char)*ptr)) {
            if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u') {
                (*vowels)++;
            } else {
                (*consonants)++;
            }
        }
        ptr++;
    }
}

int main() {
    char input[MAX_INPUT];
    char processed[MAX_INPUT];
    
    printf("Enter a string (max %d characters): ", MAX_INPUT - 1);
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    char *newline = strchr(input, '\n');
    if (newline != NULL) *newline = '\0';
    
    if (!validate_input(input)) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    strncpy(processed, input, sizeof(processed) - 1);
    processed[sizeof(processed) - 1] = '\0';
    
    printf("Original: %s\n", processed);
    
    remove_non_alpha(processed);
    printf("Letters only: %s\n", processed);
    
    int vowels, consonants;
    count_character_types(processed, &vowels, &consonants);
    printf("Vowels: %d, Consonants: %d\n", vowels, consonants);
    
    reverse_string(processed);
    printf("Reversed: %s\n", processed);
    
    return 0;
}