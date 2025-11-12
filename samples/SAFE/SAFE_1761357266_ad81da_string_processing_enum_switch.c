//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum StringOperation {
    OP_UPPERCASE,
    OP_LOWERCASE,
    OP_REVERSE,
    OP_COUNT_WORDS,
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

int main(void) {
    char buffer[256];
    enum StringOperation choice;
    
    while (1) {
        printf("Enter string (max 255 chars): ");
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            break;
        }
        
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        }
        
        if (strlen(buffer) == 0) {
            printf("Empty string\n");
            continue;
        }
        
        printf("Choose operation:\n");
        printf("0: Uppercase\n");
        printf("1: Lowercase\n");
        printf("2: Reverse\n");
        printf("3: Count words\n");
        printf("4: Exit\n");
        
        int input;
        if (scanf("%d", &input) != 1) {
            while (getchar() != '\n');
            printf("Invalid input\n");
            continue;
        }
        while (getchar() != '\n');
        
        if (input < 0 || input > 4) {
            printf("Invalid choice\n");
            continue;
        }
        
        choice = (enum StringOperation)input;
        
        switch (choice) {
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
                printf("Word count: %zu\n", count_words(buffer));
                break;
                
            case OP_EXIT:
                return 0;
                
            default:
                printf("Invalid operation\n");
                break;
        }
    }
    
    return 0;
}