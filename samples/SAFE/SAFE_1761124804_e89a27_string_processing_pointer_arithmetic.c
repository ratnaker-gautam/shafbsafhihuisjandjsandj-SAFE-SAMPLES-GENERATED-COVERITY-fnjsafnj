//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: pointer_arithmetic ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024

void reverse_string(char *str) {
    if (str == NULL) return;
    
    char *start = str;
    char *end = str;
    while (*end != '\0') {
        end++;
    }
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
    if (input == NULL) return;
    
    char *ptr = input;
    int word_count = 0;
    int in_word = 0;
    
    while (*ptr != '\0') {
        if (isalpha((unsigned char)*ptr)) {
            if (!in_word) {
                word_count++;
                in_word = 1;
            }
            
            if (islower((unsigned char)*ptr)) {
                *ptr = toupper((unsigned char)*ptr);
            } else {
                *ptr = tolower((unsigned char)*ptr);
            }
        } else {
            in_word = 0;
        }
        ptr++;
    }
    
    printf("Processed: %s\n", input);
    printf("Word count: %d\n", word_count);
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
    
    if (len == 1) {
        fprintf(stderr, "Empty input string\n");
        return 1;
    }
    
    char *copy = malloc(len + 1);
    if (copy == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    
    char *src = input;
    char *dst = copy;
    while (*src != '\0') {
        *dst = *src;
        src++;
        dst++;
    }
    *dst = '\0';
    
    printf("Original: %s\n", input);
    
    process_string(input);
    
    reverse_string(copy);
    printf("Reversed: %s\n", copy);
    
    free(copy);
    return 0;
}