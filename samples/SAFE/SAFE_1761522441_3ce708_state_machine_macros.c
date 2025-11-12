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
    size_t count;
} WordList;

static void initialize_wordlist(WordList *wl) {
    wl->count = 0;
    for (size_t i = 0; i < MAX_WORDS; i++) {
        memset(wl->data[i], 0, MAX_WORD_LEN);
    }
}

static int add_word(WordList *wl, const char *word, size_t len) {
    if (wl->count >= MAX_WORDS || len >= MAX_WORD_LEN) {
        return 0;
    }
    if (len > 0) {
        strncpy(wl->data[wl->count], word, len);
        wl->data[wl->count][len] = '\0';
        wl->count++;
    }
    return 1;
}

static void process_words(WordList *wl) {
    for (size_t i = 0; i < wl->count; i++) {
        for (size_t j = 0; wl->data[i][j] != '\0'; j++) {
            wl->data[i][j] = TO_UPPER(wl->data[i][j]);
        }
    }
}

static void print_words(const WordList *wl) {
    for (size_t i = 0; i < wl->count; i++) {
        printf("%s\n", wl->data[i]);
    }
}

int main(void) {
    int current_state = STATE_IDLE;
    char input[MAX_INPUT_LEN + 1];
    WordList words;
    size_t input_len = 0;
    size_t word_start = 0;
    int in_word = 0;
    
    initialize_wordlist(&words);
    
    printf("Enter text (max %d chars): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        TRANSITION(STATE_ERROR);
    } else {
        TRANSITION(STATE_READING);
    }
    
    while (current_state != STATE_ERROR) {
        switch (current_state) {
            case STATE_READING:
                input_len = strlen(input);
                if (input_len > 0 && input[input_len - 1] == '\n') {
                    input[input_len - 1] = '\0';
                    input_len--;
                }
                
                if (input_len > MAX_INPUT_LEN) {
                    TRANSITION(STATE_ERROR);
                    break;
                }
                
                for (size_t i = 0; i <= input_len; i++) {
                    if (i < input_len && IS_VALID_CHAR(input[i])) {
                        if (!in_word) {
                            word_start = i;
                            in_word = 1;
                        }
                    } else {
                        if (in_word) {
                            size_t word_len = i - word_start;
                            if (!add_word(&words, &input[word_start], word_len)) {
                                TRANSITION(STATE_ERROR);
                                break;
                            }
                            in_word = 0;
                        }
                    }
                }
                
                if (current_state != STATE_ERROR) {
                    TRANSITION(STATE_PROCESSING);
                }
                break;
                
            case STATE_PROCESSING:
                process_words(&words);
                TRANSITION(STATE_WRITING);
                break;
                
            case STATE_WRITING:
                printf("Processed words:\n");
                print_words(&words);
                TRANSITION(STATE_IDLE);
                break;
                
            case STATE_IDLE:
                printf("Processing complete.\n");
                return EXIT_SUCCESS;
                
            default:
                TRANSITION(STATE_ERROR);
                break;
        }
    }
    
    if (current_state == STATE_ERROR) {
        fprintf(stderr, "Error: Processing failed\n");
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}