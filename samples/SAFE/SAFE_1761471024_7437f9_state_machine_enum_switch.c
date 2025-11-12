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
                        output_pos += snprintf(output + output_pos, 
                                             sizeof(output) - output_pos,
                                             "[NUM:%d] ", number_sum);
                        number_sum = 0;
                        current_state = START;
                    } else {
                        current_state = ERROR;
                    }
                    pos++;
                } else {
                    current_state = ERROR;
                }
                break;

            case READING_WORD:
                if (isalpha(ch)) {
                    if (output_pos < sizeof(output) - 2) {
                        output[output_pos++] = ch;
                    } else {
                        current_state = ERROR;
                    }
                    pos++;
                } else if (isspace(ch) || ch == '\0') {
                    if (output_pos < sizeof(output) - 3) {
                        output[output_pos++] = ' ';
                        word_count++;
                        current_state = START;
                    } else {
                        current_state = ERROR;
                    }
                    pos++;
                } else {
                    current_state = ERROR;
                }
                break;

            case ERROR:
                fprintf(stderr, "Error: Invalid input format\n");
                return 1;

            case DONE:
                break;
        }
    }

    if (current_state == READING_NUMBER) {
        if (output_pos < sizeof(output) - 20) {
            output_pos += snprintf(output + output_pos, 
                                 sizeof(output) - output_pos,
                                 "[NUM:%d] ", number_sum);
        } else {
            current_state = ERROR;
        }
    } else if (current_state == READING_WORD) {
        if (output_pos < sizeof(output) - 1) {
            output[output_pos++] = ' ';
            word_count++;
        } else {
            current_state = ERROR;
        }
    }

    if (current_state != ERROR) {
        if (output_pos < sizeof(output)) {
            output[output_pos] = '\0';
        }
        printf("Processed output: %s\n", output);
        printf("Words found: %d\n", word_count);
    } else {
        fprintf(stderr, "Error: Invalid input format\n");
    }

    return current_state == ERROR ? 1 : 0;
}