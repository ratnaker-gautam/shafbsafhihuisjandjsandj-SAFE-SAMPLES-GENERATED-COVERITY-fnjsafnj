//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 100
#define MAX_STATES 10

typedef enum {
    STATE_START,
    STATE_IDENTIFIER,
    STATE_NUMBER,
    STATE_OPERATOR,
    STATE_ERROR,
    STATE_END
} State;

typedef enum {
    CHAR_LETTER,
    CHAR_DIGIT,
    CHAR_OPERATOR,
    CHAR_SPACE,
    CHAR_OTHER
} CharType;

CharType get_char_type(char c) {
    if (isalpha(c)) return CHAR_LETTER;
    if (isdigit(c)) return CHAR_DIGIT;
    if (c == '+' || c == '-' || c == '*' || c == '/' || c == '=') return CHAR_OPERATOR;
    if (isspace(c)) return CHAR_SPACE;
    return CHAR_OTHER;
}

State transition_table[MAX_STATES][5] = {
    {STATE_IDENTIFIER, STATE_NUMBER, STATE_OPERATOR, STATE_START, STATE_ERROR},
    {STATE_IDENTIFIER, STATE_IDENTIFIER, STATE_OPERATOR, STATE_START, STATE_ERROR},
    {STATE_ERROR, STATE_NUMBER, STATE_OPERATOR, STATE_START, STATE_ERROR},
    {STATE_IDENTIFIER, STATE_NUMBER, STATE_ERROR, STATE_START, STATE_ERROR},
    {STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_ERROR}
};

void process_input(const char *input) {
    State current_state = STATE_START;
    const char *start = input;
    const char *current = input;
    
    while (*current != '\0' && current_state != STATE_ERROR) {
        CharType ct = get_char_type(*current);
        State next_state = transition_table[current_state][ct];
        
        if (current_state != next_state && current_state != STATE_START) {
            size_t len = current - start;
            if (len > 0 && len < MAX_INPUT_LEN) {
                char token[MAX_INPUT_LEN];
                strncpy(token, start, len);
                token[len] = '\0';
                
                switch (current_state) {
                    case STATE_IDENTIFIER:
                        printf("Identifier: %s\n", token);
                        break;
                    case STATE_NUMBER:
                        printf("Number: %s\n", token);
                        break;
                    case STATE_OPERATOR:
                        printf("Operator: %s\n", token);
                        break;
                    default:
                        break;
                }
            }
            start = current;
        }
        
        current_state = next_state;
        current++;
    }
    
    if (current_state != STATE_START && current_state != STATE_ERROR) {
        size_t len = current - start;
        if (len > 0 && len < MAX_INPUT_LEN) {
            char token[MAX_INPUT_LEN];
            strncpy(token, start, len);
            token[len] = '\0';
            
            switch (current_state) {
                case STATE_IDENTIFIER:
                    printf("Identifier: %s\n", token);
                    break;
                case STATE_NUMBER:
                    printf("Number: %s\n", token);
                    break;
                case STATE_OPERATOR:
                    printf("Operator: %s\n", token);
                    break;
                default:
                    break;
            }
        }
    }
}

int main() {
    char input[MAX_INPUT_LEN];
    
    printf("Enter expression (max %d chars): ", MAX_INPUT_LEN - 1);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (len == 0 || (len == 1 && input[0] == '\n')) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    printf("Tokens:\n");
    process_input(input);
    
    return 0;
}