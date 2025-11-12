//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define STATE_IDLE 0
#define STATE_READING 1
#define STATE_PROCESSING 2
#define STATE_DONE 3
#define STATE_ERROR 4

#define MAX_INPUT_LEN 100
#define MAX_TOKENS 10
#define MAX_TOKEN_LEN 20

#define TRANSITION(new_state) do { current_state = new_state; } while(0)
#define IS_DIGIT(ch) ((ch) >= '0' && (ch) <= '9')
#define IS_ALPHA(ch) (((ch) >= 'a' && (ch) <= 'z') || ((ch) >= 'A' && (ch) <= 'Z'))

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    char tokens[MAX_TOKENS][MAX_TOKEN_LEN + 1];
    int token_count = 0;
    int current_state = STATE_IDLE;
    int input_len = 0;
    int token_pos = 0;
    int i = 0;
    
    printf("Enter text (max %d chars): ", MAX_INPUT_LEN);
    
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
                if (input_len <= 0) {
                    TRANSITION(STATE_DONE);
                } else if (token_count >= MAX_TOKENS) {
                    TRANSITION(STATE_ERROR);
                } else {
                    TRANSITION(STATE_PROCESSING);
                }
                break;
                
            case STATE_PROCESSING:
                while (i < input_len && isspace((unsigned char)input[i])) {
                    i++;
                }
                
                if (i >= input_len) {
                    TRANSITION(STATE_DONE);
                    break;
                }
                
                token_pos = 0;
                while (i < input_len && !isspace((unsigned char)input[i]) && 
                       token_pos < MAX_TOKEN_LEN) {
                    tokens[token_count][token_pos++] = input[i++];
                }
                
                if (token_pos >= MAX_TOKEN_LEN) {
                    TRANSITION(STATE_ERROR);
                } else {
                    tokens[token_count][token_pos] = '\0';
                    token_count++;
                    TRANSITION(STATE_READING);
                }
                break;
                
            case STATE_DONE:
                break;
                
            case STATE_ERROR:
                break;
                
            default:
                TRANSITION(STATE_ERROR);
                break;
        }
    }
    
    if (current_state == STATE_ERROR) {
        printf("Error: Processing failed\n");
        return EXIT_FAILURE;
    }
    
    printf("Processed %d tokens:\n", token_count);
    for (int j = 0; j < token_count; j++) {
        int all_digits = 1;
        int all_alpha = 1;
        int len = (int)strlen(tokens[j]);
        
        for (int k = 0; k < len; k++) {
            if (!IS_DIGIT(tokens[j][k])) all_digits = 0;
            if (!IS_ALPHA(tokens[j][k])) all_alpha = 0;
        }
        
        if (all_digits) {
            printf("  [%s] -> NUMBER\n", tokens[j]);
        } else if (all_alpha) {
            printf("  [%s] -> WORD\n", tokens[j]);
        } else {
            printf("  [%s] -> MIXED\n", tokens[j]);
        }
    }
    
    return EXIT_SUCCESS;
}