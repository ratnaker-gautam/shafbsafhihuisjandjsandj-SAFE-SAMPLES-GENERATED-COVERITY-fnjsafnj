//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024

int validate_input(const char *input) {
    if (input == NULL) return 0;
    size_t len = strlen(input);
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
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (!validate_input(input)) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    char *buffer = malloc(MAX_INPUT_LEN);
    if (buffer == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    
    char *src = input;
    char *dst = buffer;
    while (*src != '\0') {
        *dst = *src;
        src++;
        dst++;
    }
    *dst = '\0';
    
    printf("Original: %s\n", buffer);
    
    process_string(buffer);
    printf("Case swapped: %s\n", buffer);
    
    reverse_string(buffer);
    printf("Reversed: %s\n", buffer);
    
    free(buffer);
    return 0;
}