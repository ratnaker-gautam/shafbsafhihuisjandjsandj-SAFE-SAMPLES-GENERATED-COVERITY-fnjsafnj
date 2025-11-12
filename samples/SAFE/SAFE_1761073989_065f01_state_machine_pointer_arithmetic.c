//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: pointer_arithmetic ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT 256
#define STATE_COUNT 5

typedef enum {
    START,
    READING_DIGITS,
    READING_LETTERS,
    ERROR,
    DONE
} State;

int is_valid_char(char c) {
    return isalnum((unsigned char)c) || c == ' ' || c == '\n';
}

void process_input(const char *input, char *output) {
    State current_state = START;
    const char *input_ptr = input;
    char *output_ptr = output;
    
    while (current_state != DONE && current_state != ERROR) {
        char current_char = *input_ptr;
        
        if (current_char == '\0') {
            current_state = DONE;
            continue;
        }
        
        if (!is_valid_char(current_char)) {
            current_state = ERROR;
            continue;
        }
        
        switch (current_state) {
            case START:
                if (isdigit((unsigned char)current_char)) {
                    current_state = READING_DIGITS;
                    *output_ptr = current_char;
                    output_ptr++;
                } else if (isalpha((unsigned char)current_char)) {
                    current_state = READING_LETTERS;
                    *output_ptr = current_char;
                    output_ptr++;
                }
                break;
                
            case READING_DIGITS:
                if (isdigit((unsigned char)current_char)) {
                    *output_ptr = current_char;
                    output_ptr++;
                } else if (isalpha((unsigned char)current_char)) {
                    current_state = ERROR;
                } else if (current_char == ' ' || current_char == '\n') {
                    current_state = START;
                }
                break;
                
            case READING_LETTERS:
                if (isalpha((unsigned char)current_char)) {
                    *output_ptr = current_char;
                    output_ptr++;
                } else if (isdigit((unsigned char)current_char)) {
                    current_state = ERROR;
                } else if (current_char == ' ' || current_char == '\n') {
                    current_state = START;
                }
                break;
                
            default:
                break;
        }
        
        input_ptr++;
    }
    
    *output_ptr = '\0';
    
    if (current_state == ERROR) {
        output_ptr = output;
        *output_ptr = '\0';
    }
}

int main(void) {
    char input_buffer[MAX_INPUT + 1];
    char output_buffer[MAX_INPUT + 1];
    
    printf("Enter text (digits and letters separated by spaces): ");
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input_buffer);
    if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
        input_buffer[input_len - 1] = '\0';
    }
    
    if (strlen(input_buffer) == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    process_input(input_buffer, output_buffer);
    
    if (strlen(output_buffer) == 0) {
        printf("Invalid input format\n");
        return 1;
    }
    
    printf("Processed: %s\n", output_buffer);
    
    return 0;
}