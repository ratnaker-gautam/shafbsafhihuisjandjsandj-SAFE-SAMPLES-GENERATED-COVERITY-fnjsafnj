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
        str[i] = str[len - 1 - i];
        str[len - 1 - i] = temp;
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

size_t count_vowels(const char *str) {
    if (str == NULL) return 0;
    size_t count = 0;
    for (size_t i = 0; str[i] != '\0'; i++) {
        char c = tolower((unsigned char)str[i]);
        if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u') {
            count++;
        }
    }
    return count;
}

int main(void) {
    char input[256];
    char buffer[256];
    int choice;
    
    printf("String Processing Tool\n");
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
        fprintf(stderr, "Empty string provided\n");
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
        
        strncpy(buffer, input, sizeof(buffer) - 1);
        buffer[sizeof(buffer) - 1] = '\0';
        
        switch (choice) {
            case OP_UPPERCASE + 1:
                to_uppercase(buffer);
                printf("Result: %s\n", buffer);
                break;
                
            case OP_LOWERCASE + 1:
                to_lowercase(buffer);
                printf("Result: %s\n", buffer);
                break;
                
            case OP_REVERSE + 1:
                reverse_string(buffer);
                printf("Result: %s\n", buffer);
                break;
                
            case OP_COUNT_WORDS + 1:
                printf("Word count: %zu\n", count_words(buffer));
                break;
                
            case OP_COUNT_VOWELS + 1:
                printf("Vowel count: %zu\n", count_vowels(buffer));
                break;
                
            case OP_EXIT + 1:
                printf("Goodbye!\n");
                break;
                
            default:
                printf("Invalid choice. Please try again.\n");
                break;
        }
        
    } while (choice != OP_EXIT + 1);
    
    return 0;
}