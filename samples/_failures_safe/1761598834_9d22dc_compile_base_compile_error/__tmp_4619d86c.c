//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: state_machine
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
    EVENT_TERMINATOR,
    EVENT_INVALID
} Event;

typedef struct {
    int numbers[MAX_STATES];
    char words[MAX_STATES][MAX_INPUT_LEN];
    int num_count;
    int word_count;
} ResultData;

Event get_event(char c) {
    if (c == '\0') return EVENT_TERMINATOR;
    if (isspace((unsigned char)c)) return EVENT_SPACE;
    if (isdigit((unsigned char)c)) return EVENT_DIGIT;
    if (isalpha((unsigned char)c)) return EVENT_LETTER;
    return EVENT_INVALID;
}

State transition(State current, Event event, char c, ResultData *data) {
    static char current_word[MAX_INPUT_LEN] = {0};
    static int word_pos = 0;
    static int current_num = 0;
    static int reading_num = 0;

    switch (current) {
        case STATE_START:
            if (event == EVENT_DIGIT) {
                current_num = c - '0';
                reading_num = 1;
                return STATE_READING_NUMBER;
            } else if (event == EVENT_LETTER) {
                word_pos = 0;
                if (word_pos < MAX_INPUT_LEN - 1) {
                    current_word[word_pos++] = c;
                    current_word[word_pos] = '\0';
                }
                return STATE_READING_WORD;
            } else if (event == EVENT_SPACE) {
                return STATE_START;
            } else {
                return STATE_ERROR;
            }

        case STATE_READING_NUMBER:
            if (event == EVENT_DIGIT) {
                if (reading_num && current_num <= (INT_MAX - (c - '0')) / 10) {
                    current_num = current_num * 10 + (c - '0');
                }
                return STATE_READING_NUMBER;
            } else if (event == EVENT_SPACE || event == EVENT_TERMINATOR) {
                if (data->num_count < MAX_STATES) {
                    data->numbers[data->num_count++] = current_num;
                }
                reading_num = 0;
                current_num = 0;
                return (event == EVENT_TERMINATOR) ? STATE_END : STATE_START;
            } else {
                return STATE_ERROR;
            }

        case STATE_READING_WORD:
            if (event == EVENT_LETTER) {
                if (word_pos < MAX_INPUT_LEN - 1) {
                    current_word[word_pos++] = c;
                    current_word[word_pos] = '\0';
                }
                return STATE_READING_WORD;
            } else if (event == EVENT_SPACE || event == EVENT_TERMINATOR) {
                if (data->word_count < MAX_STATES && word_pos > 0) {
                    strncpy(data->words[data->word_count], current_word, MAX_INPUT_LEN - 1);
                    data->words[data->word_count][MAX_INPUT_LEN - 1] = '\0';
                    data->word_count++;
                }
                word_pos = 0;
                current_word[0] = '\0';
                return (event == EVENT_TERMINATOR) ? STATE_END : STATE_START;
            } else {
                return STATE_ERROR;
            }

        case STATE_ERROR:
            return STATE_ERROR;

        case STATE_END:
            return STATE_END;
    }
    return STATE_ERROR;
}

void process_input(const char *input, ResultData *data) {
    State current_state = STATE_START;
    size_t i = 0;
    char c;

    data->num_count = 0;
    data->word_count = 0;

    while (current_state != STATE_END && current_state != STATE_ERROR) {
        if (i >= MAX_INPUT_LEN || input[i] == '\0') {
            c = '\0';
        } else {
            c = input[i];
        }

        Event event = get_event(c);
        current_state = transition(current_state, event, c, data);

        if (c != '\0') {
            i++;
        } else {
            break;
        }
    }
}

void print_results(const ResultData *data) {
    printf("Numbers found: ");
    if (data->num_count == 0) {
        printf("none");
    } else {
        for (int i = 0; i < data->num_count; i++) {
            printf("%d", data->numbers[i]);
            if (i < data->num_count - 1) printf(", ");
        }
    }
    printf("\n");

    printf("Words found: ");
    if (data->word_count == 0) {
        printf("none");
    } else {
        for (int i = 0; i < data->word_count; i++) {
            printf("%s", data->words[i]);
            if (i < data->word_count - 1) printf(", ");
        }
    }
    printf("\n");
}

int main(void) {
    char input[MAX_INPUT_LEN + 1] = {0};
    ResultData data;

    printf("Enter text (max %