//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: function_pointers ; Variation: state_machine
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

typedef enum state (*state_handler)(char, int*);

enum state handle_start(char c, int* count) {
    if (c == '\0') return END;
    if (isdigit(c)) {
        (*count)++;
        return READING_NUMBER;
    }
    if (isalpha(c)) {
        (*count)++;
        return READING_WORD;
    }
    if (isspace(c)) return START;
    return ERROR;
}

enum state handle_reading_number(char c, int* count) {
    if (c == '\0') return END;
    if (isdigit(c)) return READING_NUMBER;
    if (isspace(c)) return START;
    if (isalpha(c)) return ERROR;
    return ERROR;
}

enum state handle_reading_word(char c, int* count) {
    if (c == '\0') return END;
    if (isalpha(c)) return READING_WORD;
    if (isspace(c)) return START;
    if (isdigit(c)) return ERROR;
    return ERROR;
}

enum state handle_error(char c, int* count) {
    if (c == '\0') return END;
    if (isspace(c)) return START;
    return ERROR;
}

enum state handle_end(char c, int* count) {
    return END;
}

state_handler get_handler(enum state s) {
    switch(s) {
        case START: return handle_start;
        case READING_NUMBER: return handle_reading_number;
        case READING_WORD: return handle_reading_word;
        case ERROR: return handle_error;
        case END: return handle_end;
        default: return handle_error;
    }
}

int process_input(const char* input) {
    if (input == NULL) return -1;
    
    int count = 0;
    enum state current_state = START;
    size_t len = strlen(input);
    
    if (len > 1024) return -1;
    
    for (size_t i = 0; i <= len; i++) {
        char c = input[i];
        state_handler handler = get_handler(current_state);
        current_state = handler(c, &count);
        
        if (current_state == ERROR) {
            return -1;
        }
    }
    
    return count;
}

int main(void) {
    char input[1025];
    
    printf("Enter text to analyze: ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (len == 0 || (len == 1 && input[0] == '\n')) {
        printf("Empty input\n");
        return 1;
    }
    
    int result = process_input(input);
    
    if (result == -1) {
        printf("Invalid input format\n");
        return 1;
    }
    
    printf("Number of valid tokens: %d\n", result);
    return 0;
}