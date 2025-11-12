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
    if (str == NULL || len < 2) return;
    size_t actual_len = 0;
    while (actual_len < len && str[actual_len] != '\0') {
        actual_len++;
    }
    for (size_t i = 0; i < actual_len / 2; i++) {
        char temp = str[i];
        str[i] = str[actual_len - 1 - i];
        str[actual_len - 1 - i] = temp;
    }
}

void remove_vowels(char *str, size_t len) {
    if (str == NULL || len == 0) return;
    size_t write_pos = 0;
    for (size_t i = 0; i < len && str[i] != '\0'; i++) {
        char c = (char)tolower((unsigned char)str[i]);
        if (c != 'a' && c != 'e' && c != 'i' && c != 'o' && c != 'u') {
            str[write_pos++] = str[i];
        }
    }
    if (write_pos < len) {
        str[write_pos] = '\0';
    }
}

int process_string(char *input, size_t max_len, string_processor processor) {
    if (input == NULL || processor == NULL || max_len == 0) return 0;
    size_t len = strnlen(input, max_len);
    if (len == 0 || len >= max_len) return 0;
    processor(input, len);
    return 1;
}

int main() {
    char input[256];
    string_processor processors[] = {
        to_uppercase,
        to_lowercase,
        reverse_string,
        remove_vowels
    };
    const char *names[] = {
        "UPPERCASE",
        "LOWERCASE",
        "REVERSE",
        "REMOVE VOWELS"
    };
    int num_processors = sizeof(processors) / sizeof(processors[0]);
    
    printf("Enter a string (max 255 chars): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strnlen(input, sizeof(input));
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    
    if (len == 0) {
        printf("Empty string entered\n");
        return 1;
    }
    
    printf("\nOriginal: %s\n\n", input);
    
    for (int i = 0; i < num_processors; i++) {
        char buffer[256];
        if (strlen(input) >= sizeof(buffer)) {
            printf("Input too long for processing\n");
            return 1;
        }
        strncpy(buffer, input, sizeof(buffer) - 1);
        buffer[sizeof(buffer) - 1] = '\0';
        
        if (process_string(buffer, sizeof(buffer), processors[i])) {
            printf("%s: %s\n", names[i], buffer);
        } else {
            printf("Failed to process with %s\n", names[i]);
        }
    }
    
    return 0;
}