//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

enum StringOperation {
    OP_UPPERCASE,
    OP_LOWERCASE,
    OP_REVERSE,
    OP_COUNT_VOWELS,
    OP_REMOVE_WHITESPACE,
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

size_t count_vowels(const char *str, size_t len) {
    size_t count = 0;
    for (size_t i = 0; i < len; i++) {
        char c = tolower((unsigned char)str[i]);
        if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u') {
            count++;
        }
    }
    return count;
}

void remove_whitespace(char *str, size_t *len) {
    size_t j = 0;
    for (size_t i = 0; i < *len; i++) {
        if (!isspace((unsigned char)str[i])) {
            str[j++] = str[i];
        }
    }
    str[j] = '\0';
    *len = j;
}

int main(void) {
    char buffer[256];
    char working_copy[256];
    size_t len;
    int choice;
    
    printf("Enter a string (max 255 characters): ");
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
        len--;
    }
    
    if (len == 0) {
        fprintf(stderr, "Empty string provided\n");
        return 1;
    }
    
    while (1) {
        printf("\nCurrent string: %s\n", buffer);
        printf("Choose operation:\n");
        printf("1. Convert to uppercase\n");
        printf("2. Convert to lowercase\n");
        printf("3. Reverse string\n");
        printf("4. Count vowels\n");
        printf("5. Remove whitespace\n");
        printf("6. Exit\n");
        printf("Enter choice (1-6): ");
        
        if (scanf("%d", &choice) != 1) {
            fprintf(stderr, "Invalid input\n");
            while (getchar() != '\n');
            continue;
        }
        
        while (getchar() != '\n');
        
        if (choice < 1 || choice > 6) {
            fprintf(stderr, "Invalid choice\n");
            continue;
        }
        
        enum StringOperation op = choice - 1;
        
        strncpy(working_copy, buffer, sizeof(working_copy) - 1);
        working_copy[sizeof(working_copy) - 1] = '\0';
        size_t copy_len = strlen(working_copy);
        
        switch (op) {
            case OP_UPPERCASE:
                to_uppercase(working_copy, copy_len);
                strncpy(buffer, working_copy, sizeof(buffer) - 1);
                buffer[sizeof(buffer) - 1] = '\0';
                printf("Converted to uppercase: %s\n", buffer);
                break;
                
            case OP_LOWERCASE:
                to_lowercase(working_copy, copy_len);
                strncpy(buffer, working_copy, sizeof(buffer) - 1);
                buffer[sizeof(buffer) - 1] = '\0';
                printf("Converted to lowercase: %s\n", buffer);
                break;
                
            case OP_REVERSE:
                reverse_string(working_copy, copy_len);
                strncpy(buffer, working_copy, sizeof(buffer) - 1);
                buffer[sizeof(buffer) - 1] = '\0';
                printf("Reversed string: %s\n", buffer);
                break;
                
            case OP_COUNT_VOWELS:
                printf("Number of vowels: %zu\n", count_vowels(working_copy, copy_len));
                break;
                
            case OP_REMOVE_WHITESPACE:
                remove_whitespace(working_copy, &copy_len);
                strncpy(buffer, working_copy, sizeof(buffer) - 1);
                buffer[sizeof(buffer) - 1] = '\0';
                printf("Whitespace removed: %s\n", buffer);
                break;
                
            case OP_EXIT:
                printf("Goodbye!\n");
                return 0;
        }
    }
    
    return 0;
}