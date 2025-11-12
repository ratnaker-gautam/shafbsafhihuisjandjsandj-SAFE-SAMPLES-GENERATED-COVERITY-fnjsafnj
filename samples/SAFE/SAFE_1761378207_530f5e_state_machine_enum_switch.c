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
    char number_buf[64];
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
    
    if (len >= sizeof(buffer) - 1) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    enum state current_state = START;
    size_t num_pos = 0;
    size_t word_pos = 0;
    
    for (size_t i = 0; i <= len; i++) {
        char c = buffer[i];
        
        switch (current_state) {
            case START:
                if (c == '\0') {
                    current_state = END;
                } else if (isdigit((unsigned char)c)) {
                    if (num_pos < sizeof(number_buf) - 1) {
                        number_buf[num_pos++] = c;
                    }
                    current_state = READING_NUMBER;
                } else if (isalpha((unsigned char)c)) {
                    if (word_pos < sizeof(word_buf) - 1) {
                        word_buf[word_pos++] = c;
                    }
                    current_state = READING_WORD;
                } else if (isspace((unsigned char)c)) {
                    current_state = SKIPPING_SPACE;
                }
                break;
                
            case READING_NUMBER:
                if (c == '\0' || !isdigit((unsigned char)c)) {
                    if (num_pos > 0 && num_pos < sizeof(number_buf)) {
                        number_buf[num_pos] = '\0';
                        printf("Found number: %s\n", number_buf);
                        num_count++;
                        num_pos = 0;
                    }
                    
                    if (c == '\0') {
                        current_state = END;
                    } else if (isalpha((unsigned char)c)) {
                        if (word_pos < sizeof(word_buf) - 1) {
                            word_buf[word_pos++] = c;
                        }
                        current_state = READING_WORD;
                    } else if (isspace((unsigned char)c)) {
                        current_state = SKIPPING_SPACE;
                    } else {
                        current_state = START;
                    }
                } else {
                    if (num_pos < sizeof(number_buf) - 1) {
                        number_buf[num_pos++] = c;
                    }
                }
                break;
                
            case READING_WORD:
                if (c == '\0' || !isalpha((unsigned char)c)) {
                    if (word_pos > 0 && word_pos < sizeof(word_buf)) {
                        word_buf[word_pos] = '\0';
                        printf("Found word: %s\n", word_buf);
                        word_count++;
                        word_pos = 0;
                    }
                    
                    if (c == '\0') {
                        current_state = END;
                    } else if (isdigit((unsigned char)c)) {
                        if (num_pos < sizeof(number_buf) - 1) {
                            number_buf[num_pos++] = c;
                        }
                        current_state = READING_NUMBER;
                    } else if (isspace((unsigned char)c)) {
                        current_state = SKIPPING_SPACE;
                    } else {
                        current_state = START;
                    }
                } else {
                    if (word_pos < sizeof(word_buf) - 1) {
                        word_buf[word_pos++] = c;
                    }
                }
                break;
                
            case SKIPPING_SPACE:
                if (c == '\0') {
                    current_state = END;
                } else if (!isspace((unsigned char)c)) {
                    if (isdigit((unsigned char)c)) {
                        if (num_pos < sizeof(number_buf) - 1) {
                            number_buf[num_pos++] = c;
                        }
                        current_state = READING_NUMBER;
                    } else if (isalpha((unsigned char)c)) {
                        if (word_pos < sizeof(word_buf) - 1) {
                            word_buf[word_pos++] = c;
                        }
                        current_state = READING_WORD;
                    } else {
                        current_state = START;
                    }
                }
                break;
                
            case END:
                break;
        }
    }
    
    printf("Summary: %d numbers, %d words found\n", num_count, word_count);
    return 0;
}