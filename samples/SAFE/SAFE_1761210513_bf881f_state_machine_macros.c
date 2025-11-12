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
    int word_pos = 0;
    
    printf("Enter text (max %d chars): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        state = STATE_ERROR;
    }
    
    if (state != STATE_ERROR) {
        size_t len = strlen(input);
        if (len > 0 && input[len - 1] == '\n') {
            input[len - 1] = '\0';
            len--;
        }
        
        if (len > MAX_INPUT_LEN) {
            state = STATE_ERROR;
        }
    }
    
    while (state != STATE_DONE && state != STATE_ERROR) {
        TRANSITION(STATE_IDLE, 1, STATE_READING);
        
        if (state == STATE_READING) {
            if (pos >= (int)strlen(input)) {
                state = STATE_PROCESSING;
            } else if (word_count >= MAX_WORDS) {
                state = STATE_ERROR;
            } else {
                char c = input[pos];
                if (isalpha((unsigned char)c)) {
                    if (word_pos < MAX_WORD_LEN) {
                        words[word_count][word_pos++] = c;
                    } else {
                        state = STATE_ERROR;
                    }
                } else {
                    if (word_pos > 0) {
                        words[word_count][word_pos] = '\0';
                        word_count++;
                        word_pos = 0;
                    }
                }
                pos++;
            }
        }
        
        if (state == STATE_PROCESSING) {
            if (word_pos > 0 && word_count < MAX_WORDS) {
                words[word_count][word_pos] = '\0';
                word_count++;
                word_pos = 0;
            }
            
            if (word_count == 0) {
                state = STATE_ERROR;
            } else {
                state = STATE_DONE;
            }
        }
    }
    
    if (state == STATE_DONE) {
        printf("Found %d words:\n", word_count);
        for (int i = 0; i < word_count; i++) {
            if (strlen(words[i]) > 0) {
                printf("%d: %s\n", i + 1, words[i]);
            }
        }
    } else if (state == STATE_ERROR) {
        printf("Error processing input\n");
        return 1;
    }
    
    return 0;
}