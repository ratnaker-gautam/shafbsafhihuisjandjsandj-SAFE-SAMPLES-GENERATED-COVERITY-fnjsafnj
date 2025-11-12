//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define STATE_IDLE 0
#define STATE_READING 1
#define STATE_PROCESSING 2
#define STATE_DONE 3
#define STATE_ERROR 4

#define MAX_INPUT_LEN 100
#define MAX_TOKENS 10

#define TRANSITION(new_state) do { current_state = new_state; } while(0)
#define IS_DIGIT(c) ((c) >= '0' && (c) <= '9')
#define IS_SPACE(c) ((c) == ' ' || (c) == '\t' || (c) == '\n')

typedef struct {
    int tokens[MAX_TOKENS];
    int count;
    int sum;
} ParserData;

int validate_input(const char* input) {
    if (input == NULL) return 0;
    size_t len = strlen(input);
    if (len == 0 || len > MAX_INPUT_LEN) return 0;
    for (size_t i = 0; i < len; i++) {
        if (!IS_DIGIT(input[i]) && !IS_SPACE(input[i])) return 0;
    }
    return 1;
}

void process_tokens(ParserData* data) {
    data->sum = 0;
    for (int i = 0; i < data->count; i++) {
        if (data->sum > INT_MAX - data->tokens[i]) {
            data->sum = INT_MAX;
            break;
        }
        data->sum += data->tokens[i];
    }
}

int state_machine(const char* input, ParserData* data) {
    int current_state = STATE_IDLE;
    int token_start = -1;
    data->count = 0;
    
    for (size_t i = 0; i <= strlen(input); i++) {
        char c = input[i];
        
        switch (current_state) {
            case STATE_IDLE:
                if (IS_DIGIT(c)) {
                    token_start = i;
                    TRANSITION(STATE_READING);
                } else if (c == '\0') {
                    TRANSITION(STATE_DONE);
                } else if (!IS_SPACE(c)) {
                    TRANSITION(STATE_ERROR);
                }
                break;
                
            case STATE_READING:
                if (IS_SPACE(c) || c == '\0') {
                    if (data->count >= MAX_TOKENS) {
                        TRANSITION(STATE_ERROR);
                        break;
                    }
                    char token_buf[20];
                    size_t token_len = i - token_start;
                    if (token_len >= sizeof(token_buf)) {
                        TRANSITION(STATE_ERROR);
                        break;
                    }
                    memcpy(token_buf, &input[token_start], token_len);
                    token_buf[token_len] = '\0';
                    data->tokens[data->count] = atoi(token_buf);
                    data->count++;
                    TRANSITION(c == '\0' ? STATE_DONE : STATE_IDLE);
                } else if (!IS_DIGIT(c)) {
                    TRANSITION(STATE_ERROR);
                }
                break;
                
            case STATE_PROCESSING:
                process_tokens(data);
                TRANSITION(STATE_DONE);
                break;
                
            case STATE_DONE:
            case STATE_ERROR:
                break;
        }
        
        if (current_state == STATE_ERROR || current_state == STATE_DONE) {
            break;
        }
    }
    
    if (current_state == STATE_IDLE && data->count > 0) {
        TRANSITION(STATE_PROCESSING);
        process_tokens(data);
        TRANSITION(STATE_DONE);
    }
    
    return current_state;
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    ParserData data;
    
    printf("Enter numbers separated by spaces: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (!validate_input(input)) {
        fprintf(stderr, "Invalid input format\n");
        return 1;
    }
    
    int result = state_machine(input, &data);
    
    if (result == STATE_DONE) {
        printf("Parsed %d numbers\n", data.count);
        printf("Sum: %d\n", data.sum);
        return 0;
    } else {
        fprintf(stderr, "Error processing input\n");
        return 1;
    }
}