//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

#define STATE_IDLE 0
#define STATE_READING 1
#define STATE_PROCESSING 2
#define STATE_DONE 3
#define STATE_ERROR 4

#define MAX_INPUT_LEN 100
#define MAX_WORDS 20
#define MAX_WORD_LEN 20

#define TRANSITION(new_state) do { current_state = new_state; } while(0)
#define IS_VALID_CHAR(c) (isalnum((unsigned char)c) || (c) == ' ' || (c) == '\n')
#define IS_TERMINATOR(c) ((c) == '\n' || (c) == '\0')

typedef struct {
    char words[MAX_WORDS][MAX_WORD_LEN];
    int count;
} WordList;

int validate_input(const char* input) {
    if (input == NULL) return 0;
    size_t len = strlen(input);
    if (len == 0 || len > MAX_INPUT_LEN) return 0;
    for (size_t i = 0; i < len; i++) {
        if (!IS_VALID_CHAR(input[i])) return 0;
    }
    return 1;
}

void init_wordlist(WordList* wl) {
    wl->count = 0;
    for (int i = 0; i < MAX_WORDS; i++) {
        memset(wl->words[i], 0, MAX_WORD_LEN);
    }
}

int add_word(WordList* wl, const char* word, size_t len) {
    if (wl->count >= MAX_WORDS || len == 0 || len >= MAX_WORD_LEN) return 0;
    strncpy(wl->words[wl->count], word, len);
    wl->words[wl->count][len] = '\0';
    wl->count++;
    return 1;
}

void process_state_machine(const char* input, WordList* result) {
    int current_state = STATE_IDLE;
    char current_word[MAX_WORD_LEN];
    size_t word_pos = 0;
    size_t i = 0;
    
    init_wordlist(result);
    
    while (current_state != STATE_DONE && current_state != STATE_ERROR) {
        switch (current_state) {
            case STATE_IDLE:
                if (input[i] == '\0' || input[i] == '\n') {
                    TRANSITION(STATE_DONE);
                } else if (isalnum((unsigned char)input[i])) {
                    current_word[0] = input[i];
                    word_pos = 1;
                    TRANSITION(STATE_READING);
                } else if (input[i] == ' ') {
                    i++;
                } else {
                    TRANSITION(STATE_ERROR);
                }
                break;
                
            case STATE_READING:
                if (isalnum((unsigned char)input[i])) {
                    if (word_pos < MAX_WORD_LEN - 1) {
                        current_word[word_pos++] = input[i];
                        i++;
                    } else {
                        TRANSITION(STATE_ERROR);
                    }
                } else if (input[i] == ' ' || IS_TERMINATOR(input[i])) {
                    current_word[word_pos] = '\0';
                    if (!add_word(result, current_word, word_pos)) {
                        TRANSITION(STATE_ERROR);
                        break;
                    }
                    word_pos = 0;
                    TRANSITION(STATE_PROCESSING);
                } else {
                    TRANSITION(STATE_ERROR);
                }
                break;
                
            case STATE_PROCESSING:
                if (input[i] == ' ') {
                    i++;
                    TRANSITION(STATE_IDLE);
                } else if (IS_TERMINATOR(input[i])) {
                    TRANSITION(STATE_DONE);
                } else {
                    TRANSITION(STATE_ERROR);
                }
                break;
                
            default:
                TRANSITION(STATE_ERROR);
                break;
        }
    }
}

void print_results(const WordList* wl) {
    if (wl->count == 0) {
        printf("No words found.\n");
        return;
    }
    
    printf("Found %d words:\n", wl->count);
    for (int i = 0; i < wl->count; i++) {
        printf("%d: '%s'\n", i + 1, wl->words[i]);
    }
}

int main(void) {
    char input[MAX_INPUT_LEN + 2];
    WordList word_list;
    
    printf("Enter text (alphanumeric words separated by spaces): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (!validate_input(input)) {
        fprintf(stderr, "Invalid input: must contain only alphanumeric characters and spaces, max %d characters\n", MAX_INPUT_LEN);
        return EXIT_FAILURE;
    }
    
    process_state_machine(input, &word_list);
    
    print_results(&word_list);
    
    return EXIT_SUCCESS;
}