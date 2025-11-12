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

size_t count_vowels(const char *str, size_t len) {
    size_t count = 0;
    for (size_t i = 0; i < len; i++) {
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
    
    do {
        printf("\nChoose operation:\n");
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
            printf("Invalid choice. Please enter 0-5.\n");
            continue;
        }
        
        strncpy(buffer, input, sizeof(buffer) - 1);
        buffer[sizeof(buffer) - 1] = '\0';
        size_t buf_len = strlen(buffer);
        
        switch ((enum StringOperation)choice) {
            case OP_UPPERCASE:
                to_uppercase(buffer, buf_len);
                printf("Uppercase: %s\n", buffer);
                break;
            case OP_LOWERCASE:
                to_lowercase(buffer, buf_len);
                printf("Lowercase: %s\n", buffer);
                break;
            case OP_REVERSE:
                reverse_string(buffer, buf_len);
                printf("Reversed: %s\n", buffer);
                break;
            case OP_COUNT_WORDS:
                printf("Word count: %zu\n", count_words(buffer, buf_len));
                break;
            case OP_COUNT_VOWELS:
                printf("Vowel count: %zu\n", count_vowels(buffer, buf_len));
                break;
            case OP_EXIT:
                printf("Goodbye!\n");
                break;
        }
        
    } while (choice != OP_EXIT);
    
    return 0;
}