//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

#define MAX_INPUT_LEN 100
#define STATE_IDLE 0
#define STATE_READING 1
#define STATE_PROCESSING 2
#define STATE_DONE 3
#define STATE_ERROR 4

#define TRANSITION(next) do { current_state = (next); } while(0)
#define IS_DIGIT(c) (isdigit((unsigned char)(c)))
#define IS_ALPHA(c) (isalpha((unsigned char)(c)))

int main(void) {
    int current_state = STATE_IDLE;
    char input_buffer[MAX_INPUT_LEN + 1] = {0};
    size_t input_len = 0;
    int number_count = 0;
    int word_count = 0;
    int ch;
    
    printf("Enter text (max %d chars): ", MAX_INPUT_LEN);
    
    while ((ch = getchar()) != EOF && ch != '\n') {
        if (input_len >= MAX_INPUT_LEN) {
            TRANSITION(STATE_ERROR);
            break;
        }
        
        switch (current_state) {
            case STATE_IDLE:
                if (ch != ' ' && ch != '\t') {
                    input_buffer[input_len++] = (char)ch;
                    if (IS_DIGIT(ch)) {
                        TRANSITION(STATE_READING);
                    } else if (IS_ALPHA(ch)) {
                        TRANSITION(STATE_READING);
                    } else {
                        TRANSITION(STATE_ERROR);
                    }
                }
                break;
                
            case STATE_READING:
                if (ch == ' ' || ch == '\t') {
                    TRANSITION(STATE_PROCESSING);
                } else if (IS_DIGIT(ch) || IS_ALPHA(ch)) {
                    input_buffer[input_len++] = (char)ch;
                } else {
                    TRANSITION(STATE_ERROR);
                }
                break;
                
            case STATE_PROCESSING:
                if (input_len > 0) {
                    if (IS_DIGIT(input_buffer[0])) {
                        int valid_number = 1;
                        for (size_t i = 0; i < input_len; i++) {
                            if (!IS_DIGIT(input_buffer[i])) {
                                valid_number = 0;
                                break;
                            }
                        }
                        if (valid_number) {
                            number_count++;
                        } else {
                            TRANSITION(STATE_ERROR);
                        }
                    } else if (IS_ALPHA(input_buffer[0])) {
                        int valid_word = 1;
                        for (size_t i = 0; i < input_len; i++) {
                            if (!IS_ALPHA(input_buffer[i])) {
                                valid_word = 0;
                                break;
                            }
                        }
                        if (valid_word) {
                            word_count++;
                        } else {
                            TRANSITION(STATE_ERROR);
                        }
                    } else {
                        TRANSITION(STATE_ERROR);
                    }
                    
                    input_len = 0;
                    memset(input_buffer, 0, sizeof(input_buffer));
                    
                    if (ch != ' ' && ch != '\t') {
                        input_buffer[input_len++] = (char)ch;
                        if (IS_DIGIT(ch)) {
                            TRANSITION(STATE_READING);
                        } else if (IS_ALPHA(ch)) {
                            TRANSITION(STATE_READING);
                        } else {
                            TRANSITION(STATE_ERROR);
                        }
                    } else {
                        TRANSITION(STATE_IDLE);
                    }
                }
                break;
                
            case STATE_ERROR:
                break;
                
            default:
                TRANSITION(STATE_ERROR);
                break;
        }
        
        if (current_state == STATE_ERROR) {
            break;
        }
    }
    
    if (current_state != STATE_ERROR && input_len > 0) {
        if (IS_DIGIT(input_buffer[0])) {
            int valid_number = 1;
            for (size_t i = 0; i < input_len; i++) {
                if (!IS_DIGIT(input_buffer[i])) {
                    valid_number = 0;
                    break;
                }
            }
            if (valid_number) {
                number_count++;
            } else {
                TRANSITION(STATE_ERROR);
            }
        } else if (IS_ALPHA(input_buffer[0])) {
            int valid_word = 1;
            for (size_t i = 0; i < input_len; i++) {
                if (!IS_ALPHA(input_buffer[i])) {
                    valid_word = 0;
                    break;
                }
            }
            if (valid_word) {
                word_count++;
            } else {
                TRANSITION(STATE_ERROR);
            }
        } else {
            TRANSITION(STATE_ERROR);
        }
    }
    
    if (current_state == STATE_ERROR) {
        printf("Error: Invalid input format\n");
        return EXIT_FAILURE;
    }
    
    printf("Words: %d\n", word_count);
    printf("Numbers: %d\n", number_count);
    
    return EXIT_SUCCESS;
}