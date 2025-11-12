//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: state_machine
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>

enum state {
    START,
    READING_NUMBER,
    READING_WORD,
    SKIPPING_SPACE,
    FINISHED,
    ERROR
};

int main(void) {
    char buffer[256];
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return 1;
    }
    
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
        len--;
    }
    
    if (len >= sizeof(buffer) - 1) {
        return 1;
    }
    
    enum state current_state = START;
    size_t position = 0;
    int number_count = 0;
    int word_count = 0;
    
    while (current_state != FINISHED && current_state != ERROR) {
        char current_char = position < len ? buffer[position] : '\0';
        
        switch (current_state) {
            case START:
                if (current_char == '\0') {
                    current_state = FINISHED;
                } else if (isdigit(current_char)) {
                    current_state = READING_NUMBER;
                    number_count++;
                } else if (isalpha(current_char)) {
                    current_state = READING_WORD;
                    word_count++;
                } else if (isspace(current_char)) {
                    current_state = SKIPPING_SPACE;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_NUMBER:
                if (current_char == '\0') {
                    current_state = FINISHED;
                } else if (isdigit(current_char)) {
                    current_state = READING_NUMBER;
                } else if (isspace(current_char)) {
                    current_state = SKIPPING_SPACE;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_WORD:
                if (current_char == '\0') {
                    current_state = FINISHED;
                } else if (isalpha(current_char)) {
                    current_state = READING_WORD;
                } else if (isspace(current_char)) {
                    current_state = SKIPPING_SPACE;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case SKIPPING_SPACE:
                if (current_char == '\0') {
                    current_state = FINISHED;
                } else if (isspace(current_char)) {
                    current_state = SKIPPING_SPACE;
                } else if (isdigit(current_char)) {
                    current_state = READING_NUMBER;
                    number_count++;
                } else if (isalpha(current_char)) {
                    current_state = READING_WORD;
                    word_count++;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case FINISHED:
                break;
                
            case ERROR:
                break;
        }
        
        position++;
        if (position > len + 1) {
            current_state = ERROR;
            break;
        }
    }
    
    if (current_state == ERROR) {
        printf("Invalid input format\n");
        return 1;
    }
    
    printf("Numbers: %d\nWords: %d\n", number_count, word_count);
    return 0;
}