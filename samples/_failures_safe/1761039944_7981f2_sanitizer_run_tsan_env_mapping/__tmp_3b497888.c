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

int main() {
    char input[MAX_INPUT_LEN + 1];
    char words[MAX_WORDS][MAX_WORD_LEN + 1];
    int word_count = 0;
    int state = STATE_IDLE;
    int i, j;
    
    printf("Enter text (max %d chars): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
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
    
    i = 0;
    j = 0;
    
    while (state != STATE_DONE && state != STATE_ERROR) {
        TRANSITION(STATE_IDLE, 1, STATE_READING)
        TRANSITION(STATE_READING, i >= len, STATE_PROCESSING)
        TRANSITION(STATE_READING, word_count >= MAX_WORDS, STATE_ERROR)
        
        if (state == STATE_READING) {
            if (isspace((unsigned char)input[i])) {
                if (j > 0) {
                    words[word_count][j] = '\0';
                    word_count++;
                    j = 0;
                }
                i++;
                continue;
            }
            
            if (j >= MAX_WORD_LEN) {
                state = STATE_ERROR;
                continue;
            }
            
            words[word_count][j] = input[i];
            j++;
            i++;
            continue;
        }
        
        TRANSITION(STATE_PROCESSING, 1, STATE_DONE)
        
        if (state == STATE_ERROR) {
            printf("Error: Too many words or word too long\n");
            break;
        }
    }
    
    if (state == STATE_DONE) {
        if (j > 0) {
            words[word_count][j] = '\0';
            word_count++;
        }
        
        printf("Found %d words:\n", word_count);
        for (i = 0; i < word_count; i++) {
            printf("%d: %s\n", i + 1, words[i]);
        }
    }
    
    return state == STATE_ERROR ? 1 : 0;
}