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
    int total_chars = 0;
    
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
    
    int pos = 0;
    int current_word_len = 0;
    
    while (state != STATE_DONE && state != STATE_ERROR) {
        TRANSITION(STATE_IDLE, pos < len, STATE_READING)
        TRANSITION(STATE_READING, pos >= len, STATE_PROCESSING)
        TRANSITION(STATE_PROCESSING, 1, STATE_DONE)
        
        if (state == STATE_READING) {
            if (isspace((unsigned char)input[pos])) {
                if (current_word_len > 0) {
                    if (word_count >= MAX_WORDS) {
                        state = STATE_ERROR;
                        continue;
                    }
                    words[word_count][current_word_len] = '\0';
                    word_count++;
                    current_word_len = 0;
                }
            } else {
                if (current_word_len >= MAX_WORD_LEN) {
                    state = STATE_ERROR;
                    continue;
                }
                if (word_count >= MAX_WORDS) {
                    state = STATE_ERROR;
                    continue;
                }
                words[word_count][current_word_len] = input[pos];
                current_word_len++;
                total_chars++;
            }
            pos++;
        }
        
        if (state == STATE_PROCESSING) {
            if (current_word_len > 0) {
                if (word_count >= MAX_WORDS) {
                    state = STATE_ERROR;
                    continue;
                }
                words[word_count][current_word_len] = '\0';
                word_count++;
            }
            state = STATE_DONE;
        }
    }
    
    if (state == STATE_ERROR) {
        printf("Error: Input too complex\n");
        return 1;
    }
    
    printf("Processed %d words, %d total characters\n", word_count, total_chars);
    printf("Words: ");
    for (int i = 0; i < word_count; i++) {
        printf("%s", words[i]);
        if (i < word_count - 1) {
            printf(" ");
        }
    }
    printf("\n");
    
    return 0;
}