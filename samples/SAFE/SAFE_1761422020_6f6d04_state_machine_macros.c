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
#define IS_ALPHA(c) (((c) >= 'A' && (c) <= 'Z') || ((c) >= 'a' && (c) <= 'z'))

typedef struct {
    char words[MAX_WORDS][MAX_WORD_LEN];
    int word_count;
} WordBuffer;

static int validate_input(const char* input) {
    if (input == NULL) return 0;
    size_t len = strlen(input);
    if (len == 0 || len > MAX_INPUT_LEN) return 0;
    return 1;
}

static void initialize_buffer(WordBuffer* buffer) {
    buffer->word_count = 0;
    for (int i = 0; i < MAX_WORDS; i++) {
        memset(buffer->words[i], 0, MAX_WORD_LEN);
    }
}

static int add_word(WordBuffer* buffer, const char* word, size_t len) {
    if (buffer->word_count >= MAX_WORDS || len >= MAX_WORD_LEN) return 0;
    if (strncpy(buffer->words[buffer->word_count], word, len) == NULL) return 0;
    buffer->words[buffer->word_count][len] = '\0';
    buffer->word_count++;
    return 1;
}

static void process_state_idle(WordBuffer* buffer, int* current_state) {
    printf("Enter text to process (max %d chars): ", MAX_INPUT_LEN);
    TRANSITION(STATE_READING);
}

static void process_state_reading(WordBuffer* buffer, int* current_state, char* input) {
    if (fgets(input, MAX_INPUT_LEN, stdin) == NULL) {
        TRANSITION(STATE_ERROR);
        return;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len-1] == '\n') {
        input[len-1] = '\0';
        len--;
    }
    
    if (!validate_input(input)) {
        TRANSITION(STATE_ERROR);
        return;
    }
    
    TRANSITION(STATE_PROCESSING);
}

static void process_state_processing(WordBuffer* buffer, int* current_state, const char* input) {
    initialize_buffer(buffer);
    
    const char* ptr = input;
    char current_word[MAX_WORD_LEN];
    int word_len = 0;
    
    while (*ptr != '\0') {
        if (IS_ALPHA(*ptr)) {
            if (word_len < MAX_WORD_LEN - 1) {
                current_word[word_len++] = *ptr;
            }
        } else {
            if (word_len > 0) {
                current_word[word_len] = '\0';
                if (!add_word(buffer, current_word, word_len)) {
                    TRANSITION(STATE_ERROR);
                    return;
                }
                word_len = 0;
            }
        }
        ptr++;
    }
    
    if (word_len > 0) {
        current_word[word_len] = '\0';
        if (!add_word(buffer, current_word, word_len)) {
            TRANSITION(STATE_ERROR);
            return;
        }
    }
    
    TRANSITION(STATE_WRITING);
}

static void process_state_writing(WordBuffer* buffer, int* current_state) {
    printf("Found %d words:\n", buffer->word_count);
    for (int i = 0; i < buffer->word_count; i++) {
        printf("%d: %s\n", i + 1, buffer->words[i]);
    }
    TRANSITION(STATE_IDLE);
}

static void process_state_error(WordBuffer* buffer, int* current_state) {
    printf("Error: Invalid input or processing error\n");
    TRANSITION(STATE_IDLE);
}

int main(void) {
    int current_state = STATE_IDLE;
    WordBuffer buffer;
    char input[MAX_INPUT_LEN + 2];
    
    printf("Word Processor State Machine\n");
    printf("Type 'quit' to exit\n\n");
    
    while (1) {
        switch (current_state) {
            case STATE_IDLE:
                process_state_idle(&buffer, &current_state);
                break;
            case STATE_READING:
                process_state_reading(&buffer, &current_state, input);
                break;
            case STATE_PROCESSING:
                process_state_processing(&buffer, &current_state, input);
                break;
            case STATE_WRITING:
                process_state_writing(&buffer, &current_state);
                break;
            case STATE_ERROR:
                process_state_error(&buffer, &current_state);
                break;
            default:
                current_state = STATE_ERROR;
                break;
        }
        
        if (current_state == STATE_READING && strcmp(input, "quit") == 0) {
            break;
        }
    }
    
    printf("Goodbye!\n");
    return 0;
}