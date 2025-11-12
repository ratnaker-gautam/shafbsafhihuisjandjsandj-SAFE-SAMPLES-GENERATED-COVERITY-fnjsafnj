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

int main(void) {
    char input[256];
    char buffer[256];
    enum StringOperation op;
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
    
    if (len >= sizeof(buffer)) {
        printf("Input string too long.\n");
        return 1;
    }
    
    do {
        printf("\nChoose operation:\n");
        printf("1. Convert to uppercase\n");
        printf("2. Convert to lowercase\n");
        printf("3. Reverse string\n");
        printf("4. Count words\n");
        printf("5. Exit\n");
        printf("Enter choice (1-5): ");
        
        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n');
            printf("Invalid input.\n");
            continue;
        }
        
        while (getchar() != '\n');
        
        switch (choice) {
            case 1: op = OP_UPPERCASE; break;
            case 2: op = OP_LOWERCASE; break;
            case 3: op = OP_REVERSE; break;
            case 4: op = OP_COUNT_WORDS; break;
            case 5: op = OP_EXIT; break;
            default:
                printf("Invalid choice. Please enter 1-5.\n");
                continue;
        }
        
        if (op == OP_EXIT) {
            break;
        }
        
        strncpy(buffer, input, sizeof(buffer) - 1);
        buffer[sizeof(buffer) - 1] = '\0';
        size_t buf_len = strlen(buffer);
        
        switch (op) {
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
            case OP_EXIT:
                break;
        }
        
    } while (1);
    
    return 0;
}