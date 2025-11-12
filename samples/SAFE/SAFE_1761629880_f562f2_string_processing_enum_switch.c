//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>

enum StringOperation {
    OP_UPPERCASE,
    OP_LOWERCASE,
    OP_REVERSE,
    OP_COUNT_WORDS,
    OP_COUNT_VOWELS,
    OP_REMOVE_WHITESPACE
};

void safe_strncpy(char *dest, const char *src, size_t dest_size) {
    if (dest_size == 0) return;
    size_t i;
    for (i = 0; i < dest_size - 1 && src[i] != '\0'; i++) {
        dest[i] = src[i];
    }
    dest[i] = '\0';
}

int main(void) {
    char input[256];
    char output[256];
    int choice;
    
    printf("Enter a string (max 255 chars): ");
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
    printf("4: Count vowels\n");
    printf("5: Remove whitespace\n");
    printf("Enter choice (0-5): ");
    
    if (scanf("%d", &choice) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (choice < 0 || choice > 5) {
        printf("Invalid choice\n");
        return 1;
    }
    
    enum StringOperation op = (enum StringOperation)choice;
    
    switch (op) {
        case OP_UPPERCASE:
            safe_strncpy(output, input, sizeof(output));
            for (size_t i = 0; output[i] != '\0' && i < sizeof(output) - 1; i++) {
                output[i] = toupper((unsigned char)output[i]);
            }
            printf("Uppercase: %s\n", output);
            break;
            
        case OP_LOWERCASE:
            safe_strncpy(output, input, sizeof(output));
            for (size_t i = 0; output[i] != '\0' && i < sizeof(output) - 1; i++) {
                output[i] = tolower((unsigned char)output[i]);
            }
            printf("Lowercase: %s\n", output);
            break;
            
        case OP_REVERSE:
            safe_strncpy(output, input, sizeof(output));
            size_t out_len = strlen(output);
            for (size_t i = 0; i < out_len / 2; i++) {
                char temp = output[i];
                output[i] = output[out_len - 1 - i];
                output[out_len - 1 - i] = temp;
            }
            printf("Reversed: %s\n", output);
            break;
            
        case OP_COUNT_WORDS:
            {
                int word_count = 0;
                int in_word = 0;
                for (size_t i = 0; i < len; i++) {
                    if (isspace((unsigned char)input[i])) {
                        in_word = 0;
                    } else if (!in_word) {
                        word_count++;
                        in_word = 1;
                    }
                }
                printf("Word count: %d\n", word_count);
            }
            break;
            
        case OP_COUNT_VOWELS:
            {
                int vowel_count = 0;
                for (size_t i = 0; i < len; i++) {
                    char c = tolower((unsigned char)input[i]);
                    if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u') {
                        vowel_count++;
                    }
                }
                printf("Vowel count: %d\n", vowel_count);
            }
            break;
            
        case OP_REMOVE_WHITESPACE:
            {
                size_t j = 0;
                for (size_t i = 0; i < len && j < sizeof(output) - 1; i++) {
                    if (!isspace((unsigned char)input[i])) {
                        output[j++] = input[i];
                    }
                }
                output[j] = '\0';
                printf("Without whitespace: %s\n", output);
            }
            break;
    }
    
    return 0;
}