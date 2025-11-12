//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

#define MAX_INPUT 256
#define STATE_COUNT 5

typedef enum {
    START,
    READING_NUMBER,
    READING_WORD,
    SKIPPING,
    FINISHED
} State;

typedef struct {
    State current;
    char *input;
    char *pos;
    int number_sum;
    int word_count;
} StateMachine;

void init_machine(StateMachine *sm, char *input) {
    if (sm == NULL || input == NULL) return;
    sm->current = START;
    sm->input = input;
    sm->pos = input;
    sm->number_sum = 0;
    sm->word_count = 0;
}

int is_digit_char(char c) {
    return c >= '0' && c <= '9';
}

int is_alpha_char(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

int parse_number(char **ptr) {
    if (ptr == NULL || *ptr == NULL) return 0;
    
    char *start = *ptr;
    int value = 0;
    
    while (is_digit_char(**ptr)) {
        if (value > (INT_MAX - (**ptr - '0')) / 10) {
            return 0;
        }
        value = value * 10 + (**ptr - '0');
        (*ptr)++;
    }
    
    return value;
}

void process_input(StateMachine *sm) {
    if (sm == NULL || sm->input == NULL) return;
    
    while (sm->current != FINISHED && *sm->pos != '\0') {
        switch (sm->current) {
            case START:
                if (is_digit_char(*sm->pos)) {
                    sm->current = READING_NUMBER;
                } else if (is_alpha_char(*sm->pos)) {
                    sm->current = READING_WORD;
                } else {
                    sm->current = SKIPPING;
                }
                break;
                
            case READING_NUMBER: {
                char *temp = sm->pos;
                int num = parse_number(&temp);
                if (temp > sm->pos) {
                    if (sm->number_sum <= INT_MAX - num) {
                        sm->number_sum += num;
                    }
                    sm->pos = temp;
                }
                sm->current = START;
                break;
            }
                
            case READING_WORD:
                while (is_alpha_char(*sm->pos)) {
                    sm->pos++;
                }
                if (sm->word_count < INT_MAX) {
                    sm->word_count++;
                }
                sm->current = START;
                break;
                
            case SKIPPING:
                while (*sm->pos != '\0' && !is_digit_char(*sm->pos) && !is_alpha_char(*sm->pos)) {
                    sm->pos++;
                }
                sm->current = START;
                break;
                
            case FINISHED:
                return;
        }
    }
    
    if (*sm->pos == '\0') {
        sm->current = FINISHED;
    }
}

int main(void) {
    char input_buffer[MAX_INPUT + 1] = {0};
    
    printf("Enter text (max %d chars): ", MAX_INPUT);
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input_buffer);
    if (len > 0 && input_buffer[len - 1] == '\n') {
        input_buffer[len - 1] = '\0';
    }
    
    if (len == 0 || (len == 1 && input_buffer[0] == '\n')) {
        printf("No input provided\n");
        return 1;
    }
    
    StateMachine sm;
    init_machine(&sm, input_buffer);
    process_input(&sm);
    
    printf("Sum of numbers: %d\n", sm.number_sum);
    printf("Word count: %d\n", sm.word_count);
    
    return 0;
}