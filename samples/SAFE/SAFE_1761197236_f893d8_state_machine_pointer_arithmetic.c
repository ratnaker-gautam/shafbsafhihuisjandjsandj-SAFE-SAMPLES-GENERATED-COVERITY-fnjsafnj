//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT 256
#define STATE_COUNT 5

typedef enum {
    STATE_START,
    STATE_IDENTIFIER,
    STATE_NUMBER,
    STATE_OPERATOR,
    STATE_ERROR
} State;

typedef enum {
    CHAR_ALPHA,
    CHAR_DIGIT,
    CHAR_OPERATOR,
    CHAR_SPACE,
    CHAR_OTHER
} CharType;

CharType get_char_type(char c) {
    if (isalpha(c)) return CHAR_ALPHA;
    if (isdigit(c)) return CHAR_DIGIT;
    if (strchr("+-*/=", c) != NULL) return CHAR_OPERATOR;
    if (isspace(c)) return CHAR_SPACE;
    return CHAR_OTHER;
}

State transition_table[STATE_COUNT][5] = {
    {STATE_IDENTIFIER, STATE_NUMBER, STATE_OPERATOR, STATE_START, STATE_ERROR},
    {STATE_IDENTIFIER, STATE_IDENTIFIER, STATE_OPERATOR, STATE_START, STATE_ERROR},
    {STATE_OPERATOR, STATE_NUMBER, STATE_OPERATOR, STATE_START, STATE_ERROR},
    {STATE_IDENTIFIER, STATE_NUMBER, STATE_OPERATOR, STATE_START, STATE_ERROR},
    {STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_ERROR}
};

void process_token(char *start, char *end, State state) {
    if (end > start) {
        size_t len = end - start;
        if (len >= MAX_INPUT) len = MAX_INPUT - 1;
        
        char token[MAX_INPUT];
        for (size_t i = 0; i < len && i < MAX_INPUT - 1; i++) {
            token[i] = *(start + i);
        }
        token[len] = '\0';
        
        const char *type_names[] = {"Start", "Identifier", "Number", "Operator", "Error"};
        printf("Token: '%-20s' Type: %s\n", token, type_names[state]);
    }
}

int main() {
    char input[MAX_INPUT];
    
    printf("Enter expression: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        printf("Empty input\n");
        return 0;
    }
    
    State current_state = STATE_START;
    char *current_start = input;
    char *ptr = input;
    char *end = input + input_len;
    
    while (ptr < end) {
        CharType ctype = get_char_type(*ptr);
        State next_state = transition_table[current_state][ctype];
        
        if (next_state != current_state) {
            process_token(current_start, ptr, current_state);
            current_start = ptr;
            current_state = next_state;
        }
        
        ptr++;
    }
    
    process_token(current_start, ptr, current_state);
    
    return 0;
}