//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>

typedef void (*string_processor)(char *str, size_t len);

void to_uppercase(char *str, size_t len) {
    if (str == NULL || len == 0) return;
    for (size_t i = 0; i < len && str[i] != '\0'; i++) {
        str[i] = (char)toupper((unsigned char)str[i]);
    }
}

void to_lowercase(char *str, size_t len) {
    if (str == NULL || len == 0) return;
    for (size_t i = 0; i < len && str[i] != '\0'; i++) {
        str[i] = (char)tolower((unsigned char)str[i]);
    }
}

void reverse_string(char *str, size_t len) {
    if (str == NULL || len == 0) return;
    size_t actual_len = strlen(str);
    if (actual_len > len - 1) actual_len = len - 1;
    for (size_t i = 0; i < actual_len / 2; i++) {
        char temp = str[i];
        str[i] = str[actual_len - 1 - i];
        str[actual_len - 1 - i] = temp;
    }
}

void remove_whitespace(char *str, size_t len) {
    if (str == NULL || len == 0) return;
    size_t write_pos = 0;
    for (size_t i = 0; i < len && str[i] != '\0'; i++) {
        if (!isspace((unsigned char)str[i])) {
            str[write_pos++] = str[i];
        }
    }
    if (write_pos < len) {
        str[write_pos] = '\0';
    }
}

int process_string(char *input, size_t input_len, string_processor processor) {
    if (input == NULL || processor == NULL || input_len == 0) return 0;
    
    size_t actual_len = strlen(input);
    if (actual_len >= input_len) return 0;
    
    char buffer[256];
    if (input_len > sizeof(buffer)) return 0;
    
    strncpy(buffer, input, sizeof(buffer) - 1);
    buffer[sizeof(buffer) - 1] = '\0';
    
    processor(buffer, sizeof(buffer));
    
    printf("Processed: %s\n", buffer);
    return 1;
}

int main(void) {
    char input[256];
    string_processor processors[] = {
        to_uppercase,
        to_lowercase,
        reverse_string,
        remove_whitespace
    };
    const char *names[] = {
        "Uppercase",
        "Lowercase", 
        "Reversed",
        "No whitespace"
    };
    int num_processors = sizeof(processors) / sizeof(processors[0]);
    
    printf("Enter a string to process (max 255 chars): ");
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
        printf("Empty input string\n");
        return 1;
    }
    
    if (len >= sizeof(input)) {
        printf("Input too long\n");
        return 1;
    }
    
    printf("\nOriginal: %s\n\n", input);
    
    for (int i = 0; i < num_processors; i++) {
        printf("%s: ", names[i]);
        if (!process_string(input, sizeof(input), processors[i])) {
            printf("Processing failed\n");
        }
    }
    
    return 0;
}