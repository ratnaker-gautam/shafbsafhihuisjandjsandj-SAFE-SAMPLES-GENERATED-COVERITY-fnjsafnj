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
        str[i] = str[len - 1 - i];
        str[len - 1 - i] = temp;
    }
}

int count_words(const char *str) {
    if (str == NULL) return 0;
    int count = 0;
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
    enum StringOperation op;
    int choice;
    
    printf("Enter a string (max 255 chars): ");
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return 1;
    }
    
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
    }
    
    if (strlen(buffer) == 0) {
        printf("Empty string provided.\n");
        return 1;
    }
    
    char working_copy[256];
    strncpy(working_copy, buffer, sizeof(working_copy) - 1);
    working_copy[sizeof(working_copy) - 1] = '\0';
    
    printf("Choose operation:\n");
    printf("1: Convert to uppercase\n");
    printf("2: Convert to lowercase\n");
    printf("3: Reverse string\n");
    printf("4: Count words\n");
    printf("5: Exit\n");
    
    if (scanf("%d", &choice) != 1) {
        printf("Invalid input.\n");
        return 1;
    }
    
    op = (enum StringOperation)(choice - 1);
    
    switch (op) {
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
            printf("Exiting.\n");
            break;
            
        default:
            printf("Invalid operation.\n");
            return 1;
    }
    
    return 0;
}