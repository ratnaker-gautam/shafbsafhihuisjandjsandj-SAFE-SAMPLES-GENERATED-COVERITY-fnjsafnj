//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT 256
#define MAX_STATES 8

typedef enum {
    STATE_START,
    STATE_READING_WORD,
    STATE_READING_NUMBER,
    STATE_PUNCTUATION,
    STATE_END,
    STATE_ERROR
} parser_state;

typedef enum {
    EVENT_ALPHA,
    EVENT_DIGIT,
    EVENT_SPACE,
    EVENT_PUNCT,
    EVENT_END,
    EVENT_INVALID
} parser_event;

static parser_event get_event(char c) {
    if (c == '\0') return EVENT_END;
    if (isspace(c)) return EVENT_SPACE;
    if (isalpha(c)) return EVENT_ALPHA;
    if (isdigit(c)) return EVENT_DIGIT;
    if (ispunct(c)) return EVENT_PUNCT;
    return EVENT_INVALID;
}

static void process_state(parser_state *current, parser_event ev, int *word_count, int *num_count, int *punct_count) {
    static const parser_state transition_table[MAX_STATES][6] = {
        {STATE_READING_WORD, STATE_ERROR, STATE_START, STATE_PUNCTUATION, STATE_END, STATE_ERROR},
        {STATE_READING_WORD, STATE_READING_NUMBER, STATE_START, STATE_PUNCTUATION, STATE_END, STATE_ERROR},
        {STATE_READING_WORD, STATE_READING_NUMBER, STATE_START, STATE_PUNCTUATION, STATE_END, STATE_ERROR},
        {STATE_READING_WORD, STATE_ERROR, STATE_START, STATE_PUNCTUATION, STATE_END, STATE_ERROR},
        {STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_END, STATE_ERROR},
        {STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_ERROR}
    };

    parser_state next = transition_table[*current][ev];
    
    if (*current == STATE_READING_WORD && next != STATE_READING_WORD) {
        if (word_count) (*word_count)++;
    } else if (*current == STATE_READING_NUMBER && next != STATE_READING_NUMBER) {
        if (num_count) (*num_count)++;
    } else if (*current == STATE_PUNCTUATION && next != STATE_PUNCTUATION) {
        if (punct_count) (*punct_count)++;
    }
    
    *current = next;
}

int main(void) {
    char input[MAX_INPUT + 1] = {0};
    char *ptr = input;
    int word_count = 0;
    int num_count = 0;
    int punct_count = 0;
    parser_state current_state = STATE_START;
    
    printf("Enter text to analyze: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (len >= sizeof(input)) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    while (*ptr != '\0' && current_state != STATE_END && current_state != STATE_ERROR) {
        parser_event ev = get_event(*ptr);
        process_state(&current_state, ev, &word_count, &num_count, &punct_count);
        
        if (current_state == STATE_ERROR) {
            fprintf(stderr, "Invalid input character: %c\n", *ptr);
            return 1;
        }
        
        ptr++;
    }
    
    if (*ptr == '\0' && current_state != STATE_END) {
        process_state(&current_state, EVENT_END, &word_count, &num_count, &punct_count);
    }
    
    if (current_state == STATE_ERROR) {
        fprintf(stderr, "Invalid input format\n");
        return 1;
    }
    
    printf("Analysis results:\n");
    printf("Words: %d\n", word_count);
    printf("Numbers: %d\n", num_count);
    printf("Punctuation sequences: %d\n", punct_count);
    
    return 0;
}