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
    
    char *read_ptr = str;
    char *write_ptr = str;
    
    while (*read_ptr != '\0') {
        if (isalpha((unsigned char)*read_ptr)) {
            *write_ptr = *read_ptr;
            write_ptr++;
        }
        read_ptr++;
    }
    *write_ptr = '\0';
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

int main() {
    char input[MAX_INPUT];
    char processed[MAX_INPUT];
    
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
    
    if (strlen(input) >= sizeof(processed)) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    strcpy(processed, input);
    
    printf("Original: %s\n", processed);
    
    remove_non_alpha(processed);
    printf("Alpha only: %s\n", processed);
    
    to_uppercase(processed);
    printf("Uppercase: %s\n", processed);
    
    reverse_string(processed);
    printf("Reversed: %s\n", processed);
    
    return 0;
}