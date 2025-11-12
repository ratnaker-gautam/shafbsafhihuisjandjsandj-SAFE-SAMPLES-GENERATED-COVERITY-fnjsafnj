//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>

typedef void (*string_processor)(char *str, size_t len);

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
    if (write_pos < len) {
        str[write_pos] = '\0';
    }
}

int get_valid_choice(void) {
    int choice;
    char buffer[256];
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return -1;
    }
    if (sscanf(buffer, "%d", &choice) != 1) {
        return -1;
    }
    return choice;
}

int main(void) {
    string_processor processors[] = {
        to_uppercase,
        to_lowercase,
        reverse_string,
        remove_non_alnum
    };
    
    const char *names[] = {
        "Convert to uppercase",
        "Convert to lowercase", 
        "Reverse string",
        "Remove non-alphanumeric characters"
    };
    
    const size_t num_processors = sizeof(processors) / sizeof(processors[0]);
    char input[1024];
    
    printf("Enter a string to process (max %zu characters): ", sizeof(input) - 1);
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    
    if (len == 0) {
        printf("Empty string entered.\n");
        return 1;
    }
    
    printf("\nAvailable string processors:\n");
    for (size_t i = 0; i < num_processors; i++) {
        printf("%zu. %s\n", i + 1, names[i]);
    }
    
    printf("Select a processor (1-%zu): ", num_processors);
    int choice = get_valid_choice();
    
    if (choice < 1 || choice > (int)num_processors) {
        printf("Invalid choice.\n");
        return 1;
    }
    
    char working_copy[sizeof(input)];
    if (len >= sizeof(working_copy)) {
        printf("String too long for processing.\n");
        return 1;
    }
    
    strncpy(working_copy, input, sizeof(working_copy) - 1);
    working_copy[sizeof(working_copy) - 1] = '\0';
    
    size_t working_len = strlen(working_copy);
    processors[choice - 1](working_copy, working_len);
    
    printf("\nOriginal: %s\n", input);
    printf("Processed: %s\n", working_copy);
    
    return 0;
}