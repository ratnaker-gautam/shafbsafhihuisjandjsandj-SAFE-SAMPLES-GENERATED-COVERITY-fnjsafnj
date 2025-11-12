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

static int add_word(word_buffer_t* buffer, const char* word, int len) {
    if (buffer->word_count >= MAX_WORDS || len >= MAX_WORD_LEN) return 0;
    strncpy(buffer->words[buffer->word_count], word, len);
    buffer->words[buffer->word_count][len] = '\0';
    buffer->word_count++;
    return 1;
}

static void process_words(word_buffer_t* buffer) {
    for (int i = 0; i < buffer->word_count; i++) {
        for (int j = 0; buffer->words[i][j] != '\0'; j++) {
            buffer->words[i][j] = TO_UPPER(buffer->words[i][j]);
        }
    }
}

static void print_words(const word_buffer_t* buffer) {
    for (int i = 0; i < buffer->word_count; i++) {
        printf("%s", buffer->words[i]);
        if (i < buffer->word_count - 1) printf(" ");
    }
    printf("\n");
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    word_buffer_t buffer;
    int current_state = STATE_IDLE;
    int input_len;
    char current_word[MAX_WORD_LEN];
    int word_len = 0;
    
    printf("Enter text to process: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        TRANSITION(STATE_ERROR);
    }
    
    if (current_state != STATE_ERROR) {
        input_len = strlen(input);
        if (input_len > 0 && input[input_len - 1] == '\n') {
            input[input_len - 1] = '\0';
            input_len--;
        }
        
        if (!validate_input(input)) {
            TRANSITION(STATE_ERROR);
        } else {
            TRANSITION(STATE_READING);
        }
    }
    
    if (current_state == STATE_READING) {
        initialize_buffer(&buffer);
        
        for (int i = 0; i <= input_len; i++) {
            char c = input[i];
            
            if (IS_VALID_CHAR(c)) {
                if (word_len < MAX_WORD_LEN - 1) {
                    current_word[word_len++] = c;
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
                
                if (c == '\0') break;
            }
        }
        
        if (current_state == STATE_READING) {
            TRANSITION(STATE_PROCESSING);
        }
    }
    
    if (current_state == STATE_PROCESSING) {
        process_words(&buffer);
        TRANSITION(STATE_WRITING);
    }
    
    if (current_state == STATE_WRITING) {
        printf("Processed text: ");
        print_words(&buffer);
    } else if (current_state == STATE_ERROR) {
        printf("Error: Invalid input or processing error\n");
        return 1;
    }
    
    return 0;
}