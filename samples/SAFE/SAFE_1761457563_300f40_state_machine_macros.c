//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

#define STATE_IDLE 0
#define STATE_READING 1
#define STATE_PROCESSING 2
#define STATE_DONE 3
#define STATE_ERROR 4

#define MAX_INPUT_LEN 100
#define MAX_TOKENS 10

#define TRANSITION(new_state) do { current_state = new_state; } while(0)
#define IS_VALID_CHAR(c) (isalnum((unsigned char)c) || (c) == ' ' || (c) == ',')

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    char tokens[MAX_TOKENS][MAX_INPUT_LEN + 1];
    int token_count = 0;
    int current_state = STATE_IDLE;
    int input_len = 0;
    int pos = 0;
    int token_pos = 0;
    
    printf("Enter up to %d alphanumeric tokens separated by commas: ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        TRANSITION(STATE_ERROR);
    } else {
        input_len = (int)strlen(input);
        if (input_len > 0 && input[input_len - 1] == '\n') {
            input[input_len - 1] = '\0';
            input_len--;
        }
        TRANSITION(STATE_READING);
    }
    
    while (current_state != STATE_DONE && current_state != STATE_ERROR) {
        switch (current_state) {
            case STATE_READING:
                if (input_len >= MAX_INPUT_LEN) {
                    TRANSITION(STATE_ERROR);
                    break;
                }
                
                for (int i = 0; i < input_len; i++) {
                    if (!IS_VALID_CHAR(input[i])) {
                        TRANSITION(STATE_ERROR);
                        break;
                    }
                }
                
                if (current_state != STATE_ERROR) {
                    TRANSITION(STATE_PROCESSING);
                }
                break;
                
            case STATE_PROCESSING:
                token_pos = 0;
                while (pos < input_len && token_count < MAX_TOKENS) {
                    if (input[pos] == ',') {
                        if (token_pos > 0) {
                            tokens[token_count][token_pos] = '\0';
                            token_count++;
                            token_pos = 0;
                        }
                        pos++;
                        continue;
                    }
                    
                    if (input[pos] == ' ') {
                        pos++;
                        continue;
                    }
                    
                    if (token_pos < MAX_INPUT_LEN) {
                        tokens[token_count][token_pos] = input[pos];
                        token_pos++;
                    } else {
                        TRANSITION(STATE_ERROR);
                        break;
                    }
                    pos++;
                }
                
                if (current_state != STATE_ERROR) {
                    if (token_pos > 0 && token_count < MAX_TOKENS) {
                        tokens[token_count][token_pos] = '\0';
                        token_count++;
                    }
                    TRANSITION(STATE_DONE);
                }
                break;
                
            default:
                TRANSITION(STATE_ERROR);
                break;
        }
    }
    
    if (current_state == STATE_ERROR) {
        printf("Error: Invalid input format\n");
        return EXIT_FAILURE;
    }
    
    printf("Processed %d tokens:\n", token_count);
    for (int i = 0; i < token_count; i++) {
        printf("Token %d: %s\n", i + 1, tokens[i]);
    }
    
    return EXIT_SUCCESS;
}