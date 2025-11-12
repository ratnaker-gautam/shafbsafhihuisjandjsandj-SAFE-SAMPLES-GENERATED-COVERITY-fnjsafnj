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
    int count;
} WordBuffer;

int validate_input(const char* input) {
    if (input == NULL) return 0;
    size_t len = strlen(input);
    if (len == 0 || len > MAX_INPUT_LEN) return 0;
    return 1;
}

void init_buffer(WordBuffer* buffer) {
    buffer->count = 0;
    for (int i = 0; i < MAX_WORDS; i++) {
        memset(buffer->data[i], 0, MAX_WORD_LEN);
    }
}

int process_state_machine(const char* input, WordBuffer* buffer) {
    int current_state = STATE_IDLE;
    int word_index = 0;
    int char_index = 0;
    
    if (!validate_input(input)) {
        return STATE_ERROR;
    }
    
    for (size_t i = 0; input[i] != '\0'; i++) {
        char c = input[i];
        
        switch (current_state) {
            case STATE_IDLE:
                if (IS_VALID_CHAR(c)) {
                    if (buffer->count >= MAX_WORDS) {
                        TRANSITION(STATE_ERROR);
                        break;
                    }
                    buffer->data[word_index][char_index++] = TO_UPPER(c);
                    TRANSITION(STATE_READING);
                }
                break;
                
            case STATE_READING:
                if (IS_VALID_CHAR(c)) {
                    if (char_index >= MAX_WORD_LEN - 1) {
                        TRANSITION(STATE_ERROR);
                        break;
                    }
                    buffer->data[word_index][char_index++] = TO_UPPER(c);
                } else if (c == ' ' || c == '\t') {
                    if (char_index > 0) {
                        buffer->data[word_index][char_index] = '\0';
                        buffer->count++;
                        word_index++;
                        char_index = 0;
                    }
                    TRANSITION(STATE_IDLE);
                } else {
                    TRANSITION(STATE_ERROR);
                }
                break;
                
            case STATE_ERROR:
                return STATE_ERROR;
                
            default:
                TRANSITION(STATE_ERROR);
                break;
        }
        
        if (current_state == STATE_ERROR) {
            return STATE_ERROR;
        }
    }
    
    if (current_state == STATE_READING && char_index > 0) {
        if (buffer->count >= MAX_WORDS) {
            return STATE_ERROR;
        }
        buffer->data[word_index][char_index] = '\0';
        buffer->count++;
    }
    
    return STATE_WRITING;
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    WordBuffer buffer;
    
    printf("Enter text (max %d chars): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    init_buffer(&buffer);
    
    int result = process_state_machine(input, &buffer);
    
    if (result == STATE_ERROR) {
        fprintf(stderr, "Error processing input\n");
        return EXIT_FAILURE;
    }
    
    printf("Processed %d words:\n", buffer.count);
    for (int i = 0; i < buffer.count; i++) {
        if (printf("%s\n", buffer.data[i]) < 0) {
            fprintf(stderr, "Error writing output\n");
            return EXIT_FAILURE;
        }
    }
    
    return EXIT_SUCCESS;
}