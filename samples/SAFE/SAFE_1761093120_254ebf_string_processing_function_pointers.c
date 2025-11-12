//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: function_pointers ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

typedef void (*string_operation_t)(char *str, int len);

void to_uppercase(char *str, int len) {
    if (str == NULL) return;
    for (int i = 0; i < len && str[i] != '\0'; i++) {
        str[i] = toupper((unsigned char)str[i]);
    }
}

void to_lowercase(char *str, int len) {
    if (str == NULL) return;
    for (int i = 0; i < len && str[i] != '\0'; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

void reverse_string(char *str, int len) {
    if (str == NULL || len <= 1) return;
    int start = 0;
    int end = len - 1;
    while (start < end) {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        start++;
        end--;
    }
}

void remove_non_alnum(char *str, int len) {
    if (str == NULL) return;
    int write_pos = 0;
    for (int i = 0; i < len && str[i] != '\0'; i++) {
        if (isalnum((unsigned char)str[i])) {
            str[write_pos++] = str[i];
        }
    }
    if (write_pos < len) {
        str[write_pos] = '\0';
    }
}

int get_valid_choice(void) {
    int choice;
    char input[16];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return -1;
    }
    if (sscanf(input, "%d", &choice) != 1) {
        return -1;
    }
    return choice;
}

void process_string_with_function(string_operation_t operation, char *str, int max_len) {
    if (operation == NULL || str == NULL || max_len <= 0) {
        return;
    }
    int actual_len = strnlen(str, max_len);
    if (actual_len == 0) {
        return;
    }
    operation(str, actual_len);
}

int main(void) {
    char input_string[256];
    string_operation_t operations[] = {
        to_uppercase,
        to_lowercase,
        reverse_string,
        remove_non_alnum
    };
    const char *operation_names[] = {
        "Convert to uppercase",
        "Convert to lowercase", 
        "Reverse string",
        "Remove non-alphanumeric characters"
    };
    const int num_operations = 4;
    
    printf("Enter a string to process (max 255 characters): ");
    if (fgets(input_string, sizeof(input_string), stdin) == NULL) {
        printf("Error reading input.\n");
        return 1;
    }
    
    size_t input_len = strlen(input_string);
    if (input_len > 0 && input_string[input_len - 1] == '\n') {
        input_string[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        printf("Empty string provided.\n");
        return 1;
    }
    
    printf("\nAvailable operations:\n");
    for (int i = 0; i < num_operations; i++) {
        printf("%d. %s\n", i + 1, operation_names[i]);
    }
    
    printf("\nSelect operation (1-%d): ", num_operations);
    int choice = get_valid_choice();
    
    if (choice < 1 || choice > num_operations) {
        printf("Invalid choice.\n");
        return 1;
    }
    
    char working_string[256];
    strncpy(working_string, input_string, sizeof(working_string) - 1);
    working_string[sizeof(working_string) - 1] = '\0';
    
    process_string_with_function(operations[choice - 1], working_string, sizeof(working_string));
    
    printf("\nOriginal string: %s\n", input_string);
    printf("Processed string: %s\n", working_string);
    
    return 0;
}