//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
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
    EVENT_TERMINATOR,
    EVENT_INVALID
} Event;

typedef struct {
    int numbers[100];
    char words[100][50];
    int num_count;
    int word_count;
} ResultData;

Event get_event(char c) {
    if (c == '\0') return EVENT_TERMINATOR;
    if (c == ' ' || c == '\t' || c == '\n') return EVENT_SPACE;
    if (isdigit(c)) return EVENT_DIGIT;
    if (isalpha(c)) return EVENT_LETTER;
    return EVENT_INVALID;
}

State transition(State current, Event event, ResultData* data, char* buffer, int* buf_pos, char c) {
    switch (current) {
        case STATE_START:
            switch (event) {
                case EVENT_DIGIT:
                    if (*buf_pos < 49) {
                        buffer[(*buf_pos)++] = '0';
                        buffer[(*buf_pos)++] = '\0';
                    }
                    return STATE_READING_NUMBER;
                case EVENT_LETTER:
                    if (*buf_pos < 49) {
                        buffer[(*buf_pos)++] = c;
                        buffer[(*buf_pos)++] = '\0';
                    }
                    return STATE_READING_WORD;
                case EVENT_SPACE:
                    return STATE_START;
                case EVENT_TERMINATOR:
                    return STATE_END;
                default:
                    return STATE_ERROR;
            }
            break;
            
        case STATE_READING_NUMBER:
            switch (event) {
                case EVENT_DIGIT:
                    if (*buf_pos < 49 && data->num_count < 100) {
                        buffer[(*buf_pos)++] = c;
                        buffer[*buf_pos] = '\0';
                    }
                    return STATE_READING_NUMBER;
                case EVENT_SPACE:
                    if (data->num_count < 100) {
                        data->numbers[data->num_count++] = atoi(buffer);
                    }
                    *buf_pos = 0;
                    return STATE_START;
                case EVENT_TERMINATOR:
                    if (data->num_count < 100) {
                        data->numbers[data->num_count++] = atoi(buffer);
                    }
                    return STATE_END;
                default:
                    return STATE_ERROR;
            }
            break;
            
        case STATE_READING_WORD:
            switch (event) {
                case EVENT_LETTER:
                    if (*buf_pos < 49 && data->word_count < 100) {
                        buffer[(*buf_pos)++] = c;
                        buffer[*buf_pos] = '\0';
                    }
                    return STATE_READING_WORD;
                case EVENT_SPACE:
                    if (data->word_count < 100 && *buf_pos > 0) {
                        strncpy(data->words[data->word_count], buffer, 49);
                        data->words[data->word_count][49] = '\0';
                        data->word_count++;
                    }
                    *buf_pos = 0;
                    return STATE_START;
                case EVENT_TERMINATOR:
                    if (data->word_count < 100 && *buf_pos > 0) {
                        strncpy(data->words[data->word_count], buffer, 49);
                        data->words[data->word_count][49] = '\0';
                        data->word_count++;
                    }
                    return STATE_END;
                default:
                    return STATE_ERROR;
            }
            break;
            
        case STATE_ERROR:
            return STATE_ERROR;
            
        case STATE_END:
            return STATE_END;
    }
    return STATE_ERROR;
}

void process_input(const char* input, ResultData* data) {
    State current_state = STATE_START;
    char buffer[50];
    int buf_pos = 0;
    int input_len = 0;
    
    if (input == NULL) return;
    
    input_len = strlen(input);
    if (input_len >= MAX_INPUT_LEN) return;
    
    memset(buffer, 0, sizeof(buffer));
    memset(data, 0, sizeof(ResultData));
    
    for (int i = 0; i <= input_len; i++) {
        char current_char = input[i];
        Event event = get_event(current_char);
        
        current_state = transition(current_state, event, data, buffer, &buf_pos, current_char);
        
        if (current_state == STATE_ERROR) {
            break;
        }
        if (current_state == STATE_END) {
            break;
        }
    }
}

void print_results(const ResultData* data) {
    printf("Numbers found: %d\n", data->num_count);
    for (int i = 0; i < data->num_count; i++) {
        printf("%d ", data->numbers[i]);
    }
    printf("\n");
    
    printf("Words found: %d\n", data->word_count);
    for (int i = 0; i < data->word_count; i++) {
        printf("%s ", data->words[i]);
    }
    printf("\n");
}

int main() {
    char input[MAX_INPUT_LEN + 1];
    ResultData results;
    
    printf("Enter text (max %d chars): ", MAX_IN