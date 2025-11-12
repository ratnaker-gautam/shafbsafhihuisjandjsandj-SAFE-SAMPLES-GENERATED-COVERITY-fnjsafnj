//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

enum StringOperation {
    OP_UPPERCASE,
    OP_LOWERCASE,
    OP_REVERSE,
    OP_PALINDROME_CHECK,
    OP_WORD_COUNT
};

void process_string(const char *input, enum StringOperation op) {
    if (input == NULL) {
        printf("Error: Null input\n");
        return;
    }
    
    size_t len = strlen(input);
    if (len == 0) {
        printf("Error: Empty string\n");
        return;
    }
    
    switch (op) {
        case OP_UPPERCASE: {
            char *buffer = malloc(len + 1);
            if (buffer == NULL) {
                printf("Error: Memory allocation failed\n");
                return;
            }
            strcpy(buffer, input);
            for (size_t i = 0; i < len; i++) {
                buffer[i] = toupper(buffer[i]);
            }
            printf("Uppercase: %s\n", buffer);
            free(buffer);
            break;
        }
        
        case OP_LOWERCASE: {
            char *buffer = malloc(len + 1);
            if (buffer == NULL) {
                printf("Error: Memory allocation failed\n");
                return;
            }
            strcpy(buffer, input);
            for (size_t i = 0; i < len; i++) {
                buffer[i] = tolower(buffer[i]);
            }
            printf("Lowercase: %s\n", buffer);
            free(buffer);
            break;
        }
        
        case OP_REVERSE: {
            char *buffer = malloc(len + 1);
            if (buffer == NULL) {
                printf("Error: Memory allocation failed\n");
                return;
            }
            strcpy(buffer, input);
            for (size_t i = 0; i < len / 2; i++) {
                char temp = buffer[i];
                buffer[i] = buffer[len - 1 - i];
                buffer[len - 1 - i] = temp;
            }
            printf("Reversed: %s\n", buffer);
            free(buffer);
            break;
        }
        
        case OP_PALINDROME_CHECK: {
            int is_palindrome = 1;
            for (size_t i = 0; i < len / 2; i++) {
                if (input[i] != input[len - 1 - i]) {
                    is_palindrome = 0;
                    break;
                }
            }
            printf("Palindrome: %s\n", is_palindrome ? "Yes" : "No");
            break;
        }
        
        case OP_WORD_COUNT: {
            int word_count = 0;
            int in_word = 0;
            for (size_t i = 0; i < len; i++) {
                if (isspace(input[i])) {
                    if (in_word) {
                        word_count++;
                        in_word = 0;
                    }
                } else {
                    in_word = 1;
                }
            }
            if (in_word) {
                word_count++;
            }
            printf("Word count: %d\n", word_count);
            break;
        }
        
        default:
            printf("Error: Unknown operation\n");
            break;
    }
}

int main() {
    char input[256];
    
    printf("Enter a string (max 255 characters): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    
    if (len == 0) {
        printf("Error: Empty string\n");
        return 1;
    }
    
    printf("Available operations:\n");
    printf("0 - Convert to uppercase\n");
    printf("1 - Convert to lowercase\n");
    printf("2 - Reverse string\n");
    printf("3 - Check if palindrome\n");
    printf("4 - Count words\n");
    
    printf("Select operation (0-4): ");
    int choice;
    if (scanf("%d", &choice) != 1) {
        printf("Error: Invalid input\n");
        return 1;
    }
    
    while (getchar() != '\n');
    
    if (choice < 0 || choice > 4) {
        printf("Error: Invalid operation\n");
        return 1;
    }
    
    process_string(input, (enum StringOperation)choice);
    
    return 0;
}