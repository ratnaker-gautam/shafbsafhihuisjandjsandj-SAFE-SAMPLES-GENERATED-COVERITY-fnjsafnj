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

int validate_input(const char* input) {
    if (input == NULL) return 0;
    size_t len = strlen(input);
    if (len == 0 || len > MAX_INPUT_LEN) return 0;
    return 1;
}

void initialize_buffer(WordBuffer* buffer) {
    buffer->count = 0;
    for (int i = 0; i < MAX_WORDS; i++) {
        memset(buffer->data[i], 0, MAX_WORD_LEN);
    }
}

int process_state_machine(const char* input, WordBuffer* buffer) {
    int current_state = STATE_IDLE;
    int input_pos = 0;
    int word_pos = 0;
    char current_char;
    
    initialize_buffer(buffer);
    
    while (current_state != STATE_ERROR && current_state != STATE_WRITING) {
        switch (current_state) {
            case STATE_IDLE:
                if (input_pos < (int)strlen(input)) {
                    TRANSITION(STATE_READING);
                } else {
                    TRANSITION(STATE_WRITING);
                }
                break;
                
            case STATE_READING:
                current_char = input[input_pos];
                if (IS_VALID_CHAR(current_char)) {
                    if (buffer->count < MAX_WORDS && word_pos < MAX_WORD_LEN - 1) {
                        buffer->data[buffer->count][word_pos] = TO_UPPER(current_char);
                        word_pos++;
                    } else {
                        TRANSITION(STATE_ERROR);
                    }
                    input_pos++;
                } else if (current_char == ' ' || current_char == '\t') {
                    if (word_pos > 0) {
                        buffer->count++;
                        word_pos = 0;
                    }
                    input_pos++;
                } else if (current_char == '\0') {
                    if (word_pos > 0) {
                        buffer->count++;
                    }
                    TRANSITION(STATE_PROCESSING);
                } else {
                    TRANSITION(STATE_ERROR);
                }
                break;
                
            case STATE_PROCESSING:
                if (buffer->count > 0) {
                    TRANSITION(STATE_WRITING);
                } else {
                    TRANSITION(STATE_ERROR);
                }
                break;
                
            default:
                TRANSITION(STATE_ERROR);
                break;
        }
    }
    
    return current_state;
}

int main() {
    char input[MAX_INPUT_LEN + 1];
    WordBuffer buffer;
    int result;
    
    printf("Enter text (max %d chars): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (!validate_input(input)) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    result = process_state_machine(input, &buffer);
    
    if (result == STATE_ERROR) {
        fprintf(stderr, "Processing error occurred\n");
        return 1;
    }
    
    printf("Processed %d words:\n", buffer.count);
    for (int i = 0; i < buffer.count; i++) {
        printf("%s\n", buffer.data[i]);
    }
    
    return 0;
}