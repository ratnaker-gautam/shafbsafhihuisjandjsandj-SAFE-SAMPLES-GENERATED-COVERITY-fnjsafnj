//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: macros ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define STATE_IDLE 0
#define STATE_READING 1
#define STATE_PROCESSING 2
#define STATE_WRITING 3
#define STATE_ERROR 4

#define MAX_INPUT_LEN 100
#define MAX_WORDS 50
#define MAX_WORD_LEN 20

#define TRANSITION(new_state) do { current_state = new_state; } while(0)
#define IS_VALID_CHAR(c) ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))
#define TO_UPPER(c) ((c >= 'a' && c <= 'z') ? (c - 32) : c)

typedef struct {
    char data[MAX_WORDS][MAX_WORD_LEN];
    size_t count;
} WordList;

int main(void) {
    int current_state = STATE_IDLE;
    char input[MAX_INPUT_LEN + 1] = {0};
    WordList words = {0};
    size_t input_len = 0;
    size_t word_index = 0;
    size_t char_index = 0;
    int c;
    
    printf("Enter text (max %d chars): ", MAX_INPUT_LEN);
    
    while ((c = getchar()) != EOF && c != '\n') {
        if (input_len >= MAX_INPUT_LEN) {
            TRANSITION(STATE_ERROR);
            break;
        }
        
        input[input_len++] = (char)c;
    }
    
    if (current_state == STATE_ERROR) {
        printf("Error: Input too long\n");
        return 1;
    }
    
    input[input_len] = '\0';
    TRANSITION(STATE_READING);
    
    for (size_t i = 0; i <= input_len; i++) {
        char current_char = input[i];
        
        switch (current_state) {
            case STATE_READING:
                if (IS_VALID_CHAR(current_char)) {
                    if (word_index >= MAX_WORDS) {
                        TRANSITION(STATE_ERROR);
                        break;
                    }
                    if (char_index >= MAX_WORD_LEN - 1) {
                        TRANSITION(STATE_ERROR);
                        break;
                    }
                    words.data[word_index][char_index++] = TO_UPPER(current_char);
                    TRANSITION(STATE_PROCESSING);
                } else if (current_char == ' ' || current_char == '\t' || current_char == '\0') {
                    if (char_index > 0) {
                        words.data[word_index][char_index] = '\0';
                        word_index++;
                        words.count++;
                        char_index = 0;
                    }
                } else if (current_char != '\0') {
                    TRANSITION(STATE_ERROR);
                }
                break;
                
            case STATE_PROCESSING:
                if (IS_VALID_CHAR(current_char)) {
                    if (char_index >= MAX_WORD_LEN - 1) {
                        TRANSITION(STATE_ERROR);
                        break;
                    }
                    words.data[word_index][char_index++] = TO_UPPER(current_char);
                } else if (current_char == ' ' || current_char == '\t' || current_char == '\0') {
                    words.data[word_index][char_index] = '\0';
                    word_index++;
                    words.count++;
                    char_index = 0;
                    TRANSITION(STATE_READING);
                } else {
                    TRANSITION(STATE_ERROR);
                }
                break;
                
            case STATE_ERROR:
                printf("Error: Invalid input or buffer overflow\n");
                return 1;
                
            default:
                TRANSITION(STATE_ERROR);
                break;
        }
        
        if (current_state == STATE_ERROR) {
            break;
        }
    }
    
    if (current_state != STATE_ERROR) {
        TRANSITION(STATE_WRITING);
    }
    
    if (current_state == STATE_WRITING) {
        printf("Processed words (%zu):\n", words.count);
        for (size_t i = 0; i < words.count; i++) {
            if (words.data[i][0] != '\0') {
                printf("%s\n", words.data[i]);
            }
        }
    }
    
    return current_state == STATE_ERROR ? 1 : 0;
}