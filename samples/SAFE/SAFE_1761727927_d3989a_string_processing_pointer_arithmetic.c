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

void count_character_types(const char *str, int *alpha, int *digit, int *other) {
    if (str == NULL || alpha == NULL || digit == NULL || other == NULL) return;
    *alpha = 0;
    *digit = 0;
    *other = 0;
    const char *ptr = str;
    while (*ptr != '\0') {
        if (isalpha((unsigned char)*ptr)) {
            (*alpha)++;
        } else if (isdigit((unsigned char)*ptr)) {
            (*digit)++;
        } else {
            (*other)++;
        }
        ptr++;
    }
}

int main() {
    char input[MAX_INPUT];
    printf("Enter a string (max %d characters): ", MAX_INPUT - 1);
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
    
    char processed[MAX_INPUT];
    if (strlen(input) >= sizeof(processed)) {
        fprintf(stderr, "Input too long for processing\n");
        return 1;
    }
    strcpy(processed, input);
    
    printf("Original: %s\n", processed);
    
    reverse_string(processed);
    printf("Reversed: %s\n", processed);
    
    strcpy(processed, input);
    remove_non_alpha(processed);
    printf("Letters only: %s\n", processed);
    
    int alpha_count, digit_count, other_count;
    count_character_types(input, &alpha_count, &digit_count, &other_count);
    printf("Character types - Letters: %d, Digits: %d, Other: %d\n", 
           alpha_count, digit_count, other_count);
    
    return 0;
}