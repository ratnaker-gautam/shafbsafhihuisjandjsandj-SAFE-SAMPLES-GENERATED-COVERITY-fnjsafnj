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

#define TRANSITION(new_state) do { state = new_state; } while(0)
#define IS_VALID_CHAR(c) ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))
#define IS_TERMINATOR(c) (c == '\n' || c == '\0')

typedef struct {
    char words[MAX_WORDS][MAX_WORD_LEN];
    int word_count;
    int total_chars;
} Document;

void initialize_document(Document *doc) {
    if (doc == NULL) return;
    doc->word_count = 0;
    doc->total_chars = 0;
    for (int i = 0; i < MAX_WORDS; i++) {
        memset(doc->words[i], 0, MAX_WORD_LEN);
    }
}

int process_input(const char *input, Document *doc) {
    if (input == NULL || doc == NULL) {
        return STATE_ERROR;
    }
    
    int state = STATE_IDLE;
    int word_index = 0;
    int char_index = 0;
    
    for (int i = 0; input[i] != '\0'; i++) {
        char c = input[i];
        
        switch (state) {
            case STATE_IDLE:
                if (IS_VALID_CHAR(c)) {
                    if (doc->word_count >= MAX_WORDS) {
                        return STATE_ERROR;
                    }
                    if (MAX_WORD_LEN > 0) {
                        doc->words[doc->word_count][0] = c;
                    }
                    char_index = 1;
                    doc->total_chars++;
                    if (doc->total_chars < 0) {
                        return STATE_ERROR;
                    }
                    TRANSITION(STATE_READING);
                } else if (!IS_TERMINATOR(c) && c != ' ') {
                    return STATE_ERROR;
                }
                break;
                
            case STATE_READING:
                if (IS_VALID_CHAR(c)) {
                    if (char_index >= MAX_WORD_LEN - 1) {
                        return STATE_ERROR;
                    }
                    if (char_index < MAX_WORD_LEN) {
                        doc->words[doc->word_count][char_index] = c;
                    }
                    char_index++;
                    doc->total_chars++;
                    if (doc->total_chars < 0) {
                        return STATE_ERROR;
                    }
                } else if (c == ' ') {
                    if (doc->word_count < MAX_WORDS - 1) {
                        doc->word_count++;
                    }
                    char_index = 0;
                    TRANSITION(STATE_IDLE);
                } else if (IS_TERMINATOR(c)) {
                    if (doc->word_count < MAX_WORDS) {
                        doc->word_count++;
                    }
                    TRANSITION(STATE_PROCESSING);
                } else {
                    return STATE_ERROR;
                }
                break;
                
            case STATE_PROCESSING:
                break;
                
            default:
                return STATE_ERROR;
        }
        
        if (doc->total_chars >= MAX_INPUT_LEN) {
            return STATE_ERROR;
        }
    }
    
    if (state == STATE_READING) {
        if (doc->word_count < MAX_WORDS) {
            doc->word_count++;
        }
    }
    
    return STATE_PROCESSING;
}

void print_document(const Document *doc) {
    if (doc == NULL) return;
    printf("Processed %d words with %d total characters:\n", 
           doc->word_count, doc->total_chars);
    for (int i = 0; i < doc->word_count && i < MAX_WORDS; i++) {
        if (doc->words[i][0] != '\0') {
            printf("Word %d: %s\n", i + 1, doc->words[i]);
        }
    }
}

int main(void) {
    char input[MAX_INPUT_LEN + 2];
    Document doc;
    int result;
    
    printf("Enter text (max %d chars): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (len >= MAX_INPUT_LEN) {
        fprintf(stderr, "Input too long\n");
        return EXIT_FAILURE;
    }
    
    initialize_document(&doc);
    result = process_input(input, &doc);
    
    if (result == STATE_ERROR) {
        fprintf(stderr, "Error processing input\n");
        return EXIT_FAILURE;
    }
    
    print_document(&doc);
    return EXIT_SUCCESS;
}