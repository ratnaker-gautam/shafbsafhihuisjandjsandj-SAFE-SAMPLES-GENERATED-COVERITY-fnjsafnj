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
    int number_valid = 0;
    char word[64];
    size_t word_pos = 0;
    int ch;
    int i = 0;

    printf("Enter text (max 255 chars): ");
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }

    size_t len = strcspn(buffer, "\n");
    if (len < sizeof(buffer)) {
        buffer[len] = '\0';
    }

    while (buffer[i] != '\0' && current_state != ERROR && current_state != DONE) {
        ch = (unsigned char)buffer[i];

        switch (current_state) {
            case START:
                if (isspace(ch)) {
                    i++;
                } else if (isdigit(ch)) {
                    current_state = READING_NUMBER;
                    number = ch - '0';
                    number_valid = 1;
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
                    int digit = ch - '0';
                    if (number <= (INT_MAX - digit) / 10) {
                        number = number * 10 + digit;
                    } else {
                        current_state = ERROR;
                    }
                    i++;
                } else if (isspace(ch) || ch == '\0') {
                    if (number_valid) {
                        int written = snprintf(output + output_pos, 
                                             sizeof(output) - output_pos,
                                             "Number: %d ", number);
                        if (written > 0 && (size_t)written < sizeof(output) - output_pos) {
                            output_pos += (size_t)written;
                        }
                    }
                    number = 0;
                    number_valid = 0;
                    current_state = START;
                    i++;
                } else {
                    current_state = ERROR;
                }
                break;

            case READING_WORD:
                if (isalnum(ch)) {
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
                        if (written > 0 && (size_t)written < sizeof(output) - output_pos) {
                            output_pos += (size_t)written;
                        }
                    }
                    word_pos = 0;
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

    if (current_state == READING_NUMBER && number_valid) {
        int written = snprintf(output + output_pos,
                             sizeof(output) - output_pos,
                             "Number: %d ", number);
        if (written > 0 && (size_t)written < sizeof(output) - output_pos) {
            output_pos += (size_t)written;
        }
    } else if (current_state == READING_WORD && word_pos > 0) {
        word[word_pos] = '\0';
        int written = snprintf(output + output_pos,
                             sizeof(output) - output_pos,
                             "Word: %s ", word);
        if (written > 0 && (size_t)written < sizeof(output) - output_pos) {
            output_pos += (size_t)written;
        }
    }

    if (current_state == ERROR) {
        printf("Error: Invalid input format\n");
        return 1;
    }

    if (output_pos > 0) {
        printf("Parsed: %s\n", output);
    } else {
        printf("No valid tokens found\n");
    }

    return 0;
}