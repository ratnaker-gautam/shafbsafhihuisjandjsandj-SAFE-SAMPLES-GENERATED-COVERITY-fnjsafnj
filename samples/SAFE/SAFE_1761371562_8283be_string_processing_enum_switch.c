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

int main(void) {
    char input[256];
    char buffer[256];
    int choice;
    
    while (1) {
        printf("Enter a string (max 255 chars): ");
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        size_t len = strlen(input);
        if (len > 0 && input[len-1] == '\n') {
            input[len-1] = '\0';
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
        printf("3: Count words\n");
        printf("4: Exit\n");
        printf("Enter choice: ");
        
        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n');
            printf("Invalid input.\n");
            continue;
        }
        while (getchar() != '\n');
        
        if (choice < 0 || choice > 4) {
            printf("Invalid choice.\n");
            continue;
        }
        
        enum StringOperation op = (enum StringOperation)choice;
        
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
                return 0;
            default:
                printf("Invalid operation.\n");
                break;
        }
    }
    
    return 0;
}