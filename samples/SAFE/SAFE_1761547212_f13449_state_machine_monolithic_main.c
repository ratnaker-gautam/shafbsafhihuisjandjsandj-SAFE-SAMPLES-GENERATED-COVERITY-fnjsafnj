//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: state_machine
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
    int number = 0;
    int valid_input = 0;
    
    while (current_state != EXIT) {
        switch (current_state) {
            case INIT:
                printf("State Machine Demo\n");
                printf("Commands: number, text, quit\n");
                printf("Enter command: ");
                current_state = READ_COMMAND;
                break;
                
            case READ_COMMAND:
                if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
                    current_state = ERROR;
                    break;
                }
                
                buffer[strcspn(buffer, "\n")] = '\0';
                
                if (strlen(buffer) == 0) {
                    printf("Empty input. Try again.\n");
                    current_state = READ_COMMAND;
                } else if (strcmp(buffer, "quit") == 0) {
                    current_state = EXIT;
                } else if (strcmp(buffer, "number") == 0) {
                    printf("Enter a number (0-100): ");
                    current_state = PROCESS_NUMBER;
                } else if (strcmp(buffer, "text") == 0) {
                    printf("Enter text: ");
                    current_state = PROCESS_TEXT;
                } else {
                    printf("Unknown command: %s\n", buffer);
                    current_state = READ_COMMAND;
                }
                break;
                
            case PROCESS_NUMBER:
                if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
                    current_state = ERROR;
                    break;
                }
                
                buffer[strcspn(buffer, "\n")] = '\0';
                valid_input = 1;
                
                for (size_t i = 0; i < strlen(buffer); i++) {
                    if (!isdigit(buffer[i]) && !(i == 0 && buffer[i] == '-')) {
                        valid_input = 0;
                        break;
                    }
                }
                
                if (valid_input) {
                    long temp = strtol(buffer, NULL, 10);
                    if (temp >= 0 && temp <= 100) {
                        number = (int)temp;
                        printf("Number accepted: %d\n", number);
                        current_state = READ_COMMAND;
                    } else {
                        printf("Number out of range (0-100). Try again.\n");
                        current_state = PROCESS_NUMBER;
                    }
                } else {
                    printf("Invalid number format. Try again.\n");
                    current_state = PROCESS_NUMBER;
                }
                break;
                
            case PROCESS_TEXT:
                if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
                    current_state = ERROR;
                    break;
                }
                
                buffer[strcspn(buffer, "\n")] = '\0';
                
                if (strlen(buffer) > 0) {
                    printf("Text processed: %s\n", buffer);
                    current_state = READ_COMMAND;
                } else {
                    printf("Empty text. Try again.\n");
                    current_state = PROCESS_TEXT;
                }
                break;
                
            case ERROR:
                printf("Input error occurred.\n");
                current_state = EXIT;
                break;
                
            case EXIT:
                break;
        }
    }
    
    printf("Goodbye!\n");
    return 0;
}