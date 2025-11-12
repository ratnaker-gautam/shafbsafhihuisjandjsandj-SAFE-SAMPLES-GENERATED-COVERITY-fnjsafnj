//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum state {
    START,
    READING_NUMBER,
    READING_WORD,
    SKIPPING_SPACE,
    END
};

int main(void) {
    char buffer[256];
    char number_buf[32];
    char word_buf[64];
    int num_idx = 0;
    int word_idx = 0;
    enum state current_state = START;
    
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
    
    printf("Processing text: %s\n", buffer);
    printf("Found elements:\n");
    
    for (size_t i = 0; i <= len; i++) {
        char c = buffer[i];
        
        switch (current_state) {
            case START:
                if (isdigit(c)) {
                    if (num_idx < sizeof(number_buf) - 1) {
                        number_buf[num_idx++] = c;
                        current_state = READING_NUMBER;
                    }
                } else if (isalpha(c)) {
                    if (word_idx < sizeof(word_buf) - 1) {
                        word_buf[word_idx++] = c;
                        current_state = READING_WORD;
                    }
                } else if (c == ' ' || c == '\t') {
                    current_state = SKIPPING_SPACE;
                } else if (c == '\0') {
                    current_state = END;
                }
                break;
                
            case READING_NUMBER:
                if (isdigit(c)) {
                    if (num_idx < sizeof(number_buf) - 1) {
                        number_buf[num_idx++] = c;
                    }
                } else {
                    number_buf[num_idx] = '\0';
                    printf("Number: %s\n", number_buf);
                    num_idx = 0;
                    
                    if (isalpha(c)) {
                        if (word_idx < sizeof(word_buf) - 1) {
                            word_buf[word_idx++] = c;
                            current_state = READING_WORD;
                        }
                    } else if (c == ' ' || c == '\t') {
                        current_state = SKIPPING_SPACE;
                    } else if (c == '\0') {
                        current_state = END;
                    } else {
                        current_state = START;
                    }
                }
                break;
                
            case READING_WORD:
                if (isalpha(c)) {
                    if (word_idx < sizeof(word_buf) - 1) {
                        word_buf[word_idx++] = c;
                    }
                } else {
                    word_buf[word_idx] = '\0';
                    printf("Word: %s\n", word_buf);
                    word_idx = 0;
                    
                    if (isdigit(c)) {
                        if (num_idx < sizeof(number_buf) - 1) {
                            number_buf[num_idx++] = c;
                            current_state = READING_NUMBER;
                        }
                    } else if (c == ' ' || c == '\t') {
                        current_state = SKIPPING_SPACE;
                    } else if (c == '\0') {
                        current_state = END;
                    } else {
                        current_state = START;
                    }
                }
                break;
                
            case SKIPPING_SPACE:
                if (!(c == ' ' || c == '\t')) {
                    if (isdigit(c)) {
                        if (num_idx < sizeof(number_buf) - 1) {
                            number_buf[num_idx++] = c;
                            current_state = READING_NUMBER;
                        }
                    } else if (isalpha(c)) {
                        if (word_idx < sizeof(word_buf) - 1) {
                            word_buf[word_idx++] = c;
                            current_state = READING_WORD;
                        }
                    } else if (c == '\0') {
                        current_state = END;
                    } else {
                        current_state = START;
                    }
                }
                break;
                
            case END:
                if (num_idx > 0) {
                    number_buf[num_idx] = '\0';
                    printf("Number: %s\n", number_buf);
                    num_idx = 0;
                }
                if (word_idx > 0) {
                    word_buf[word_idx] = '\0';
                    printf("Word: %s\n", word_buf);
                    word_idx = 0;
                }
                printf("Processing complete.\n");
                return 0;
        }
    }
    
    return 0;
}