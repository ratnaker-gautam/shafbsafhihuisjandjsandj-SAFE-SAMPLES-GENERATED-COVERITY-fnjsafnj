//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>

enum StringOp {
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
    if (len == 0) return;
    size_t i = 0;
    size_t j = len - 1;
    while (i < j) {
        char temp = str[i];
        str[i] = str[j];
        str[j] = temp;
        i++;
        j--;
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
    for (size_t i = 0; i < len && str[i] != '\0'; i++) {
        char c = tolower((unsigned char)str[i]);
        if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u') {
            count++;
        }
    }
    return count;
}

int main(void) {
    char input[256];
    enum StringOp operation;
    int op_input;
    
    printf("Enter a string (max 255 chars): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len-1] == '\n') {
        input[len-1] = '\0';
        len--;
    }
    
    if (len == 0) {
        printf("Empty string provided.\n");
        return 1;
    }
    
    printf("Choose operation:\n");
    printf("0: Uppercase\n");
    printf("1: Lowercase\n");
    printf("2: Reverse\n");
    printf("3: Count words\n");
    printf("4: Count vowels\n");
    printf("Enter choice (0-4): ");
    
    if (scanf("%d", &op_input) != 1) {
        printf("Invalid input.\n");
        return 1;
    }
    
    if (op_input < 0 || op_input > 4) {
        printf("Invalid operation choice.\n");
        return 1;
    }
    
    operation = (enum StringOp)op_input;
    char buffer[256];
    
    switch (operation) {
        case OP_UPPERCASE:
            strncpy(buffer, input, sizeof(buffer)-1);
            buffer[sizeof(buffer)-1] = '\0';
            to_uppercase(buffer, sizeof(buffer));
            printf("Uppercase: %s\n", buffer);
            break;
            
        case OP_LOWERCASE:
            strncpy(buffer, input, sizeof(buffer)-1);
            buffer[sizeof(buffer)-1] = '\0';
            to_lowercase(buffer, sizeof(buffer));
            printf("Lowercase: %s\n", buffer);
            break;
            
        case OP_REVERSE:
            strncpy(buffer, input, sizeof(buffer)-1);
            buffer[sizeof(buffer)-1] = '\0';
            reverse_string(buffer, strlen(buffer));
            printf("Reversed: %s\n", buffer);
            break;
            
        case OP_COUNT_WORDS:
            printf("Word count: %zu\n", count_words(input, len));
            break;
            
        case OP_COUNT_VOWELS:
            printf("Vowel count: %zu\n", count_vowels(input, len));
            break;
    }
    
    return 0;
}