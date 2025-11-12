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
    if (isspace((unsigned char)c)) return EVENT_SPACE;
    if (isdigit((unsigned char)c)) return EVENT_DIGIT;
    if (isalpha((unsigned char)c)) return EVENT_LETTER;
    return EVENT_INVALID;
}

State handle_start(Event ev, char c, ResultData* data) {
    switch (ev) {
        case EVENT_DIGIT:
            if (data->num_count < MAX_STATES) {
                data->numbers[data->num_count] = c - '0';
                return STATE_READING_NUMBER;
            }
            return STATE_ERROR;
        case EVENT_LETTER:
            if (data->word_count < MAX_STATES) {
                data->words[data->word_count][0] = c;
                data->words[data->word_count][1] = '\0';
                return STATE_READING_WORD;
            }
            return STATE_ERROR;
        case EVENT_SPACE:
            return STATE_START;
        case EVENT_EOF:
            return STATE_END;
        default:
            return STATE_ERROR;
    }
}

State handle_reading_number(Event ev, char c, ResultData* data) {
    switch (ev) {
        case EVENT_DIGIT:
            if (data->num_count < MAX_STATES) {
                int current = data->numbers[data->num_count];
                if (current <= (INT_MAX - (c - '0')) / 10) {
                    data->numbers[data->num_count] = current * 10 + (c - '0');
                    return STATE_READING_NUMBER;
                }
            }
            return STATE_ERROR;
        case EVENT_SPACE:
            data->num_count++;
            return STATE_START;
        case EVENT_EOF:
            data->num_count++;
            return STATE_END;
        default:
            return STATE_ERROR;
    }
}

State handle_reading_word(Event ev, char c, ResultData* data) {
    switch (ev) {
        case EVENT_LETTER:
            if (data->word_count < MAX_STATES) {
                size_t len = strlen(data->words[data->word_count]);
                if (len < MAX_INPUT_LEN - 1) {
                    data->words[data->word_count][len] = c;
                    data->words[data->word_count][len + 1] = '\0';
                    return STATE_READING_WORD;
                }
            }
            return STATE_ERROR;
        case EVENT_SPACE:
            data->word_count++;
            return STATE_START;
        case EVENT_EOF:
            data->word_count++;
            return STATE_END;
        default:
            return STATE_ERROR;
    }
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
        default:
            return current;
    }
}

int process_input(const char* input, ResultData* data) {
    State current = STATE_START;
    size_t i = 0;
    
    memset(data, 0, sizeof(ResultData));
    
    while (current != STATE_END && current != STATE_ERROR) {
        if (i >= MAX_INPUT_LEN) {
            return 0;
        }
        
        char c = input[i];
        Event ev = get_event(c);
        current = process_state(current, ev, c, data);
        
        if (c == '\0' || c == '\n') {
            break;
        }
        i++;
    }
    
    return current == STATE_END;
}

void print_results(const ResultData* data) {
    printf("Numbers found: ");
    for (int i = 0; i < data->num_count; i++) {
        printf("%d ", data->numbers[i]);
    }
    printf("\n");
    
    printf("Words found: ");
    for (int i = 0; i < data->word_count; i++) {
        printf("%s ", data->words[i]);
    }
    printf("\n");
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    ResultData data;
    
    printf("Enter text (max %d chars): ", MAX_INPUT_LEN);
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1]