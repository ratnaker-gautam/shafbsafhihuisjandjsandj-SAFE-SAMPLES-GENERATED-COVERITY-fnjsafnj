//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

enum State {
    INIT,
    READING_NUMBER,
    READING_WORD,
    SKIPPING_SPACE,
    FINISHED,
    ERROR
};

struct Parser {
    enum State current_state;
    int number_count;
    int word_count;
    int current_number;
    char current_word[32];
    int word_pos;
};

void parser_init(struct Parser *p) {
    p->current_state = INIT;
    p->number_count = 0;
    p->word_count = 0;
    p->current_number = 0;
    memset(p->current_word, 0, sizeof(p->current_word));
    p->word_pos = 0;
}

int is_valid_char(char c) {
    return isalnum((unsigned char)c) || isspace((unsigned char)c) || c == '\0';
}

void process_char(struct Parser *p, char c) {
    if (!is_valid_char(c)) {
        p->current_state = ERROR;
        return;
    }

    switch (p->current_state) {
        case INIT:
            if (isdigit((unsigned char)c)) {
                p->current_state = READING_NUMBER;
                p->current_number = c - '0';
            } else if (isalpha((unsigned char)c)) {
                p->current_state = READING_WORD;
                p->current_word[0] = c;
                p->word_pos = 1;
            } else if (isspace((unsigned char)c)) {
                p->current_state = SKIPPING_SPACE;
            } else if (c == '\0') {
                p->current_state = FINISHED;
            }
            break;

        case READING_NUMBER:
            if (isdigit((unsigned char)c)) {
                if (p->current_number <= (INT_MAX - (c - '0')) / 10) {
                    p->current_number = p->current_number * 10 + (c - '0');
                } else {
                    p->current_state = ERROR;
                }
            } else if (isspace((unsigned char)c) || c == '\0') {
                p->number_count++;
                p->current_state = (c == '\0') ? FINISHED : SKIPPING_SPACE;
                p->current_number = 0;
            } else {
                p->current_state = ERROR;
            }
            break;

        case READING_WORD:
            if (isalpha((unsigned char)c)) {
                if (p->word_pos < 31) {
                    p->current_word[p->word_pos++] = c;
                } else {
                    p->current_state = ERROR;
                }
            } else if (isspace((unsigned char)c) || c == '\0') {
                p->current_word[p->word_pos] = '\0';
                p->word_count++;
                p->current_state = (c == '\0') ? FINISHED : SKIPPING_SPACE;
                p->word_pos = 0;
                memset(p->current_word, 0, sizeof(p->current_word));
            } else {
                p->current_state = ERROR;
            }
            break;

        case SKIPPING_SPACE:
            if (isdigit((unsigned char)c)) {
                p->current_state = READING_NUMBER;
                p->current_number = c - '0';
            } else if (isalpha((unsigned char)c)) {
                p->current_state = READING_WORD;
                p->current_word[0] = c;
                p->word_pos = 1;
            } else if (isspace((unsigned char)c)) {
                break;
            } else if (c == '\0') {
                p->current_state = FINISHED;
            } else {
                p->current_state = ERROR;
            }
            break;

        case FINISHED:
        case ERROR:
            break;
    }
}

int main(void) {
    char input[1024];
    struct Parser parser;
    
    printf("Enter text (max 1023 chars): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }

    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }

    parser_init(&parser);

    for (size_t i = 0; i <= len; i++) {
        char c = input[i];
        process_char(&parser, c);
        
        if (parser.current_state == ERROR) {
            fprintf(stderr, "Invalid input format\n");
            return 1;
        }
        
        if (parser.current_state == FINISHED) {
            break;
        }
    }

    printf("Numbers found: %d\n", parser.number_count);
    printf("Words found: %d\n", parser.word_count);

    return 0;
}