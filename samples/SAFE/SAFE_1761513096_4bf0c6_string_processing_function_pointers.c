//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

typedef char* (*string_processor)(char*);

char* to_uppercase(char* str) {
    if (str == NULL) return NULL;
    char* ptr = str;
    while (*ptr) {
        *ptr = toupper((unsigned char)*ptr);
        ptr++;
    }
    return str;
}

char* to_lowercase(char* str) {
    if (str == NULL) return NULL;
    char* ptr = str;
    while (*ptr) {
        *ptr = tolower((unsigned char)*ptr);
        ptr++;
    }
    return str;
}

char* reverse_string(char* str) {
    if (str == NULL) return NULL;
    size_t len = strlen(str);
    if (len == 0) return str;
    
    char* start = str;
    char* end = str + len - 1;
    
    while (start < end) {
        char temp = *start;
        *start = *end;
        *end = temp;
        start++;
        end--;
    }
    return str;
}

char* remove_whitespace(char* str) {
    if (str == NULL) return NULL;
    
    char* read_ptr = str;
    char* write_ptr = str;
    
    while (*read_ptr) {
        if (!isspace((unsigned char)*read_ptr)) {
            *write_ptr = *read_ptr;
            write_ptr++;
        }
        read_ptr++;
    }
    *write_ptr = '\0';
    return str;
}

void process_string(char* input, string_processor processor) {
    if (input == NULL || processor == NULL) {
        printf("Invalid input or processor\n");
        return;
    }
    
    char buffer[256];
    if (strlen(input) >= sizeof(buffer)) {
        printf("Input too long\n");
        return;
    }
    
    strcpy(buffer, input);
    char* result = processor(buffer);
    if (result != NULL) {
        printf("Result: %s\n", result);
    }
}

int main() {
    char input[256];
    string_processor processors[] = {
        to_uppercase,
        to_lowercase,
        reverse_string,
        remove_whitespace
    };
    
    const char* names[] = {
        "Uppercase",
        "Lowercase", 
        "Reversed",
        "No whitespace"
    };
    
    int num_processors = sizeof(processors) / sizeof(processors[0]);
    
    printf("Enter a string to process: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (strlen(input) == 0) {
        printf("Empty input string\n");
        return 1;
    }
    
    printf("\nAvailable transformations:\n");
    for (int i = 0; i < num_processors; i++) {
        printf("%d. %s\n", i + 1, names[i]);
    }
    
    printf("Choose transformation (1-%d): ", num_processors);
    
    int choice;
    if (scanf("%d", &choice) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (choice < 1 || choice > num_processors) {
        printf("Invalid choice\n");
        return 1;
    }
    
    process_string(input, processors[choice - 1]);
    
    return 0;
}