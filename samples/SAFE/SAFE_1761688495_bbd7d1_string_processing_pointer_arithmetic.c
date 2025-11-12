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

void count_character_types(const char *str, int *alpha, int *digit, int *space) {
    if (str == NULL || alpha == NULL || digit == NULL || space == NULL) return;
    *alpha = 0;
    *digit = 0;
    *space = 0;
    const char *ptr = str;
    while (*ptr != '\0') {
        if (isalpha((unsigned char)*ptr)) (*alpha)++;
        else if (isdigit((unsigned char)*ptr)) (*digit)++;
        else if (isspace((unsigned char)*ptr)) (*space)++;
        ptr++;
    }
}

void remove_duplicates(char *str) {
    if (str == NULL) return;
    char *read_ptr = str;
    char *write_ptr = str;
    int seen[256] = {0};
    while (*read_ptr != '\0') {
        unsigned char c = (unsigned char)*read_ptr;
        if (!seen[c]) {
            seen[c] = 1;
            *write_ptr = *read_ptr;
            write_ptr++;
        }
        read_ptr++;
    }
    *write_ptr = '\0';
}

int main(void) {
    char input[MAX_INPUT];
    printf("Enter a string (max %d characters): ", MAX_INPUT - 1);
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
    
    char *processed = malloc(MAX_INPUT);
    if (processed == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return EXIT_FAILURE;
    }
    
    strcpy(processed, input);
    printf("Original: %s\n", processed);
    
    reverse_string(processed);
    printf("Reversed: %s\n", processed);
    
    int alpha, digit, space;
    count_character_types(input, &alpha, &digit, &space);
    printf("Alphabetic: %d, Digits: %d, Spaces: %d\n", alpha, digit, space);
    
    strcpy(processed, input);
    remove_duplicates(processed);
    printf("Without duplicates: %s\n", processed);
    
    free(processed);
    return EXIT_SUCCESS;
}