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
#define IS_VALID_CHAR(c) ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || c == ' ')

typedef struct {
    char words[MAX_WORDS][MAX_WORD_LEN];
    int count;
} WordList;

void initialize_wordlist(WordList *wl) {
    wl->count = 0;
    for (int i = 0; i < MAX_WORDS; i++) {
        memset(wl->words[i], 0, MAX_WORD_LEN);
    }
}

int add_word(WordList *wl, const char *word) {
    if (wl->count >= MAX_WORDS || word == NULL) return 0;
    size_t len = strlen(word);
    if (len == 0 || len >= MAX_WORD_LEN) return 0;
    strncpy(wl->words[wl->count], word, MAX_WORD_LEN - 1);
    wl->words[wl->count][MAX_WORD_LEN - 1] = '\0';
    wl->count++;
    return 1;
}

int validate_input(const char *input) {
    if (input == NULL) return 0;
    size_t len = strlen(input);
    if (len == 0 || len > MAX_INPUT_LEN) return 0;
    for (size_t i = 0; i < len; i++) {
        if (!IS_VALID_CHAR(input[i])) return 0;
    }
    return 1;
}

int process_input(const char *input, WordList *wl) {
    if (!validate_input(input)) return 0;
    
    char buffer[MAX_INPUT_LEN + 1];
    strncpy(buffer, input, MAX_INPUT_LEN);
    buffer[MAX_INPUT_LEN] = '\0';
    
    char *token = strtok(buffer, " ");
    while (token != NULL && wl->count < MAX_WORDS) {
        if (!add_word(wl, token)) return 0;
        token = strtok(NULL, " ");
    }
    return 1;
}

void print_words(const WordList *wl) {
    if (wl->count == 0) {
        printf("No words to display.\n");
        return;
    }
    printf("Processed words (%d):\n", wl->count);
    for (int i = 0; i < wl->count; i++) {
        printf("%d: %s\n", i + 1, wl->words[i]);
    }
}

int main(void) {
    int current_state = STATE_IDLE;
    char input[MAX_INPUT_LEN + 1];
    WordList wordlist;
    
    initialize_wordlist(&wordlist);
    
    printf("Word Processor State Machine\n");
    printf("Enter text (alphanumeric and spaces only, max %d chars):\n", MAX_INPUT_LEN);
    
    while (current_state != STATE_ERROR) {
        switch (current_state) {
            case STATE_IDLE:
                printf("> ");
                if (fgets(input, sizeof(input), stdin) != NULL) {
                    size_t len = strlen(input);
                    if (len > 0 && input[len - 1] == '\n') {
                        input[len - 1] = '\0';
                    }
                    if (strlen(input) > 0) {
                        TRANSITION(STATE_READING);
                    } else {
                        printf("Empty input. Try again.\n");
                    }
                } else {
                    TRANSITION(STATE_ERROR);
                }
                break;
                
            case STATE_READING:
                if (validate_input(input)) {
                    TRANSITION(STATE_PROCESSING);
                } else {
                    printf("Invalid input. Only alphanumeric characters and spaces allowed.\n");
                    TRANSITION(STATE_IDLE);
                }
                break;
                
            case STATE_PROCESSING:
                initialize_wordlist(&wordlist);
                if (process_input(input, &wordlist)) {
                    TRANSITION(STATE_WRITING);
                } else {
                    printf("Processing failed.\n");
                    TRANSITION(STATE_ERROR);
                }
                break;
                
            case STATE_WRITING:
                print_words(&wordlist);
                TRANSITION(STATE_IDLE);
                break;
                
            default:
                TRANSITION(STATE_ERROR);
                break;
        }
    }
    
    printf("State machine terminated due to error.\n");
    return 1;
}