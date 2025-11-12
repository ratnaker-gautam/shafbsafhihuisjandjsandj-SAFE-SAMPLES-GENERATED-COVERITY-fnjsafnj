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

static void initialize_buffer(word_buffer_t *buffer) {
    buffer->word_count = 0;
    buffer->current_pos = 0;
    for (int i = 0; i < MAX_WORDS; i++) {
        memset(buffer->words[i], 0, MAX_WORD_LEN);
    }
}

static int add_word(word_buffer_t *buffer, const char *word, int len) {
    if (buffer->word_count >= MAX_WORDS || len >= MAX_WORD_LEN) {
        return -1;
    }
    if (len > 0) {
        strncpy(buffer->words[buffer->word_count], word, len);
        buffer->words[buffer->word_count][len] = '\0';
        buffer->word_count++;
    }
    return 0;
}

static void process_words(word_buffer_t *buffer) {
    for (int i = 0; i < buffer->word_count; i++) {
        for (int j = 0; buffer->words[i][j] != '\0'; j++) {
            buffer->words[i][j] = TO_UPPER(buffer->words[i][j]);
        }
    }
}

static void print_words(const word_buffer_t *buffer) {
    for (int i = 0; i < buffer->word_count; i++) {
        printf("%s ", buffer->words[i]);
    }
    printf("\n");
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    word_buffer_t buffer;
    int current_state = STATE_IDLE;
    char current_word[MAX_WORD_LEN];
    int word_len = 0;
    int input_len = 0;
    
    initialize_buffer(&buffer);
    memset(input, 0, sizeof(input));
    memset(current_word, 0, sizeof(current_word));
    
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
    
    for (int i = 0; i <= input_len && current_state != STATE_ERROR; i++) {
        switch (current_state) {
            case STATE_READING:
                if (i < input_len) {
                    char c = input[i];
                    if (IS_VALID_CHAR(c)) {
                        if (word_len < MAX_WORD_LEN - 1) {
                            current_word[word_len++] = c;
                        } else {
                            TRANSITION(STATE_ERROR);
                        }
                    } else if (word_len > 0) {
                        if (add_word(&buffer, current_word, word_len) != 0) {
                            TRANSITION(STATE_ERROR);
                        } else {
                            word_len = 0;
                            memset(current_word, 0, sizeof(current_word));
                        }
                    }
                } else {
                    if (word_len > 0) {
                        if (add_word(&buffer, current_word, word_len) != 0) {
                            TRANSITION(STATE_ERROR);
                        } else {
                            word_len = 0;
                        }
                    }
                    TRANSITION(STATE_PROCESSING);
                }
                break;
                
            case STATE_PROCESSING:
                process_words(&buffer);
                TRANSITION(STATE_WRITING);
                break;
                
            case STATE_WRITING:
                printf("Processed words: ");
                print_words(&buffer);
                TRANSITION(STATE_IDLE);
                break;
                
            case STATE_ERROR:
                printf("Error: Invalid input or buffer overflow\n");
                break;
                
            default:
                TRANSITION(STATE_ERROR);
                break;
        }
    }
    
    if (current_state == STATE_IDLE) {
        printf("Processing complete.\n");
    }
    
    return (current_state == STATE_ERROR) ? 1 : 0;
}