//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>

enum string_operation {
    OP_UPPERCASE,
    OP_LOWERCASE,
    OP_REVERSE,
    OP_WORD_COUNT,
    OP_CHAR_COUNT
};

void string_to_uppercase(char *str, size_t len) {
    for (size_t i = 0; i < len && str[i] != '\0'; i++) {
        str[i] = toupper((unsigned char)str[i]);
    }
}

void string_to_lowercase(char *str, size_t len) {
    for (size_t i = 0; i < len && str[i] != '\0'; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

void string_reverse(char *str, size_t len) {
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

size_t count_chars(const char *str, size_t len) {
    size_t count = 0;
    for (size_t i = 0; i < len && str[i] != '\0'; i++) {
        count++;
    }
    return count;
}

int main(void) {
    char input[256];
    int choice;
    
    printf("Enter a string (max 255 characters): ");
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
    
    printf("Choose operation:\n");
    printf("0: Convert to uppercase\n");
    printf("1: Convert to lowercase\n");
    printf("2: Reverse string\n");
    printf("3: Count words\n");
    printf("4: Count characters\n");
    printf("Enter choice (0-4): ");
    
    if (scanf("%d", &choice) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    char buffer[256];
    if (len >= sizeof(buffer)) {
        printf("Input too long for processing\n");
        return 1;
    }
    strncpy(buffer, input, sizeof(buffer) - 1);
    buffer[sizeof(buffer) - 1] = '\0';
    
    switch (choice) {
        case OP_UPPERCASE:
            string_to_uppercase(buffer, sizeof(buffer));
            printf("Result: %s\n", buffer);
            break;
            
        case OP_LOWERCASE:
            string_to_lowercase(buffer, sizeof(buffer));
            printf("Result: %s\n", buffer);
            break;
            
        case OP_REVERSE:
            string_reverse(buffer, len);
            printf("Result: %s\n", buffer);
            break;
            
        case OP_WORD_COUNT:
            printf("Word count: %zu\n", count_words(buffer, sizeof(buffer)));
            break;
            
        case OP_CHAR_COUNT:
            printf("Character count: %zu\n", count_chars(buffer, sizeof(buffer)));
            break;
            
        default:
            printf("Invalid choice\n");
            return 1;
    }
    
    return 0;
}