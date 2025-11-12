//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>

typedef void (*string_processor)(char *str);

void to_uppercase(char *str) {
    if (str == NULL) return;
    for (size_t i = 0; str[i] != '\0'; i++) {
        str[i] = (char)toupper((unsigned char)str[i]);
    }
}

void to_lowercase(char *str) {
    if (str == NULL) return;
    for (size_t i = 0; str[i] != '\0'; i++) {
        str[i] = (char)tolower((unsigned char)str[i]);
    }
}

void reverse_string(char *str) {
    if (str == NULL) return;
    size_t len = strlen(str);
    for (size_t i = 0; i < len / 2; i++) {
        char temp = str[i];
        str[i] = str[len - i - 1];
        str[len - i - 1] = temp;
    }
}

void remove_vowels(char *str) {
    if (str == NULL) return;
    size_t write_pos = 0;
    for (size_t i = 0; str[i] != '\0'; i++) {
        char c = (char)tolower((unsigned char)str[i]);
        if (c != 'a' && c != 'e' && c != 'i' && c != 'o' && c != 'u') {
            str[write_pos++] = str[i];
        }
    }
    str[write_pos] = '\0';
}

int process_string(char *input, string_processor processor) {
    if (input == NULL || processor == NULL) return 0;
    size_t len = strlen(input);
    if (len == 0 || len > 1023) return 0;
    processor(input);
    return 1;
}

int main() {
    char input[1024];
    string_processor processors[] = {to_uppercase, to_lowercase, reverse_string, remove_vowels};
    const char *names[] = {"UPPERCASE", "lowercase", "REVERSED", "NO VOWELS"};
    
    printf("Enter a string (max 1023 chars): ");
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
        printf("Empty string entered\n");
        return 1;
    }
    
    printf("\nOriginal: %s\n", input);
    printf("Processing results:\n");
    
    for (int i = 0; i < 4; i++) {
        char buffer[1024];
        if (strlen(input) >= sizeof(buffer)) {
            printf("Input too long for processing\n");
            return 1;
        }
        strcpy(buffer, input);
        
        if (process_string(buffer, processors[i])) {
            printf("%s: %s\n", names[i], buffer);
        } else {
            printf("Failed to process with %s\n", names[i]);
        }
    }
    
    return 0;
}