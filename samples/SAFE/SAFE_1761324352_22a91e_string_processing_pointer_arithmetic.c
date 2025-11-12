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

void count_letters(const char *str, int *counts) {
    if (str == NULL || counts == NULL) return;
    for (int i = 0; i < 26; i++) counts[i] = 0;
    const char *ptr = str;
    while (*ptr != '\0') {
        if (isalpha((unsigned char)*ptr)) {
            char c = tolower((unsigned char)*ptr);
            counts[c - 'a']++;
        }
        ptr++;
    }
}

void print_letter_counts(const int *counts) {
    if (counts == NULL) return;
    for (int i = 0; i < 26; i++) {
        if (counts[i] > 0) {
            printf("%c: %d\n", 'a' + i, counts[i]);
        }
    }
}

int main(void) {
    char input[MAX_INPUT];
    char processed[MAX_INPUT];
    int letter_counts[26];
    
    printf("Enter a string: ");
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
    
    reverse_string(processed);
    printf("Reversed: %s\n", processed);
    
    count_letters(processed, letter_counts);
    printf("Letter counts:\n");
    print_letter_counts(letter_counts);
    
    return 0;
}