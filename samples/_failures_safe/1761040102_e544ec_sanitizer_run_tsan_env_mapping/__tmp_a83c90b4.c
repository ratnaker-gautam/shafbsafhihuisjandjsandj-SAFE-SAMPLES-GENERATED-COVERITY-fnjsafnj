//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: enum_switch ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

enum StringOp {
    OP_UPPERCASE,
    OP_LOWERCASE,
    OP_REVERSE,
    OP_REMOVE_WHITESPACE,
    OP_COUNT_WORDS
};

void string_uppercase(char *str, size_t len) {
    for (size_t i = 0; i < len; i++) {
        str[i] = toupper((unsigned char)str[i]);
    }
}

void string_lowercase(char *str, size_t len) {
    for (size_t i = 0; i < len; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

void string_reverse(char *str, size_t len) {
    for (size_t i = 0; i < len / 2; i++) {
        char temp = str[i];
        str[i] = str[len - 1 - i];
        str[len - 1 - i] = temp;
    }
}

void string_remove_whitespace(char *str, size_t len) {
    size_t j = 0;
    for (size_t i = 0; i < len; i++) {
        if (!isspace((unsigned char)str[i])) {
            str[j++] = str[i];
        }
    }
    str[j] = '\0';
}

size_t string_count_words(const char *str, size_t len) {
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
    int choice;
    
    printf("Enter a string (max 255 chars): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    
    if (len == 0) {
        printf("Empty string provided\n");
        return 1;
    }
    
    if (len >= sizeof(buffer)) {
        printf("Input too long\n");
        return 1;
    }
    
    printf("Choose operation:\n");
    printf("1. Convert to uppercase\n");
    printf("2. Convert to lowercase\n");
    printf("3. Reverse string\n");
    printf("4. Remove whitespace\n");
    printf("5. Count words\n");
    printf("Enter choice (1-5): ");
    
    if (scanf("%d", &choice) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    while (getchar() != '\n');
    
    strncpy(buffer, input, sizeof(buffer) - 1);
    buffer[sizeof(buffer) - 1] = '\0';
    
    enum StringOp op;
    switch (choice) {
        case 1: op = OP_UPPERCASE; break;
        case 2: op = OP_LOWERCASE; break;
        case 3: op = OP_REVERSE; break;
        case 4: op = OP_REMOVE_WHITESPACE; break;
        case 5: op = OP_COUNT_WORDS; break;
        default:
            printf("Invalid choice\n");
            return 1;
    }
    
    switch (op) {
        case OP_UPPERCASE:
            string_uppercase(buffer, strlen(buffer));
            printf("Result: %s\n", buffer);
            break;
            
        case OP_LOWERCASE:
            string_lowercase(buffer, strlen(buffer));
            printf("Result: %s\n", buffer);
            break;
            
        case OP_REVERSE:
            string_reverse(buffer, strlen(buffer));
            printf("Result: %s\n", buffer);
            break;
            
        case OP_REMOVE_WHITESPACE:
            string_remove_whitespace(buffer, strlen(buffer));
            printf("Result: %s\n", buffer);
            break;
            
        case OP_COUNT_WORDS:
            printf("Word count: %zu\n", string_count_words(buffer, strlen(buffer)));
            break;
    }
    
    return 0;
}