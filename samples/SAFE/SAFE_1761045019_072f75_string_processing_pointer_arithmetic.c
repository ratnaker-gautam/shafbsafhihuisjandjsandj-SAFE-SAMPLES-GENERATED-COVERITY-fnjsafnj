//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: pointer_arithmetic ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_LEN 256

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
        if (isalpha(*read)) {
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
        *ptr = toupper(*ptr);
        ptr++;
    }
}

int validate_input(const char *input) {
    if (input == NULL) return 0;
    
    const char *ptr = input;
    while (*ptr != '\0') {
        if (!isprint(*ptr)) return 0;
        ptr++;
    }
    
    return strlen(input) > 0 && strlen(input) < MAX_LEN;
}

int main() {
    char input[MAX_LEN];
    char buffer[MAX_LEN];
    
    printf("Enter a string (max %d characters): ", MAX_LEN - 1);
    
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