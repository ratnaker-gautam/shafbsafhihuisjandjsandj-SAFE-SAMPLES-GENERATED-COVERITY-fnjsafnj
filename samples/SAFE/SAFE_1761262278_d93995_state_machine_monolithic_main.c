//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum State { START, READING_NUMBER, READING_WORD, ERROR, DONE };

int main(void) {
    char input[256];
    char buffer[256];
    int buffer_index = 0;
    enum State current_state = START;
    int number_sum = 0;
    int word_count = 0;
    int number_count = 0;
    int i = 0;
    int valid_input = 0;

    printf("Enter text (max 255 chars): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }

    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }

    if (input_len == 0) {
        printf("No input provided\n");
        return 1;
    }

    while (i <= input_len && current_state != ERROR && current_state != DONE) {
        char c = (i < input_len) ? input[i] : ' ';

        switch (current_state) {
            case START:
                if (isspace(c)) {
                    i++;
                } else if (isdigit(c)) {
                    current_state = READING_NUMBER;
                    buffer_index = 0;
                    if (buffer_index < 255) {
                        buffer[buffer_index++] = c;
                    }
                    i++;
                } else if (isalpha(c)) {
                    current_state = READING_WORD;
                    buffer_index = 0;
                    if (buffer_index < 255) {
                        buffer[buffer_index++] = tolower(c);
                    }
                    i++;
                } else {
                    current_state = ERROR;
                }
                break;

            case READING_NUMBER:
                if (isdigit(c)) {
                    if (buffer_index < 255) {
                        buffer[buffer_index++] = c;
                    }
                    i++;
                } else if (isspace(c) || i == input_len) {
                    if (buffer_index > 0 && buffer_index < 256) {
                        buffer[buffer_index] = '\0';
                        int num = atoi(buffer);
                        number_sum += num;
                        number_count++;
                    }
                    buffer_index = 0;
                    current_state = START;
                    i++;
                } else {
                    current_state = ERROR;
                }
                break;

            case READING_WORD:
                if (isalpha(c)) {
                    if (buffer_index < 255) {
                        buffer[buffer_index++] = tolower(c);
                    }
                    i++;
                } else if (isspace(c) || i == input_len) {
                    if (buffer_index > 0 && buffer_index < 256) {
                        buffer[buffer_index] = '\0';
                        word_count++;
                    }
                    buffer_index = 0;
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

    if (current_state == ERROR) {
        printf("Invalid input format\n");
        return 1;
    }

    printf("Words found: %d\n", word_count);
    printf("Numbers found: %d\n", number_count);
    if (number_count > 0) {
        printf("Sum of numbers: %d\n", number_sum);
    }

    return 0;
}