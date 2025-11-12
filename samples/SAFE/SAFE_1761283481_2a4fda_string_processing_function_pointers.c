//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

typedef char* (*string_operation)(char*);

char* to_uppercase(char* str) {
    if (str == NULL) return NULL;
    char* result = malloc(strlen(str) + 1);
    if (result == NULL) return NULL;
    strcpy(result, str);
    for (char* p = result; *p; p++) {
        *p = toupper((unsigned char)*p);
    }
    return result;
}

char* to_lowercase(char* str) {
    if (str == NULL) return NULL;
    char* result = malloc(strlen(str) + 1);
    if (result == NULL) return NULL;
    strcpy(result, str);
    for (char* p = result; *p; p++) {
        *p = tolower((unsigned char)*p);
    }
    return result;
}

char* reverse_string(char* str) {
    if (str == NULL) return NULL;
    size_t len = strlen(str);
    char* result = malloc(len + 1);
    if (result == NULL) return NULL;
    strcpy(result, str);
    for (size_t i = 0; i < len / 2; i++) {
        char temp = result[i];
        result[i] = result[len - 1 - i];
        result[len - 1 - i] = temp;
    }
    return result;
}

char* remove_whitespace(char* str) {
    if (str == NULL) return NULL;
    size_t len = strlen(str);
    char* result = malloc(len + 1);
    if (result == NULL) return NULL;
    size_t j = 0;
    for (size_t i = 0; i < len; i++) {
        if (!isspace((unsigned char)str[i])) {
            result[j++] = str[i];
        }
    }
    result[j] = '\0';
    return result;
}

void process_string(char* input, string_operation op) {
    if (input == NULL || op == NULL) {
        printf("Invalid input or operation\n");
        return;
    }
    char* result = op(input);
    if (result == NULL) {
        printf("Memory allocation failed\n");
        return;
    }
    printf("Result: %s\n", result);
    free(result);
}

int main() {
    char input[256];
    printf("Enter a string (max 255 characters): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    input[strcspn(input, "\n")] = '\0';
    if (strlen(input) == 0) {
        printf("Empty string entered\n");
        return 1;
    }
    printf("Original: %s\n", input);
    printf("Choose operation:\n");
    printf("1. Uppercase\n");
    printf("2. Lowercase\n");
    printf("3. Reverse\n");
    printf("4. Remove whitespace\n");
    int choice;
    if (scanf("%d", &choice) != 1) {
        printf("Invalid choice\n");
        return 1;
    }
    while (getchar() != '\n');
    string_operation operations[] = {to_uppercase, to_lowercase, reverse_string, remove_whitespace};
    if (choice < 1 || choice > 4) {
        printf("Invalid choice\n");
        return 1;
    }
    process_string(input, operations[choice - 1]);
    return 0;
}