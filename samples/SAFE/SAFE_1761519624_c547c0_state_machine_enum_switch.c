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
    printf("Enter text to analyze: ");
    
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
        len--;
    }
    
    if (len >= sizeof(buffer) - 1) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    enum state current_state = START;
    size_t pos = 0;
    int word_count = 0;
    int number_count = 0;
    
    while (current_state != FINISHED && current_state != ERROR) {
        char c = (pos < len) ? buffer[pos] : '\0';
        
        switch (current_state) {
            case START:
                if (c == '\0') {
                    current_state = FINISHED;
                } else if (isdigit((unsigned char)c)) {
                    current_state = READING_NUMBER;
                    number_count++;
                } else if (isalpha((unsigned char)c)) {
                    current_state = READING_WORD;
                    word_count++;
                } else if (isspace((unsigned char)c)) {
                    current_state = SKIPPING_SPACE;
                } else {
                    current_state = ERROR;
                }
                pos++;
                break;
                
            case READING_NUMBER:
                if (c == '\0') {
                    current_state = FINISHED;
                } else if (isdigit((unsigned char)c)) {
                    pos++;
                } else if (isspace((unsigned char)c)) {
                    current_state = SKIPPING_SPACE;
                    pos++;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_WORD:
                if (c == '\0') {
                    current_state = FINISHED;
                } else if (isalpha((unsigned char)c)) {
                    pos++;
                } else if (isspace((unsigned char)c)) {
                    current_state = SKIPPING_SPACE;
                    pos++;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case SKIPPING_SPACE:
                if (c == '\0') {
                    current_state = FINISHED;
                } else if (isspace((unsigned char)c)) {
                    pos++;
                } else if (isdigit((unsigned char)c)) {
                    current_state = READING_NUMBER;
                    number_count++;
                    pos++;
                } else if (isalpha((unsigned char)c)) {
                    current_state = READING_WORD;
                    word_count++;
                    pos++;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case FINISHED:
                break;
                
            case ERROR:
                fprintf(stderr, "Invalid character at position %zu\n", pos);
                break;
        }
    }
    
    if (current_state == FINISHED) {
        printf("Analysis complete:\n");
        printf("Words found: %d\n", word_count);
        printf("Numbers found: %d\n", number_count);
    } else {
        printf("Analysis failed due to invalid input\n");
        return 1;
    }
    
    return 0;
}