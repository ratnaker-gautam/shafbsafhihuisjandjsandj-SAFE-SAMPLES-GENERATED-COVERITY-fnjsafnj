//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: pointer_arithmetic ; Variation: string_processing
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

void process_string(char *str) {
    if (str == NULL) return;
    
    char *read_ptr = str;
    char *write_ptr = str;
    int word_count = 0;
    int in_word = 0;
    
    while (*read_ptr != '\0') {
        if (isalpha((unsigned char)*read_ptr)) {
            if (!in_word) {
                word_count++;
                in_word = 1;
            }
            *write_ptr = toupper((unsigned char)*read_ptr);
            write_ptr++;
        } else if (isspace((unsigned char)*read_ptr)) {
            if (in_word) {
                in_word = 0;
            }
        }
        read_ptr++;
    }
    
    *write_ptr = '\0';
}

int main() {
    char input[MAX_LENGTH];
    char processed[MAX_LENGTH];
    
    printf("Enter a string (max %d characters): ", MAX_LENGTH - 1);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (!is_valid_input(input)) {
        fprintf(stderr, "Invalid input: string is empty or too long\n");
        return 1;
    }
    
    strncpy(processed, input, sizeof(processed) - 1);
    processed[sizeof(processed) - 1] = '\0';
    
    process_string(processed);
    
    if (strlen(processed) == 0) {
        printf("No alphabetic characters found in input\n");
    } else {
        printf("Processed string: %s\n", processed);
    }
    
    return 0;
}