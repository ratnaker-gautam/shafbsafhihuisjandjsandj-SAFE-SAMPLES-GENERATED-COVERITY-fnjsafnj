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
#define IS_VALID_CHAR(c) ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == ' ' || c == '\n')

typedef struct {
    char words[MAX_WORDS][MAX_WORD_LEN];
    int word_count;
    char current_word[MAX_WORD_LEN];
    int word_pos;
} WordProcessor;

void initialize_processor(WordProcessor *wp) {
    wp->word_count = 0;
    wp->word_pos = 0;
    memset(wp->words, 0, sizeof(wp->words));
    memset(wp->current_word, 0, sizeof(wp->current_word));
}

int add_word(WordProcessor *wp) {
    if (wp->word_count >= MAX_WORDS) {
        return -1;
    }
    if (wp->word_pos == 0) {
        return 0;
    }
    if (wp->word_pos >= MAX_WORD_LEN) {
        return -1;
    }
    
    strncpy(wp->words[wp->word_count], wp->current_word, MAX_WORD_LEN - 1);
    wp->words[wp->word_count][MAX_WORD_LEN - 1] = '\0';
    wp->word_count++;
    wp->word_pos = 0;
    memset(wp->current_word, 0, sizeof(wp->current_word));
    return 0;
}

int process_char(WordProcessor *wp, char c) {
    if (!IS_VALID_CHAR(c)) {
        return -1;
    }
    
    if (c == ' ' || c == '\n') {
        if (add_word(wp) != 0) {
            return -1;
        }
    } else {
        if (wp->word_pos >= MAX_WORD_LEN - 1) {
            return -1;
        }
        wp->current_word[wp->word_pos] = c;
        wp->word_pos++;
        wp->current_word[wp->word_pos] = '\0';
    }
    return 0;
}

void display_words(WordProcessor *wp) {
    printf("Processed words (%d):\n", wp->word_count);
    for (int i = 0; i < wp->word_count; i++) {
        printf("%d: %s\n", i + 1, wp->words[i]);
    }
}

int main(void) {
    WordProcessor processor;
    int current_state = STATE_IDLE;
    char input[MAX_INPUT_LEN + 1];
    int input_len;
    
    initialize_processor(&processor);
    
    printf("Enter text (max %d chars): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        TRANSITION(STATE_ERROR);
    } else {
        TRANSITION(STATE_READING);
    }
    
    if (current_state == STATE_READING) {
        input_len = strlen(input);
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
        for (int i = 0; i < input_len; i++) {
            if (process_char(&processor, input[i]) != 0) {
                TRANSITION(STATE_ERROR);
                break;
            }
        }
        
        if (current_state != STATE_ERROR) {
            if (add_word(&processor) != 0) {
                TRANSITION(STATE_ERROR);
            } else {
                TRANSITION(STATE_WRITING);
            }
        }
    }
    
    if (current_state == STATE_WRITING) {
        display_words(&processor);
        printf("Processing completed successfully.\n");
    } else if (current_state == STATE_ERROR) {
        printf("Error: Invalid input or processing error occurred.\n");
        return 1;
    }
    
    return 0;
}