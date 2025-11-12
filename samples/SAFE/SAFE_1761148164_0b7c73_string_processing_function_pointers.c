//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

typedef void (*string_operation_t)(char *str, int *result);

void string_length(char *str, int *result) {
    if (str == NULL || result == NULL) return;
    *result = 0;
    while (*str != '\0') {
        (*result)++;
        str++;
    }
}

void count_uppercase(char *str, int *result) {
    if (str == NULL || result == NULL) return;
    *result = 0;
    while (*str != '\0') {
        if (isupper((unsigned char)*str)) {
            (*result)++;
        }
        str++;
    }
}

void count_digits(char *str, int *result) {
    if (str == NULL || result == NULL) return;
    *result = 0;
    while (*str != '\0') {
        if (isdigit((unsigned char)*str)) {
            (*result)++;
        }
        str++;
    }
}

void reverse_string(char *str, int *result) {
    if (str == NULL || result == NULL) return;
    int len = 0;
    char *temp = str;
    while (*temp != '\0') {
        len++;
        temp++;
    }
    
    int i = 0;
    int j = len - 1;
    while (i < j) {
        char tmp = str[i];
        str[i] = str[j];
        str[j] = tmp;
        i++;
        j--;
    }
    *result = len;
}

void process_string(char *str, string_operation_t operation, const char *operation_name) {
    if (str == NULL || operation == NULL || operation_name == NULL) return;
    
    int result = 0;
    char buffer[256];
    
    if (strlen(str) >= 255) {
        printf("Input string too long\n");
        return;
    }
    
    strncpy(buffer, str, 255);
    buffer[255] = '\0';
    
    if (operation == reverse_string) {
        operation(buffer, &result);
        printf("%s: %s (length: %d)\n", operation_name, buffer, result);
    } else {
        operation(str, &result);
        printf("%s: %d\n", operation_name, result);
    }
}

int main() {
    char input[256];
    string_operation_t operations[] = {
        string_length,
        count_uppercase,
        count_digits,
        reverse_string
    };
    
    const char *operation_names[] = {
        "String length",
        "Uppercase count",
        "Digit count",
        "Reversed string"
    };
    
    int num_operations = sizeof(operations) / sizeof(operations[0]);
    
    printf("Enter a string (max 255 characters): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (strlen(input) == 0) {
        printf("Empty string provided\n");
        return 1;
    }
    
    printf("\nProcessing results:\n");
    for (int i = 0; i < num_operations; i++) {
        process_string(input, operations[i], operation_names[i]);
    }
    
    return 0;
}