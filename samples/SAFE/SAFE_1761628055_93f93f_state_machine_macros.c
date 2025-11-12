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
#define IS_TERMINATOR(c) (c == '\n' || c == '\0')

typedef struct {
    char words[MAX_WORDS][MAX_WORD_LEN];
    int word_count;
    int current_length;
} word_buffer_t;

static void initialize_buffer(word_buffer_t *buffer) {
    buffer->word_count = 0;
    buffer->current_length = 0;
    memset(buffer->words, 0, sizeof(buffer->words));
}

static int add_char_to_word(word_buffer_t *buffer, char c) {
    if (buffer->current_length >= MAX_WORD_LEN - 1) {
        return -1;
    }
    buffer->words[buffer->word_count][buffer->current_length++] = c;
    buffer->words[buffer->word_count][buffer->current_length] = '\0';
    return 0;
}

static int finalize_word(word_buffer_t *buffer) {
    if (buffer->current_length > 0) {
        buffer->word_count++;
        buffer->current_length = 0;
        if (buffer->word_count >= MAX_WORDS) {
            return -1;
        }
    }
    return 0;
}

static void process_words(word_buffer_t *buffer) {
    for (int i = 0; i < buffer->word_count; i++) {
        for (int j = 0; buffer->words[i][j] != '\0'; j++) {
            if (buffer->words[i][j] >= 'a' && buffer->words[i][j] <= 'z') {
                buffer->words[i][j] = buffer->words[i][j] - 'a' + 'A';
            }
        }
    }
}

static void output_results(const word_buffer_t *buffer) {
    printf("Processed words (%d):\n", buffer->word_count);
    for (int i = 0; i < buffer->word_count; i++) {
        printf("%s\n", buffer->words[i]);
    }
}

int main(void) {
    char input[MAX_INPUT_LEN + 2];
    int current_state = STATE_IDLE;
    word_buffer_t buffer;
    int input_len;
    
    initialize_buffer(&buffer);
    
    printf("Enter text (max %d chars): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        TRANSITION(STATE_ERROR);
    } else {
        TRANSITION(STATE_READING);
    }
    
    if (current_state == STATE_READING) {
        input_len = (int)strlen(input);
        if (input_len > 0 && input[input_len - 1] == '\n') {
            input[input_len - 1] = '\0';
            input_len--;
        }
        
        if (input_len > MAX_INPUT_LEN) {
            TRANSITION(STATE_ERROR);
        } else {
            TRANSITION(STATE_PROCESSING);
        }
    }
    
    if (current_state == STATE_PROCESSING) {
        for (int i = 0; i <= input_len; i++) {
            char c = input[i];
            
            if (IS_VALID_CHAR(c)) {
                if (add_char_to_word(&buffer, c) != 0) {
                    TRANSITION(STATE_ERROR);
                    break;
                }
            } else if (IS_TERMINATOR(c) || c == ' ') {
                if (finalize_word(&buffer) != 0) {
                    TRANSITION(STATE_ERROR);
                    break;
                }
            } else {
                TRANSITION(STATE_ERROR);
                break;
            }
        }
        
        if (current_state == STATE_PROCESSING) {
            process_words(&buffer);
            TRANSITION(STATE_WRITING);
        }
    }
    
    if (current_state == STATE_WRITING) {
        output_results(&buffer);
    } else if (current_state == STATE_ERROR) {
        printf("Error: Invalid input or buffer overflow\n");
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}