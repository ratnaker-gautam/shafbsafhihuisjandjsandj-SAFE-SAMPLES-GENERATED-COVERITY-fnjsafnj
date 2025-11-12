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
    OP_REMOVE_WHITESPACE
};

void process_string(const char *input, enum StringOperation op, char *output, size_t output_size) {
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
            
        case OP_COUNT_VOWELS:
            {
                int count = 0;
                for (size_t i = 0; i < len; i++) {
                    char c = tolower((unsigned char)input[i]);
                    if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u') {
                        count++;
                    }
                }
                snprintf(output, output_size, "%d", count);
            }
            break;
            
        case OP_REMOVE_WHITESPACE:
            {
                size_t j = 0;
                for (size_t i = 0; i < len; i++) {
                    if (!isspace((unsigned char)input[i])) {
                        if (j < output_size - 1) {
                            output[j++] = input[i];
                        }
                    }
                }
                output[j] = '\0';
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
        printf("Empty string provided.\n");
        return 1;
    }
    
    printf("Choose operation:\n");
    printf("1. Convert to uppercase\n");
    printf("2. Convert to lowercase\n");
    printf("3. Reverse string\n");
    printf("4. Count vowels\n");
    printf("5. Remove whitespace\n");
    printf("Enter choice (1-5): ");
    
    if (scanf("%d", &choice) != 1) {
        printf("Invalid input.\n");
        return 1;
    }
    
    while (getchar() != '\n');
    
    enum StringOperation op;
    switch (choice) {
        case 1: op = OP_UPPERCASE; break;
        case 2: op = OP_LOWERCASE; break;
        case 3: op = OP_REVERSE; break;
        case 4: op = OP_COUNT_VOWELS; break;
        case 5: op = OP_REMOVE_WHITESPACE; break;
        default:
            printf("Invalid choice.\n");
            return 1;
    }
    
    process_string(input, op, output, sizeof(output));
    
    printf("Input: %s\n", input);
    printf("Result: %s\n", output);
    
    return 0;
}