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
    int number = 0;
    int has_number = 0;
    char word[64];
    size_t word_pos = 0;
    int ch;
    int i = 0;

    printf("Enter text (max 255 chars): ");
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }

    buffer[strcspn(buffer, "\n")] = '\0';

    while (buffer[i] != '\0' && current_state != ERROR && current_state != DONE) {
        ch = buffer[i];

        switch (current_state) {
            case START:
                if (isspace(ch)) {
                    i++;
                } else if (isdigit(ch)) {
                    current_state = READING_NUMBER;
                    number = ch - '0';
                    has_number = 1;
                    i++;
                } else if (isalpha(ch)) {
                    current_state = READING_WORD;
                    if (word_pos < sizeof(word) - 1) {
                        word[word_pos++] = (char)ch;
                    }
                    i++;
                } else {
                    current_state = ERROR;
                }
                break;

            case READING_NUMBER:
                if (isdigit(ch)) {
                    if (number <= (INT_MAX - (ch - '0')) / 10) {
                        number = number * 10 + (ch - '0');
                    } else {
                        current_state = ERROR;
                    }
                    i++;
                } else if (isspace(ch) || ch == '\0') {
                    if (has_number) {
                        int written = snprintf(output + output_pos, 
                                             sizeof(output) - output_pos,
                                             "Number: %d ", number);
                        if (written > 0 && output_pos + (size_t)written < sizeof(output)) {
                            output_pos += (size_t)written;
                        }
                        has_number = 0;
                        number = 0;
                    }
                    current_state = START;
                    i++;
                } else {
                    current_state = ERROR;
                }
                break;

            case READING_WORD:
                if (isalpha(ch)) {
                    if (word_pos < sizeof(word) - 1) {
                        word[word_pos++] = (char)ch;
                    }
                    i++;
                } else if (isspace(ch) || ch == '\0') {
                    if (word_pos > 0) {
                        word[word_pos] = '\0';
                        int written = snprintf(output + output_pos,
                                             sizeof(output) - output_pos,
                                             "Word: %s ", word);
                        if (written > 0 && output_pos + (size_t)written < sizeof(output)) {
                            output_pos += (size_t)written;
                        }
                        word_pos = 0;
                    }
                    current_state = START;
                    i++;
                } else {
                    current_state = ERROR;
                }
                break;

            case ERROR:
                break;

            case DONE:
                break;
        }
    }

    if (current_state == READING_NUMBER && has_number) {
        int written = snprintf(output + output_pos,
                             sizeof(output) - output_pos,
                             "Number: %d ", number);
        if (written > 0 && output_pos + (size_t)written < sizeof(output)) {
            output_pos += (size_t)written;
        }
    } else if (current_state == READING_WORD && word_pos > 0) {
        word[word_pos] = '\0';
        int written = snprintf(output + output_pos,
                             sizeof(output) - output_pos,
                             "Word: %s ", word);
        if (written > 0 && output_pos + (size_t)written < sizeof(output)) {
            output_pos += (size_t)written;
        }
    }

    if (current_state == ERROR) {
        printf("Error: Invalid input format\n");
        return 1;
    }

    if (output_pos > 0 && output_pos < sizeof(output)) {
        output[output_pos] = '\0';
        printf("Processed: %s\n", output);
    } else {
        printf("No valid tokens found\n");
    }

    return 0;
}