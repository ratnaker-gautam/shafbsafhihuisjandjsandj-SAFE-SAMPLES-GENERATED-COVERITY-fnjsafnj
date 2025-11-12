//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>

typedef void (*string_operation_t)(char *str, size_t len);

void to_uppercase(char *str, size_t len) {
    if (str == NULL) return;
    for (size_t i = 0; i < len && str[i] != '\0'; i++) {
        str[i] = toupper((unsigned char)str[i]);
    }
}

void to_lowercase(char *str, size_t len) {
    if (str == NULL) return;
    for (size_t i = 0; i < len && str[i] != '\0'; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

void reverse_string(char *str, size_t len) {
    if (str == NULL || len == 0) return;
    size_t i = 0;
    size_t j = len - 1;
    while (i < j) {
        char temp = str[i];
        str[i] = str[j];
        str[j] = temp;
        i++;
        j--;
    }
}

void remove_non_alnum(char *str, size_t len) {
    if (str == NULL) return;
    size_t write_pos = 0;
    for (size_t i = 0; i < len && str[i] != '\0'; i++) {
        if (isalnum((unsigned char)str[i])) {
            str[write_pos++] = str[i];
        }
    }
    str[write_pos] = '\0';
}

int safe_strlen(const char *str, size_t max_len) {
    if (str == NULL) return 0;
    size_t len = 0;
    while (len < max_len && str[len] != '\0') {
        len++;
    }
    return len;
}

void process_string(char *input, size_t max_input_len, string_operation_t operation) {
    if (input == NULL || operation == NULL) return;
    size_t len = safe_strlen(input, max_input_len);
    if (len == 0 || len >= max_input_len) return;
    operation(input, len);
}

int main(void) {
    char input[256];
    int choice;
    
    printf("Enter a string (max 255 characters): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return 1;
    }
    
    size_t input_len = safe_strlen(input, sizeof(input));
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        printf("Empty string entered.\n");
        return 1;
    }
    
    printf("Choose operation:\n");
    printf("1. Convert to uppercase\n");
    printf("2. Convert to lowercase\n");
    printf("3. Reverse string\n");
    printf("4. Remove non-alphanumeric characters\n");
    printf("Enter choice (1-4): ");
    
    if (scanf("%d", &choice) != 1) {
        return 1;
    }
    
    string_operation_t operations[] = {
        to_uppercase,
        to_lowercase,
        reverse_string,
        remove_non_alnum
    };
    
    if (choice < 1 || choice > 4) {
        printf("Invalid choice.\n");
        return 1;
    }
    
    char working_copy[256];
    if (input_len >= sizeof(working_copy)) {
        return 1;
    }
    strncpy(working_copy, input, sizeof(working_copy) - 1);
    working_copy[sizeof(working_copy) - 1] = '\0';
    
    process_string(working_copy, sizeof(working_copy), operations[choice - 1]);
    
    printf("Original: %s\n", input);
    printf("Result: %s\n", working_copy);
    
    return 0;
}