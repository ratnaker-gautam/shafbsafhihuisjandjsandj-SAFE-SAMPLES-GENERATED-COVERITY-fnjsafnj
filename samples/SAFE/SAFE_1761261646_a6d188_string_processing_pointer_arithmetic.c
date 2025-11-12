//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_LENGTH 256

int is_valid_input(const char *str) {
    if (str == NULL) return 0;
    size_t len = strlen(str);
    if (len == 0 || len >= MAX_LENGTH) return 0;
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

void remove_non_alnum(char *str) {
    if (str == NULL) return;
    char *read = str;
    char *write = str;
    while (*read != '\0') {
        if (isalnum((unsigned char)*read)) {
            *write = *read;
            write++;
        }
        read++;
    }
    *write = '\0';
}

void count_char_types(const char *str, int *alpha, int *digit, int *other) {
    if (str == NULL || alpha == NULL || digit == NULL || other == NULL) return;
    *alpha = 0;
    *digit = 0;
    *other = 0;
    const char *ptr = str;
    while (*ptr != '\0') {
        if (isalpha((unsigned char)*ptr)) (*alpha)++;
        else if (isdigit((unsigned char)*ptr)) (*digit)++;
        else (*other)++;
        ptr++;
    }
}

int main() {
    char input[MAX_LENGTH];
    char buffer[MAX_LENGTH];
    
    printf("Enter a string (max %d chars): ", MAX_LENGTH - 1);
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
    
    printf("Original: %s\n", input);
    
    strncpy(buffer, input, sizeof(buffer) - 1);
    buffer[sizeof(buffer) - 1] = '\0';
    reverse_string(buffer);
    printf("Reversed: %s\n", buffer);
    
    strncpy(buffer, input, sizeof(buffer) - 1);
    buffer[sizeof(buffer) - 1] = '\0';
    remove_non_alnum(buffer);
    printf("Alphanumeric only: %s\n", buffer);
    
    int alpha_count, digit_count, other_count;
    count_char_types(input, &alpha_count, &digit_count, &other_count);
    printf("Character types - Letters: %d, Digits: %d, Other: %d\n", 
           alpha_count, digit_count, other_count);
    
    return 0;
}