//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: monolithic_main ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum state {
    INIT,
    READ_COMMAND,
    PROCESS_NUMBER,
    PROCESS_TEXT,
    EXIT,
    ERROR
};

int main(void) {
    enum state current_state = INIT;
    char buffer[256];
    int number;
    int valid_input = 0;
    
    printf("State Machine Demo - Enter commands (number, text, quit):\n");
    
    while (current_state != EXIT) {
        switch (current_state) {
            case INIT:
                printf("> ");
                current_state = READ_COMMAND;
                break;
                
            case READ_COMMAND:
                if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
                    current_state = EXIT;
                    break;
                }
                
                size_t len = strlen(buffer);
                if (len > 0 && buffer[len - 1] == '\n') {
                    buffer[len - 1] = '\0';
                    len--;
                }
                
                if (len == 0) {
                    current_state = INIT;
                    break;
                }
                
                if (strcmp(buffer, "quit") == 0) {
                    current_state = EXIT;
                } else if (strcmp(buffer, "number") == 0) {
                    current_state = PROCESS_NUMBER;
                } else if (strcmp(buffer, "text") == 0) {
                    current_state = PROCESS_TEXT;
                } else {
                    printf("Unknown command. Use 'number', 'text', or 'quit'\n");
                    current_state = INIT;
                }
                break;
                
            case PROCESS_NUMBER:
                printf("Enter a number: ");
                if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
                    current_state = EXIT;
                    break;
                }
                
                if (sscanf(buffer, "%d", &number) == 1) {
                    printf("You entered: %d\n", number);
                    if (number > 0) {
                        printf("Positive number\n");
                    } else if (number < 0) {
                        printf("Negative number\n");
                    } else {
                        printf("Zero\n");
                    }
                    valid_input = 1;
                } else {
                    printf("Invalid number format\n");
                    valid_input = 0;
                }
                
                current_state = valid_input ? INIT : PROCESS_NUMBER;
                break;
                
            case PROCESS_TEXT:
                printf("Enter text: ");
                if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
                    current_state = EXIT;
                    break;
                }
                
                size_t text_len = strlen(buffer);
                if (text_len > 0 && buffer[text_len - 1] == '\n') {
                    buffer[text_len - 1] = '\0';
                    text_len--;
                }
                
                if (text_len > 0) {
                    printf("Text length: %zu\n", text_len);
                    
                    int has_alpha = 0;
                    int has_digit = 0;
                    for (size_t i = 0; i < text_len; i++) {
                        if (isalpha((unsigned char)buffer[i])) has_alpha = 1;
                        if (isdigit((unsigned char)buffer[i])) has_digit = 1;
                    }
                    
                    if (has_alpha && has_digit) {
                        printf("Mixed alphanumeric text\n");
                    } else if (has_alpha) {
                        printf("Alphabetic text\n");
                    } else if (has_digit) {
                        printf("Numeric text\n");
                    } else {
                        printf("Other characters\n");
                    }
                    valid_input = 1;
                } else {
                    printf("Empty text input\n");
                    valid_input = 0;
                }
                
                current_state = valid_input ? INIT : PROCESS_TEXT;
                break;
                
            case ERROR:
                printf("An error occurred\n");
                current_state = INIT;
                break;
                
            case EXIT:
                break;
        }
    }
    
    printf("Goodbye!\n");
    return 0;
}