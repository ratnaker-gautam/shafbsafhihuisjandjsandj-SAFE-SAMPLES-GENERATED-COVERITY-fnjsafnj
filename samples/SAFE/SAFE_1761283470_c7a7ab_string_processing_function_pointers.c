//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>

typedef void (*string_operation_t)(char *str, size_t len);

void to_uppercase(char *str, size_t len) {
    if (str == NULL) return;
    for (size_t i = 0; i < len && str[i] != '\0'; i++) {
        str[i] = (char)toupper((unsigned char)str[i]);
    }
}

void to_lowercase(char *str, size_t len) {
    if (str == NULL) return;
    for (size_t i = 0; i < len && str[i] != '\0'; i++) {
        str[i] = (char)tolower((unsigned char)str[i]);
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
    if (write_pos < len) {
        str[write_pos] = '\0';
    }
}

int safe_strlen(const char *str, size_t max_len) {
    if (str == NULL) return 0;
    for (size_t i = 0; i < max_len; i++) {
        if (str[i] == '\0') return (int)i;
    }
    return (int)max_len;
}

void apply_operation(string_operation_t operation, char *str, size_t max_len) {
    if (operation == NULL || str == NULL) return;
    int len = safe_strlen(str, max_len);
    if (len > 0) {
        operation(str, (size_t)len);
    }
}

int main(void) {
    char input[256];
    string_operation_t operations[] = {
        to_uppercase,
        to_lowercase,
        reverse_string,
        remove_whitespace
    };
    const char *operation_names[] = {
        "Uppercase",
        "Lowercase", 
        "Reverse",
        "Remove whitespace"
    };
    const int num_operations = 4;
    
    printf("Enter a string (max 255 characters): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return 1;
    }
    
    int len = safe_strlen(input, sizeof(input));
    if (len > 0 && input[len-1] == '\n') {
        input[len-1] = '\0';
        len--;
    }
    
    if (len == 0) {
        printf("Empty string entered.\n");
        return 1;
    }
    
    printf("\nOriginal: '%s'\n\n", input);
    
    for (int i = 0; i < num_operations; i++) {
        char buffer[256];
        if (strlen(input) >= sizeof(buffer)) {
            printf("Input too long for processing.\n");
            return 1;
        }
        strncpy(buffer, input, sizeof(buffer));
        buffer[sizeof(buffer)-1] = '\0';
        
        apply_operation(operations[i], buffer, sizeof(buffer));
        printf("%s: '%s'\n", operation_names[i], buffer);
    }
    
    return 0;
}