//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024

int validate_input(const char *input) {
    if (input == NULL) return 0;
    size_t len = strlen(input);
    if (len == 0 || len >= MAX_INPUT_LEN) return 0;
    return 1;
}

void reverse_string(char *str) {
    if (str == NULL || *str == '\0') return;
    
    char *start = str;
    char *end = str;
    while (*end != '\0') end++;
    end--;
    
    while (start < end) {
        char temp = *start;
        *start = *end;
        *end = temp;
        start++;
        end--;
    }
}

void process_string(char *input) {
    if (!validate_input(input)) return;
    
    char *ptr = input;
    int word_count = 0;
    int in_word = 0;
    
    while (*ptr != '\0') {
        if (isalpha((unsigned char)*ptr)) {
            if (!in_word) {
                word_count++;
                in_word = 1;
            }
        } else {
            in_word = 0;
        }
        ptr++;
    }
    
    printf("Word count: %d\n", word_count);
    
    reverse_string(input);
    printf("Reversed: %s\n", input);
    
    reverse_string(input);
    
    ptr = input;
    while (*ptr != '\0') {
        if (islower((unsigned char)*ptr)) {
            *ptr = toupper((unsigned char)*ptr);
        }
        ptr++;
    }
    printf("Uppercase: %s\n", input);
}

int main() {
    char input[MAX_INPUT_LEN];
    
    printf("Enter a string (max %d characters): ", MAX_INPUT_LEN - 1);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (!validate_input(input)) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    process_string(input);
    
    return 0;
}