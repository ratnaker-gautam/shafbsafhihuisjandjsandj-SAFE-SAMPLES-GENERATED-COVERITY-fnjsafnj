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

void reverse_string(char *start, char *end) {
    if (start == NULL || end == NULL || start >= end) return;
    
    char *left = start;
    char *right = end;
    
    while (left < right) {
        char temp = *left;
        *left = *right;
        *right = temp;
        left++;
        right--;
    }
}

void process_string(char *str) {
    if (!is_valid_input(str)) return;
    
    char *current = str;
    char *word_start = NULL;
    
    while (*current != '\0') {
        if (isalpha((unsigned char)*current)) {
            if (word_start == NULL) {
                word_start = current;
            }
        } else {
            if (word_start != NULL) {
                reverse_string(word_start, current - 1);
                word_start = NULL;
            }
        }
        current++;
    }
    
    if (word_start != NULL) {
        reverse_string(word_start, current - 1);
    }
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    
    printf("Enter a string (max %d characters): ", MAX_INPUT_LEN);
    
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
    
    process_string(input);
    
    printf("Processed string: %s\n", input);
    
    return EXIT_SUCCESS;
}