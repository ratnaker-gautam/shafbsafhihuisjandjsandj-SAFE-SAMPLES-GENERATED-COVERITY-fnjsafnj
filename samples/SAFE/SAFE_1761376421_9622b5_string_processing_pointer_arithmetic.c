//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024

int is_valid_input(const char *str, size_t len) {
    if (str == NULL || len == 0 || len >= MAX_INPUT_LEN) {
        return 0;
    }
    const char *p = str;
    while (*p != '\0') {
        if (!isprint(*p)) {
            return 0;
        }
        p++;
    }
    return 1;
}

void reverse_string(char *str, size_t len) {
    if (str == NULL || len < 2) {
        return;
    }
    char *start = str;
    char *end = str + len - 1;
    while (start < end) {
        char temp = *start;
        *start = *end;
        *end = temp;
        start++;
        end--;
    }
}

void remove_non_alpha(char *str, size_t len) {
    if (str == NULL || len == 0) {
        return;
    }
    char *read_ptr = str;
    char *write_ptr = str;
    while (*read_ptr != '\0' && (size_t)(read_ptr - str) < len) {
        if (isalpha(*read_ptr)) {
            *write_ptr = *read_ptr;
            write_ptr++;
        }
        read_ptr++;
    }
    *write_ptr = '\0';
}

void count_char_types(const char *str, size_t len, int *vowels, int *consonants) {
    if (str == NULL || vowels == NULL || consonants == NULL) {
        return;
    }
    *vowels = 0;
    *consonants = 0;
    const char *p = str;
    while (*p != '\0' && (size_t)(p - str) < len) {
        char c = tolower(*p);
        if (isalpha(c)) {
            if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u') {
                (*vowels)++;
            } else {
                (*consonants)++;
            }
        }
        p++;
    }
}

int main(void) {
    char input[MAX_INPUT_LEN];
    char processed[MAX_INPUT_LEN];
    
    printf("Enter a string (max %d characters): ", MAX_INPUT_LEN - 1);
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }
    
    if (!is_valid_input(input, input_len)) {
        fprintf(stderr, "Invalid input\n");
        return EXIT_FAILURE;
    }
    
    printf("Original: %s\n", input);
    printf("Length: %zu\n", input_len);
    
    strncpy(processed, input, sizeof(processed) - 1);
    processed[sizeof(processed) - 1] = '\0';
    
    reverse_string(processed, strlen(processed));
    printf("Reversed: %s\n", processed);
    
    strncpy(processed, input, sizeof(processed) - 1);
    processed[sizeof(processed) - 1] = '\0';
    
    remove_non_alpha(processed, strlen(processed));
    printf("Letters only: %s\n", processed);
    
    int vowels, consonants;
    count_char_types(input, input_len, &vowels, &consonants);
    printf("Vowels: %d, Consonants: %d\n", vowels, consonants);
    
    return EXIT_SUCCESS;
}