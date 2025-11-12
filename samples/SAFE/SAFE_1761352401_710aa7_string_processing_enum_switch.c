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
    for (char *p = str; *p; p++) {
        *p = toupper((unsigned char)*p);
    }
}

void to_lowercase(char *str) {
    if (str == NULL) return;
    for (char *p = str; *p; p++) {
        *p = tolower((unsigned char)*p);
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

int count_words(const char *str) {
    if (str == NULL) return 0;
    int count = 0;
    int in_word = 0;
    for (const char *p = str; *p; p++) {
        if (isspace((unsigned char)*p)) {
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
    enum StringOperation choice;
    
    while (1) {
        printf("String Processor Menu:\n");
        printf("1. Convert to uppercase\n");
        printf("2. Convert to lowercase\n");
        printf("3. Reverse string\n");
        printf("4. Count words\n");
        printf("5. Exit\n");
        printf("Enter choice (1-5): ");
        
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        int num = atoi(input);
        if (num < 1 || num > 5) {
            printf("Invalid choice. Please enter 1-5.\n");
            continue;
        }
        
        choice = (enum StringOperation)(num - 1);
        
        if (choice == OP_EXIT) {
            break;
        }
        
        printf("Enter string: ");
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            break;
        }
        
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        }
        
        if (strlen(buffer) == 0) {
            printf("Empty string provided.\n");
            continue;
        }
        
        char working_copy[256];
        if (strlen(buffer) >= sizeof(working_copy)) {
            printf("Input string too long.\n");
            continue;
        }
        strcpy(working_copy, buffer);
        
        switch (choice) {
            case OP_UPPERCASE:
                to_uppercase(working_copy);
                printf("Result: %s\n", working_copy);
                break;
            case OP_LOWERCASE:
                to_lowercase(working_copy);
                printf("Result: %s\n", working_copy);
                break;
            case OP_REVERSE:
                reverse_string(working_copy);
                printf("Result: %s\n", working_copy);
                break;
            case OP_COUNT_WORDS:
                printf("Word count: %d\n", count_words(working_copy));
                break;
            case OP_EXIT:
                break;
        }
    }
    
    return 0;
}