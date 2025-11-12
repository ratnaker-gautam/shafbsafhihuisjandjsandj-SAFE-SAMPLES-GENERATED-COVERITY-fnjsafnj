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
        } else {
            state = STATE_READING;
        }
    }
    
    while (state != STATE_DONE && state != STATE_ERROR) {
        switch (state) {
            case STATE_READING:
                if (pos >= (int)strlen(input)) {
                    if (word_pos > 0 && word_count < MAX_WORDS) {
                        words[word_count][word_pos] = '\0';
                        word_count++;
                    }
                    state = STATE_PROCESSING;
                } else {
                    char c = input[pos];
                    if (isalpha(c)) {
                        if (word_count < MAX_WORDS && word_pos < MAX_WORD_LEN) {
                            words[word_count][word_pos] = c;
                            word_pos++;
                        } else {
                            state = STATE_ERROR;
                        }
                    } else {
                        if (word_pos > 0 && word_count < MAX_WORDS) {
                            words[word_count][word_pos] = '\0';
                            word_count++;
                            word_pos = 0;
                        }
                    }
                    pos++;
                }
                break;
                
            case STATE_PROCESSING:
                if (word_count == 0) {
                    printf("No words found.\n");
                } else {
                    printf("Found %d words:\n", word_count);
                    for (int i = 0; i < word_count; i++) {
                        printf("%d: %s\n", i + 1, words[i]);
                    }
                }
                state = STATE_DONE;
                break;
                
            default:
                state = STATE_ERROR;
                break;
        }
    }
    
    if (state == STATE_ERROR) {
        printf("Error: Invalid input or processing error.\n");
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}