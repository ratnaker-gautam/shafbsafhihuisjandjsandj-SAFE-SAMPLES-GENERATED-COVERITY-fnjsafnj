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

void initialize_wordlist(WordList *wl) {
    wl->count = 0;
    for (int i = 0; i < MAX_WORDS; i++) {
        memset(wl->data[i], 0, MAX_WORD_LEN);
    }
}

int add_word(WordList *wl, const char *word) {
    if (wl->count >= MAX_WORDS || strlen(word) >= MAX_WORD_LEN) {
        return 0;
    }
    strncpy(wl->data[wl->count], word, MAX_WORD_LEN - 1);
    wl->data[wl->count][MAX_WORD_LEN - 1] = '\0';
    wl->count++;
    return 1;
}

void process_word(char *word) {
    for (int i = 0; word[i] != '\0'; i++) {
        word[i] = TO_UPPER(word[i]);
    }
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    char current_word[MAX_WORD_LEN];
    int word_pos = 0;
    int current_state = STATE_IDLE;
    WordList words;
    
    initialize_wordlist(&words);
    memset(input, 0, sizeof(input));
    memset(current_word, 0, sizeof(current_word));
    
    printf("Enter text (max %d chars): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        TRANSITION(STATE_ERROR);
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    
    if (len >= MAX_INPUT_LEN) {
        TRANSITION(STATE_ERROR);
    }
    
    for (size_t i = 0; i <= len; i++) {
        char c = input[i];
        
        switch (current_state) {
            case STATE_IDLE:
                if (IS_VALID_CHAR(c)) {
                    if (word_pos < MAX_WORD_LEN - 1) {
                        current_word[word_pos++] = c;
                        TRANSITION(STATE_READING);
                    } else {
                        TRANSITION(STATE_ERROR);
                    }
                } else if (c == '\0') {
                    TRANSITION(STATE_WRITING);
                }
                break;
                
            case STATE_READING:
                if (IS_VALID_CHAR(c)) {
                    if (word_pos < MAX_WORD_LEN - 1) {
                        current_word[word_pos++] = c;
                    } else {
                        TRANSITION(STATE_ERROR);
                    }
                } else {
                    current_word[word_pos] = '\0';
                    if (word_pos > 0) {
                        TRANSITION(STATE_PROCESSING);
                    } else {
                        TRANSITION(STATE_IDLE);
                    }
                    word_pos = 0;
                }
                break;
                
            case STATE_PROCESSING:
                process_word(current_word);
                if (!add_word(&words, current_word)) {
                    TRANSITION(STATE_ERROR);
                    break;
                }
                if (c == '\0') {
                    TRANSITION(STATE_WRITING);
                } else {
                    TRANSITION(STATE_IDLE);
                }
                break;
                
            case STATE_WRITING:
                printf("Processed words (%d):\n", words.count);
                for (int j = 0; j < words.count; j++) {
                    printf("%s\n", words.data[j]);
                }
                TRANSITION(STATE_IDLE);
                break;
                
            case STATE_ERROR:
                printf("Error: Invalid input or buffer overflow\n");
                return 1;
        }
        
        if (current_state == STATE_ERROR) {
            break;
        }
    }
    
    if (current_state != STATE_WRITING && current_state != STATE_ERROR) {
        TRANSITION(STATE_WRITING);
    }
    
    return 0;
}