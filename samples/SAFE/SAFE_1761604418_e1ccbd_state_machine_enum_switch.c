//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: state_machine
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

enum state {
    START,
    READING_NUMBER,
    READING_WORD,
    SKIPPING_SPACE,
    FINISHED,
    ERROR
};

int main(void) {
    enum state current_state = START;
    char buffer[256];
    size_t buffer_pos = 0;
    int ch;
    int number_sum = 0;
    int word_count = 0;
    
    printf("Enter text (max 255 chars): ");
    
    while (current_state != FINISHED && current_state != ERROR) {
        if (current_state == START) {
            ch = getchar();
            if (ch == EOF || ch == '\n') {
                current_state = FINISHED;
            } else if (isdigit(ch)) {
                current_state = READING_NUMBER;
                buffer_pos = 0;
                if (buffer_pos < sizeof(buffer) - 1) {
                    buffer[buffer_pos++] = (char)ch;
                }
            } else if (isalpha(ch)) {
                current_state = READING_WORD;
                buffer_pos = 0;
                if (buffer_pos < sizeof(buffer) - 1) {
                    buffer[buffer_pos++] = (char)ch;
                }
            } else if (isspace(ch)) {
                current_state = SKIPPING_SPACE;
            } else {
                current_state = ERROR;
            }
        } else if (current_state == READING_NUMBER) {
            ch = getchar();
            if (ch == EOF || ch == '\n') {
                if (buffer_pos < sizeof(buffer)) {
                    buffer[buffer_pos] = '\0';
                }
                int num = 0;
                if (sscanf(buffer, "%d", &num) == 1) {
                    if (num >= 0 && number_sum <= INT_MAX - num) {
                        number_sum += num;
                    }
                }
                current_state = FINISHED;
            } else if (isdigit(ch)) {
                if (buffer_pos < sizeof(buffer) - 1) {
                    buffer[buffer_pos++] = (char)ch;
                } else {
                    current_state = ERROR;
                }
            } else if (isspace(ch)) {
                if (buffer_pos < sizeof(buffer)) {
                    buffer[buffer_pos] = '\0';
                }
                int num = 0;
                if (sscanf(buffer, "%d", &num) == 1) {
                    if (num >= 0 && number_sum <= INT_MAX - num) {
                        number_sum += num;
                    }
                }
                current_state = SKIPPING_SPACE;
            } else {
                current_state = ERROR;
            }
        } else if (current_state == READING_WORD) {
            ch = getchar();
            if (ch == EOF || ch == '\n') {
                if (buffer_pos < sizeof(buffer)) {
                    buffer[buffer_pos] = '\0';
                }
                word_count++;
                current_state = FINISHED;
            } else if (isalpha(ch)) {
                if (buffer_pos < sizeof(buffer) - 1) {
                    buffer[buffer_pos++] = (char)ch;
                } else {
                    current_state = ERROR;
                }
            } else if (isspace(ch)) {
                if (buffer_pos < sizeof(buffer)) {
                    buffer[buffer_pos] = '\0';
                }
                word_count++;
                current_state = SKIPPING_SPACE;
            } else {
                current_state = ERROR;
            }
        } else if (current_state == SKIPPING_SPACE) {
            ch = getchar();
            if (ch == EOF || ch == '\n') {
                current_state = FINISHED;
            } else if (isdigit(ch)) {
                current_state = READING_NUMBER;
                buffer_pos = 0;
                if (buffer_pos < sizeof(buffer) - 1) {
                    buffer[buffer_pos++] = (char)ch;
                }
            } else if (isalpha(ch)) {
                current_state = READING_WORD;
                buffer_pos = 0;
                if (buffer_pos < sizeof(buffer) - 1) {
                    buffer[buffer_pos++] = (char)ch;
                }
            } else if (!isspace(ch)) {
                current_state = ERROR;
            }
        }
    }
    
    if (current_state == ERROR) {
        printf("Error: Invalid input format\n");
        return 1;
    }
    
    printf("Sum of numbers: %d\n", number_sum);
    printf("Word count: %d\n", word_count);
    
    return 0;
}