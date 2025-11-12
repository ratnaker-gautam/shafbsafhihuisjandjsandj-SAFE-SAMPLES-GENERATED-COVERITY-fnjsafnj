//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT 256
#define STATE_COUNT 5

typedef enum {
    STATE_START,
    STATE_READING_WORD,
    STATE_READING_NUMBER,
    STATE_PUNCTUATION,
    STATE_END
} State;

typedef enum {
    CHAR_LETTER,
    CHAR_DIGIT,
    CHAR_SPACE,
    CHAR_PUNCT,
    CHAR_END
} CharType;

CharType get_char_type(char c) {
    if (c == '\0') return CHAR_END;
    if (isspace((unsigned char)c)) return CHAR_SPACE;
    if (isalpha((unsigned char)c)) return CHAR_LETTER;
    if (isdigit((unsigned char)c)) return CHAR_DIGIT;
    return CHAR_PUNCT;
}

State transition_table[STATE_COUNT][5] = {
    {STATE_READING_WORD, STATE_READING_NUMBER, STATE_START, STATE_PUNCTUATION, STATE_END},
    {STATE_READING_WORD, STATE_READING_WORD, STATE_START, STATE_PUNCTUATION, STATE_END},
    {STATE_READING_NUMBER, STATE_READING_NUMBER, STATE_START, STATE_PUNCTUATION, STATE_END},
    {STATE_READING_WORD, STATE_READING_NUMBER, STATE_START, STATE_PUNCTUATION, STATE_END},
    {STATE_END, STATE_END, STATE_END, STATE_END, STATE_END}
};

void process_input(const char *input) {
    State current_state = STATE_START;
    const char *ptr = input;
    int word_count = 0;
    int number_count = 0;
    int punct_count = 0;
    
    while (current_state != STATE_END) {
        CharType type = get_char_type(*ptr);
        State next_state = transition_table[current_state][type];
        
        if (current_state == STATE_START && next_state == STATE_READING_WORD) {
            word_count++;
        } else if (current_state == STATE_START && next_state == STATE_READING_NUMBER) {
            number_count++;
        } else if (current_state == STATE_START && next_state == STATE_PUNCTUATION) {
            punct_count++;
        }
        
        current_state = next_state;
        
        if (current_state != STATE_END) {
            ptr++;
            if (ptr - input >= MAX_INPUT) {
                break;
            }
        }
    }
    
    printf("Words: %d\n", word_count);
    printf("Numbers: %d\n", number_count);
    printf("Punctuation sequences: %d\n", punct_count);
}

int main(void) {
    char input[MAX_INPUT + 1] = {0};
    
    printf("Enter text (max %d characters): ", MAX_INPUT);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (len == 0 || (len == 1 && input[0] == '\n')) {
        printf("No input provided\n");
        return 0;
    }
    
    process_input(input);
    
    return 0;
}