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
#define STATE_DONE 5

#define MAX_INPUT_LEN 100
#define MAX_WORDS 50
#define MAX_WORD_LEN 20

#define TRANSITION(new_state) do { current_state = new_state; } while(0)
#define CHECK_BOUNDS(len, max) ((len) >= 0 && (len) < (max))

typedef struct {
    char words[MAX_WORDS][MAX_WORD_LEN];
    int count;
} WordBuffer;

void initialize_buffer(WordBuffer *buffer) {
    buffer->count = 0;
    for (int i = 0; i < MAX_WORDS; i++) {
        buffer->words[i][0] = '\0';
    }
}

int safe_strncpy(char *dest, const char *src, size_t n) {
    if (dest == NULL || src == NULL || n == 0) return 0;
    size_t i;
    for (i = 0; i < n - 1 && src[i] != '\0'; i++) {
        dest[i] = src[i];
    }
    dest[i] = '\0';
    return 1;
}

int is_valid_char(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == ' ';
}

int process_input(const char *input, WordBuffer *buffer) {
    if (input == NULL || buffer == NULL) return 0;
    
    int input_len = strlen(input);
    if (!CHECK_BOUNDS(input_len, MAX_INPUT_LEN)) return 0;
    
    char word[MAX_WORD_LEN];
    int word_len = 0;
    int in_word = 0;
    
    for (int i = 0; i <= input_len; i++) {
        char c = input[i];
        
        if (is_valid_char(c) && c != ' ' && word_len < MAX_WORD_LEN - 1) {
            word[word_len++] = c;
            in_word = 1;
        } else if (in_word) {
            word[word_len] = '\0';
            if (buffer->count < MAX_WORDS) {
                if (!safe_strncpy(buffer->words[buffer->count], word, MAX_WORD_LEN)) {
                    return 0;
                }
                buffer->count++;
            }
            word_len = 0;
            in_word = 0;
        }
        
        if (c == '\0') break;
    }
    
    return 1;
}

void print_words(const WordBuffer *buffer) {
    if (buffer == NULL) return;
    printf("Processed words (%d):\n", buffer->count);
    for (int i = 0; i < buffer->count; i++) {
        printf("%d: %s\n", i + 1, buffer->words[i]);
    }
}

int main(void) {
    int current_state = STATE_IDLE;
    char input[MAX_INPUT_LEN + 1];
    WordBuffer buffer;
    
    initialize_buffer(&buffer);
    
    while (current_state != STATE_DONE && current_state != STATE_ERROR) {
        switch (current_state) {
            case STATE_IDLE:
                printf("Enter text (or 'quit' to exit): ");
                if (fgets(input, sizeof(input), stdin) != NULL) {
                    size_t len = strlen(input);
                    if (len > 0 && input[len - 1] == '\n') {
                        input[len - 1] = '\0';
                    }
                    if (strcmp(input, "quit") == 0) {
                        TRANSITION(STATE_DONE);
                    } else {
                        TRANSITION(STATE_READING);
                    }
                } else {
                    TRANSITION(STATE_ERROR);
                }
                break;
                
            case STATE_READING:
                if (strlen(input) == 0) {
                    printf("Empty input, returning to idle.\n");
                    TRANSITION(STATE_IDLE);
                } else {
                    TRANSITION(STATE_PROCESSING);
                }
                break;
                
            case STATE_PROCESSING:
                if (process_input(input, &buffer)) {
                    printf("Successfully processed input.\n");
                    TRANSITION(STATE_WRITING);
                } else {
                    printf("Error processing input.\n");
                    TRANSITION(STATE_ERROR);
                }
                break;
                
            case STATE_WRITING:
                print_words(&buffer);
                initialize_buffer(&buffer);
                TRANSITION(STATE_IDLE);
                break;
                
            case STATE_ERROR:
                printf("An error occurred. Resetting...\n");
                initialize_buffer(&buffer);
                TRANSITION(STATE_IDLE);
                break;
                
            default:
                TRANSITION(STATE_ERROR);
                break;
        }
    }
    
    printf("Program completed.\n");
    return 0;
}