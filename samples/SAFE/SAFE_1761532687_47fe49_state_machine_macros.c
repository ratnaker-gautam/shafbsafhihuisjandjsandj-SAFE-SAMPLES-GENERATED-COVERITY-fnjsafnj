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

void initialize_wordlist(WordList* list) {
    list->count = 0;
    for (int i = 0; i < MAX_WORDS; i++) {
        memset(list->data[i], 0, MAX_WORD_LEN);
    }
}

int add_word(WordList* list, const char* word) {
    if (list->count >= MAX_WORDS) return 0;
    size_t len = strlen(word);
    if (len == 0 || len >= MAX_WORD_LEN) return 0;
    
    strncpy(list->data[list->count], word, MAX_WORD_LEN - 1);
    list->data[list->count][MAX_WORD_LEN - 1] = '\0';
    list->count++;
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
    
    printf("Enter text (max %d chars): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        TRANSITION(STATE_ERROR);
    }
    
    if (current_state != STATE_ERROR) {
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
    
    initialize_wordlist(&words);
    memset(current_word, 0, sizeof(current_word));
    word_pos = 0;
    
    if (current_state == STATE_READING) {
        TRANSITION(STATE_PROCESSING);
        
        for (int i = 0; input[i] != '\0' && current_state == STATE_PROCESSING; i++) {
            char c = input[i];
            
            if (IS_VALID_CHAR(c)) {
                if (word_pos < MAX_WORD_LEN - 1) {
                    current_word[word_pos++] = c;
                } else {
                    TRANSITION(STATE_ERROR);
                    break;
                }
            } else if (word_pos > 0) {
                current_word[word_pos] = '\0';
                if (!add_word(&words, current_word)) {
                    TRANSITION(STATE_ERROR);
                    break;
                }
                process_word(words.data[words.count - 1]);
                word_pos = 0;
                memset(current_word, 0, sizeof(current_word));
            }
        }
        
        if (current_state == STATE_PROCESSING && word_pos > 0) {
            current_word[word_pos] = '\0';
            if (!add_word(&words, current_word)) {
                TRANSITION(STATE_ERROR);
            } else {
                process_word(words.data[words.count - 1]);
            }
        }
        
        if (current_state == STATE_PROCESSING) {
            TRANSITION(STATE_WRITING);
        }
    }
    
    if (current_state == STATE_WRITING) {
        printf("Processed words (%d):\n", words.count);
        for (int i = 0; i < words.count; i++) {
            printf("%s\n", words.data[i]);
        }
    } else if (current_state == STATE_ERROR) {
        printf("Error: Invalid input or processing error\n");
        return 1;
    }
    
    return 0;
}