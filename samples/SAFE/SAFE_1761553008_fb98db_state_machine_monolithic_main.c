//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum State {
    INIT,
    READ_COMMAND,
    PROCESS_NUMBER,
    PROCESS_TEXT,
    EXIT,
    ERROR
};

int main(void) {
    enum State current_state = INIT;
    char input_buffer[256];
    int number_value = 0;
    int valid_input = 1;
    
    printf("State Machine Demo - Enter commands (number, text, exit):\n");
    
    while (current_state != EXIT) {
        switch (current_state) {
            case INIT:
                printf("> ");
                if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
                    current_state = ERROR;
                    break;
                }
                
                size_t len = strlen(input_buffer);
                if (len > 0 && input_buffer[len - 1] == '\n') {
                    input_buffer[len - 1] = '\0';
                    len--;
                }
                
                if (len == 0) {
                    current_state = INIT;
                    break;
                }
                
                if (strcmp(input_buffer, "exit") == 0) {
                    current_state = EXIT;
                } else if (strncmp(input_buffer, "number", 6) == 0) {
                    current_state = READ_COMMAND;
                } else if (strncmp(input_buffer, "text", 4) == 0) {
                    current_state = PROCESS_TEXT;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READ_COMMAND:
                printf("Enter a number: ");
                if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
                    current_state = ERROR;
                    break;
                }
                
                char *endptr;
                number_value = strtol(input_buffer, &endptr, 10);
                if (endptr == input_buffer || *endptr != '\n') {
                    current_state = ERROR;
                } else if (number_value < -1000 || number_value > 1000) {
                    current_state = ERROR;
                } else {
                    current_state = PROCESS_NUMBER;
                }
                break;
                
            case PROCESS_NUMBER:
                printf("Processed number: %d\n", number_value);
                if (number_value > 0) {
                    printf("Number is positive\n");
                } else if (number_value < 0) {
                    printf("Number is negative\n");
                } else {
                    printf("Number is zero\n");
                }
                current_state = INIT;
                break;
                
            case PROCESS_TEXT:
                printf("Enter text: ");
                if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
                    current_state = ERROR;
                    break;
                }
                
                size_t text_len = strlen(input_buffer);
                if (text_len > 0 && input_buffer[text_len - 1] == '\n') {
                    input_buffer[text_len - 1] = '\0';
                    text_len--;
                }
                
                if (text_len == 0) {
                    current_state = ERROR;
                    break;
                }
                
                int has_alpha = 0;
                int has_digit = 0;
                for (size_t i = 0; i < text_len; i++) {
                    if (isalpha((unsigned char)input_buffer[i])) {
                        has_alpha = 1;
                    }
                    if (isdigit((unsigned char)input_buffer[i])) {
                        has_digit = 1;
                    }
                }
                
                printf("Text analysis: ");
                if (has_alpha && has_digit) {
                    printf("Mixed alphanumeric\n");
                } else if (has_alpha) {
                    printf("Alphabetic only\n");
                } else if (has_digit) {
                    printf("Numeric only\n");
                } else {
                    printf("Other characters\n");
                }
                current_state = INIT;
                break;
                
            case ERROR:
                printf("Error: Invalid input or command\n");
                current_state = INIT;
                break;
                
            case EXIT:
                break;
        }
    }
    
    printf("Goodbye!\n");
    return 0;
}