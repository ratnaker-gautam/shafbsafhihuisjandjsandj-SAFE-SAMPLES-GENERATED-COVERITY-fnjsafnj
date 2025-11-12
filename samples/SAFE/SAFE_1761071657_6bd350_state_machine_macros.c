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

#define TRANSITION(current, input, next) \
    if (state == (current) && (input)) { \
        state = (next); \
        continue; \
    }

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    char words[MAX_WORDS][MAX_WORD_LEN + 1];
    int word_count = 0;
    int state = STATE_IDLE;
    int pos = 0;
    int i;
    
    printf("Enter text (max %d chars): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    
    if (len == 0) {
        printf("Empty input\n");
        return 1;
    }
    
    if (len > MAX_INPUT_LEN) {
        printf("Input too long\n");
        return 1;
    }
    
    while (state != STATE_DONE && state != STATE_ERROR) {
        TRANSITION(STATE_IDLE, 1, STATE_READING)
        TRANSITION(STATE_READING, pos >= len, STATE_DONE)
        TRANSITION(STATE_READING, word_count >= MAX_WORDS, STATE_ERROR)
        TRANSITION(STATE_READING, isspace(input[pos]), STATE_READING)
        TRANSITION(STATE_READING, !isspace(input[pos]), STATE_PROCESSING)
        
        if (state == STATE_PROCESSING) {
            int word_len = 0;
            int start_pos = pos;
            
            while (pos < len && !isspace(input[pos]) && word_len < MAX_WORD_LEN) {
                word_len++;
                pos++;
            }
            
            if (word_len > 0) {
                if (word_len > MAX_WORD_LEN) {
                    state = STATE_ERROR;
                    continue;
                }
                
                for (i = 0; i < word_len; i++) {
                    words[word_count][i] = input[start_pos + i];
                }
                words[word_count][word_len] = '\0';
                word_count++;
            }
            
            state = STATE_READING;
            continue;
        }
        
        if (state == STATE_READING) {
            pos++;
        }
    }
    
    if (state == STATE_ERROR) {
        printf("Error: Too many words or word too long\n");
        return 1;
    }
    
    printf("Found %d words:\n", word_count);
    for (i = 0; i < word_count; i++) {
        printf("%d: '%s'\n", i + 1, words[i]);
    }
    
    return 0;
}