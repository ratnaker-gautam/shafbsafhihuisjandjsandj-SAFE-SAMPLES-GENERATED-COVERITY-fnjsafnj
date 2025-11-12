//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>

enum StringOperation {
    OP_UPPERCASE,
    OP_LOWERCASE,
    OP_REVERSE,
    OP_WORD_COUNT,
    OP_CHAR_COUNT
};

void toUpperCase(char *str) {
    if (str == NULL) return;
    for (int i = 0; str[i] != '\0'; i++) {
        str[i] = toupper((unsigned char)str[i]);
    }
}

void toLowerCase(char *str) {
    if (str == NULL) return;
    for (int i = 0; str[i] != '\0'; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

void reverseString(char *str) {
    if (str == NULL) return;
    int len = strlen(str);
    for (int i = 0; i < len / 2; i++) {
        char temp = str[i];
        str[i] = str[len - i - 1];
        str[len - i - 1] = temp;
    }
}

int countWords(const char *str) {
    if (str == NULL) return 0;
    int count = 0;
    int inWord = 0;
    for (int i = 0; str[i] != '\0'; i++) {
        if (isspace((unsigned char)str[i])) {
            inWord = 0;
        } else if (!inWord) {
            count++;
            inWord = 1;
        }
    }
    return count;
}

int countCharacters(const char *str) {
    if (str == NULL) return 0;
    return strlen(str);
}

int main() {
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
    }
    
    if (strlen(input) == 0) {
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
    strncpy(buffer, input, sizeof(buffer) - 1);
    buffer[sizeof(buffer) - 1] = '\0';
    
    switch (choice) {
        case OP_UPPERCASE:
            toUpperCase(buffer);
            printf("Result: %s\n", buffer);
            break;
        case OP_LOWERCASE:
            toLowerCase(buffer);
            printf("Result: %s\n", buffer);
            break;
        case OP_REVERSE:
            reverseString(buffer);
            printf("Result: %s\n", buffer);
            break;
        case OP_WORD_COUNT:
            printf("Word count: %d\n", countWords(buffer));
            break;
        case OP_CHAR_COUNT:
            printf("Character count: %d\n", countCharacters(buffer));
            break;
        default:
            printf("Invalid choice\n");
            return 1;
    }
    
    return 0;
}