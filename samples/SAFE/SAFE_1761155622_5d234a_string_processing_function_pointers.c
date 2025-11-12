//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: string_processing
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
        if (str[end] == '\0') end--;
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        start++;
        end--;
    }
}

void remove_whitespace(char *str, int len) {
    if (str == NULL) return;
    int write_pos = 0;
    for (int i = 0; i < len && str[i] != '\0'; i++) {
        if (!isspace((unsigned char)str[i])) {
            str[write_pos++] = str[i];
        }
    }
    if (write_pos < len) {
        str[write_pos] = '\0';
    }
}

int safe_strlen(const char *str, int max_len) {
    if (str == NULL) return 0;
    int len = 0;
    while (len < max_len && str[len] != '\0') {
        len++;
    }
    return len;
}

void process_string(char *input, int max_len, string_operation_t operation) {
    if (input == NULL || operation == NULL) return;
    int actual_len = safe_strlen(input, max_len);
    if (actual_len == 0) return;
    operation(input, actual_len);
}

int main() {
    char input[256];
    int choice;
    
    printf("Enter a string (max 255 characters): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    int len = safe_strlen(input, sizeof(input));
    if (len > 0 && input[len-1] == '\n') {
        input[len-1] = '\0';
        len--;
    }
    
    if (len == 0) {
        printf("Empty string entered\n");
        return 1;
    }
    
    printf("Choose operation:\n");
    printf("1. Convert to uppercase\n");
    printf("2. Convert to lowercase\n");
    printf("3. Reverse string\n");
    printf("4. Remove whitespace\n");
    printf("Enter choice (1-4): ");
    
    if (scanf("%d", &choice) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    string_operation_t operations[] = {
        to_uppercase,
        to_lowercase,
        reverse_string,
        remove_whitespace
    };
    
    if (choice < 1 || choice > 4) {
        printf("Invalid choice\n");
        return 1;
    }
    
    char buffer[256];
    if (strlen(input) >= sizeof(buffer)) {
        printf("Input too long\n");
        return 1;
    }
    strcpy(buffer, input);
    
    process_string(buffer, sizeof(buffer), operations[choice-1]);
    
    printf("Original: %s\n", input);
    printf("Result: %s\n", buffer);
    
    return 0;
}