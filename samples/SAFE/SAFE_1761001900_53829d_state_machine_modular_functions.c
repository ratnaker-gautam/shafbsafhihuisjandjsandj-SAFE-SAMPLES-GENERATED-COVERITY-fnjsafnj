//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: modular_functions ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 100
#define MAX_STATES 10

typedef enum {
    STATE_START,
    STATE_READING_NUMBER,
    STATE_READING_WORD,
    STATE_ERROR,
    STATE_END
} State;

typedef enum {
    EVENT_DIGIT,
    EVENT_LETTER,
    EVENT_SPACE,
    EVENT_EOF,
    EVENT_INVALID
} Event;

typedef struct {
    int number_count;
    int word_count;
    int error_count;
} Statistics;

State handle_start(Event ev, char ch, Statistics* stats);
State handle_reading_number(Event ev, char ch, Statistics* stats);
State handle_reading_word(Event ev, char ch, Statistics* stats);
State handle_error(Event ev, char ch, Statistics* stats);
State handle_end(Event ev, char ch, Statistics* stats);

Event get_event(char ch);
void print_statistics(const Statistics* stats);

int main(void) {
    State current_state = STATE_START;
    Statistics stats = {0, 0, 0};
    char input[MAX_INPUT_LEN + 1];
    
    printf("Enter text (max %d chars): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    
    for (size_t i = 0; i <= len; i++) {
        char ch = (i < len) ? input[i] : '\0';
        Event ev = get_event(ch);
        
        switch (current_state) {
            case STATE_START:
                current_state = handle_start(ev, ch, &stats);
                break;
            case STATE_READING_NUMBER:
                current_state = handle_reading_number(ev, ch, &stats);
                break;
            case STATE_READING_WORD:
                current_state = handle_reading_word(ev, ch, &stats);
                break;
            case STATE_ERROR:
                current_state = handle_error(ev, ch, &stats);
                break;
            case STATE_END:
                current_state = handle_end(ev, ch, &stats);
                break;
        }
        
        if (current_state == STATE_END) {
            break;
        }
    }
    
    print_statistics(&stats);
    return 0;
}

Event get_event(char ch) {
    if (ch == '\0') {
        return EVENT_EOF;
    } else if (isspace((unsigned char)ch)) {
        return EVENT_SPACE;
    } else if (isdigit((unsigned char)ch)) {
        return EVENT_DIGIT;
    } else if (isalpha((unsigned char)ch)) {
        return EVENT_LETTER;
    } else {
        return EVENT_INVALID;
    }
}

State handle_start(Event ev, char ch, Statistics* stats) {
    switch (ev) {
        case EVENT_DIGIT:
            return STATE_READING_NUMBER;
        case EVENT_LETTER:
            return STATE_READING_WORD;
        case EVENT_SPACE:
            return STATE_START;
        case EVENT_EOF:
            return STATE_END;
        case EVENT_INVALID:
            stats->error_count++;
            return STATE_ERROR;
    }
    return STATE_START;
}

State handle_reading_number(Event ev, char ch, Statistics* stats) {
    switch (ev) {
        case EVENT_DIGIT:
            return STATE_READING_NUMBER;
        case EVENT_SPACE:
            stats->number_count++;
            return STATE_START;
        case EVENT_EOF:
            stats->number_count++;
            return STATE_END;
        case EVENT_LETTER:
        case EVENT_INVALID:
            stats->error_count++;
            return STATE_ERROR;
    }
    return STATE_READING_NUMBER;
}

State handle_reading_word(Event ev, char ch, Statistics* stats) {
    switch (ev) {
        case EVENT_LETTER:
            return STATE_READING_WORD;
        case EVENT_SPACE:
            stats->word_count++;
            return STATE_START;
        case EVENT_EOF:
            stats->word_count++;
            return STATE_END;
        case EVENT_DIGIT:
        case EVENT_INVALID:
            stats->error_count++;
            return STATE_ERROR;
    }
    return STATE_READING_WORD;
}

State handle_error(Event ev, char ch, Statistics* stats) {
    switch (ev) {
        case EVENT_SPACE:
            return STATE_START;
        case EVENT_EOF:
            return STATE_END;
        default:
            return STATE_ERROR;
    }
}

State handle_end(Event ev, char ch, Statistics* stats) {
    return STATE_END;
}

void print_statistics(const Statistics* stats) {
    printf("Analysis Results:\n");
    printf("Numbers found: %d\n", stats->number_count);
    printf("Words found: %d\n", stats->word_count);
    printf("Errors found: %d\n", stats->error_count);
}