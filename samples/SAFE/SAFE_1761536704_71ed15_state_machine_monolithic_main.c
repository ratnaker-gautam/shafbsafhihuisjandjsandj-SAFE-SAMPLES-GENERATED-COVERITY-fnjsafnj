//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum State {
    STATE_START,
    STATE_READING_NUMBER,
    STATE_READING_WORD,
    STATE_ERROR,
    STATE_DONE
};

int main(void) {
    char input[256];
    char buffer[256];
    int buffer_index = 0;
    enum State current_state = STATE_START;
    int number_sum = 0;
    int word_count = 0;
    int number_count = 0;
    int input_index = 0;
    int valid_input = 1;

    printf("Enter text (max 255 chars): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }

    input[strcspn(input, "\n")] = '\0';

    while (input[input_index] != '\0' && current_state != STATE_ERROR && current_state != STATE_DONE) {
        char c = input[input_index];

        switch (current_state) {
            case STATE_START:
                if (isdigit(c)) {
                    buffer_index = 0;
                    buffer[buffer_index++] = c;
                    current_state = STATE_READING_NUMBER;
                } else if (isalpha(c)) {
                    buffer_index = 0;
                    buffer[buffer_index++] = tolower(c);
                    current_state = STATE_READING_WORD;
                } else if (c != ' ' && c != '\t') {
                    current_state = STATE_ERROR;
                }
                break;

            case STATE_READING_NUMBER:
                if (isdigit(c)) {
                    if (buffer_index < 255) {
                        buffer[buffer_index++] = c;
                    } else {
                        current_state = STATE_ERROR;
                    }
                } else if (c == ' ' || c == '\t' || c == '\0') {
                    buffer[buffer_index] = '\0';
                    int num = atoi(buffer);
                    number_sum += num;
                    number_count++;
                    current_state = STATE_START;
                } else {
                    current_state = STATE_ERROR;
                }
                break;

            case STATE_READING_WORD:
                if (isalpha(c)) {
                    if (buffer_index < 255) {
                        buffer[buffer_index++] = tolower(c);
                    } else {
                        current_state = STATE_ERROR;
                    }
                } else if (c == ' ' || c == '\t' || c == '\0') {
                    buffer[buffer_index] = '\0';
                    if (strlen(buffer) > 0) {
                        word_count++;
                    }
                    current_state = STATE_START;
                } else {
                    current_state = STATE_ERROR;
                }
                break;

            default:
                current_state = STATE_ERROR;
                break;
        }

        input_index++;
    }

    if (current_state == STATE_READING_NUMBER) {
        buffer[buffer_index] = '\0';
        int num = atoi(buffer);
        number_sum += num;
        number_count++;
        current_state = STATE_DONE;
    } else if (current_state == STATE_READING_WORD) {
        buffer[buffer_index] = '\0';
        if (strlen(buffer) > 0) {
            word_count++;
        }
        current_state = STATE_DONE;
    } else if (current_state == STATE_START) {
        current_state = STATE_DONE;
    }

    if (current_state == STATE_ERROR) {
        printf("Invalid input format\n");
        valid_input = 0;
    }

    if (valid_input) {
        printf("Words: %d\n", word_count);
        printf("Numbers: %d\n", number_count);
        if (number_count > 0) {
            printf("Number sum: %d\n", number_sum);
        }
    }

    return valid_input ? 0 : 1;
}