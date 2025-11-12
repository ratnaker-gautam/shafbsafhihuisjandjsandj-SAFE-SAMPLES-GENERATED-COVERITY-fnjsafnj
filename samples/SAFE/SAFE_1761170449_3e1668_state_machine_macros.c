//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: state_machine
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
#define IS_VALID_CHAR(c) ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == ' ' || c == '\n')

typedef struct {
    char words[MAX_WORDS][MAX_WORD_LEN];
    int word_count;
} WordBuffer;

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    WordBuffer buffer;
    int current_state = STATE_IDLE;
    int input_len;
    int i, j, word_idx;
    char current_word[MAX_WORD_LEN + 1];
    int word_pos;
    
    memset(&buffer, 0, sizeof(buffer));
    
    while (current_state != STATE_ERROR) {
        switch (current_state) {
            case STATE_IDLE:
                printf("Enter text (max %d chars): ", MAX_INPUT_LEN);
                if (fgets(input, sizeof(input), stdin) == NULL) {
                    TRANSITION(STATE_ERROR);
                    break;
                }
                
                input_len = strlen(input);
                if (input_len > 0 && input[input_len - 1] == '\n') {
                    input[input_len - 1] = '\0';
                    input_len--;
                }
                
                if (input_len == 0) {
                    printf("Empty input. Exiting.\n");
                    return 0;
                }
                
                if (input_len > MAX_INPUT_LEN) {
                    printf("Input too long.\n");
                    TRANSITION(STATE_ERROR);
                    break;
                }
                
                for (i = 0; i < input_len; i++) {
                    if (!IS_VALID_CHAR(input[i])) {
                        printf("Invalid character in input.\n");
                        TRANSITION(STATE_ERROR);
                        break;
                    }
                }
                
                if (current_state != STATE_ERROR) {
                    TRANSITION(STATE_READING);
                }
                break;
                
            case STATE_READING:
                word_idx = 0;
                word_pos = 0;
                memset(current_word, 0, sizeof(current_word));
                
                for (i = 0; i <= input_len; i++) {
                    if (input[i] == ' ' || input[i] == '\0') {
                        if (word_pos > 0) {
                            if (word_idx >= MAX_WORDS) {
                                printf("Too many words.\n");
                                TRANSITION(STATE_ERROR);
                                break;
                            }
                            
                            if (word_pos >= MAX_WORD_LEN) {
                                printf("Word too long.\n");
                                TRANSITION(STATE_ERROR);
                                break;
                            }
                            
                            strncpy(buffer.words[word_idx], current_word, MAX_WORD_LEN);
                            buffer.words[word_idx][MAX_WORD_LEN - 1] = '\0';
                            word_idx++;
                            word_pos = 0;
                            memset(current_word, 0, sizeof(current_word));
                        }
                    } else {
                        if (word_pos >= MAX_WORD_LEN - 1) {
                            printf("Word too long.\n");
                            TRANSITION(STATE_ERROR);
                            break;
                        }
                        current_word[word_pos++] = input[i];
                    }
                }
                
                if (current_state != STATE_ERROR) {
                    buffer.word_count = word_idx;
                    TRANSITION(STATE_PROCESSING);
                }
                break;
                
            case STATE_PROCESSING:
                if (buffer.word_count == 0) {
                    printf("No words to process.\n");
                    TRANSITION(STATE_ERROR);
                    break;
                }
                
                for (i = 0; i < buffer.word_count; i++) {
                    for (j = 0; buffer.words[i][j] != '\0'; j++) {
                        if (buffer.words[i][j] >= 'a' && buffer.words[i][j] <= 'z') {
                            buffer.words[i][j] = buffer.words[i][j] - 'a' + 'A';
                        }
                    }
                }
                
                TRANSITION(STATE_WRITING);
                break;
                
            case STATE_WRITING:
                printf("Processed words (%d):\n", buffer.word_count);
                for (i = 0; i < buffer.word_count; i++) {
                    printf("%d: %s\n", i + 1, buffer.words[i]);
                }
                printf("Processing complete.\n");
                return 0;
                
            default:
                TRANSITION(STATE_ERROR);
                break;
        }
    }
    
    printf("State machine error occurred.\n");
    return 1;
}