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
    OP_COUNT_VOWELS
};

void string_uppercase(char *str, size_t len) {
    for (size_t i = 0; i < len; i++) {
        str[i] = toupper((unsigned char)str[i]);
    }
}

void string_lowercase(char *str, size_t len) {
    for (size_t i = 0; i < len; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

void string_reverse(char *str, size_t len) {
    for (size_t i = 0; i < len / 2; i++) {
        char temp = str[i];
        str[i] = str[len - 1 - i];
        str[len - 1 - i] = temp;
    }
}

size_t string_count_words(const char *str, size_t len) {
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

size_t string_count_vowels(const char *str, size_t len) {
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
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    
    if (len == 0) {
        fprintf(stderr, "Empty string provided\n");
        return 1;
    }
    
    printf("Available operations:\n");
    printf("0: Convert to uppercase\n");
    printf("1: Convert to lowercase\n");
    printf("2: Reverse string\n");
    printf("3: Count words\n");
    printf("4: Count vowels\n");
    printf("Select operation (0-4): ");
    
    if (scanf("%d", &choice) != 1) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    if (choice < 0 || choice > 4) {
        fprintf(stderr, "Invalid operation choice\n");
        return 1;
    }
    
    strncpy(buffer, input, sizeof(buffer) - 1);
    buffer[sizeof(buffer) - 1] = '\0';
    size_t buf_len = strlen(buffer);
    
    enum StringOperation op = (enum StringOperation)choice;
    
    switch (op) {
        case OP_UPPERCASE:
            string_uppercase(buffer, buf_len);
            printf("Uppercase result: %s\n", buffer);
            break;
            
        case OP_LOWERCASE:
            string_lowercase(buffer, buf_len);
            printf("Lowercase result: %s\n", buffer);
            break;
            
        case OP_REVERSE:
            string_reverse(buffer, buf_len);
            printf("Reversed result: %s\n", buffer);
            break;
            
        case OP_COUNT_WORDS:
            printf("Word count: %zu\n", string_count_words(buffer, buf_len));
            break;
            
        case OP_COUNT_VOWELS:
            printf("Vowel count: %zu\n", string_count_vowels(buffer, buf_len));
            break;
    }
    
    return 0;
}