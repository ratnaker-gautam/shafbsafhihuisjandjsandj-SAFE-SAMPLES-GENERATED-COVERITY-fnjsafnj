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
                input_buffer[strcspn(input_buffer, "\n")] = '\0';
                if (strlen(input_buffer) == 0) {
                    current_state = INIT;
                    break;
                }
                current_state = READ_COMMAND;
                break;
                
            case READ_COMMAND:
                if (strcmp(input_buffer, "exit") == 0) {
                    current_state = EXIT;
                } else if (strcmp(input_buffer, "number") == 0) {
                    current_state = PROCESS_NUMBER;
                } else if (strcmp(input_buffer, "text") == 0) {
                    current_state = PROCESS_TEXT;
                } else {
                    printf("Unknown command: %s\n", input_buffer);
                    current_state = INIT;
                }
                break;
                
            case PROCESS_NUMBER:
                printf("Enter a number (0-100): ");
                if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
                    current_state = ERROR;
                    break;
                }
                input_buffer[strcspn(input_buffer, "\n")] = '\0';
                
                valid_input = 1;
                for (size_t i = 0; i < strlen(input_buffer); i++) {
                    if (!isdigit(input_buffer[i])) {
                        valid_input = 0;
                        break;
                    }
                }
                
                if (valid_input && strlen(input_buffer) > 0) {
                    number_value = atoi(input_buffer);
                    if (number_value >= 0 && number_value <= 100) {
                        printf("Valid number: %d\n", number_value);
                    } else {
                        printf("Number out of range: %d\n", number_value);
                    }
                } else {
                    printf("Invalid number format: %s\n", input_buffer);
                }
                current_state = INIT;
                break;
                
            case PROCESS_TEXT:
                printf("Enter text (max 50 chars): ");
                if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
                    current_state = ERROR;
                    break;
                }
                input_buffer[strcspn(input_buffer, "\n")] = '\0';
                
                if (strlen(input_buffer) > 50) {
                    printf("Text too long. Truncated to: ");
                    input_buffer[50] = '\0';
                }
                printf("Text processed: %s\n", input_buffer);
                current_state = INIT;
                break;
                
            case ERROR:
                printf("Input error occurred\n");
                current_state = EXIT;
                break;
                
            case EXIT:
                break;
        }
    }
    
    printf("Goodbye!\n");
    return 0;
}