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
    size_t start = 0;
    size_t end = len - 1;
    while (start < end) {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        start++;
        end--;
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
    if (write_pos < len) {
        str[write_pos] = '\0';
    }
}

int get_valid_choice(void) {
    int choice;
    char buffer[100];
    
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return -1;
    }
    
    if (sscanf(buffer, "%d", &choice) != 1) {
        return -1;
    }
    
    if (choice < 1 || choice > 4) {
        return -1;
    }
    
    return choice;
}

int get_input_string(char *buffer, size_t size) {
    if (buffer == NULL || size == 0) {
        return 0;
    }
    
    printf("Enter a string (max %zu characters): ", size - 1);
    
    if (fgets(buffer, size, stdin) == NULL) {
        return 0;
    }
    
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
        len--;
    }
    
    if (len == 0) {
        return 0;
    }
    
    return 1;
}

int main(void) {
    char input[256];
    char working_buffer[256];
    string_operation_t operations[] = {
        to_uppercase,
        to_lowercase,
        reverse_string,
        remove_whitespace
    };
    
    const char *operation_names[] = {
        "Convert to uppercase",
        "Convert to lowercase", 
        "Reverse string",
        "Remove whitespace"
    };
    
    printf("String Processing Tool\n");
    printf("=====================\n");
    
    if (!get_input_string(input, sizeof(input))) {
        printf("Error: Invalid input or empty string.\n");
        return 1;
    }
    
    printf("\nAvailable operations:\n");
    for (int i = 0; i < 4; i++) {
        printf("%d. %s\n", i + 1, operation_names[i]);
    }
    
    printf("\nSelect operation (1-4): ");
    int choice = get_valid_choice();
    
    if (choice == -1) {
        printf("Error: Invalid choice.\n");
        return 1;
    }
    
    strncpy(working_buffer, input, sizeof(working_buffer) - 1);
    working_buffer[sizeof(working_buffer) - 1] = '\0';
    
    size_t len = strlen(working_buffer);
    if (len >= sizeof(working_buffer)) {
        len = sizeof(working_buffer) - 1;
    }
    
    operations[choice - 1](working_buffer, len);
    
    printf("\nOriginal: %s\n", input);
    printf("Result:   %s\n", working_buffer);
    
    return 0;
}