//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

enum StringOp {
    OP_UPPERCASE,
    OP_LOWERCASE,
    OP_REVERSE,
    OP_COUNT_WORDS,
    OP_EXIT
};

void to_uppercase(char *str, size_t len) {
    for (size_t i = 0; i < len; i++) {
        str[i] = toupper((unsigned char)str[i]);
    }
}

void to_lowercase(char *str, size_t len) {
    for (size_t i = 0; i < len; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

void reverse_string(char *str, size_t len) {
    for (size_t i = 0; i < len / 2; i++) {
        char temp = str[i];
        str[i] = str[len - 1 - i];
        str[len - 1 - i] = temp;
    }
}

size_t count_words(const char *str, size_t len) {
    size_t count = 0;
    int in_word = 0;
    
    for (size_t i = 0; i < len; i++) {
        if (isspace((unsigned char)str[i])) {
            in_word = 0;
        } else {
            if (!in_word) {
                count++;
                in_word = 1;
            }
        }
    }
    return count;
}

int main(void) {
    char buffer[256];
    enum StringOp choice;
    int valid_input;
    
    printf("String Processing Tool\n");
    printf("Available operations:\n");
    printf("0: Convert to uppercase\n");
    printf("1: Convert to lowercase\n");
    printf("2: Reverse string\n");
    printf("3: Count words\n");
    printf("4: Exit\n");
    
    while (1) {
        printf("\nEnter your choice (0-4): ");
        
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            printf("Error reading input\n");
            continue;
        }
        
        valid_input = 0;
        if (sscanf(buffer, "%d", (int*)&choice) == 1) {
            if (choice >= OP_UPPERCASE && choice <= OP_EXIT) {
                valid_input = 1;
            }
        }
        
        if (!valid_input) {
            printf("Invalid choice. Please enter 0-4.\n");
            continue;
        }
        
        if (choice == OP_EXIT) {
            printf("Goodbye!\n");
            break;
        }
        
        printf("Enter a string (max 255 characters): ");
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            printf("Error reading input\n");
            continue;
        }
        
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
            len--;
        }
        
        if (len == 0) {
            printf("Empty string provided.\n");
            continue;
        }
        
        char result[256];
        if (len >= sizeof(result)) {
            printf("Input string too long.\n");
            continue;
        }
        strcpy(result, buffer);
        
        switch (choice) {
            case OP_UPPERCASE:
                to_uppercase(result, len);
                printf("Uppercase: %s\n", result);
                break;
                
            case OP_LOWERCASE:
                to_lowercase(result, len);
                printf("Lowercase: %s\n", result);
                break;
                
            case OP_REVERSE:
                reverse_string(result, len);
                printf("Reversed: %s\n", result);
                break;
                
            case OP_COUNT_WORDS:
                printf("Word count: %zu\n", count_words(result, len));
                break;
                
            default:
                break;
        }
    }
    
    return 0;
}