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
    
    while (current_state != STATE_ERROR) {
        switch (current_state) {
            case STATE_IDLE:
                TRANSITION(STATE_READING);
                break;
                
            case STATE_READING:
                if (input_pos >= MAX_INPUT_LEN) {
                    TRANSITION(STATE_PROCESSING);
                    break;
                }
                
                int result = scanf("%c", &ch);
                if (result != 1) {
                    TRANSITION(STATE_ERROR);
                    break;
                }
                
                if (ch == '\n') {
                    TRANSITION(STATE_PROCESSING);
                    break;
                }
                
                if (!IS_VALID_CHAR(ch)) {
                    TRANSITION(STATE_ERROR);
                    break;
                }
                
                input_buffer[input_pos] = ch;
                input_pos++;
                break;
                
            case STATE_PROCESSING:
                if (input_pos == 0) {
                    TRANSITION(STATE_ERROR);
                    break;
                }
                
                for (size_t i = 0; i < input_pos && process_pos < MAX_BUFFER_SIZE; i++) {
                    if (IS_VALID_CHAR(input_buffer[i])) {
                        process_buffer[process_pos] = TO_UPPER(input_buffer[i]);
                        process_pos++;
                    }
                }
                
                if (process_pos == 0) {
                    TRANSITION(STATE_ERROR);
                } else {
                    TRANSITION(STATE_WRITING);
                }
                break;
                
            case STATE_WRITING:
                printf("Processed text: ");
                for (size_t i = 0; i < process_pos; i++) {
                    printf("%c", process_buffer[i]);
                }
                printf("\n");
                TRANSITION(STATE_ERROR);
                break;
                
            case STATE_ERROR:
                break;
        }
    }
    
    if (current_state == STATE_ERROR && process_pos == 0) {
        printf("Error: Invalid input or processing failed\n");
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}