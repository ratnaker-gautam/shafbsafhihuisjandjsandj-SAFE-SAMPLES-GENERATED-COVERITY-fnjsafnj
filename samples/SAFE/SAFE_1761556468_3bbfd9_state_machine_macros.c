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

#define TRANSITION(current, input, next) if(state == (current) && (input)) { state = (next); }

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    char words[MAX_WORDS][MAX_WORD_LEN + 1];
    int word_count = 0;
    int state = STATE_IDLE;
    int pos = 0;
    int i;
    
    printf("Enter text (max %d chars): ", MAX_INPUT_LEN);
    
    if(fgets(input, sizeof(input), stdin) == NULL) {
        state = STATE_ERROR;
    }
    
    if(state != STATE_ERROR) {
        size_t len = strlen(input);
        if(len > 0 && input[len-1] == '\n') {
            input[len-1] = '\0';
            len--;
        }
        
        if(len > MAX_INPUT_LEN) {
            state = STATE_ERROR;
        } else {
            state = STATE_READING;
        }
    }
    
    while(state != STATE_DONE && state != STATE_ERROR) {
        TRANSITION(STATE_READING, pos >= (int)strlen(input), STATE_PROCESSING)
        else TRANSITION(STATE_READING, word_count >= MAX_WORDS, STATE_PROCESSING)
        else TRANSITION(STATE_PROCESSING, 1, STATE_DONE)
        
        if(state == STATE_READING) {
            while(pos < (int)strlen(input) && isspace(input[pos])) {
                pos++;
            }
            
            if(pos < (int)strlen(input)) {
                int word_len = 0;
                int start_pos = pos;
                
                while(pos < (int)strlen(input) && !isspace(input[pos]) && word_len < MAX_WORD_LEN) {
                    word_len++;
                    pos++;
                }
                
                if(word_len > 0 && word_count < MAX_WORDS) {
                    strncpy(words[word_count], &input[start_pos], word_len);
                    words[word_count][word_len] = '\0';
                    word_count++;
                }
            }
        }
        
        if(state == STATE_PROCESSING) {
            printf("Found %d words:\n", word_count);
            for(i = 0; i < word_count; i++) {
                printf("%d: '%s'\n", i+1, words[i]);
            }
        }
    }
    
    if(state == STATE_ERROR) {
        printf("Error processing input\n");
        return 1;
    }
    
    return 0;
}