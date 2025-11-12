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
    for (int i = 0; str[i] != '\0'; i++) {
        str[i] = toupper((unsigned char)str[i]);
    }
}

void to_lowercase(char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

void reverse_string(char *str) {
    int len = strlen(str);
    for (int i = 0; i < len / 2; i++) {
        char temp = str[i];
        str[i] = str[len - 1 - i];
        str[len - 1 - i] = temp;
    }
}

int count_words(const char *str) {
    int count = 0;
    int in_word = 0;
    for (int i = 0; str[i] != '\0'; i++) {
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
    int count = 0;
    for (int i = 0; str[i] != '\0'; i++) {
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
    int choice;
    
    printf("String Processing Tool\n");
    printf("Available operations:\n");
    printf("1. Convert to uppercase\n");
    printf("2. Convert to lowercase\n");
    printf("3. Reverse string\n");
    printf("4. Count words\n");
    printf("5. Count vowels\n");
    printf("6. Exit\n");
    
    while (1) {
        printf("\nEnter a string (max 255 characters): ");
        if (fgets(input, sizeof(input), stdin) == NULL) {
            printf("Error reading input\n");
            continue;
        }
        
        size_t len = strlen(input);
        if (len > 0 && input[len - 1] == '\n') {
            input[len - 1] = '\0';
            len--;
        }
        
        if (len == 0) {
            printf("Empty string entered\n");
            continue;
        }
        
        if (len >= sizeof(buffer)) {
            printf("Input too long\n");
            continue;
        }
        
        printf("Choose operation (1-6): ");
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input\n");
            while (getchar() != '\n');
            continue;
        }
        while (getchar() != '\n');
        
        if (choice < 1 || choice > 6) {
            printf("Invalid choice\n");
            continue;
        }
        
        enum StringOperation op = choice - 1;
        
        strcpy(buffer, input);
        
        switch (op) {
            case OP_UPPERCASE:
                to_uppercase(buffer);
                printf("Result: %s\n", buffer);
                break;
                
            case OP_LOWERCASE:
                to_lowercase(buffer);
                printf("Result: %s\n", buffer);
                break;
                
            case OP_REVERSE:
                reverse_string(buffer);
                printf("Result: %s\n", buffer);
                break;
                
            case OP_COUNT_WORDS:
                printf("Word count: %d\n", count_words(buffer));
                break;
                
            case OP_COUNT_VOWELS:
                printf("Vowel count: %d\n", count_vowels(buffer));
                break;
                
            case OP_EXIT:
                printf("Goodbye!\n");
                return 0;
                
            default:
                printf("Invalid operation\n");
                break;
        }
    }
    
    return 0;
}