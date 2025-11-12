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
    while (*(end + 1) != '\0') end++;
    
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

int main(void) {
    char input[MAX_INPUT_LEN];
    
    printf("Enter a string: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    char *newline = input;
    while (*newline != '\0') {
        if (*newline == '\n') {
            *newline = '\0';
            break;
        }
        newline++;
    }
    
    if (!is_valid_input(input)) {
        fprintf(stderr, "Invalid input: empty or too long\n");
        return 1;
    }
    
    char *copy = malloc(strlen(input) + 1);
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
    
    reverse_string(copy);
    printf("Reversed: %s\n", copy);
    
    process_string(copy);
    printf("Processed: %s\n", copy);
    
    free(copy);
    return 0;
}