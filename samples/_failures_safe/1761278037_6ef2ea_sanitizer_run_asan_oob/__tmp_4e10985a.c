//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024

int validate_input(const char *input) {
    if (input == NULL) return 0;
    size_t len = strlen(input);
    if (len == 0 || len >= MAX_INPUT_LEN) return 0;
    return 1;
}

void process_string(char *str) {
    if (str == NULL) return;
    
    char *read_ptr = str;
    char *write_ptr = str;
    int word_count = 0;
    
    while (*read_ptr != '\0') {
        while (*read_ptr != '\0' && isspace(*read_ptr)) {
            read_ptr++;
        }
        
        if (*read_ptr == '\0') break;
        
        word_count++;
        char *word_start = read_ptr;
        
        while (*read_ptr != '\0' && !isspace(*read_ptr)) {
            read_ptr++;
        }
        
        size_t word_len = read_ptr - word_start;
        
        if (word_len > 0) {
            char *word_end = word_start + word_len - 1;
            while (word_end > word_start) {
                char temp = *word_start;
                *word_start = *word_end;
                *word_end = temp;
                word_start++;
                word_end--;
            }
            
            for (char *p = word_start - (word_len - 1); p < read_ptr; p++) {
                *write_ptr = *p;
                write_ptr++;
            }
        }
        
        if (*read_ptr != '\0') {
            *write_ptr = ' ';
            write_ptr++;
        }
    }
    
    *write_ptr = '\0';
    
    if (word_count > 0) {
        char *final_ptr = str;
        int char_count = 0;
        while (*final_ptr != '\0') {
            if (!isspace(*final_ptr)) {
                char_count++;
            }
            final_ptr++;
        }
        
        printf("Processed %d words containing %d non-space characters\n", 
               word_count, char_count);
    }
}

int main(void) {
    char input[MAX_INPUT_LEN];
    
    printf("Enter a string to process: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (!validate_input(input)) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    char *buffer = malloc(MAX_INPUT_LEN);
    if (buffer == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    
    strncpy(buffer, input, MAX_INPUT_LEN - 1);
    buffer[MAX_INPUT_LEN - 1] = '\0';
    
    process_string(buffer);
    
    printf("Result: %s\n", buffer);
    
    free(buffer);
    return 0;
}