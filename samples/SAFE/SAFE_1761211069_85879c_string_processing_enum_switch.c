//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum StringOperation {
    OP_UPPERCASE,
    OP_LOWERCASE,
    OP_REVERSE,
    OP_COUNT_WORDS,
    OP_EXIT
};

void to_uppercase(char *str) {
    if (str == NULL) return;
    for (size_t i = 0; str[i] != '\0'; i++) {
        str[i] = toupper((unsigned char)str[i]);
    }
}

void to_lowercase(char *str) {
    if (str == NULL) return;
    for (size_t i = 0; str[i] != '\0'; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

void reverse_string(char *str) {
    if (str == NULL) return;
    size_t len = strlen(str);
    for (size_t i = 0; i < len / 2; i++) {
        char temp = str[i];
        str[i] = str[len - i - 1];
        str[len - i - 1] = temp;
    }
}

size_t count_words(const char *str) {
    if (str == NULL) return 0;
    size_t count = 0;
    int in_word = 0;
    for (size_t i = 0; str[i] != '\0'; i++) {
        if (isspace((unsigned char)str[i])) {
            in_word = 0;
        } else if (!in_word) {
            count++;
            in_word = 1;
        }
    }
    return count;
}

int main(void) {
    char buffer[256];
    enum StringOperation op;
    int choice;
    
    printf("String Processing Tool\n");
    printf("Available operations:\n");
    printf("1. Convert to uppercase\n");
    printf("2. Convert to lowercase\n");
    printf("3. Reverse string\n");
    printf("4. Count words\n");
    printf("5. Exit\n");
    
    while (1) {
        printf("\nEnter your choice (1-5): ");
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please enter a number.\n");
            while (getchar() != '\n');
            continue;
        }
        
        if (choice < 1 || choice > 5) {
            printf("Invalid choice. Please enter 1-5.\n");
            continue;
        }
        
        op = (enum StringOperation)(choice - 1);
        
        if (op == OP_EXIT) {
            printf("Goodbye!\n");
            break;
        }
        
        printf("Enter a string (max 255 characters): ");
        while (getchar() != '\n');
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            printf("Error reading input.\n");
            continue;
        }
        
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        }
        
        if (strlen(buffer) == 0) {
            printf("Empty string provided.\n");
            continue;
        }
        
        char result[256];
        strncpy(result, buffer, sizeof(result) - 1);
        result[sizeof(result) - 1] = '\0';
        
        switch (op) {
            case OP_UPPERCASE:
                to_uppercase(result);
                printf("Uppercase: %s\n", result);
                break;
            case OP_LOWERCASE:
                to_lowercase(result);
                printf("Lowercase: %s\n", result);
                break;
            case OP_REVERSE:
                reverse_string(result);
                printf("Reversed: %s\n", result);
                break;
            case OP_COUNT_WORDS:
                printf("Word count: %zu\n", count_words(result));
                break;
            case OP_EXIT:
                break;
        }
    }
    
    return 0;
}