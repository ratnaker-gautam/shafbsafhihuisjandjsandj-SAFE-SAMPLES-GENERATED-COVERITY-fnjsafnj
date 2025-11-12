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
} parser_t;

void init_parser(parser_t *p) {
    p->state = STATE_START;
    p->word_count = 0;
    p->number_count = 0;
    p->punct_count = 0;
}

int is_valid_input(const char *input) {
    if (input == NULL) return 0;
    size_t len = strlen(input);
    if (len == 0 || len >= MAX_INPUT_LEN) return 0;
    return 1;
}

void process_char(parser_t *p, char c) {
    switch (p->state) {
        case STATE_START:
            if (isalpha(c)) {
                p->state = STATE_WORD;
                p->word_count++;
            } else if (isdigit(c)) {
                p->state = STATE_NUMBER;
                p->number_count++;
            } else if (ispunct(c)) {
                p->state = STATE_PUNCT;
                p->punct_count++;
            }
            break;
            
        case STATE_WORD:
            if (!isalpha(c)) {
                if (isdigit(c)) {
                    p->state = STATE_NUMBER;
                    p->number_count++;
                } else if (ispunct(c)) {
                    p->state = STATE_PUNCT;
                    p->punct_count++;
                } else if (isspace(c)) {
                    p->state = STATE_START;
                }
            }
            break;
            
        case STATE_NUMBER:
            if (!isdigit(c)) {
                if (isalpha(c)) {
                    p->state = STATE_WORD;
                    p->word_count++;
                } else if (ispunct(c)) {
                    p->state = STATE_PUNCT;
                    p->punct_count++;
                } else if (isspace(c)) {
                    p->state = STATE_START;
                }
            }
            break;
            
        case STATE_PUNCT:
            if (!ispunct(c)) {
                if (isalpha(c)) {
                    p->state = STATE_WORD;
                    p->word_count++;
                } else if (isdigit(c)) {
                    p->state = STATE_NUMBER;
                    p->number_count++;
                } else if (isspace(c)) {
                    p->state = STATE_START;
                }
            }
            break;
    }
}

void process_string(parser_t *p, const char *str) {
    if (str == NULL || p == NULL) return;
    
    const char *ptr = str;
    while (*ptr != '\0') {
        process_char(p, *ptr);
        ptr++;
    }
    p->state = STATE_END;
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    parser_t parser;
    
    printf("Enter text to analyze: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (!is_valid_input(input)) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    init_parser(&parser);
    process_string(&parser, input);
    
    printf("Analysis results:\n");
    printf("Words: %d\n", parser.word_count);
    printf("Numbers: %d\n", parser.number_count);
    printf("Punctuation sequences: %d\n", parser.punct_count);
    
    return 0;
}