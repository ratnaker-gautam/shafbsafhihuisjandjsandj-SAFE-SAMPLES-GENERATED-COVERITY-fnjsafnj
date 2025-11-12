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

int perform_operation(const char *input, char *output, size_t output_size, enum StringOperation op) {
    if (input == NULL || output == NULL || output_size == 0) {
        return -1;
    }
    
    size_t len = strlen(input);
    if (len >= output_size) {
        return -1;
    }
    
    switch (op) {
        case OP_UPPERCASE:
            for (size_t i = 0; i <= len; i++) {
                if (i < output_size) {
                    output[i] = (char)toupper((unsigned char)input[i]);
                } else {
                    return -1;
                }
            }
            break;
            
        case OP_LOWERCASE:
            for (size_t i = 0; i <= len; i++) {
                if (i < output_size) {
                    output[i] = (char)tolower((unsigned char)input[i]);
                } else {
                    return -1;
                }
            }
            break;
            
        case OP_REVERSE:
            if (len > 0) {
                for (size_t i = 0; i < len; i++) {
                    if (i < output_size) {
                        output[i] = input[len - 1 - i];
                    } else {
                        return -1;
                    }
                }
                output[len] = '\0';
            } else {
                output[0] = '\0';
            }
            break;
            
        case OP_PALINDROME_CHECK:
            strncpy(output, input, output_size - 1);
            output[output_size - 1] = '\0';
            
            if (len > 0) {
                for (size_t i = 0; i < len / 2; i++) {
                    char left = (char)tolower((unsigned char)input[i]);
                    char right = (char)tolower((unsigned char)input[len - 1 - i]);
                    if (left != right) {
                        return 0;
                    }
                }
            }
            return 1;
            
        case OP_COUNT_WORDS:
            strncpy(output, input, output_size - 1);
            output[output_size - 1] = '\0';
            
            int word_count = 0;
            int in_word = 0;
            
            for (size_t i = 0; i < len; i++) {
                if (isspace((unsigned char)input[i])) {
                    in_word = 0;
                } else {
                    if (!in_word) {
                        word_count++;
                        in_word = 1;
                    }
                }
            }
            return word_count;
            
        default:
            return -1;
    }
    
    return 0;
}

int main(void) {
    char input[256];
    char output[256];
    int choice;
    
    printf("Enter a string (max 255 chars): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    printf("Choose operation:\n");
    printf("1. Convert to uppercase\n");
    printf("2. Convert to lowercase\n");
    printf("3. Reverse string\n");
    printf("4. Check if palindrome\n");
    printf("5. Count words\n");
    printf("Enter choice (1-5): ");
    
    if (scanf("%d", &choice) != 1) {
        return 1;
    }
    
    while (getchar() != '\n');
    
    enum StringOperation op;
    switch (choice) {
        case 1: op = OP_UPPERCASE; break;
        case 2: op = OP_LOWERCASE; break;
        case 3: op = OP_REVERSE; break;
        case 4: op = OP_PALINDROME_CHECK; break;
        case 5: op = OP_COUNT_WORDS; break;
        default:
            printf("Invalid choice\n");
            return 1;
    }
    
    int result = perform_operation(input, output, sizeof(output), op);
    
    if (result == -1) {
        printf("Error: Operation failed\n");
        return 1;
    }
    
    switch (op) {
        case OP_UPPERCASE:
        case OP_LOWERCASE:
        case OP_REVERSE:
            printf("Result: %s\n", output);
            break;
            
        case OP_PALINDROME_CHECK:
            if (result == 1) {
                printf("The string is a palindrome\n");
            } else {
                printf("The string is not a palindrome\n");
            }
            break;
            
        case OP_COUNT_WORDS:
            printf("Word count: %d\n", result);
            break;
            
        default:
            break;
    }
    
    return 0;
}