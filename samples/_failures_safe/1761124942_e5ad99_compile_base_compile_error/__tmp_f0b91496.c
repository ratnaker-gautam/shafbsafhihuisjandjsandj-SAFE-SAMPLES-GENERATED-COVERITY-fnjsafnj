//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: modular_functions ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>
#include <limits.h>

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
    int numbers[MAX_STATES];
    char words[MAX_STATES][MAX_INPUT_LEN];
    int num_count;
    int word_count;
} ResultData;

Event get_event(char c) {
    if (c == '\0' || c == '\n') return EVENT_EOF;
    if (isspace(c)) return EVENT_SPACE;
    if (isdigit(c)) return EVENT_DIGIT;
    if (isalpha(c)) return EVENT_LETTER;
    return EVENT_INVALID;
}

State handle_start(Event ev, char c, ResultData* data) {
    if (ev == EVENT_DIGIT) {
        if (data->num_count < MAX_STATES) {
            data->numbers[data->num_count] = c - '0';
            return STATE_READING_NUMBER;
        }
        return STATE_ERROR;
    } else if (ev == EVENT_LETTER) {
        if (data->word_count < MAX_STATES) {
            data->words[data->word_count][0] = c;
            data->words[data->word_count][1] = '\0';
            return STATE_READING_WORD;
        }
        return STATE_ERROR;
    } else if (ev == EVENT_SPACE) {
        return STATE_START;
    } else if (ev == EVENT_EOF) {
        return STATE_END;
    }
    return STATE_ERROR;
}

State handle_reading_number(Event ev, char c, ResultData* data) {
    if (ev == EVENT_DIGIT) {
        int current_num = data->numbers[data->num_count];
        if (current_num <= (INT_MAX - (c - '0')) / 10) {
            data->numbers[data->num_count] = current_num * 10 + (c - '0');
            return STATE_READING_NUMBER;
        }
        return STATE_ERROR;
    } else if (ev == EVENT_SPACE) {
        data->num_count++;
        return STATE_START;
    } else if (ev == EVENT_EOF) {
        data->num_count++;
        return STATE_END;
    }
    return STATE_ERROR;
}

State handle_reading_word(Event ev, char c, ResultData* data) {
    if (ev == EVENT_LETTER) {
        size_t len = strlen(data->words[data->word_count]);
        if (len < MAX_INPUT_LEN - 1) {
            data->words[data->word_count][len] = c;
            data->words[data->word_count][len + 1] = '\0';
            return STATE_READING_WORD;
        }
        return STATE_ERROR;
    } else if (ev == EVENT_SPACE) {
        data->word_count++;
        return STATE_START;
    } else if (ev == EVENT_EOF) {
        data->word_count++;
        return STATE_END;
    }
    return STATE_ERROR;
}

State process_state(State current, Event ev, char c, ResultData* data) {
    switch (current) {
        case STATE_START:
            return handle_start(ev, c, data);
        case STATE_READING_NUMBER:
            return handle_reading_number(ev, c, data);
        case STATE_READING_WORD:
            return handle_reading_word(ev, c, data);
        case STATE_ERROR:
        case STATE_END:
            return current;
        default:
            return STATE_ERROR;
    }
}

void print_results(const ResultData* data) {
    printf("Numbers found: ");
    for (int i = 0; i < data->num_count; i++) {
        printf("%d ", data->numbers[i]);
    }
    printf("\nWords found: ");
    for (int i = 0; i < data->word_count; i++) {
        printf("%s ", data->words[i]);
    }
    printf("\n");
}

int main(void) {
    char input[MAX_INPUT_LEN + 2];
    ResultData data = {0};
    State current_state = STATE_START;
    
    printf("Enter text (max %d chars): ", MAX_INPUT_LEN);
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    } else if (len >= MAX_INPUT_LEN) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    for (size_t i = 0; input[i] != '\0'; i++) {
        Event ev = get_event(input[i]);
        current_state = process_state(current_state, ev, input[i], &data);
        if (current_state == STATE_ERROR) {
            fprintf(stderr, "Error processing input at position %zu\n", i);
            return 1;
        }
        if (current_state == STATE_END) {
            break;
        }
    }
    
    if (current