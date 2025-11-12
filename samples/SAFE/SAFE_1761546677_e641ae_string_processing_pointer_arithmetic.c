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

void count_character_types(const char *str, int *alpha, int *digit, int *space) {
    if (str == NULL || alpha == NULL || digit == NULL || space == NULL) return;
    *alpha = 0;
    *digit = 0;
    *space = 0;
    const char *ptr = str;
    while (*ptr != '\0') {
        if (isalpha(*ptr)) (*alpha)++;
        else if (isdigit(*ptr)) (*digit)++;
        else if (isspace(*ptr)) (*space)++;
        ptr++;
    }
}

void remove_consecutive_duplicates(char *str) {
    if (str == NULL) return;
    char *read_ptr = str;
    char *write_ptr = str;
    char prev = '\0';
    while (*read_ptr != '\0') {
        if (*read_ptr != prev) {
            *write_ptr = *read_ptr;
            write_ptr++;
        }
        prev = *read_ptr;
        read_ptr++;
    }
    *write_ptr = '\0';
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
    
    char processed[MAX_INPUT_LEN];
    if (strlen(input) >= sizeof(processed)) {
        fprintf(stderr, "Input too long\n");
        return EXIT_FAILURE;
    }
    strcpy(processed, input);
    
    printf("Original: %s\n", processed);
    
    reverse_string(processed);
    printf("Reversed: %s\n", processed);
    
    reverse_string(processed);
    
    int alpha_count, digit_count, space_count;
    count_character_types(processed, &alpha_count, &digit_count, &space_count);
    printf("Letters: %d, Digits: %d, Spaces: %d\n", alpha_count, digit_count, space_count);
    
    remove_consecutive_duplicates(processed);
    printf("Without consecutive duplicates: %s\n", processed);
    
    return EXIT_SUCCESS;
}