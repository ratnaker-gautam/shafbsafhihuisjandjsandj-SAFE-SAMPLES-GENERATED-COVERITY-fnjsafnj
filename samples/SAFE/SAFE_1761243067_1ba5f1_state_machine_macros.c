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
#define MAX_WORDS 20
#define MAX_WORD_LEN 20

#define TRANSITION(current, input, next) if(state == current && condition == input) { state = next; }

int main(void) {
    char input_buffer[MAX_INPUT_LEN + 1];
    char words[MAX_WORDS][MAX_WORD_LEN + 1];
    int word_count = 0;
    int state = STATE_IDLE;
    int position = 0;
    
    printf("Enter text (max %d chars): ", MAX_INPUT_LEN);
    
    if(fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        state = STATE_ERROR;
    }
    
    if(state != STATE_ERROR) {
        size_t len = strlen(input_buffer);
        if(len > 0 && input_buffer[len - 1] == '\n') {
            input_buffer[len - 1] = '\0';
            len--;
        }
        
        if(len >= MAX_INPUT_LEN) {
            state = STATE_ERROR;
        }
    }
    
    while(state != STATE_DONE && state != STATE_ERROR) {
        int condition = 0;
        
        switch(state) {
            case STATE_IDLE:
                if(strlen(input_buffer) > 0) {
                    condition = 1;
                } else {
                    condition = 2;
                }
                break;
                
            case STATE_READING:
                if(position < (int)strlen(input_buffer)) {
                    condition = 3;
                } else {
                    condition = 4;
                }
                break;
                
            case STATE_PROCESSING:
                if(word_count < MAX_WORDS) {
                    condition = 5;
                } else {
                    condition = 6;
                }
                break;
                
            default:
                condition = 7;
                break;
        }
        
        TRANSITION(STATE_IDLE, 1, STATE_READING)
        TRANSITION(STATE_IDLE, 2, STATE_DONE)
        TRANSITION(STATE_READING, 3, STATE_PROCESSING)
        TRANSITION(STATE_READING, 4, STATE_DONE)
        TRANSITION(STATE_PROCESSING, 5, STATE_READING)
        TRANSITION(STATE_PROCESSING, 6, STATE_DONE)
        
        if(state == STATE_PROCESSING && condition == 5) {
            int word_pos = 0;
            while(position < (int)strlen(input_buffer) && 
                  !isspace((unsigned char)input_buffer[position]) && 
                  word_pos < MAX_WORD_LEN) {
                words[word_count][word_pos++] = input_buffer[position++];
            }
            
            if(word_pos > 0) {
                words[word_count][word_pos] = '\0';
                word_count++;
            }
            
            while(position < (int)strlen(input_buffer) && 
                  isspace((unsigned char)input_buffer[position])) {
                position++;
            }
        }
        
        if(state == STATE_ERROR) {
            break;
        }
    }
    
    if(state == STATE_ERROR) {
        printf("Error: Invalid input or buffer overflow\n");
        return 1;
    }
    
    printf("Found %d words:\n", word_count);
    for(int i = 0; i < word_count; i++) {
        printf("%d: '%s'\n", i + 1, words[i]);
    }
    
    return 0;
}