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

void remove_whitespace(char *str, size_t len) {
    if (str == NULL) return;
    size_t write_pos = 0;
    for (size_t i = 0; i < len && str[i] != '\0'; i++) {
        if (!isspace((unsigned char)str[i])) {
            str[write_pos] = str[i];
            write_pos++;
        }
    }
    str[write_pos] = '\0';
}

int validate_input(const char *input, size_t max_len) {
    if (input == NULL) return 0;
    size_t len = strlen(input);
    if (len == 0 || len >= max_len) return 0;
    for (size_t i = 0; i < len; i++) {
        if (!isprint((unsigned char)input[i])) return 0;
    }
    return 1;
}

void process_string(char *str, string_operation_t operation) {
    if (str == NULL || operation == NULL) return;
    size_t len = strlen(str);
    if (len == 0) return;
    operation(str, len);
}

int main(void) {
    char input[256];
    char buffer[256];
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
    
    if (!validate_input(input, sizeof(buffer))) {
        printf("Invalid input\n");
        return 1;
    }
    
    printf("Choose operation:\n");
    printf("1. Convert to uppercase\n");
    printf("2. Convert to lowercase\n");
    printf("3. Reverse string\n");
    printf("4. Remove whitespace\n");
    printf("Enter choice (1-4): ");
    
    if (scanf("%d", &choice) != 1) {
        printf("Invalid choice\n");
        return 1;
    }
    
    while (getchar() != '\n');
    
    strncpy(buffer, input, sizeof(buffer) - 1);
    buffer[sizeof(buffer) - 1] = '\0';
    
    string_operation_t operation = NULL;
    
    switch (choice) {
        case 1:
            operation = to_uppercase;
            break;
        case 2:
            operation = to_lowercase;
            break;
        case 3:
            operation = reverse_string;
            break;
        case 4:
            operation = remove_whitespace;
            break;
        default:
            printf("Invalid choice\n");
            return 1;
    }
    
    process_string(buffer, operation);
    
    printf("Original: %s\n", input);
    printf("Result: %s\n", buffer);
    
    return 0;
}