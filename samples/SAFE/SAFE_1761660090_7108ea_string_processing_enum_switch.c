//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>

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
    for (size_t i = 0; i < len; i++) {
        char c = tolower((unsigned char)str[i]);
        if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u') {
            count++;
        }
    }
    return count;
}

int main(void) {
    char buffer[256];
    enum StringOp operation;
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
        printf("Empty string provided.\n");
        return 1;
    }
    
    printf("Choose operation:\n");
    printf("1. Convert to uppercase\n");
    printf("2. Convert to lowercase\n");
    printf("3. Reverse string\n");
    printf("4. Count words\n");
    printf("5. Count vowels\n");
    printf("Enter choice (1-5): ");
    
    if (scanf("%d", &choice) != 1) {
        printf("Invalid input.\n");
        return 1;
    }
    
    if (choice < 1 || choice > 5) {
        printf("Invalid choice.\n");
        return 1;
    }
    
    operation = (enum StringOp)(choice - 1);
    
    char work_buffer[256];
    if (strlen(buffer) >= sizeof(work_buffer)) {
        printf("String too long for processing.\n");
        return 1;
    }
    strcpy(work_buffer, buffer);
    
    switch (operation) {
        case OP_UPPERCASE:
            to_uppercase(work_buffer, strlen(work_buffer));
            printf("Uppercase: %s\n", work_buffer);
            break;
            
        case OP_LOWERCASE:
            to_lowercase(work_buffer, strlen(work_buffer));
            printf("Lowercase: %s\n", work_buffer);
            break;
            
        case OP_REVERSE:
            reverse_string(work_buffer, strlen(work_buffer));
            printf("Reversed: %s\n", work_buffer);
            break;
            
        case OP_COUNT_WORDS:
            printf("Word count: %zu\n", count_words(buffer, strlen(buffer)));
            break;
            
        case OP_COUNT_VOWELS:
            printf("Vowel count: %zu\n", count_vowels(buffer, strlen(buffer)));
            break;
    }
    
    return 0;
}