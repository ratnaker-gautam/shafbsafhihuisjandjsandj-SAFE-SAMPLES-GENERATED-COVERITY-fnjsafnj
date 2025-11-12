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
    int total_chars = 0;
    
    printf("Enter text (max %d chars): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    
    if (len == 0) {
        fprintf(stderr, "Empty input\n");
        return EXIT_FAILURE;
    }
    
    if (len > MAX_INPUT_LEN) {
        fprintf(stderr, "Input too long\n");
        return EXIT_FAILURE;
    }
    
    int pos = 0;
    int word_pos = 0;
    
    while (state != STATE_DONE && state != STATE_ERROR) {
        TRANSITION(STATE_IDLE, 1, STATE_READING)
        
        TRANSITION(STATE_READING, pos < len, STATE_PROCESSING)
        TRANSITION(STATE_READING, pos >= len, STATE_DONE)
        
        if (state == STATE_PROCESSING) {
            if (word_count >= MAX_WORDS) {
                state = STATE_ERROR;
                continue;
            }
            
            while (pos < len && isspace(input[pos])) {
                pos++;
            }
            
            if (pos >= len) {
                state = STATE_DONE;
                continue;
            }
            
            word_pos = 0;
            while (pos < len && !isspace(input[pos]) && word_pos < MAX_WORD_LEN) {
                words[word_count][word_pos++] = input[pos++];
            }
            
            if (word_pos >= MAX_WORD_LEN) {
                state = STATE_ERROR;
                continue;
            }
            
            words[word_count][word_pos] = '\0';
            word_count++;
            total_chars += word_pos;
            state = STATE_READING;
        }
    }
    
    if (state == STATE_ERROR) {
        fprintf(stderr, "Processing error\n");
        return EXIT_FAILURE;
    }
    
    printf("Processed %d words:\n", word_count);
    for (int i = 0; i < word_count; i++) {
        printf("%d: %s\n", i + 1, words[i]);
    }
    printf("Total characters in words: %d\n", total_chars);
    
    return EXIT_SUCCESS;
}