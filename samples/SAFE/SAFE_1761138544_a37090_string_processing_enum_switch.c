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
    OP_COUNT_VOWELS
};

int perform_operation(const char *input, enum StringOp op, char *output, size_t output_size) {
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
                if (i < output_size - 1) {
                    output[i] = toupper((unsigned char)input[i]);
                } else {
                    return -1;
                }
            }
            break;
            
        case OP_LOWERCASE:
            for (size_t i = 0; i <= len; i++) {
                if (i < output_size - 1) {
                    output[i] = tolower((unsigned char)input[i]);
                } else {
                    return -1;
                }
            }
            break;
            
        case OP_REVERSE:
            if (len >= output_size) {
                return -1;
            }
            for (size_t i = 0; i < len; i++) {
                if (i < output_size - 1) {
                    output[i] = input[len - 1 - i];
                } else {
                    return -1;
                }
            }
            output[len] = '\0';
            break;
            
        case OP_COUNT_WORDS:
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
                
                if (snprintf(output, output_size, "%d", count) >= (int)output_size) {
                    return -1;
                }
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
                
                if (snprintf(output, output_size, "%d", count) >= (int)output_size) {
                    return -1;
                }
            }
            break;
            
        default:
            return -1;
    }
    
    return 0;
}

int main(void) {
    char input[256];
    char output[256];
    int choice;
    
    printf("Enter a string (max 255 characters): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (strlen(input) == 0) {
        fprintf(stderr, "Empty string not allowed\n");
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
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    while (getchar() != '\n');
    
    enum StringOp op;
    switch (choice) {
        case 1: op = OP_UPPERCASE; break;
        case 2: op = OP_LOWERCASE; break;
        case 3: op = OP_REVERSE; break;
        case 4: op = OP_COUNT_WORDS; break;
        case 5: op = OP_COUNT_VOWELS; break;
        default:
            fprintf(stderr, "Invalid choice\n");
            return 1;
    }
    
    if (perform_operation(input, op, output, sizeof(output)) != 0) {
        fprintf(stderr, "Operation failed\n");
        return 1;
    }
    
    printf("Result: %s\n", output);
    
    return 0;
}