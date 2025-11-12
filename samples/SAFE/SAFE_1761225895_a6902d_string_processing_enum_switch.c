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
    char buffer[256];
    enum StringOp choice;
    int valid_input;
    
    printf("Enter a string (max 255 characters): ");
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
        len--;
    }
    
    if (len == 0) {
        fprintf(stderr, "Empty string provided\n");
        return 1;
    }
    
    do {
        printf("\nChoose operation:\n");
        printf("0: Convert to uppercase\n");
        printf("1: Convert to lowercase\n");
        printf("2: Reverse string\n");
        printf("3: Count words\n");
        printf("4: Exit\n");
        printf("Enter choice (0-4): ");
        
        valid_input = 0;
        char input[16];
        if (fgets(input, sizeof(input), stdin) != NULL) {
            char *endptr;
            long temp = strtol(input, &endptr, 10);
            if (endptr != input && *endptr == '\n' && temp >= 0 && temp <= 4) {
                choice = (enum StringOp)temp;
                valid_input = 1;
            }
        }
        
        if (!valid_input) {
            printf("Invalid choice. Please enter 0-4.\n");
            continue;
        }
        
        char working_copy[256];
        if (len >= sizeof(working_copy)) {
            fprintf(stderr, "String too long for processing\n");
            return 1;
        }
        strncpy(working_copy, buffer, sizeof(working_copy));
        working_copy[sizeof(working_copy) - 1] = '\0';
        
        switch (choice) {
            case OP_UPPERCASE:
                to_uppercase(working_copy, len);
                printf("Uppercase: %s\n", working_copy);
                break;
            case OP_LOWERCASE:
                to_lowercase(working_copy, len);
                printf("Lowercase: %s\n", working_copy);
                break;
            case OP_REVERSE:
                reverse_string(working_copy, len);
                printf("Reversed: %s\n", working_copy);
                break;
            case OP_COUNT_WORDS:
                printf("Word count: %zu\n", count_words(working_copy, len));
                break;
            case OP_EXIT:
                printf("Goodbye!\n");
                break;
        }
    } while (choice != OP_EXIT);
    
    return 0;
}