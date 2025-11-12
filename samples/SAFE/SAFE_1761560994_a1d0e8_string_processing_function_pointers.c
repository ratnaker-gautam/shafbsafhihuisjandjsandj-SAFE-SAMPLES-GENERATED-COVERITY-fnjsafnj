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
    if (str == NULL) return;
    int actual_len = 0;
    while (actual_len < len && str[actual_len] != '\0') {
        actual_len++;
    }
    for (int i = 0; i < actual_len / 2; i++) {
        char temp = str[i];
        str[i] = str[actual_len - 1 - i];
        str[actual_len - 1 - i] = temp;
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

int get_valid_choice(void) {
    int choice;
    char input[16];
    
    printf("Choose operation:\n");
    printf("1. Convert to uppercase\n");
    printf("2. Convert to lowercase\n");
    printf("3. Reverse string\n");
    printf("4. Remove whitespace\n");
    printf("Enter choice (1-4): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return -1;
    }
    
    if (sscanf(input, "%d", &choice) != 1) {
        return -1;
    }
    
    if (choice < 1 || choice > 4) {
        return -1;
    }
    
    return choice;
}

int get_input_string(char *buffer, int max_len) {
    if (buffer == NULL || max_len <= 0) {
        return 0;
    }
    
    printf("Enter a string (max %d characters): ", max_len - 1);
    
    if (fgets(buffer, max_len, stdin) == NULL) {
        return 0;
    }
    
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
    } else if (len == max_len - 1) {
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
    }
    
    return 1;
}

int main(void) {
    const int MAX_LENGTH = 256;
    char input_string[MAX_LENGTH];
    string_operation_t operations[] = {
        to_uppercase,
        to_lowercase,
        reverse_string,
        remove_whitespace
    };
    
    if (!get_input_string(input_string, MAX_LENGTH)) {
        fprintf(stderr, "Error reading input string\n");
        return 1;
    }
    
    int choice = get_valid_choice();
    if (choice == -1) {
        fprintf(stderr, "Invalid choice\n");
        return 1;
    }
    
    int input_len = strlen(input_string);
    if (input_len >= MAX_LENGTH) {
        fprintf(stderr, "Input string too long\n");
        return 1;
    }
    
    operations[choice - 1](input_string, MAX_LENGTH);
    
    printf("Result: %s\n", input_string);
    
    return 0;
}