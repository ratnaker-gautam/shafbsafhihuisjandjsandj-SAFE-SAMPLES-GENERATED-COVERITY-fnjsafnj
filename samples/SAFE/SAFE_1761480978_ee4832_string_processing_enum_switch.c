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
    OP_COUNT_VOWELS,
    OP_TRIM
};

void string_uppercase(char *str) {
    if (str == NULL) return;
    for (size_t i = 0; str[i] != '\0'; i++) {
        str[i] = toupper((unsigned char)str[i]);
    }
}

void string_lowercase(char *str) {
    if (str == NULL) return;
    for (size_t i = 0; str[i] != '\0'; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

void string_reverse(char *str) {
    if (str == NULL) return;
    size_t len = strlen(str);
    for (size_t i = 0; i < len / 2; i++) {
        char temp = str[i];
        str[i] = str[len - 1 - i];
        str[len - 1 - i] = temp;
    }
}

size_t string_count_words(const char *str) {
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

size_t string_count_vowels(const char *str) {
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

void string_trim(char *str) {
    if (str == NULL) return;
    size_t len = strlen(str);
    if (len == 0) return;
    
    size_t start = 0;
    while (isspace((unsigned char)str[start]) && start < len) {
        start++;
    }
    
    size_t end = len - 1;
    while (end > start && isspace((unsigned char)str[end])) {
        end--;
    }
    
    if (start > 0 || end < len - 1) {
        size_t new_len = end - start + 1;
        for (size_t i = 0; i < new_len; i++) {
            str[i] = str[start + i];
        }
        str[new_len] = '\0';
    }
}

int main(void) {
    char input[256];
    int choice;
    
    printf("Enter a string (max 255 characters): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input.\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (strlen(input) == 0) {
        printf("Empty string provided.\n");
        return 1;
    }
    
    printf("Choose operation:\n");
    printf("0: Convert to uppercase\n");
    printf("1: Convert to lowercase\n");
    printf("2: Reverse string\n");
    printf("3: Count words\n");
    printf("4: Count vowels\n");
    printf("5: Trim whitespace\n");
    printf("Enter choice (0-5): ");
    
    if (scanf("%d", &choice) != 1) {
        printf("Invalid input.\n");
        return 1;
    }
    
    while (getchar() != '\n');
    
    char buffer[256];
    strncpy(buffer, input, sizeof(buffer) - 1);
    buffer[sizeof(buffer) - 1] = '\0';
    
    enum StringOp op = (enum StringOp)choice;
    
    switch (op) {
        case OP_UPPERCASE:
            string_uppercase(buffer);
            printf("Result: %s\n", buffer);
            break;
            
        case OP_LOWERCASE:
            string_lowercase(buffer);
            printf("Result: %s\n", buffer);
            break;
            
        case OP_REVERSE:
            string_reverse(buffer);
            printf("Result: %s\n", buffer);
            break;
            
        case OP_COUNT_WORDS:
            printf("Word count: %zu\n", string_count_words(buffer));
            break;
            
        case OP_COUNT_VOWELS:
            printf("Vowel count: %zu\n", string_count_vowels(buffer));
            break;
            
        case OP_TRIM:
            string_trim(buffer);
            printf("Result: %s\n", buffer);
            break;
            
        default:
            printf("Invalid operation choice.\n");
            return 1;
    }
    
    return 0;
}