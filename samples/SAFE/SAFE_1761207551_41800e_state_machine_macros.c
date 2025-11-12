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
    if (!input) return 0;
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
    if (len == 0) return 1;
    
    strncpy(buffer->words[buffer->word_count], word, len);
    buffer->words[buffer->word_count][len] = '\0';
    buffer->word_count++;
    return 1;
}

static void process_words(word_buffer_t* buffer) {
    for (int i = 0; i < buffer->word_count; i++) {
        char* word = buffer->words[i];
        for (int j = 0; word[j] != '\0'; j++) {
            word[j] = TO_UPPER(word[j]);
        }
    }
}

static void print_words(const word_buffer_t* buffer) {
    for (int i = 0; i < buffer->word_count; i++) {
        printf("%s", buffer->words[i]);
        if (i < buffer->word_count - 1) {
            printf(" ");
        }
    }
    printf("\n");
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    word_buffer_t buffer;
    int current_state = STATE_IDLE;
    
    printf("Enter text (max %d chars): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        TRANSITION(STATE_ERROR);
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    
    if (!validate_input(input)) {
        TRANSITION(STATE_ERROR);
        printf("Invalid input\n");
        return 1;
    }
    
    TRANSITION(STATE_READING);
    initialize_buffer(&buffer);
    
    char current_word[MAX_WORD_LEN];
    int word_len = 0;
    
    for (size_t i = 0; i <= len; i++) {
        char c = input[i];
        
        switch (current_state) {
            case STATE_READING:
                if (IS_VALID_CHAR(c)) {
                    if (word_len < MAX_WORD_LEN - 1) {
                        current_word[word_len++] = c;
                    }
                } else {
                    if (word_len > 0) {
                        if (!add_word(&buffer, current_word, word_len)) {
                            TRANSITION(STATE_ERROR);
                            break;
                        }
                        word_len = 0;
                    }
                    if (c == '\0') {
                        TRANSITION(STATE_PROCESSING);
                    }
                }
                break;
                
            case STATE_PROCESSING:
                process_words(&buffer);
                TRANSITION(STATE_WRITING);
                break;
                
            case STATE_WRITING:
                printf("Processed: ");
                print_words(&buffer);
                TRANSITION(STATE_IDLE);
                break;
                
            case STATE_ERROR:
                printf("Error occurred during processing\n");
                return 1;
                
            default:
                TRANSITION(STATE_ERROR);
                break;
        }
        
        if (current_state == STATE_ERROR) {
            break;
        }
    }
    
    if (current_state != STATE_IDLE && current_state != STATE_ERROR) {
        printf("Processing incomplete\n");
        return 1;
    }
    
    return 0;
}