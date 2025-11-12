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
#define TO_UPPER(c) ((c >= 'a' && c <= 'z') ? (c - 32) : c)

typedef struct {
    char data[MAX_WORDS][MAX_WORD_LEN];
    int count;
} WordList;

int validate_input(const char* input) {
    if (input == NULL) return 0;
    size_t len = strlen(input);
    if (len == 0 || len > MAX_INPUT_LEN) return 0;
    return 1;
}

void initialize_wordlist(WordList* wl) {
    wl->count = 0;
    for (int i = 0; i < MAX_WORDS; i++) {
        memset(wl->data[i], 0, MAX_WORD_LEN);
    }
}

int add_word(WordList* wl, const char* word) {
    if (wl->count >= MAX_WORDS) return 0;
    size_t len = strlen(word);
    if (len == 0 || len >= MAX_WORD_LEN) return 0;
    strncpy(wl->data[wl->count], word, MAX_WORD_LEN - 1);
    wl->data[wl->count][MAX_WORD_LEN - 1] = '\0';
    wl->count++;
    return 1;
}

void process_word(char* word) {
    if (word == NULL || strlen(word) == 0) return;
    for (char* p = word; *p; p++) {
        *p = TO_UPPER(*p);
    }
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    WordList words;
    int current_state = STATE_IDLE;
    char current_word[MAX_WORD_LEN];
    int word_pos = 0;
    
    initialize_wordlist(&words);
    memset(input, 0, sizeof(input));
    memset(current_word, 0, sizeof(current_word));
    
    printf("Enter text (max %d chars): ", MAX_INPUT_LEN);
    if (fgets(input, sizeof(input), stdin) == NULL) {
        TRANSITION(STATE_ERROR);
    } else {
        size_t len = strlen(input);
        if (len > 0 && input[len - 1] == '\n') {
            input[len - 1] = '\0';
        }
        if (!validate_input(input)) {
            TRANSITION(STATE_ERROR);
        } else {
            TRANSITION(STATE_READING);
        }
    }
    
    for (int i = 0; input[i] != '\0' && current_state != STATE_ERROR; i++) {
        switch (current_state) {
            case STATE_READING:
                if (IS_VALID_CHAR(input[i])) {
                    if (word_pos < MAX_WORD_LEN - 1) {
                        current_word[word_pos++] = input[i];
                    } else {
                        TRANSITION(STATE_ERROR);
                    }
                } else {
                    if (word_pos > 0) {
                        current_word[word_pos] = '\0';
                        TRANSITION(STATE_PROCESSING);
                        i--;
                    }
                }
                break;
                
            case STATE_PROCESSING:
                process_word(current_word);
                if (!add_word(&words, current_word)) {
                    TRANSITION(STATE_ERROR);
                } else {
                    word_pos = 0;
                    memset(current_word, 0, sizeof(current_word));
                    TRANSITION(STATE_READING);
                }
                break;
                
            default:
                TRANSITION(STATE_ERROR);
                break;
        }
    }
    
    if (current_state == STATE_READING && word_pos > 0) {
        current_word[word_pos] = '\0';
        process_word(current_word);
        if (!add_word(&words, current_word)) {
            TRANSITION(STATE_ERROR);
        }
    }
    
    if (current_state == STATE_ERROR) {
        printf("Error: Invalid input or processing error\n");
        return 1;
    }
    
    TRANSITION(STATE_WRITING);
    printf("Processed words (%d):\n", words.count);
    for (int i = 0; i < words.count; i++) {
        printf("%s\n", words.data[i]);
    }
    
    return 0;
}