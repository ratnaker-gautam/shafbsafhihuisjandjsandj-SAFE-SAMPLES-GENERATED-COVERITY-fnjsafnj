//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

enum StringOperation {
    OP_UPPERCASE,
    OP_LOWERCASE,
    OP_REVERSE,
    OP_PALINDROME_CHECK,
    OP_WORD_COUNT
};

int validateInput(const char *str, size_t max_len) {
    if (str == NULL) return 0;
    size_t len = strlen(str);
    if (len == 0 || len >= max_len) return 0;
    for (size_t i = 0; i < len; i++) {
        if (!isprint((unsigned char)str[i])) return 0;
    }
    return 1;
}

void performOperation(enum StringOperation op, const char *input, char *output, size_t output_size) {
    switch (op) {
        case OP_UPPERCASE: {
            size_t len = strlen(input);
            if (len >= output_size) return;
            for (size_t i = 0; i < len; i++) {
                output[i] = toupper((unsigned char)input[i]);
            }
            output[len] = '\0';
            break;
        }
        case OP_LOWERCASE: {
            size_t len = strlen(input);
            if (len >= output_size) return;
            for (size_t i = 0; i < len; i++) {
                output[i] = tolower((unsigned char)input[i]);
            }
            output[len] = '\0';
            break;
        }
        case OP_REVERSE: {
            size_t len = strlen(input);
            if (len >= output_size) return;
            for (size_t i = 0; i < len; i++) {
                output[i] = input[len - 1 - i];
            }
            output[len] = '\0';
            break;
        }
        case OP_PALINDROME_CHECK: {
            size_t len = strlen(input);
            int is_palindrome = 1;
            for (size_t i = 0; i < len / 2; i++) {
                if (input[i] != input[len - 1 - i]) {
                    is_palindrome = 0;
                    break;
                }
            }
            strcpy(output, is_palindrome ? "Palindrome" : "Not palindrome");
            break;
        }
        case OP_WORD_COUNT: {
            int count = 0;
            int in_word = 0;
            for (size_t i = 0; input[i] != '\0'; i++) {
                if (isspace((unsigned char)input[i])) {
                    in_word = 0;
                } else if (!in_word) {
                    count++;
                    in_word = 1;
                }
            }
            snprintf(output, output_size, "%d", count);
            break;
        }
    }
}

int main() {
    char input[256];
    char output[256];
    int choice;
    
    printf("Enter a string (max 255 chars): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Input error\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (!validateInput(input, sizeof(input))) {
        printf("Invalid input\n");
        return 1;
    }
    
    printf("Choose operation:\n");
    printf("1: Uppercase\n");
    printf("2: Lowercase\n");
    printf("3: Reverse\n");
    printf("4: Palindrome check\n");
    printf("5: Word count\n");
    printf("Enter choice (1-5): ");
    
    if (scanf("%d", &choice) != 1) {
        printf("Invalid choice\n");
        return 1;
    }
    
    while (getchar() != '\n');
    
    enum StringOperation op;
    switch (choice) {
        case 1: op = OP_UPPERCASE; break;
        case 2: op = OP_LOWERCASE; break;
        case 3: op = OP_REVERSE; break;
        case 4: op = OP_PALINDROME_CHECK; break;
        case 5: op = OP_WORD_COUNT; break;
        default:
            printf("Invalid choice\n");
            return 1;
    }
    
    performOperation(op, input, output, sizeof(output));
    printf("Result: %s\n", output);
    
    return 0;
}