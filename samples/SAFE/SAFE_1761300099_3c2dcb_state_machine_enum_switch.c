//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

enum state {
    START,
    READING_NUMBER,
    READING_WORD,
    ERROR,
    DONE
};

int main(void) {
    char buffer[256];
    char output[512] = {0};
    size_t output_pos = 0;
    enum state current_state = START;
    int number_sum = 0;
    int word_count = 0;
    int ch;
    size_t pos = 0;
    size_t buffer_len;
    
    printf("Enter text (max 255 chars): ");
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    buffer[strcspn(buffer, "\n")] = '\0';
    buffer_len = strlen(buffer);
    
    while (current_state != DONE && current_state != ERROR && pos < buffer_len) {
        ch = buffer[pos];
        
        switch (current_state) {
            case START:
                if (isspace(ch)) {
                    pos++;
                } else if (isdigit(ch)) {
                    current_state = READING_NUMBER;
                } else if (isalpha(ch)) {
                    current_state = READING_WORD;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_NUMBER:
                if (isdigit(ch)) {
                    int digit = ch - '0';
                    if (number_sum <= (INT_MAX - digit) / 10) {
                        number_sum = number_sum * 10 + digit;
                    } else {
                        current_state = ERROR;
                    }
                    pos++;
                } else if (isspace(ch) || ch == '\0') {
                    if (output_pos < sizeof(output) - 20) {
                        int written = snprintf(output + output_pos, 
                                             sizeof(output) - output_pos,
                                             "[NUM:%d] ", number_sum);
                        if (written > 0) {
                            output_pos += written;
                        }
                    }
                    number_sum = 0;
                    current_state = START;
                    pos++;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_WORD:
                if (isalpha(ch)) {
                    if (output_pos < sizeof(output) - 2) {
                        output[output_pos++] = ch;
                    }
                    pos++;
                } else if (isspace(ch) || ch == '\0') {
                    if (output_pos < sizeof(output) - 10) {
                        int written = snprintf(output + output_pos,
                                             sizeof(output) - output_pos,
                                             "[WORD] ");
                        if (written > 0) {
                            output_pos += written;
                        }
                    }
                    word_count++;
                    current_state = START;
                    pos++;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case ERROR:
                if (output_pos < sizeof(output) - 10) {
                    size_t copy_len = sizeof(output) - output_pos - 1;
                    if (copy_len > 8) copy_len = 8;
                    strncpy(output + output_pos, "[ERROR] ", copy_len);
                    output[sizeof(output) - 1] = '\0';
                    output_pos = strlen(output);
                }
                current_state = DONE;
                break;
                
            case DONE:
                break;
        }
    }
    
    if (current_state == READING_NUMBER) {
        if (output_pos < sizeof(output) - 20) {
            int written = snprintf(output + output_pos,
                                 sizeof(output) - output_pos,
                                 "[NUM:%d] ", number_sum);
            if (written > 0) {
                output_pos += written;
            }
        }
    } else if (current_state == READING_WORD) {
        if (output_pos < sizeof(output) - 10) {
            int written = snprintf(output + output_pos,
                                 sizeof(output) - output_pos,
                                 "[WORD] ");
            if (written > 0) {
                output_pos += written;
            }
        }
        word_count++;
    }
    
    if (current_state == ERROR) {
        printf("Processing error occurred\n");
    } else {
        printf("Processed output: %s\n", output);
        printf("Words found: %d\n", word_count);
    }
    
    return current_state == ERROR ? 1 : 0;
}