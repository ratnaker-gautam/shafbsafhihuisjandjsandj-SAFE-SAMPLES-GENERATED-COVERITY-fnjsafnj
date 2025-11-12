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
    
    printf("Enter text to process (max %d chars): ", MAX_INPUT_LEN);
    
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
        
        TRANSITION(STATE_READING, pos < len, STATE_PROCESSING)
        TRANSITION(STATE_READING, pos >= len, STATE_DONE)
        
        if (state == STATE_PROCESSING) {
            while (pos < len && isspace((unsigned char)input[pos])) {
                pos++;
            }
            
            if (pos >= len) {
                state = STATE_DONE;
                continue;
            }
            
            int word_start = pos;
            while (pos < len && !isspace((unsigned char)input[pos])) {
                pos++;
            }
            
            int word_len = pos - word_start;
            if (word_len > MAX_WORD_LEN) {
                state = STATE_ERROR;
                continue;
            }
            
            if (word_count >= MAX_WORDS) {
                state = STATE_ERROR;
                continue;
            }
            
            if (word_len > 0) {
                for (i = 0; i < word_len; i++) {
                    words[word_count][i] = input[word_start + i];
                }
                words[word_count][word_len] = '\0';
                word_count++;
            }
            
            state = STATE_READING;
        }
    }
    
    if (state == STATE_ERROR) {
        printf("Error: Input too complex\n");
        return 1;
    }
    
    printf("Processed %d words:\n", word_count);
    for (i = 0; i < word_count; i++) {
        printf("%d: %s\n", i + 1, words[i]);
    }
    
    return 0;
}