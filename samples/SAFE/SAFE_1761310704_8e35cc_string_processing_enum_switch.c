//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

enum StringOp {
    OP_UPPERCASE,
    OP_LOWERCASE,
    OP_REVERSE,
    OP_WORD_COUNT,
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
    char working[256];
    int choice;
    
    printf("Enter a string (max 255 characters): ");
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return 1;
    }
    
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
        len--;
    }
    
    if (len == 0) {
        printf("Empty string entered.\n");
        return 1;
    }
    
    while (1) {
        printf("\nCurrent string: %s\n", buffer);
        printf("Choose operation:\n");
        printf("1. Convert to uppercase\n");
        printf("2. Convert to lowercase\n");
        printf("3. Reverse string\n");
        printf("4. Count words\n");
        printf("5. Exit\n");
        printf("Enter choice (1-5): ");
        
        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n');
            printf("Invalid input. Please enter a number.\n");
            continue;
        }
        
        while (getchar() != '\n');
        
        if (choice < 1 || choice > 5) {
            printf("Invalid choice. Please enter 1-5.\n");
            continue;
        }
        
        enum StringOp op = choice - 1;
        
        switch (op) {
            case OP_UPPERCASE:
                strncpy(working, buffer, sizeof(working) - 1);
                working[sizeof(working) - 1] = '\0';
                to_uppercase(working, strlen(working));
                strncpy(buffer, working, sizeof(buffer) - 1);
                buffer[sizeof(buffer) - 1] = '\0';
                printf("Converted to uppercase: %s\n", buffer);
                break;
                
            case OP_LOWERCASE:
                strncpy(working, buffer, sizeof(working) - 1);
                working[sizeof(working) - 1] = '\0';
                to_lowercase(working, strlen(working));
                strncpy(buffer, working, sizeof(buffer) - 1);
                buffer[sizeof(buffer) - 1] = '\0';
                printf("Converted to lowercase: %s\n", buffer);
                break;
                
            case OP_REVERSE:
                strncpy(working, buffer, sizeof(working) - 1);
                working[sizeof(working) - 1] = '\0';
                reverse_string(working, strlen(working));
                strncpy(buffer, working, sizeof(buffer) - 1);
                buffer[sizeof(buffer) - 1] = '\0';
                printf("Reversed string: %s\n", buffer);
                break;
                
            case OP_WORD_COUNT:
                printf("Word count: %zu\n", count_words(buffer, strlen(buffer)));
                break;
                
            case OP_EXIT:
                printf("Goodbye!\n");
                return 0;
                
            default:
                printf("Invalid operation.\n");
                break;
        }
    }
    
    return 0;
}