//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

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

void count_letters(const char *str, int *counts) {
    if (str == NULL || counts == NULL) return;
    for (int i = 0; i < 26; i++) {
        counts[i] = 0;
    }
    const char *ptr = str;
    while (*ptr != '\0') {
        if (isalpha((unsigned char)*ptr)) {
            char c = tolower((unsigned char)*ptr);
            counts[c - 'a']++;
        }
        ptr++;
    }
}

int main(void) {
    char input[MAX_INPUT];
    char buffer[MAX_INPUT];
    
    printf("Enter a string: ");
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
    
    strncpy(buffer, input, sizeof(buffer) - 1);
    buffer[sizeof(buffer) - 1] = '\0';
    
    printf("Original: %s\n", buffer);
    
    reverse_string(buffer);
    printf("Reversed: %s\n", buffer);
    
    strncpy(buffer, input, sizeof(buffer) - 1);
    buffer[sizeof(buffer) - 1] = '\0';
    
    remove_non_alpha(buffer);
    printf("Letters only: %s\n", buffer);
    
    int letter_counts[26];
    count_letters(input, letter_counts);
    
    printf("Letter frequencies:\n");
    for (int i = 0; i < 26; i++) {
        if (letter_counts[i] > 0) {
            printf("%c: %d\n", 'a' + i, letter_counts[i]);
        }
    }
    
    return 0;
}