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
#define STATE_DONE 5

#define MAX_INPUT_LEN 100
#define MAX_WORDS 50
#define MAX_WORD_LEN 20

#define TRANSITION(new_state) do { current_state = new_state; } while(0)
#define IS_VALID_CHAR(c) ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))
#define IS_TERMINATOR(c) (c == '\0' || c == '\n')

typedef struct {
    char words[MAX_WORDS][MAX_WORD_LEN];
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
        wl->words[i][0] = '\0';
    }
}

int add_word(WordList* wl, const char* word) {
    if (wl->count >= MAX_WORDS) return 0;
    size_t len = strlen(word);
    if (len == 0 || len >= MAX_WORD_LEN) return 0;
    
    strncpy(wl->words[wl->count], word, MAX_WORD_LEN - 1);
    wl->words[wl->count][MAX_WORD_LEN - 1] = '\0';
    wl->count++;
    return 1;
}

void process_state_machine(const char* input) {
    if (!validate_input(input)) {
        printf("Invalid input\n");
        return;
    }

    int current_state = STATE_IDLE;
    WordList wordlist;
    char current_word[MAX_WORD_LEN];
    int word_pos = 0;
    const char* ptr = input;
    
    initialize_wordlist(&wordlist);
    current_word[0] = '\0';

    while (current_state != STATE_DONE && current_state != STATE_ERROR) {
        switch (current_state) {
            case STATE_IDLE:
                if (*ptr != '\0' && *ptr != '\n') {
                    TRANSITION(STATE_READING);
                } else {
                    TRANSITION(STATE_DONE);
                }
                break;
                
            case STATE_READING:
                if (IS_VALID_CHAR(*ptr)) {
                    if (word_pos < MAX_WORD_LEN - 1) {
                        current_word[word_pos++] = *ptr;
                        current_word[word_pos] = '\0';
                    } else {
                        TRANSITION(STATE_ERROR);
                    }
                    ptr++;
                } else if (*ptr == ' ' || IS_TERMINATOR(*ptr)) {
                    if (word_pos > 0) {
                        TRANSITION(STATE_PROCESSING);
                    } else {
                        ptr++;
                        if (IS_TERMINATOR(*ptr)) {
                            TRANSITION(STATE_DONE);
                        } else {
                            TRANSITION(STATE_READING);
                        }
                    }
                } else {
                    TRANSITION(STATE_ERROR);
                }
                break;
                
            case STATE_PROCESSING:
                if (!add_word(&wordlist, current_word)) {
                    TRANSITION(STATE_ERROR);
                    break;
                }
                word_pos = 0;
                current_word[0] = '\0';
                TRANSITION(STATE_WRITING);
                break;
                
            case STATE_WRITING:
                printf("Word %d: %s\n", wordlist.count, wordlist.words[wordlist.count - 1]);
                if (IS_TERMINATOR(*ptr)) {
                    TRANSITION(STATE_DONE);
                } else {
                    ptr++;
                    TRANSITION(STATE_READING);
                }
                break;
                
            case STATE_ERROR:
                printf("Error processing input\n");
                TRANSITION(STATE_DONE);
                break;
                
            case STATE_DONE:
                break;
        }
    }
    
    if (current_state == STATE_DONE) {
        printf("Processing complete. Found %d words.\n", wordlist.count);
    }
}

int main() {
    char input[MAX_INPUT_LEN + 1];
    
    printf("Enter text (max %d chars): ", MAX_INPUT_LEN);
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    process_state_machine(input);
    return 0;
}