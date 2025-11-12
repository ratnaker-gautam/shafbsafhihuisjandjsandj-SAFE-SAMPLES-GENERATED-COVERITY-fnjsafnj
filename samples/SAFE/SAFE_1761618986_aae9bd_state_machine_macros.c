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
    strncpy(buffer->words[buffer->word_count], word, len);
    buffer->words[buffer->word_count][len] = '\0';
    buffer->word_count++;
    return 0;
}

static void process_word(word_buffer_t *buffer, int index) {
    char *word = buffer->words[index];
    int len = strlen(word);
    for (int i = 0; i < len; i++) {
        word[i] = TO_UPPER(word[i]);
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
    int input_len;
    
    initialize_buffer(&buffer);
    
    printf("Enter text (max %d chars): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        TRANSITION(STATE_ERROR);
        return 1;
    }
    
    input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len > MAX_INPUT_LEN) {
        TRANSITION(STATE_ERROR);
        return 1;
    }
    
    TRANSITION(STATE_READING);
    
    for (int i = 0; i <= input_len; i++) {
        switch (current_state) {
            case STATE_READING:
                if (i < input_len && IS_VALID_CHAR(input[i])) {
                    if (word_len < MAX_WORD_LEN - 1) {
                        current_word[word_len++] = input[i];
                    } else {
                        TRANSITION(STATE_ERROR);
                    }
                } else {
                    if (word_len > 0) {
                        current_word[word_len] = '\0';
                        if (add_word(&buffer, current_word, word_len) != 0) {
                            TRANSITION(STATE_ERROR);
                            break;
                        }
                        word_len = 0;
                    }
                    if (i == input_len) {
                        TRANSITION(STATE_PROCESSING);
                    }
                }
                break;
                
            case STATE_PROCESSING:
                for (int j = 0; j < buffer.word_count; j++) {
                    process_word(&buffer, j);
                }
                TRANSITION(STATE_WRITING);
                break;
                
            case STATE_WRITING:
                printf("Processed words: ");
                print_words(&buffer);
                TRANSITION(STATE_IDLE);
                break;
                
            case STATE_ERROR:
                printf("Error: Invalid input or buffer overflow\n");
                return 1;
                
            default:
                TRANSITION(STATE_ERROR);
                break;
        }
        
        if (current_state == STATE_ERROR) {
            break;
        }
    }
    
    return 0;
}