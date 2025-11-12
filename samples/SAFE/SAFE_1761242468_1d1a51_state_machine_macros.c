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

#define TRANSITION(state_var, new_state) do { state_var = new_state; } while(0)
#define IS_VALID_CHAR(c) ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))
#define TO_UPPER(c) ((c >= 'a' && c <= 'z') ? (c - 32) : c)

typedef struct {
    char words[MAX_WORDS][MAX_WORD_LEN];
    int word_count;
} WordList;

static int validate_input(const char* input) {
    if (!input) return 0;
    size_t len = strlen(input);
    if (len == 0 || len > MAX_INPUT_LEN) return 0;
    return 1;
}

static void process_word(WordList* list, const char* word, size_t len) {
    if (!list || !word || len == 0 || len >= MAX_WORD_LEN || list->word_count >= MAX_WORDS) {
        return;
    }
    
    for (size_t i = 0; i < len; i++) {
        list->words[list->word_count][i] = TO_UPPER(word[i]);
    }
    list->words[list->word_count][len] = '\0';
    list->word_count++;
}

static void process_input(const char* input, WordList* list) {
    if (!validate_input(input) || !list) {
        return;
    }
    
    int current_state = STATE_IDLE;
    char current_word[MAX_WORD_LEN];
    size_t word_pos = 0;
    size_t input_len = strlen(input);
    
    for (size_t i = 0; i <= input_len; i++) {
        char c = input[i];
        
        switch (current_state) {
            case STATE_IDLE:
                if (IS_VALID_CHAR(c)) {
                    current_word[0] = TO_UPPER(c);
                    word_pos = 1;
                    TRANSITION(current_state, STATE_READING);
                }
                break;
                
            case STATE_READING:
                if (IS_VALID_CHAR(c)) {
                    if (word_pos < MAX_WORD_LEN - 1) {
                        current_word[word_pos++] = TO_UPPER(c);
                    } else {
                        TRANSITION(current_state, STATE_ERROR);
                    }
                } else {
                    current_word[word_pos] = '\0';
                    TRANSITION(current_state, STATE_PROCESSING);
                    if (i > 0) i--;
                }
                break;
                
            case STATE_PROCESSING:
                process_word(list, current_word, word_pos);
                word_pos = 0;
                if (i < input_len && IS_VALID_CHAR(input[i])) {
                    TRANSITION(current_state, STATE_READING);
                    if (i > 0) i--;
                } else {
                    TRANSITION(current_state, STATE_IDLE);
                }
                break;
                
            case STATE_ERROR:
                return;
                
            default:
                TRANSITION(current_state, STATE_ERROR);
                break;
        }
    }
    
    if (current_state == STATE_READING && word_pos > 0) {
        current_word[word_pos] = '\0';
        process_word(list, current_word, word_pos);
    }
}

static void print_results(const WordList* list) {
    if (!list || list->word_count == 0) {
        printf("No valid words found.\n");
        return;
    }
    
    printf("Found %d words:\n", list->word_count);
    for (int i = 0; i < list->word_count; i++) {
        printf("%d: %s\n", i + 1, list->words[i]);
    }
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    WordList word_list = {0};
    
    printf("Enter text to process: ");
    
    if (!fgets(input, sizeof(input), stdin)) {
        printf("Error reading input.\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (!validate_input(input)) {
        printf("Invalid input.\n");
        return 1;
    }
    
    process_input(input, &word_list);
    print_results(&word_list);
    
    return 0;
}