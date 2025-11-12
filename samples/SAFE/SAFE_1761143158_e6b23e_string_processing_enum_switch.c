//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

enum StringOp {
    OP_UPPERCASE,
    OP_LOWERCASE,
    OP_REVERSE,
    OP_COUNT_VOWELS,
    OP_COUNT_WORDS,
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

int count_vowels(const char *str, size_t len) {
    int count = 0;
    for (size_t i = 0; i < len; i++) {
        char c = tolower((unsigned char)str[i]);
        if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u') {
            count++;
        }
    }
    return count;
}

int count_words(const char *str, size_t len) {
    int count = 0;
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

int main(void) {
    char buffer[256];
    enum StringOp choice;
    
    while (1) {
        printf("Enter a string (max 255 chars): ");
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            break;
        }
        
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
            len--;
        }
        
        if (len == 0) {
            printf("Empty string entered.\n");
            continue;
        }
        
        printf("Choose operation:\n");
        printf("0: Uppercase\n");
        printf("1: Lowercase\n");
        printf("2: Reverse\n");
        printf("3: Count vowels\n");
        printf("4: Count words\n");
        printf("5: Exit\n");
        printf("Enter choice: ");
        
        int input;
        if (scanf("%d", &input) != 1) {
            while (getchar() != '\n');
            printf("Invalid input.\n");
            continue;
        }
        while (getchar() != '\n');
        
        if (input < 0 || input > 5) {
            printf("Invalid choice.\n");
            continue;
        }
        
        choice = (enum StringOp)input;
        
        switch (choice) {
            case OP_UPPERCASE:
                to_uppercase(buffer, len);
                printf("Result: %s\n", buffer);
                break;
            case OP_LOWERCASE:
                to_lowercase(buffer, len);
                printf("Result: %s\n", buffer);
                break;
            case OP_REVERSE:
                reverse_string(buffer, len);
                printf("Result: %s\n", buffer);
                break;
            case OP_COUNT_VOWELS:
                printf("Vowel count: %d\n", count_vowels(buffer, len));
                break;
            case OP_COUNT_WORDS:
                printf("Word count: %d\n", count_words(buffer, len));
                break;
            case OP_EXIT:
                return 0;
        }
    }
    
    return 0;
}