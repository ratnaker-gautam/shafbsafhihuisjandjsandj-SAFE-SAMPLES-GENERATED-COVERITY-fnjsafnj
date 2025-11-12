//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

enum StringOperation {
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
        str[i] = str[len - i - 1];
        str[len - i - 1] = temp;
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
    char input[256];
    char buffer[256];
    enum StringOperation op;
    int choice;
    
    printf("Enter a string (max 255 characters): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    
    if (len == 0) {
        printf("Empty string entered.\n");
        return 1;
    }
    
    if (len >= sizeof(buffer)) {
        printf("Input too long.\n");
        return 1;
    }
    
    strcpy(buffer, input);
    
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
            printf("Invalid input.\n");
            while (getchar() != '\n');
            continue;
        }
        
        while (getchar() != '\n');
        
        if (choice < 1 || choice > 5) {
            printf("Invalid choice. Please enter 1-5.\n");
            continue;
        }
        
        op = (enum StringOperation)(choice - 1);
        
        switch (op) {
            case OP_UPPERCASE:
                to_uppercase(buffer, strlen(buffer));
                printf("Converted to uppercase: %s\n", buffer);
                break;
                
            case OP_LOWERCASE:
                to_lowercase(buffer, strlen(buffer));
                printf("Converted to lowercase: %s\n", buffer);
                break;
                
            case OP_REVERSE:
                reverse_string(buffer, strlen(buffer));
                printf("Reversed string: %s\n", buffer);
                break;
                
            case OP_COUNT_WORDS:
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