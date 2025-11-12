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
#define IS_VALID_CHAR(c) ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))
#define TO_UPPER(c) ((c >= 'a' && c <= 'z') ? (c - 32) : c)

typedef struct {
    char data[MAX_WORDS][MAX_WORD_LEN];
    int count;
} WordBuffer;

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    WordBuffer buffer;
    int current_state = STATE_IDLE;
    int input_len = 0;
    int word_start = 0;
    int word_len = 0;
    int i = 0;
    
    memset(&buffer, 0, sizeof(buffer));
    
    printf("Enter text (max %d chars): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        TRANSITION(STATE_ERROR);
    } else {
        input_len = (int)strlen(input);
        if (input_len > 0 && input[input_len - 1] == '\n') {
            input[input_len - 1] = '\0';
            input_len--;
        }
        TRANSITION(STATE_READING);
    }
    
    while (current_state != STATE_ERROR && current_state != STATE_IDLE) {
        switch (current_state) {
            case STATE_READING:
                if (input_len <= 0 || input_len > MAX_INPUT_LEN) {
                    TRANSITION(STATE_ERROR);
                    break;
                }
                
                for (i = 0; i < input_len && buffer.count < MAX_WORDS; i++) {
                    if (IS_VALID_CHAR(input[i])) {
                        if (word_len == 0) {
                            word_start = i;
                        }
                        if (word_len < MAX_WORD_LEN - 1) {
                            word_len++;
                        }
                    } else {
                        if (word_len > 0) {
                            if (buffer.count < MAX_WORDS) {
                                int copy_len = word_len;
                                if (copy_len >= MAX_WORD_LEN) {
                                    copy_len = MAX_WORD_LEN - 1;
                                }
                                memcpy(buffer.data[buffer.count], &input[word_start], copy_len);
                                buffer.data[buffer.count][copy_len] = '\0';
                                buffer.count++;
                            }
                            word_len = 0;
                        }
                    }
                }
                
                if (word_len > 0 && buffer.count < MAX_WORDS) {
                    int copy_len = word_len;
                    if (copy_len >= MAX_WORD_LEN) {
                        copy_len = MAX_WORD_LEN - 1;
                    }
                    memcpy(buffer.data[buffer.count], &input[word_start], copy_len);
                    buffer.data[buffer.count][copy_len] = '\0';
                    buffer.count++;
                    word_len = 0;
                }
                
                TRANSITION(STATE_PROCESSING);
                break;
                
            case STATE_PROCESSING:
                if (buffer.count <= 0 || buffer.count > MAX_WORDS) {
                    TRANSITION(STATE_ERROR);
                    break;
                }
                
                for (i = 0; i < buffer.count; i++) {
                    int j = 0;
                    while (buffer.data[i][j] != '\0' && j < MAX_WORD_LEN - 1) {
                        buffer.data[i][j] = TO_UPPER(buffer.data[i][j]);
                        j++;
                    }
                }
                
                TRANSITION(STATE_WRITING);
                break;
                
            case STATE_WRITING:
                if (buffer.count <= 0 || buffer.count > MAX_WORDS) {
                    TRANSITION(STATE_ERROR);
                    break;
                }
                
                printf("Processed words (%d):\n", buffer.count);
                for (i = 0; i < buffer.count; i++) {
                    if (printf("%s\n", buffer.data[i]) < 0) {
                        TRANSITION(STATE_ERROR);
                        break;
                    }
                }
                
                TRANSITION(STATE_IDLE);
                break;
                
            case STATE_ERROR:
                printf("Error occurred during processing\n");
                return EXIT_FAILURE;
                
            default:
                TRANSITION(STATE_ERROR);
                break;
        }
    }
    
    return EXIT_SUCCESS;
}