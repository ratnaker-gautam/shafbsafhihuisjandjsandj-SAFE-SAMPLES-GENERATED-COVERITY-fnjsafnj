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

void safe_strncpy(char *dest, const char *src, size_t n) {
    if (dest == NULL || src == NULL || n == 0) return;
    size_t i;
    for (i = 0; i < n - 1 && src[i] != '\0'; i++) {
        dest[i] = src[i];
    }
    dest[i] = '\0';
}

int main() {
    char input[256];
    char buffer[256];
    int choice;
    
    printf("Enter a string (max 255 chars): ");
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
        printf("Empty string provided\n");
        return 1;
    }
    
    printf("Choose operation:\n");
    printf("1. Convert to uppercase\n");
    printf("2. Convert to lowercase\n");
    printf("3. Reverse string\n");
    printf("4. Check if palindrome\n");
    printf("5. Count words\n");
    printf("Enter choice (1-5): ");
    
    if (scanf("%d", &choice) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    while (getchar() != '\n');
    
    if (choice < 1 || choice > 5) {
        printf("Invalid choice\n");
        return 1;
    }
    
    enum StringOperation op = choice - 1;
    
    switch (op) {
        case OP_UPPERCASE:
            safe_strncpy(buffer, input, sizeof(buffer));
            for (size_t i = 0; i < len && buffer[i] != '\0'; i++) {
                buffer[i] = toupper((unsigned char)buffer[i]);
            }
            printf("Uppercase: %s\n", buffer);
            break;
            
        case OP_LOWERCASE:
            safe_strncpy(buffer, input, sizeof(buffer));
            for (size_t i = 0; i < len && buffer[i] != '\0'; i++) {
                buffer[i] = tolower((unsigned char)buffer[i]);
            }
            printf("Lowercase: %s\n", buffer);
            break;
            
        case OP_REVERSE:
            safe_strncpy(buffer, input, sizeof(buffer));
            for (size_t i = 0; i < len / 2; i++) {
                char temp = buffer[i];
                buffer[i] = buffer[len - 1 - i];
                buffer[len - 1 - i] = temp;
            }
            printf("Reversed: %s\n", buffer);
            break;
            
        case OP_PALINDROME_CHECK: {
            int is_palindrome = 1;
            for (size_t i = 0; i < len / 2; i++) {
                if (tolower((unsigned char)input[i]) != tolower((unsigned char)input[len - 1 - i])) {
                    is_palindrome = 0;
                    break;
                }
            }
            printf("The string is %s a palindrome\n", is_palindrome ? "" : "not");
            break;
        }
            
        case OP_WORD_COUNT: {
            int word_count = 0;
            int in_word = 0;
            for (size_t i = 0; i < len; i++) {
                if (isspace((unsigned char)input[i])) {
                    in_word = 0;
                } else if (!in_word) {
                    word_count++;
                    in_word = 1;
                }
            }
            printf("Word count: %d\n", word_count);
            break;
        }
    }
    
    return 0;
}