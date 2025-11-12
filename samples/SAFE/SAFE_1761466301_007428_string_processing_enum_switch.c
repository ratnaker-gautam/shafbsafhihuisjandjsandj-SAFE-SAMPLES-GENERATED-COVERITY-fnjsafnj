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
    OP_COUNT_VOWELS,
    OP_TRIM
};

void trimString(char *str) {
    int start = 0, end = strlen(str) - 1;
    while (isspace((unsigned char)str[start])) start++;
    while (end >= start && isspace((unsigned char)str[end])) end--;
    int len = end - start + 1;
    if (len > 0) {
        memmove(str, str + start, len);
    }
    str[len] = '\0';
}

int countWords(const char *str) {
    int count = 0, inWord = 0;
    for (size_t i = 0; i < strlen(str); i++) {
        if (isspace((unsigned char)str[i])) {
            inWord = 0;
        } else if (!inWord) {
            count++;
            inWord = 1;
        }
    }
    return count;
}

int countVowels(const char *str) {
    int count = 0;
    for (size_t i = 0; i < strlen(str); i++) {
        char c = tolower((unsigned char)str[i]);
        if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u') {
            count++;
        }
    }
    return count;
}

void reverseString(char *str) {
    size_t len = strlen(str);
    for (size_t i = 0; i < len / 2; i++) {
        char temp = str[i];
        str[i] = str[len - i - 1];
        str[len - i - 1] = temp;
    }
}

void toUpperCase(char *str) {
    for (size_t i = 0; i < strlen(str); i++) {
        str[i] = toupper((unsigned char)str[i]);
    }
}

void toLowerCase(char *str) {
    for (size_t i = 0; i < strlen(str); i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

int main() {
    char input[256];
    char buffer[256];
    int choice;
    
    printf("Enter a string (max 255 chars): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len-1] == '\n') {
        input[len-1] = '\0';
    }
    
    if (strlen(input) == 0) {
        printf("Empty string provided\n");
        return 1;
    }
    
    printf("Choose operation:\n");
    printf("1. Convert to uppercase\n");
    printf("2. Convert to lowercase\n");
    printf("3. Reverse string\n");
    printf("4. Count words\n");
    printf("5. Count vowels\n");
    printf("6. Trim whitespace\n");
    printf("Enter choice (1-6): ");
    
    if (scanf("%d", &choice) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (choice < 1 || choice > 6) {
        printf("Invalid choice\n");
        return 1;
    }
    
    strncpy(buffer, input, sizeof(buffer) - 1);
    buffer[sizeof(buffer) - 1] = '\0';
    
    enum StringOp op = choice - 1;
    
    switch (op) {
        case OP_UPPERCASE:
            toUpperCase(buffer);
            printf("Uppercase: %s\n", buffer);
            break;
            
        case OP_LOWERCASE:
            toLowerCase(buffer);
            printf("Lowercase: %s\n", buffer);
            break;
            
        case OP_REVERSE:
            reverseString(buffer);
            printf("Reversed: %s\n", buffer);
            break;
            
        case OP_COUNT_WORDS:
            printf("Word count: %d\n", countWords(buffer));
            break;
            
        case OP_COUNT_VOWELS:
            printf("Vowel count: %d\n", countVowels(buffer));
            break;
            
        case OP_TRIM:
            trimString(buffer);
            printf("Trimmed: '%s'\n", buffer);
            break;
    }
    
    return 0;
}