//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024

int is_valid_input(const char *str) {
    if (str == NULL) return 0;
    size_t len = strlen(str);
    if (len == 0 || len >= MAX_INPUT_LEN) return 0;
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

void capitalize_words(char *str) {
    if (str == NULL) return;
    char *ptr = str;
    int in_word = 0;
    while (*ptr != '\0') {
        if (isalpha((unsigned char)*ptr)) {
            if (!in_word) {
                *ptr = toupper((unsigned char)*ptr);
                in_word = 1;
            }
        } else {
            in_word = 0;
        }
        ptr++;
    }
}

int count_words(const char *str) {
    if (str == NULL) return 0;
    int count = 0;
    int in_word = 0;
    const char *ptr = str;
    while (*ptr != '\0') {
        if (isalpha((unsigned char)*ptr)) {
            if (!in_word) {
                count++;
                in_word = 1;
            }
        } else {
            in_word = 0;
        }
        ptr++;
    }
    return count;
}

void process_string(char *str) {
    if (str == NULL) return;
    printf("Original: %s\n", str);
    printf("Word count: %d\n", count_words(str));
    reverse_string(str);
    printf("Reversed: %s\n", str);
    reverse_string(str);
    capitalize_words(str);
    printf("Capitalized: %s\n", str);
}

int main(void) {
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
    
    if (!is_valid_input(input)) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    process_string(input);
    return 0;
}