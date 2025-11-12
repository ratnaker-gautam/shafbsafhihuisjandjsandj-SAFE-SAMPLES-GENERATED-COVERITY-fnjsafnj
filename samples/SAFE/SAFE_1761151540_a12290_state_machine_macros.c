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
#define IS_VALID_CHAR(c) ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == ' ')

typedef struct {
    char words[MAX_WORDS][MAX_WORD_LEN];
    int word_count;
} WordBuffer;

int validate_input(const char* input) {
    if (input == NULL) return 0;
    size_t len = strlen(input);
    if (len == 0 || len > MAX_INPUT_LEN) return 0;
    for (size_t i = 0; i < len; i++) {
        if (!IS_VALID_CHAR(input[i])) return 0;
    }
    return 1;
}

int process_words(const char* input, WordBuffer* buffer) {
    if (buffer == NULL || input == NULL) return 0;
    
    buffer->word_count = 0;
    size_t input_len = strlen(input);
    size_t word_start = 0;
    size_t word_len = 0;
    
    for (size_t i = 0; i <= input_len; i++) {
        if (i < input_len && input[i] != ' ') {
            if (word_len < MAX_WORD_LEN - 1) {
                buffer->words[buffer->word_count][word_len] = input[i];
                word_len++;
            }
        } else {
            if (word_len > 0) {
                buffer->words[buffer->word_count][word_len] = '\0';
                buffer->word_count++;
                if (buffer->word_count >= MAX_WORDS) break;
                word_len = 0;
            }
        }
    }
    
    return buffer->word_count > 0;
}

void print_words(const WordBuffer* buffer) {
    if (buffer == NULL || buffer->word_count == 0) return;
    
    printf("Processed words (%d):\n", buffer->word_count);
    for (int i = 0; i < buffer->word_count; i++) {
        printf("%d: %s\n", i + 1, buffer->words[i]);
    }
}

int main(void) {
    int current_state = STATE_IDLE;
    char input[MAX_INPUT_LEN + 1];
    WordBuffer word_buffer;
    int running = 1;
    
    printf("Word Processor State Machine\n");
    printf("Enter text (letters and spaces only, max %d chars):\n", MAX_INPUT_LEN);
    
    while (running) {
        switch (current_state) {
            case STATE_IDLE:
                if (fgets(input, sizeof(input), stdin) != NULL) {
                    size_t len = strlen(input);
                    if (len > 0 && input[len - 1] == '\n') {
                        input[len - 1] = '\0';
                    }
                    TRANSITION(STATE_READING);
                } else {
                    TRANSITION(STATE_ERROR);
                }
                break;
                
            case STATE_READING:
                if (validate_input(input)) {
                    TRANSITION(STATE_PROCESSING);
                } else {
                    printf("Invalid input. Use only letters and spaces.\n");
                    TRANSITION(STATE_IDLE);
                }
                break;
                
            case STATE_PROCESSING:
                memset(&word_buffer, 0, sizeof(word_buffer));
                if (process_words(input, &word_buffer)) {
                    TRANSITION(STATE_WRITING);
                } else {
                    printf("No valid words found.\n");
                    TRANSITION(STATE_IDLE);
                }
                break;
                
            case STATE_WRITING:
                print_words(&word_buffer);
                TRANSITION(STATE_IDLE);
                break;
                
            case STATE_ERROR:
                printf("Input error occurred.\n");
                running = 0;
                break;
                
            default:
                TRANSITION(STATE_ERROR);
                break;
        }
        
        if (current_state == STATE_IDLE) {
            printf("\nEnter more text or Ctrl+D to exit:\n");
        }
    }
    
    printf("Exiting word processor.\n");
    return 0;
}