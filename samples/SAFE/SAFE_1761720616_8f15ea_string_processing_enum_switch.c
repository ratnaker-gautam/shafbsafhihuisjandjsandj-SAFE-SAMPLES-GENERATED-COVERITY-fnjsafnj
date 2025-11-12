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
    char buffer[256];
    enum StringOperation op;
    int choice;
    
    printf("Enter a string (max 255 characters): ");
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
    }
    
    if (strlen(buffer) == 0) {
        fprintf(stderr, "Empty string not allowed\n");
        return 1;
    }
    
    printf("Choose operation:\n");
    printf("0: Convert to uppercase\n");
    printf("1: Convert to lowercase\n");
    printf("2: Reverse string\n");
    printf("3: Count words\n");
    printf("4: Count vowels\n");
    printf("5: Exit\n");
    printf("Enter choice (0-5): ");
    
    if (scanf("%d", &choice) != 1) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    if (choice < 0 || choice > 5) {
        fprintf(stderr, "Invalid choice\n");
        return 1;
    }
    
    op = (enum StringOperation)choice;
    
    char work_buffer[256];
    strncpy(work_buffer, buffer, sizeof(work_buffer) - 1);
    work_buffer[sizeof(work_buffer) - 1] = '\0';
    
    switch (op) {
        case OP_UPPERCASE:
            to_uppercase(work_buffer);
            printf("Uppercase: %s\n", work_buffer);
            break;
        case OP_LOWERCASE:
            to_lowercase(work_buffer);
            printf("Lowercase: %s\n", work_buffer);
            break;
        case OP_REVERSE:
            reverse_string(work_buffer);
            printf("Reversed: %s\n", work_buffer);
            break;
        case OP_COUNT_WORDS:
            printf("Word count: %zu\n", count_words(work_buffer));
            break;
        case OP_COUNT_VOWELS:
            printf("Vowel count: %zu\n", count_vowels(work_buffer));
            break;
        case OP_EXIT:
            printf("Exiting\n");
            break;
    }
    
    return 0;
}