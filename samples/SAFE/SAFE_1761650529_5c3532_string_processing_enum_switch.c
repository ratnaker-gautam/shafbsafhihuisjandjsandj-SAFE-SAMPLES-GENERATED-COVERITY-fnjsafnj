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

int count_words(const char *str) {
    if (str == NULL) return 0;
    int count = 0;
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

int count_vowels(const char *str) {
    if (str == NULL) return 0;
    int count = 0;
    for (size_t i = 0; str[i] != '\0'; i++) {
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
    enum StringOperation choice;
    
    printf("String Processing Menu:\n");
    printf("0: Convert to uppercase\n");
    printf("1: Convert to lowercase\n");
    printf("2: Reverse string\n");
    printf("3: Count words\n");
    printf("4: Count vowels\n");
    printf("5: Exit\n");
    
    while (1) {
        printf("\nEnter choice (0-5): ");
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        if (sscanf(input, "%d", (int*)&choice) != 1) {
            printf("Invalid input. Please enter a number 0-5.\n");
            continue;
        }
        
        if (choice == OP_EXIT) {
            printf("Goodbye!\n");
            break;
        }
        
        if (choice < OP_UPPERCASE || choice > OP_EXIT) {
            printf("Invalid choice. Please enter 0-5.\n");
            continue;
        }
        
        printf("Enter a string (max 255 chars): ");
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            break;
        }
        
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        }
        
        if (strlen(buffer) == 0) {
            printf("Empty string provided.\n");
            continue;
        }
        
        char working[256];
        strncpy(working, buffer, sizeof(working) - 1);
        working[sizeof(working) - 1] = '\0';
        
        switch (choice) {
            case OP_UPPERCASE:
                to_uppercase(working);
                printf("Result: %s\n", working);
                break;
            case OP_LOWERCASE:
                to_lowercase(working);
                printf("Result: %s\n", working);
                break;
            case OP_REVERSE:
                reverse_string(working);
                printf("Result: %s\n", working);
                break;
            case OP_COUNT_WORDS:
                printf("Word count: %d\n", count_words(working));
                break;
            case OP_COUNT_VOWELS:
                printf("Vowel count: %d\n", count_vowels(working));
                break;
            default:
                break;
        }
    }
    
    return 0;
}