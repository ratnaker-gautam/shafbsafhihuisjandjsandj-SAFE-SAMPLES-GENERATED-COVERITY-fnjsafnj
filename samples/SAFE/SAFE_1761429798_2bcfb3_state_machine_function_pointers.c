//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum state {
    START,
    READING_NUMBER,
    READING_WORD,
    ERROR,
    END
};

typedef void (*state_handler)(char, enum state*);

void handle_start(char c, enum state* next_state) {
    if (isdigit(c)) {
        putchar(c);
        *next_state = READING_NUMBER;
    } else if (isalpha(c)) {
        putchar(c);
        *next_state = READING_WORD;
    } else if (c == '\n' || c == '\0') {
        *next_state = END;
    } else if (isspace(c)) {
        *next_state = START;
    } else {
        *next_state = ERROR;
    }
}

void handle_reading_number(char c, enum state* next_state) {
    if (isdigit(c)) {
        putchar(c);
        *next_state = READING_NUMBER;
    } else if (isspace(c)) {
        printf(" [NUM] ");
        *next_state = START;
    } else if (c == '\n' || c == '\0') {
        printf(" [NUM]\n");
        *next_state = END;
    } else {
        *next_state = ERROR;
    }
}

void handle_reading_word(char c, enum state* next_state) {
    if (isalpha(c)) {
        putchar(c);
        *next_state = READING_WORD;
    } else if (isspace(c)) {
        printf(" [WORD] ");
        *next_state = START;
    } else if (c == '\n' || c == '\0') {
        printf(" [WORD]\n");
        *next_state = END;
    } else {
        *next_state = ERROR;
    }
}

void handle_error(char c, enum state* next_state) {
    printf(" [ERROR]");
    if (c == '\n' || c == '\0') {
        putchar('\n');
        *next_state = END;
    } else {
        *next_state = START;
    }
}

void handle_end(char c, enum state* next_state) {
    *next_state = END;
}

int main(void) {
    state_handler handlers[] = {
        handle_start,
        handle_reading_number,
        handle_reading_word,
        handle_error,
        handle_end
    };
    
    char input[1024];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return 1;
    }
    
    size_t len = strlen(input);
    if (len >= sizeof(input)) {
        return 1;
    }
    
    enum state current_state = START;
    
    for (size_t i = 0; i < len; i++) {
        if (current_state >= START && current_state <= END) {
            handlers[current_state](input[i], &current_state);
        } else {
            current_state = ERROR;
        }
        
        if (current_state == END) {
            break;
        }
    }
    
    if (current_state != END) {
        handlers[current_state]('\0', &current_state);
    }
    
    return 0;
}