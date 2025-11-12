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
        printf("Empty input\n");
        return EXIT_SUCCESS;
    }
    
    if (len > MAX_INPUT_LEN) {
        fprintf(stderr, "Input too long\n");
        return EXIT_FAILURE;
    }
    
    int pos = 0;
    while (state != STATE_DONE && state != STATE_ERROR) {
        TRANSITION(STATE_IDLE, pos < len, STATE_READING)
        TRANSITION(STATE_READING, pos >= len, STATE_PROCESSING)
        TRANSITION(STATE_PROCESSING, 1, STATE_DONE)
        
        if (state == STATE_READING) {
            while (pos < len && isspace((unsigned char)input[pos])) {
                pos++;
            }
            
            if (pos >= len) {
                state = STATE_PROCESSING;
                continue;
            }
            
            if (word_count >= MAX_WORDS) {
                state = STATE_ERROR;
                continue;
            }
            
            int word_len = 0;
            int start_pos = pos;
            
            while (pos < len && !isspace((unsigned char)input[pos])) {
                if (word_len < MAX_WORD_LEN) {
                    words[word_count][word_len] = input[pos];
                    word_len++;
                }
                pos++;
                total_chars++;
            }
            
            if (word_len > 0) {
                if (word_len > MAX_WORD_LEN) {
                    state = STATE_ERROR;
                    continue;
                }
                words[word_count][word_len] = '\0';
                word_count++;
            }
        }
        
        if (state == STATE_PROCESSING) {
            printf("Processing complete:\n");
            printf("Words found: %d\n", word_count);
            printf("Total characters: %d\n", total_chars);
            printf("Word list:\n");
            
            for (int i = 0; i < word_count; i++) {
                if (i < MAX_WORDS) {
                    printf("%d: %s\n", i + 1, words[i]);
                }
            }
            
            state = STATE_DONE;
        }
    }
    
    if (state == STATE_ERROR) {
        fprintf(stderr, "Error processing input\n");
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}