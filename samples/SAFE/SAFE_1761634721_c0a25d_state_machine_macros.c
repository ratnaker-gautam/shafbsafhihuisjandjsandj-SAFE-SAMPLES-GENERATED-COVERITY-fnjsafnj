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
#define TO_UPPER(c) ((c >= 'a' && c <= 'z') ? (c - 'a' + 'A') : c)

typedef struct {
    char words[MAX_WORDS][MAX_WORD_LEN];
    int word_count;
    int current_pos;
} word_buffer_t;

static int validate_input(const char* input) {
    if (input == NULL) return 0;
    size_t len = strlen(input);
    if (len == 0 || len > MAX_INPUT_LEN) return 0;
    return 1;
}

static void initialize_buffer(word_buffer_t* buffer) {
    buffer->word_count = 0;
    buffer->current_pos = 0;
    for (int i = 0; i < MAX_WORDS; i++) {
        memset(buffer->words[i], 0, MAX_WORD_LEN);
    }
}

static int add_word(word_buffer_t* buffer, const char* word, size_t len) {
    if (buffer->word_count >= MAX_WORDS || len >= MAX_WORD_LEN) return 0;
    if (strncpy(buffer->words[buffer->word_count], word, len) == NULL) return 0;
    buffer->words[buffer->word_count][len] = '\0';
    buffer->word_count++;
    return 1;
}

static void process_word(word_buffer_t* buffer, int index) {
    if (index < 0 || index >= buffer->word_count) return;
    char* word = buffer->words[index];
    for (int i = 0; word[i] != '\0' && i < MAX_WORD_LEN - 1; i++) {
        word[i] = TO_UPPER(word[i]);
    }
}

static void print_words(const word_buffer_t* buffer) {
    for (int i = 0; i < buffer->word_count; i++) {
        printf("%s ", buffer->words[i]);
    }
    printf("\n");
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    word_buffer_t buffer;
    int current_state = STATE_IDLE;
    int input_valid = 0;
    
    printf("Enter text (max %d chars): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        TRANSITION(STATE_ERROR);
    } else {
        size_t len = strlen(input);
        if (len > 0 && input[len - 1] == '\n') {
            input[len - 1] = '\0';
        }
        input_valid = validate_input(input);
        if (!input_valid) {
            TRANSITION(STATE_ERROR);
        } else {
            TRANSITION(STATE_READING);
        }
    }
    
    initialize_buffer(&buffer);
    
    while (current_state != STATE_ERROR && current_state != STATE_WRITING) {
        switch (current_state) {
            case STATE_READING: {
                char current_word[MAX_WORD_LEN];
                int word_len = 0;
                int i = 0;
                
                while (input[i] != '\0' && i < MAX_INPUT_LEN) {
                    if (IS_VALID_CHAR(input[i])) {
                        if (word_len < MAX_WORD_LEN - 1) {
                            current_word[word_len++] = input[i];
                        }
                    } else {
                        if (word_len > 0) {
                            current_word[word_len] = '\0';
                            if (!add_word(&buffer, current_word, word_len)) {
                                TRANSITION(STATE_ERROR);
                                break;
                            }
                            word_len = 0;
                        }
                    }
                    i++;
                }
                
                if (word_len > 0 && current_state != STATE_ERROR) {
                    current_word[word_len] = '\0';
                    if (!add_word(&buffer, current_word, word_len)) {
                        TRANSITION(STATE_ERROR);
                    }
                }
                
                if (current_state != STATE_ERROR) {
                    TRANSITION(STATE_PROCESSING);
                }
                break;
            }
            
            case STATE_PROCESSING: {
                if (buffer.word_count == 0) {
                    TRANSITION(STATE_ERROR);
                    break;
                }
                
                for (int i = 0; i < buffer.word_count; i++) {
                    process_word(&buffer, i);
                }
                
                TRANSITION(STATE_WRITING);
                break;
            }
            
            default:
                TRANSITION(STATE_ERROR);
                break;
        }
    }
    
    if (current_state == STATE_WRITING) {
        printf("Processed words: ");
        print_words(&buffer);
    } else {
        printf("Error: Invalid input or processing error\n");
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}