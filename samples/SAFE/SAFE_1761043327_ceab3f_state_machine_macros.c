//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: macros ; Variation: state_machine
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
#define MAX_WORDS 20
#define MAX_WORD_LEN 20

#define TRANSITION(current, input, next) if(state == current && condition == input) { state = next; }

int main(void) {
    char input_buffer[MAX_INPUT_LEN + 1];
    char words[MAX_WORDS][MAX_WORD_LEN + 1];
    int word_count = 0;
    int state = STATE_IDLE;
    int input_len;
    
    printf("Enter text to process (max %d chars): ", MAX_INPUT_LEN);
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        state = STATE_ERROR;
    }
    
    if (state != STATE_ERROR) {
        input_len = strlen(input_buffer);
        if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
            input_buffer[input_len - 1] = '\0';
            input_len--;
        }
        
        if (input_len >= MAX_INPUT_LEN) {
            state = STATE_ERROR;
        }
    }
    
    int pos = 0;
    int word_pos = 0;
    
    while (state != STATE_DONE && state != STATE_ERROR) {
        int condition = 0;
        
        if (state == STATE_IDLE) {
            if (pos < input_len) {
                condition = 1;
            } else {
                condition = 2;
            }
        } else if (state == STATE_READING) {
            if (pos < input_len && !isspace((unsigned char)input_buffer[pos])) {
                condition = 3;
            } else {
                condition = 4;
            }
        } else if (state == STATE_PROCESSING) {
            condition = 5;
        }
        
        TRANSITION(STATE_IDLE, 1, STATE_READING);
        TRANSITION(STATE_IDLE, 2, STATE_DONE);
        TRANSITION(STATE_READING, 3, STATE_READING);
        TRANSITION(STATE_READING, 4, STATE_PROCESSING);
        TRANSITION(STATE_PROCESSING, 5, STATE_IDLE);
        
        if (state == STATE_READING && condition == 3) {
            if (word_count < MAX_WORDS && word_pos < MAX_WORD_LEN) {
                words[word_count][word_pos++] = input_buffer[pos];
                words[word_count][word_pos] = '\0';
            } else {
                state = STATE_ERROR;
            }
            pos++;
        } else if (state == STATE_PROCESSING && condition == 5) {
            if (word_pos > 0) {
                word_count++;
                word_pos = 0;
            }
            while (pos < input_len && isspace((unsigned char)input_buffer[pos])) {
                pos++;
            }
        }
    }
    
    if (state == STATE_ERROR) {
        printf("Error: Input processing failed\n");
        return 1;
    }
    
    printf("Processed %d words:\n", word_count);
    for (int i = 0; i < word_count; i++) {
        printf("%d: '%s'\n", i + 1, words[i]);
    }
    
    return 0;
}