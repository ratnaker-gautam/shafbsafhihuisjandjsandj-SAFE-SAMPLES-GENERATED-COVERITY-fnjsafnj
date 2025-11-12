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
#define CHECK_BOUNDS(len, max) ((len) >= 0 && (len) < (max))

typedef struct {
    char words[MAX_WORDS][MAX_WORD_LEN];
    int word_count;
} WordBuffer;

void initialize_buffer(WordBuffer *buffer) {
    buffer->word_count = 0;
    for (int i = 0; i < MAX_WORDS; i++) {
        memset(buffer->words[i], 0, MAX_WORD_LEN);
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

int is_whitespace(char c) {
    return c == ' ' || c == '\t' || c == '\n';
}

int is_alpha(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

int process_input(const char *input, WordBuffer *buffer) {
    if (input == NULL || buffer == NULL) return 0;
    
    int input_len = 0;
    while (input[input_len] != '\0' && input_len < MAX_INPUT_LEN - 1) {
        input_len++;
    }
    
    int word_start = -1;
    buffer->word_count = 0;
    
    for (int i = 0; i <= input_len && buffer->word_count < MAX_WORDS; i++) {
        char c = input[i];
        
        if (is_alpha(c)) {
            if (word_start == -1) {
                word_start = i;
            }
        } else {
            if (word_start != -1) {
                int word_len = i - word_start;
                if (word_len > 0 && word_len < MAX_WORD_LEN) {
                    if (!safe_strncpy(buffer->words[buffer->word_count], 
                                    &input[word_start], word_len + 1)) {
                        return 0;
                    }
                    buffer->word_count++;
                }
                word_start = -1;
            }
        }
    }
    
    return 1;
}

void process_words(WordBuffer *buffer) {
    if (buffer == NULL) return;
    
    for (int i = 0; i < buffer->word_count; i++) {
        for (int j = 0; buffer->words[i][j] != '\0'; j++) {
            if (buffer->words[i][j] >= 'a' && buffer->words[i][j] <= 'z') {
                buffer->words[i][j] = buffer->words[i][j] - 'a' + 'A';
            }
        }
    }
}

void output_result(const WordBuffer *buffer) {
    if (buffer == NULL) return;
    
    printf("Processed %d words:\n", buffer->word_count);
    for (int i = 0; i < buffer->word_count; i++) {
        printf("%s\n", buffer->words[i]);
    }
}

int main(void) {
    int current_state = STATE_IDLE;
    char input[MAX_INPUT_LEN];
    WordBuffer buffer;
    
    initialize_buffer(&buffer);
    
    while (current_state != STATE_ERROR) {
        switch (current_state) {
            case STATE_IDLE:
                printf("Enter text (or 'quit' to exit): ");
                if (fgets(input, sizeof(input), stdin) == NULL) {
                    TRANSITION(STATE_ERROR);
                    break;
                }
                
                size_t len = strlen(input);
                if (len > 0 && input[len - 1] == '\n') {
                    input[len - 1] = '\0';
                }
                
                if (strcmp(input, "quit") == 0) {
                    printf("Goodbye!\n");
                    return 0;
                }
                
                TRANSITION(STATE_READING);
                break;
                
            case STATE_READING:
                if (strlen(input) == 0) {
                    printf("Empty input, try again.\n");
                    TRANSITION(STATE_IDLE);
                } else {
                    TRANSITION(STATE_PROCESSING);
                }
                break;
                
            case STATE_PROCESSING:
                if (!process_input(input, &buffer)) {
                    TRANSITION(STATE_ERROR);
                    break;
                }
                
                if (buffer.word_count == 0) {
                    printf("No words found in input.\n");
                    TRANSITION(STATE_IDLE);
                } else {
                    process_words(&buffer);
                    TRANSITION(STATE_WRITING);
                }
                break;
                
            case STATE_WRITING:
                output_result(&buffer);
                TRANSITION(STATE_IDLE);
                break;
                
            default:
                TRANSITION(STATE_ERROR);
                break;
        }
    }
    
    fprintf(stderr, "Error: