//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>

enum string_operation {
    OP_UPPERCASE,
    OP_LOWERCASE,
    OP_REVERSE,
    OP_PALINDROME_CHECK,
    OP_WORD_COUNT
};

void process_string(const char *input, enum string_operation op, char *output, size_t output_size) {
    if (input == NULL || output == NULL || output_size == 0) {
        return;
    }
    
    size_t len = strlen(input);
    if (len >= output_size) {
        return;
    }
    
    switch (op) {
        case OP_UPPERCASE:
            for (size_t i = 0; i < len; i++) {
                output[i] = toupper((unsigned char)input[i]);
            }
            output[len] = '\0';
            break;
            
        case OP_LOWERCASE:
            for (size_t i = 0; i < len; i++) {
                output[i] = tolower((unsigned char)input[i]);
            }
            output[len] = '\0';
            break;
            
        case OP_REVERSE:
            for (size_t i = 0; i < len; i++) {
                output[i] = input[len - 1 - i];
            }
            output[len] = '\0';
            break;
            
        case OP_PALINDROME_CHECK:
            {
                int is_palindrome = 1;
                for (size_t i = 0; i < len / 2; i++) {
                    if (input[i] != input[len - 1 - i]) {
                        is_palindrome = 0;
                        break;
                    }
                }
                if (is_palindrome) {
                    strcpy(output, "Palindrome");
                } else {
                    strcpy(output, "Not palindrome");
                }
            }
            break;
            
        case OP_WORD_COUNT:
            {
                int count = 0;
                int in_word = 0;
                
                for (size_t i = 0; i < len; i++) {
                    if (isspace((unsigned char)input[i])) {
                        in_word = 0;
                    } else {
                        if (!in_word) {
                            count++;
                            in_word = 1;
                        }
                    }
                }
                
                snprintf(output, output_size, "%d words", count);
            }
            break;
    }
}

int main(void) {
    char input[256];
    char output[256];
    int choice;
    
    printf("Enter a string (max 255 characters): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (strlen(input) == 0) {
        printf("Empty string entered.\n");
        return 1;
    }
    
    printf("Choose operation:\n");
    printf("1. Convert to uppercase\n");
    printf("2. Convert to lowercase\n");
    printf("3. Reverse string\n");
    printf("4. Check if palindrome\n");
    printf("5. Count words\n");
    printf("Enter choice (1-5): ");
    
    if (scanf("%d", &choice) != 1) {
        printf("Invalid input.\n");
        return 1;
    }
    
    while (getchar() != '\n');
    
    enum string_operation op;
    switch (choice) {
        case 1: op = OP_UPPERCASE; break;
        case 2: op = OP_LOWERCASE; break;
        case 3: op = OP_REVERSE; break;
        case 4: op = OP_PALINDROME_CHECK; break;
        case 5: op = OP_WORD_COUNT; break;
        default:
            printf("Invalid choice.\n");
            return 1;
    }
    
    process_string(input, op, output, sizeof(output));
    
    printf("Input: %s\n", input);
    printf("Result: %s\n", output);
    
    return 0;
}