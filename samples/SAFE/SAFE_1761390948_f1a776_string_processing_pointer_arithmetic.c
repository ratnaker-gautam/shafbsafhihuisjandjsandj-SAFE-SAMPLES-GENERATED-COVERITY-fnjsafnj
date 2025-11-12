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

void process_string(char *str) {
    if (str == NULL) return;
    
    char *read_ptr = str;
    char *write_ptr = str;
    int word_count = 0;
    int in_word = 0;
    
    while (*read_ptr != '\0') {
        if (isalnum((unsigned char)*read_ptr)) {
            if (!in_word) {
                word_count++;
                in_word = 1;
            }
            *write_ptr = *read_ptr;
            write_ptr++;
        } else if (isspace((unsigned char)*read_ptr)) {
            if (in_word) {
                in_word = 0;
            }
        }
        read_ptr++;
    }
    
    *write_ptr = '\0';
    
    if (word_count > 0) {
        char *start = str;
        char *end = write_ptr - 1;
        
        while (start < end) {
            char temp = *start;
            *start = *end;
            *end = temp;
            start++;
            end--;
        }
    }
}

int main() {
    char input[MAX_INPUT_LEN + 1];
    
    printf("Enter a string (max %d characters): ", MAX_INPUT_LEN);
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
    
    char *buffer = malloc(MAX_INPUT_LEN + 1);
    if (buffer == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    
    strncpy(buffer, input, MAX_INPUT_LEN);
    buffer[MAX_INPUT_LEN] = '\0';
    
    process_string(buffer);
    
    printf("Processed string: %s\n", buffer);
    
    free(buffer);
    return 0;
}