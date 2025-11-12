//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: enum_switch ; Variation: string_processing
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
    
    while (1) {
        printf("Enter a string (max 255 chars): ");
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        size_t len = strlen(input);
        if (len > 0 && input[len - 1] == '\n') {
            input[len - 1] = '\0';
        }
        
        if (strlen(input) == 0) {
            printf("Empty string entered.\n");
            continue;
        }
        
        printf("Choose operation:\n");
        printf("0: Convert to uppercase\n");
        printf("1: Convert to lowercase\n");
        printf("2: Reverse string\n");
        printf("3: Count words\n");
        printf("4: Count vowels\n");
        printf("5: Exit\n");
        printf("Enter choice: ");
        
        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n');
            printf("Invalid input.\n");
            continue;
        }
        while (getchar() != '\n');
        
        if (choice < 0 || choice > 5) {
            printf("Invalid choice.\n");
            continue;
        }
        
        if (choice == OP_EXIT) {
            break;
        }
        
        strncpy(buffer, input, sizeof(buffer) - 1);
        buffer[sizeof(buffer) - 1] = '\0';
        
        switch (choice) {
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
                printf("Word count: %zu\n", count_words(buffer));
                break;
            case OP_COUNT_VOWELS:
                printf("Vowel count: %zu\n", count_vowels(buffer));
                break;
            default:
                printf("Invalid operation.\n");
                break;
        }
    }
    
    return 0;
}