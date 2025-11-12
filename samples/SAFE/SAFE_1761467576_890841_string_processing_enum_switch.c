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
    OP_COUNT_VOWELS
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
    const char *vowels = "AEIOUaeiou";
    
    for (size_t i = 0; i < len; i++) {
        for (size_t j = 0; j < 10; j++) {
            if (str[i] == vowels[j]) {
                count++;
                break;
            }
        }
    }
    return count;
}

int main(void) {
    char buffer[256];
    char working[256];
    int choice;
    
    printf("Enter a string (max 255 chars): ");
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
        len--;
    }
    
    if (len == 0) {
        fprintf(stderr, "Empty string\n");
        return 1;
    }
    
    if (len >= sizeof(working)) {
        fprintf(stderr, "String too long\n");
        return 1;
    }
    
    printf("Choose operation:\n");
    printf("0: Uppercase\n");
    printf("1: Lowercase\n");
    printf("2: Reverse\n");
    printf("3: Count words\n");
    printf("4: Count vowels\n");
    printf("Enter choice (0-4): ");
    
    if (scanf("%d", &choice) != 1) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    strncpy(working, buffer, sizeof(working) - 1);
    working[sizeof(working) - 1] = '\0';
    
    switch (choice) {
        case OP_UPPERCASE:
            to_uppercase(working, len);
            printf("Result: %s\n", working);
            break;
            
        case OP_LOWERCASE:
            to_lowercase(working, len);
            printf("Result: %s\n", working);
            break;
            
        case OP_REVERSE:
            reverse_string(working, len);
            printf("Result: %s\n", working);
            break;
            
        case OP_COUNT_WORDS:
            printf("Word count: %zu\n", count_words(buffer, len));
            break;
            
        case OP_COUNT_VOWELS:
            printf("Vowel count: %zu\n", count_vowels(buffer, len));
            break;
            
        default:
            fprintf(stderr, "Invalid choice\n");
            return 1;
    }
    
    return 0;
}