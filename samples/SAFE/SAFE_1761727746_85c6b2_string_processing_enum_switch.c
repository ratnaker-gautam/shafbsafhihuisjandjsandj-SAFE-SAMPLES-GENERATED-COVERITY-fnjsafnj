//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>

enum string_operation {
    OP_UPPERCASE,
    OP_LOWERCASE,
    OP_REVERSE,
    OP_COUNT_WORDS,
    OP_COUNT_VOWELS
};

void process_string(const char *input, enum string_operation op) {
    char buffer[256];
    size_t len = strlen(input);
    
    if (len >= sizeof(buffer)) {
        printf("Error: Input too long\n");
        return;
    }
    
    switch (op) {
        case OP_UPPERCASE:
            strncpy(buffer, input, sizeof(buffer));
            for (size_t i = 0; i < len; i++) {
                buffer[i] = toupper((unsigned char)buffer[i]);
            }
            buffer[sizeof(buffer) - 1] = '\0';
            printf("Uppercase: %s\n", buffer);
            break;
            
        case OP_LOWERCASE:
            strncpy(buffer, input, sizeof(buffer));
            for (size_t i = 0; i < len; i++) {
                buffer[i] = tolower((unsigned char)buffer[i]);
            }
            buffer[sizeof(buffer) - 1] = '\0';
            printf("Lowercase: %s\n", buffer);
            break;
            
        case OP_REVERSE:
            strncpy(buffer, input, sizeof(buffer));
            for (size_t i = 0; i < len / 2; i++) {
                char temp = buffer[i];
                buffer[i] = buffer[len - 1 - i];
                buffer[len - 1 - i] = temp;
            }
            buffer[sizeof(buffer) - 1] = '\0';
            printf("Reversed: %s\n", buffer);
            break;
            
        case OP_COUNT_WORDS:
            {
                int count = 0;
                int in_word = 0;
                for (size_t i = 0; i < len; i++) {
                    if (isspace((unsigned char)input[i])) {
                        in_word = 0;
                    } else if (!in_word) {
                        count++;
                        in_word = 1;
                    }
                }
                printf("Word count: %d\n", count);
            }
            break;
            
        case OP_COUNT_VOWELS:
            {
                int count = 0;
                for (size_t i = 0; i < len; i++) {
                    char c = tolower((unsigned char)input[i]);
                    if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u') {
                        count++;
                    }
                }
                printf("Vowel count: %d\n", count);
            }
            break;
            
        default:
            printf("Error: Unknown operation\n");
            break;
    }
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
        len--;
    }
    
    if (len == 0) {
        printf("Error: Empty string\n");
        return 1;
    }
    
    printf("Choose operation:\n");
    printf("1. Convert to uppercase\n");
    printf("2. Convert to lowercase\n");
    printf("3. Reverse string\n");
    printf("4. Count words\n");
    printf("5. Count vowels\n");
    printf("Enter choice (1-5): ");
    
    if (scanf("%d", &choice) != 1) {
        printf("Error: Invalid input\n");
        return 1;
    }
    
    while (getchar() != '\n');
    
    enum string_operation op;
    switch (choice) {
        case 1: op = OP_UPPERCASE; break;
        case 2: op = OP_LOWERCASE; break;
        case 3: op = OP_REVERSE; break;
        case 4: op = OP_COUNT_WORDS; break;
        case 5: op = OP_COUNT_VOWELS; break;
        default:
            printf("Error: Invalid choice\n");
            return 1;
    }
    
    process_string(input, op);
    return 0;
}