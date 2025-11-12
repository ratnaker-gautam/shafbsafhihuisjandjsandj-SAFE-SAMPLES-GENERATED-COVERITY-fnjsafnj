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
    enum StringOperation op;
    int choice;
    
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
    
    printf("\nAvailable operations:\n");
    printf("1. Convert to uppercase\n");
    printf("2. Convert to lowercase\n");
    printf("3. Reverse string\n");
    printf("4. Count words\n");
    printf("5. Exit\n");
    printf("Choose operation (1-5): ");
    
    if (scanf("%d", &choice) != 1) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    if (choice < 1 || choice > 5) {
        fprintf(stderr, "Invalid choice\n");
        return 1;
    }
    
    op = (enum StringOperation)(choice - 1);
    
    char result[256];
    if (strlen(buffer) >= sizeof(result)) {
        fprintf(stderr, "String too long for processing\n");
        return 1;
    }
    strcpy(result, buffer);
    size_t result_len = strlen(result);
    
    switch (op) {
        case OP_UPPERCASE:
            to_uppercase(result, result_len);
            printf("Uppercase: %s\n", result);
            break;
            
        case OP_LOWERCASE:
            to_lowercase(result, result_len);
            printf("Lowercase: %s\n", result);
            break;
            
        case OP_REVERSE:
            reverse_string(result, result_len);
            printf("Reversed: %s\n", result);
            break;
            
        case OP_COUNT_WORDS:
            printf("Word count: %zu\n", count_words(result, result_len));
            break;
            
        case OP_EXIT:
            printf("Exiting...\n");
            break;
    }
    
    return 0;
}