//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>

enum StringOperation {
    OP_UPPERCASE,
    OP_LOWERCASE,
    OP_REVERSE,
    OP_PALINDROME_CHECK,
    OP_COUNT_WORDS
};

void safe_strncpy(char *dest, const char *src, size_t dest_size) {
    if (dest_size == 0) return;
    size_t i;
    for (i = 0; i < dest_size - 1 && src[i] != '\0'; i++) {
        dest[i] = src[i];
    }
    dest[i] = '\0';
}

size_t safe_strlen(const char *str, size_t max_len) {
    size_t len = 0;
    while (len < max_len && str[len] != '\0') {
        len++;
    }
    return len;
}

void to_uppercase(char *str, size_t buf_size) {
    size_t len = safe_strlen(str, buf_size);
    for (size_t i = 0; i < len; i++) {
        str[i] = (char)toupper((unsigned char)str[i]);
    }
}

void to_lowercase(char *str, size_t buf_size) {
    size_t len = safe_strlen(str, buf_size);
    for (size_t i = 0; i < len; i++) {
        str[i] = (char)tolower((unsigned char)str[i]);
    }
}

void reverse_string(char *str, size_t buf_size) {
    size_t len = safe_strlen(str, buf_size);
    for (size_t i = 0; i < len / 2; i++) {
        char temp = str[i];
        str[i] = str[len - 1 - i];
        str[len - 1 - i] = temp;
    }
}

int is_palindrome(const char *str, size_t buf_size) {
    size_t len = safe_strlen(str, buf_size);
    for (size_t i = 0; i < len / 2; i++) {
        if (str[i] != str[len - 1 - i]) {
            return 0;
        }
    }
    return 1;
}

int count_words(const char *str, size_t buf_size) {
    size_t len = safe_strlen(str, buf_size);
    int count = 0;
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
        return 1;
    }
    
    size_t input_len = safe_strlen(input, sizeof(input));
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
    }
    
    printf("Choose operation:\n");
    printf("1. Convert to uppercase\n");
    printf("2. Convert to lowercase\n");
    printf("3. Reverse string\n");
    printf("4. Check if palindrome\n");
    printf("5. Count words\n");
    printf("Enter choice (1-5): ");
    
    if (scanf("%d", &choice) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    while (getchar() != '\n');
    
    if (choice < 1 || choice > 5) {
        printf("Invalid choice\n");
        return 1;
    }
    
    enum StringOperation op = (enum StringOperation)(choice - 1);
    safe_strncpy(buffer, input, sizeof(buffer));
    
    switch (op) {
        case OP_UPPERCASE:
            to_uppercase(buffer, sizeof(buffer));
            printf("Uppercase: %s\n", buffer);
            break;
            
        case OP_LOWERCASE:
            to_lowercase(buffer, sizeof(buffer));
            printf("Lowercase: %s\n", buffer);
            break;
            
        case OP_REVERSE:
            reverse_string(buffer, sizeof(buffer));
            printf("Reversed: %s\n", buffer);
            break;
            
        case OP_PALINDROME_CHECK:
            if (is_palindrome(buffer, sizeof(buffer))) {
                printf("The string is a palindrome\n");
            } else {
                printf("The string is not a palindrome\n");
            }
            break;
            
        case OP_COUNT_WORDS:
            printf("Word count: %d\n", count_words(buffer, sizeof(buffer)));
            break;
    }
    
    return 0;
}