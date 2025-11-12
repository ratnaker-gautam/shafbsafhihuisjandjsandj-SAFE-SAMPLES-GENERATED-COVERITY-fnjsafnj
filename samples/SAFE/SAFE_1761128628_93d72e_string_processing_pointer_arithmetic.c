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
    if (str == NULL || *str == '\0') return;
    
    char *start = str;
    char *end = str;
    while (*(end + 1) != '\0') {
        end++;
    }
    
    while (start < end) {
        char temp = *start;
        *start = *end;
        *end = temp;
        start++;
        end--;
    }
}

void process_string(char *str) {
    if (str == NULL) return;
    
    char *ptr = str;
    while (*ptr != '\0') {
        if (islower((unsigned char)*ptr)) {
            *ptr = toupper((unsigned char)*ptr);
        } else if (isupper((unsigned char)*ptr)) {
            *ptr = tolower((unsigned char)*ptr);
        }
        ptr++;
    }
}

int count_words(const char *str) {
    if (str == NULL || *str == '\0') return 0;
    
    int count = 0;
    int in_word = 0;
    const char *ptr = str;
    
    while (*ptr != '\0') {
        if (isspace((unsigned char)*ptr)) {
            in_word = 0;
        } else if (!in_word) {
            count++;
            in_word = 1;
        }
        ptr++;
    }
    
    return count;
}

int main(void) {
    char input[MAX_INPUT_LEN];
    
    printf("Enter a string (max %d characters): ", MAX_INPUT_LEN - 1);
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (!is_valid_input(input)) {
        fprintf(stderr, "Invalid input\n");
        return EXIT_FAILURE;
    }
    
    char *working_copy = malloc(strlen(input) + 1);
    if (working_copy == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return EXIT_FAILURE;
    }
    
    char *src = input;
    char *dst = working_copy;
    while (*src != '\0') {
        *dst = *src;
        src++;
        dst++;
    }
    *dst = '\0';
    
    printf("Original: %s\n", input);
    
    process_string(working_copy);
    printf("Case swapped: %s\n", working_copy);
    
    reverse_string(working_copy);
    printf("Reversed: %s\n", working_copy);
    
    int word_count = count_words(input);
    printf("Word count: %d\n", word_count);
    
    free(working_copy);
    return EXIT_SUCCESS;
}