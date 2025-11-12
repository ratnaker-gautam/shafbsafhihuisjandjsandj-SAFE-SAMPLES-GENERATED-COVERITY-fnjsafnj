//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_INPUT 256

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

void to_uppercase(char *str) {
    if (str == NULL) return;
    
    char *ptr = str;
    while (*ptr != '\0') {
        *ptr = toupper((unsigned char)*ptr);
        ptr++;
    }
}

int validate_input(const char *input) {
    if (input == NULL) return 0;
    
    const char *ptr = input;
    size_t len = 0;
    
    while (*ptr != '\0') {
        if (len >= MAX_INPUT - 1) return 0;
        if (!isprint((unsigned char)*ptr)) return 0;
        ptr++;
        len++;
    }
    
    return len > 0;
}

int main(void) {
    char input[MAX_INPUT];
    char buffer[MAX_INPUT];
    
    printf("Enter a string (max %d characters): ", MAX_INPUT - 1);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    char *newline = strchr(input, '\n');
    if (newline != NULL) {
        *newline = '\0';
    }
    
    if (!validate_input(input)) {
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
    remove_non_alpha(buffer);
    printf("Alpha only: %s\n", buffer);
    
    strncpy(buffer, input, sizeof(buffer) - 1);
    buffer[sizeof(buffer) - 1] = '\0';
    to_uppercase(buffer);
    printf("Uppercase: %s\n", buffer);
    
    return 0;
}