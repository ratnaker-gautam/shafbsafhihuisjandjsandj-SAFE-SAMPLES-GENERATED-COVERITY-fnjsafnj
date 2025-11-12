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
    char buffer[256];
    int number;
    int valid_input = 0;
    
    printf("State Machine Demo\n");
    printf("Commands: number, text, exit\n");
    
    while (current_state != EXIT) {
        switch (current_state) {
            case INIT:
                printf("Enter command: ");
                if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
                    current_state = ERROR;
                    break;
                }
                buffer[strcspn(buffer, "\n")] = '\0';
                current_state = READ_COMMAND;
                break;
                
            case READ_COMMAND:
                if (strlen(buffer) == 0) {
                    printf("Empty input. Try again.\n");
                    current_state = INIT;
                    break;
                }
                
                for (size_t i = 0; i < strlen(buffer); i++) {
                    buffer[i] = tolower(buffer[i]);
                }
                
                if (strcmp(buffer, "number") == 0) {
                    current_state = PROCESS_NUMBER;
                } else if (strcmp(buffer, "text") == 0) {
                    current_state = PROCESS_TEXT;
                } else if (strcmp(buffer, "exit") == 0) {
                    current_state = EXIT;
                } else {
                    printf("Unknown command: %s\n", buffer);
                    current_state = INIT;
                }
                break;
                
            case PROCESS_NUMBER:
                printf("Enter a number (0-100): ");
                if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
                    current_state = ERROR;
                    break;
                }
                
                if (sscanf(buffer, "%d", &number) != 1) {
                    printf("Invalid number format.\n");
                    current_state = INIT;
                    break;
                }
                
                if (number < 0 || number > 100) {
                    printf("Number out of range (0-100).\n");
                    current_state = INIT;
                    break;
                }
                
                printf("Number squared: %d\n", number * number);
                current_state = INIT;
                break;
                
            case PROCESS_TEXT:
                printf("Enter text (max 255 chars): ");
                if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
                    current_state = ERROR;
                    break;
                }
                
                buffer[strcspn(buffer, "\n")] = '\0';
                
                if (strlen(buffer) == 0) {
                    printf("Empty text input.\n");
                    current_state = INIT;
                    break;
                }
                
                printf("Text length: %zu\n", strlen(buffer));
                printf("Text in uppercase: ");
                for (size_t i = 0; i < strlen(buffer); i++) {
                    if (i < sizeof(buffer) - 1) {
                        putchar(toupper(buffer[i]));
                    }
                }
                printf("\n");
                current_state = INIT;
                break;
                
            case ERROR:
                printf("Input error occurred.\n");
                current_state = INIT;
                break;
                
            default:
                current_state = ERROR;
                break;
        }
    }
    
    printf("Goodbye!\n");
    return 0;
}