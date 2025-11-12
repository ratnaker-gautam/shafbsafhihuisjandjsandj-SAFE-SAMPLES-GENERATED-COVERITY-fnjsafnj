//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>

enum StringOperation {
    OP_UPPERCASE,
    OP_LOWERCASE,
    OP_REVERSE,
    OP_COUNT_WORDS,
    OP_COUNT_VOWELS
};

void to_uppercase(char *str, size_t len) {
    for (size_t i = 0; i < len && str[i] != '\0'; i++) {
        str[i] = toupper((unsigned char)str[i]);
    }
}

void to_lowercase(char *str, size_t len) {
    for (size_t i = 0; i < len && str[i] != '\0'; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

void reverse_string(char *str, size_t len) {
    size_t actual_len = 0;
    while (actual_len < len && str[actual_len] != '\0') {
        actual_len++;
    }
    
    for (size_t i = 0; i < actual_len / 2; i++) {
        char temp = str[i];
        str[i] = str[actual_len - 1 - i];
        str[actual_len - 1 - i] = temp;
    }
}

size_t count_words(const char *str, size_t len) {
    size_t count = 0;
    int in_word = 0;
    
    for (size_t i = 0; i < len && str[i] != '\0'; i++) {
        if (isspace((unsigned char)str[i])) {
            in_word = 0;
        } else if (!in_word) {
            count++;
            in_word = 1;
        }
    }
    return count;
}

size_t count_vowels(const char *str, size_t len) {
    size_t count = 0;
    const char *vowels = "aeiouAEIOU";
    
    for (size_t i = 0; i < len && str[i] != '\0'; i++) {
        for (size_t j = 0; vowels[j] != '\0'; j++) {
            if (str[i] == vowels[j]) {
                count++;
                break;
            }
        }
    }
    return count;
}

int main(void) {
    char input[256];
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
        printf("Empty string provided.\n");
        return 1;
    }
    
    printf("Choose operation:\n");
    printf("0: Convert to uppercase\n");
    printf("1: Convert to lowercase\n");
    printf("2: Reverse string\n");
    printf("3: Count words\n");
    printf("4: Count vowels\n");
    printf("Enter choice (0-4): ");
    
    if (scanf("%d", &choice) != 1) {
        printf("Invalid input.\n");
        return 1;
    }
    
    if (choice < 0 || choice > 4) {
        printf("Invalid choice.\n");
        return 1;
    }
    
    char buffer[256];
    strncpy(buffer, input, sizeof(buffer) - 1);
    buffer[sizeof(buffer) - 1] = '\0';
    
    enum StringOperation op = (enum StringOperation)choice;
    
    switch (op) {
        case OP_UPPERCASE:
            to_uppercase(buffer, sizeof(buffer));
            printf("Result: %s\n", buffer);
            break;
            
        case OP_LOWERCASE:
            to_lowercase(buffer, sizeof(buffer));
            printf("Result: %s\n", buffer);
            break;
            
        case OP_REVERSE:
            reverse_string(buffer, sizeof(buffer));
            printf("Result: %s\n", buffer);
            break;
            
        case OP_COUNT_WORDS:
            printf("Word count: %zu\n", count_words(buffer, sizeof(buffer)));
            break;
            
        case OP_COUNT_VOWELS:
            printf("Vowel count: %zu\n", count_vowels(buffer, sizeof(buffer)));
            break;
    }
    
    return 0;
}