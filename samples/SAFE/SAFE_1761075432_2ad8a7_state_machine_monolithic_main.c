//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: monolithic_main ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum State {
    START,
    READING_NUMBER,
    READING_WORD,
    SKIPPING_SPACE,
    FINISHED,
    ERROR
};

int main(void) {
    char input[1024];
    char buffer[1024];
    int buf_index = 0;
    enum State current_state = START;
    int number_sum = 0;
    int word_count = 0;
    int number_count = 0;
    int i = 0;
    int input_len = 0;

    printf("Enter text (max 1023 chars): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }

    input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }

    if (input_len >= sizeof(input)) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }

    while (i <= input_len && current_state != FINISHED && current_state != ERROR) {
        char c = (i < input_len) ? input[i] : '\0';

        switch (current_state) {
            case START:
                if (c == '\0') {
                    current_state = FINISHED;
                } else if (isdigit(c)) {
                    buf_index = 0;
                    if (buf_index < sizeof(buffer) - 1) {
                        buffer[buf_index++] = c;
                    }
                    current_state = READING_NUMBER;
                } else if (isalpha(c)) {
                    buf_index = 0;
                    if (buf_index < sizeof(buffer) - 1) {
                        buffer[buf_index++] = tolower(c);
                    }
                    current_state = READING_WORD;
                } else if (isspace(c)) {
                    current_state = SKIPPING_SPACE;
                } else {
                    current_state = ERROR;
                }
                i++;
                break;

            case READING_NUMBER:
                if (c == '\0') {
                    buffer[buf_index] = '\0';
                    if (buf_index > 0) {
                        int num = atoi(buffer);
                        number_sum += num;
                        number_count++;
                    }
                    current_state = FINISHED;
                } else if (isdigit(c)) {
                    if (buf_index < sizeof(buffer) - 1) {
                        buffer[buf_index++] = c;
                    }
                    i++;
                } else if (isspace(c)) {
                    buffer[buf_index] = '\0';
                    if (buf_index > 0) {
                        int num = atoi(buffer);
                        number_sum += num;
                        number_count++;
                    }
                    current_state = SKIPPING_SPACE;
                    i++;
                } else {
                    current_state = ERROR;
                }
                break;

            case READING_WORD:
                if (c == '\0') {
                    buffer[buf_index] = '\0';
                    if (buf_index > 0) {
                        word_count++;
                    }
                    current_state = FINISHED;
                } else if (isalpha(c)) {
                    if (buf_index < sizeof(buffer) - 1) {
                        buffer[buf_index++] = tolower(c);
                    }
                    i++;
                } else if (isspace(c)) {
                    buffer[buf_index] = '\0';
                    if (buf_index > 0) {
                        word_count++;
                    }
                    current_state = SKIPPING_SPACE;
                    i++;
                } else {
                    current_state = ERROR;
                }
                break;

            case SKIPPING_SPACE:
                if (c == '\0') {
                    current_state = FINISHED;
                } else if (isspace(c)) {
                    i++;
                } else if (isdigit(c)) {
                    buf_index = 0;
                    if (buf_index < sizeof(buffer) - 1) {
                        buffer[buf_index++] = c;
                    }
                    current_state = READING_NUMBER;
                    i++;
                } else if (isalpha(c)) {
                    buf_index = 0;
                    if (buf_index < sizeof(buffer) - 1) {
                        buffer[buf_index++] = tolower(c);
                    }
                    current_state = READING_WORD;
                    i++;
                } else {
                    current_state = ERROR;
                }
                break;

            case FINISHED:
                break;

            case ERROR:
                break;
        }
    }

    if (current_state == ERROR) {
        fprintf(stderr, "Invalid input format\n");
        return 1;
    }

    printf("Words found: %d\n", word_count);
    printf("Numbers found: %d\n", number_count);
    if (number_count > 0) {
        printf("Sum of numbers: %d\n", number_sum);
        printf("Average of numbers: %.2f\n", (double)number_sum / number_count);
    } else {
        printf("No numbers to sum or average\n");
    }

    return 0;
}