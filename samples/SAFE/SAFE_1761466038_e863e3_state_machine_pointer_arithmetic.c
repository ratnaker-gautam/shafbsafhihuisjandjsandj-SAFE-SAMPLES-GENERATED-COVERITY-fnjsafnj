//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 256
#define STATE_START 0
#define STATE_WORD 1
#define STATE_NUMBER 2
#define STATE_PUNCT 3
#define STATE_END 4

typedef struct {
    int state;
    int word_count;
    int number_count;
    int punct_count;
} state_machine_t;

void process_char(state_machine_t *sm, char c) {
    switch (sm->state) {
        case STATE_START:
            if (isalpha(c)) {
                sm->state = STATE_WORD;
                sm->word_count++;
            } else if (isdigit(c)) {
                sm->state = STATE_NUMBER;
                sm->number_count++;
            } else if (ispunct(c)) {
                sm->state = STATE_PUNCT;
                sm->punct_count++;
            }
            break;
            
        case STATE_WORD:
            if (!isalpha(c)) {
                if (isdigit(c)) {
                    sm->state = STATE_NUMBER;
                    sm->number_count++;
                } else if (ispunct(c)) {
                    sm->state = STATE_PUNCT;
                    sm->punct_count++;
                } else if (isspace(c)) {
                    sm->state = STATE_START;
                }
            }
            break;
            
        case STATE_NUMBER:
            if (!isdigit(c)) {
                if (isalpha(c)) {
                    sm->state = STATE_WORD;
                    sm->word_count++;
                } else if (ispunct(c)) {
                    sm->state = STATE_PUNCT;
                    sm->punct_count++;
                } else if (isspace(c)) {
                    sm->state = STATE_START;
                }
            }
            break;
            
        case STATE_PUNCT:
            if (!ispunct(c)) {
                if (isalpha(c)) {
                    sm->state = STATE_WORD;
                    sm->word_count++;
                } else if (isdigit(c)) {
                    sm->state = STATE_NUMBER;
                    sm->number_count++;
                } else if (isspace(c)) {
                    sm->state = STATE_START;
                }
            }
            break;
    }
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    state_machine_t sm = {0};
    
    printf("Enter text to analyze (max %d chars): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    
    if (len == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    char *ptr = input;
    char *end = ptr + len;
    
    while (ptr < end) {
        process_char(&sm, *ptr);
        ptr++;
    }
    
    printf("Analysis results:\n");
    printf("Words: %d\n", sm.word_count);
    printf("Numbers: %d\n", sm.number_count);
    printf("Punctuation sequences: %d\n", sm.punct_count);
    
    return 0;
}