//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

enum state {
    START,
    READING_NUMBER,
    READING_WORD,
    SKIPPING_SPACE,
    FINISHED
};

int main(void) {
    char buffer[256];
    char number_buf[32];
    char word_buf[64];
    int num_count = 0;
    int word_count = 0;
    
    printf("Enter text (max 255 chars): ");
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len-1] == '\n') {
        buffer[len-1] = '\0';
        len--;
    }
    
    if (len >= sizeof(buffer)) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    enum state current_state = START;
    size_t num_idx = 0;
    size_t word_idx = 0;
    
    for (size_t i = 0; i <= len; i++) {
        char c = buffer[i];
        
        switch (current_state) {
            case START:
                if (isdigit(c)) {
                    if (num_idx < sizeof(number_buf) - 1) {
                        number_buf[num_idx++] = c;
                    }
                    current_state = READING_NUMBER;
                } else if (isalpha(c)) {
                    if (word_idx < sizeof(word_buf) - 1) {
                        word_buf[word_idx++] = tolower(c);
                    }
                    current_state = READING_WORD;
                } else if (c == ' ' || c == '\t') {
                    current_state = SKIPPING_SPACE;
                } else if (c == '\0') {
                    current_state = FINISHED;
                }
                break;
                
            case READING_NUMBER:
                if (isdigit(c)) {
                    if (num_idx < sizeof(number_buf) - 1) {
                        number_buf[num_idx++] = c;
                    }
                } else {
                    if (num_idx > 0) {
                        number_buf[num_idx] = '\0';
                        long num = strtol(number_buf, NULL, 10);
                        if (num > 0) {
                            printf("Found number: %ld\n", num);
                            num_count++;
                        }
                        num_idx = 0;
                    }
                    
                    if (isalpha(c)) {
                        if (word_idx < sizeof(word_buf) - 1) {
                            word_buf[word_idx++] = tolower(c);
                        }
                        current_state = READING_WORD;
                    } else if (c == ' ' || c == '\t') {
                        current_state = SKIPPING_SPACE;
                    } else if (c == '\0') {
                        current_state = FINISHED;
                    } else {
                        current_state = START;
                    }
                }
                break;
                
            case READING_WORD:
                if (isalnum(c)) {
                    if (word_idx < sizeof(word_buf) - 1) {
                        word_buf[word_idx++] = tolower(c);
                    }
                } else {
                    if (word_idx > 0) {
                        word_buf[word_idx] = '\0';
                        printf("Found word: %s\n", word_buf);
                        word_count++;
                        word_idx = 0;
                    }
                    
                    if (isdigit(c)) {
                        if (num_idx < sizeof(number_buf) - 1) {
                            number_buf[num_idx++] = c;
                        }
                        current_state = READING_NUMBER;
                    } else if (c == ' ' || c == '\t') {
                        current_state = SKIPPING_SPACE;
                    } else if (c == '\0') {
                        current_state = FINISHED;
                    } else {
                        current_state = START;
                    }
                }
                break;
                
            case SKIPPING_SPACE:
                if (isdigit(c)) {
                    if (num_idx < sizeof(number_buf) - 1) {
                        number_buf[num_idx++] = c;
                    }
                    current_state = READING_NUMBER;
                } else if (isalpha(c)) {
                    if (word_idx < sizeof(word_buf) - 1) {
                        word_buf[word_idx++] = tolower(c);
                    }
                    current_state = READING_WORD;
                } else if (c == '\0') {
                    current_state = FINISHED;
                } else if (c != ' ' && c != '\t') {
                    current_state = START;
                }
                break;
                
            case FINISHED:
                break;
        }
        
        if (current_state == FINISHED) {
            break;
        }
    }
    
    printf("Summary: %d numbers, %d words found\n", num_count, word_count);
    return 0;
}