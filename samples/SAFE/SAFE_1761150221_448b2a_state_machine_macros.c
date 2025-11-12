//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define STATE_IDLE 0
#define STATE_READING 1
#define STATE_PROCESSING 2
#define STATE_WRITING 3
#define STATE_ERROR 4

#define MAX_INPUT_LEN 100
#define MAX_BUFFER_SIZE 50

#define TRANSITION(new_state) do { current_state = new_state; } while(0)
#define IS_VALID_CHAR(c) ((c) >= 'a' && (c) <= 'z')
#define TO_UPPER(c) ((c) - 32)

int main(void) {
    int current_state = STATE_IDLE;
    char input_buffer[MAX_INPUT_LEN + 1] = {0};
    char process_buffer[MAX_BUFFER_SIZE + 1] = {0};
    size_t input_pos = 0;
    size_t process_pos = 0;
    char ch;
    
    printf("Enter text (a-z only, max %d chars): ", MAX_INPUT_LEN);
    
    while (current_state != STATE_ERROR && current_state != STATE_IDLE) {
        switch (current_state) {
            case STATE_IDLE:
                TRANSITION(STATE_READING);
                break;
                
            case STATE_READING:
                if (fgets(input_buffer, sizeof(input_buffer), stdin) != NULL) {
                    size_t len = strlen(input_buffer);
                    if (len > 0 && input_buffer[len - 1] == '\n') {
                        input_buffer[len - 1] = '\0';
                        len--;
                    }
                    
                    if (len > MAX_INPUT_LEN) {
                        printf("Input too long\n");
                        TRANSITION(STATE_ERROR);
                    } else {
                        input_pos = 0;
                        process_pos = 0;
                        TRANSITION(STATE_PROCESSING);
                    }
                } else {
                    printf("Read error\n");
                    TRANSITION(STATE_ERROR);
                }
                break;
                
            case STATE_PROCESSING:
                if (input_pos < strlen(input_buffer)) {
                    ch = input_buffer[input_pos];
                    if (!IS_VALID_CHAR(ch)) {
                        printf("Invalid character: %c\n", ch);
                        TRANSITION(STATE_ERROR);
                        break;
                    }
                    
                    if (process_pos < MAX_BUFFER_SIZE) {
                        process_buffer[process_pos++] = TO_UPPER(ch);
                        input_pos++;
                    } else {
                        printf("Processing buffer full\n");
                        TRANSITION(STATE_ERROR);
                    }
                } else {
                    process_buffer[process_pos] = '\0';
                    TRANSITION(STATE_WRITING);
                }
                break;
                
            case STATE_WRITING:
                printf("Processed: %s\n", process_buffer);
                printf("Continue? (y/n): ");
                
                if (fgets(input_buffer, sizeof(input_buffer), stdin) != NULL) {
                    if (input_buffer[0] == 'y' || input_buffer[0] == 'Y') {
                        memset(input_buffer, 0, sizeof(input_buffer));
                        memset(process_buffer, 0, sizeof(process_buffer));
                        TRANSITION(STATE_IDLE);
                    } else {
                        printf("Exiting\n");
                        return 0;
                    }
                } else {
                    printf("Read error\n");
                    TRANSITION(STATE_ERROR);
                }
                break;
                
            case STATE_ERROR:
                printf("Error state reached. Resetting.\n");
                memset(input_buffer, 0, sizeof(input_buffer));
                memset(process_buffer, 0, sizeof(process_buffer));
                TRANSITION(STATE_IDLE);
                break;
        }
    }
    
    return 0;
}