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
    OP_COUNT_VOWELS,
    OP_EXIT
};

void to_uppercase(char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        str[i] = toupper((unsigned char)str[i]);
    }
}

void to_lowercase(char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

void reverse_string(char *str) {
    int len = strlen(str);
    for (int i = 0; i < len / 2; i++) {
        char temp = str[i];
        str[i] = str[len - 1 - i];
        str[len - 1 - i] = temp;
    }
}

int count_words(const char *str) {
    int count = 0;
    int in_word = 0;
    for (int i = 0; str[i] != '\0'; i++) {
        if (isspace((unsigned char)str[i])) {
            in_word = 0;
        } else if (!in_word) {
            count++;
            in_word = 1;
        }
    }
    return count;
}

int count_vowels(const char *str) {
    int count = 0;
    for (int i = 0; str[i] != '\0'; i++) {
        char c = tolower((unsigned char)str[i]);
        if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u') {
            count++;
        }
    }
    return count;
}

int main() {
    char input[256];
    char buffer[256];
    int choice;
    
    printf("Enter a string (max 255 characters): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (strlen(input) == 0) {
        fprintf(stderr, "Empty string not allowed\n");
        return 1;
    }
    
    do {
        printf("\nChoose operation:\n");
        printf("1. Convert to uppercase\n");
        printf("2. Convert to lowercase\n");
        printf("3. Reverse string\n");
        printf("4. Count words\n");
        printf("5. Count vowels\n");
        printf("6. Exit\n");
        printf("Enter choice (1-6): ");
        
        if (scanf("%d", &choice) != 1) {
            fprintf(stderr, "Invalid input\n");
            while (getchar() != '\n');
            continue;
        }
        
        while (getchar() != '\n');
        
        if (choice < 1 || choice > 6) {
            fprintf(stderr, "Invalid choice. Please enter 1-6.\n");
            continue;
        }
        
        strncpy(buffer, input, sizeof(buffer) - 1);
        buffer[sizeof(buffer) - 1] = '\0';
        
        enum StringOperation op = choice - 1;
        
        switch (op) {
            case OP_UPPERCASE:
                to_uppercase(buffer);
                printf("Uppercase: %s\n", buffer);
                break;
                
            case OP_LOWERCASE:
                to_lowercase(buffer);
                printf("Lowercase: %s\n", buffer);
                break;
                
            case OP_REVERSE:
                reverse_string(buffer);
                printf("Reversed: %s\n", buffer);
                break;
                
            case OP_COUNT_WORDS:
                printf("Word count: %d\n", count_words(buffer));
                break;
                
            case OP_COUNT_VOWELS:
                printf("Vowel count: %d\n", count_vowels(buffer));
                break;
                
            case OP_EXIT:
                printf("Goodbye!\n");
                break;
                
            default:
                fprintf(stderr, "Invalid operation\n");
                break;
        }
        
    } while (choice != 6);
    
    return 0;
}