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
#define IS_VALID_CHAR(c) ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == ' ' || c == '\n' || c == '\0')

typedef struct {
    char words[MAX_WORDS][MAX_WORD_LEN];
    int word_count;
    int char_count;
} Document;

static void initialize_document(Document *doc) {
    doc->word_count = 0;
    doc->char_count = 0;
    for (int i = 0; i < MAX_WORDS; i++) {
        memset(doc->words[i], 0, MAX_WORD_LEN);
    }
}

static int validate_input(const char *input) {
    if (input == NULL) return 0;
    size_t len = strlen(input);
    if (len == 0 || len > MAX_INPUT_LEN) return 0;
    for (size_t i = 0; i < len; i++) {
        if (!IS_VALID_CHAR(input[i])) return 0;
    }
    return 1;
}

static int process_input(Document *doc, const char *input) {
    if (doc == NULL || input == NULL) return 0;
    
    int state = STATE_IDLE;
    int word_idx = 0;
    int char_idx = 0;
    
    for (size_t i = 0; input[i] != '\0'; i++) {
        char c = input[i];
        
        switch (state) {
            case STATE_IDLE:
                if (c != ' ' && c != '\n') {
                    state = STATE_READING;
                    if (doc->word_count >= MAX_WORDS) return 0;
                    if (char_idx >= MAX_WORD_LEN - 1) return 0;
                    doc->words[doc->word_count][char_idx++] = c;
                    doc->char_count++;
                }
                break;
                
            case STATE_READING:
                if (c == ' ' || c == '\n') {
                    state = STATE_IDLE;
                    doc->words[doc->word_count][char_idx] = '\0';
                    doc->word_count++;
                    char_idx = 0;
                } else {
                    if (char_idx >= MAX_WORD_LEN - 1) return 0;
                    doc->words[doc->word_count][char_idx++] = c;
                    doc->char_count++;
                }
                break;
        }
    }
    
    if (state == STATE_READING && char_idx > 0) {
        doc->words[doc->word_count][char_idx] = '\0';
        doc->word_count++;
    }
    
    return 1;
}

static void print_document(const Document *doc) {
    if (doc == NULL || doc->word_count == 0) return;
    
    printf("Processed document (%d words, %d characters):\n", doc->word_count, doc->char_count);
    for (int i = 0; i < doc->word_count; i++) {
        printf("%s", doc->words[i]);
        if (i < doc->word_count - 1) printf(" ");
    }
    printf("\n");
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    Document doc;
    int current_state = STATE_IDLE;
    
    printf("Enter text to process (letters and spaces only, max %d chars): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        TRANSITION(STATE_ERROR);
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (!validate_input(input)) {
        TRANSITION(STATE_ERROR);
        fprintf(stderr, "Invalid input\n");
        return EXIT_FAILURE;
    }
    
    TRANSITION(STATE_READING);
    initialize_document(&doc);
    
    if (!process_input(&doc, input)) {
        TRANSITION(STATE_ERROR);
        fprintf(stderr, "Error processing input\n");
        return EXIT_FAILURE;
    }
    
    TRANSITION(STATE_PROCESSING);
    
    if (doc.word_count == 0) {
        TRANSITION(STATE_ERROR);
        fprintf(stderr, "No words found in input\n");
        return EXIT_FAILURE;
    }
    
    TRANSITION(STATE_WRITING);
    print_document(&doc);
    
    return EXIT_SUCCESS;
}