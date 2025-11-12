//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum State {
    INIT,
    READING_NUMBERS,
    READING_WORDS,
    PROCESSING,
    DONE,
    ERROR
};

struct StateMachine {
    enum State current_state;
    int number_count;
    int word_count;
    int total_numbers;
    int total_words;
};

void init_state_machine(struct StateMachine *sm) {
    sm->current_state = INIT;
    sm->number_count = 0;
    sm->word_count = 0;
    sm->total_numbers = 0;
    sm->total_words = 0;
}

int is_valid_input(const char *input) {
    if (input == NULL) return 0;
    size_t len = strlen(input);
    if (len == 0 || len > 100) return 0;
    for (size_t i = 0; i < len; i++) {
        if (!isprint(input[i])) return 0;
    }
    return 1;
}

int is_number(const char *str) {
    if (str == NULL || *str == '\0') return 0;
    char *endptr;
    strtol(str, &endptr, 10);
    return *endptr == '\0';
}

void process_input(struct StateMachine *sm, const char *input) {
    if (!is_valid_input(input)) {
        sm->current_state = ERROR;
        return;
    }

    switch (sm->current_state) {
        case INIT:
            printf("Starting processing. Enter 'numbers' or 'words' to begin.\n");
            if (strcmp(input, "numbers") == 0) {
                sm->current_state = READING_NUMBERS;
                printf("Reading numbers mode. Enter numbers one per line.\n");
            } else if (strcmp(input, "words") == 0) {
                sm->current_state = READING_WORDS;
                printf("Reading words mode. Enter words one per line.\n");
            } else {
                sm->current_state = ERROR;
            }
            break;

        case READING_NUMBERS:
            if (strcmp(input, "process") == 0) {
                sm->current_state = PROCESSING;
            } else if (is_number(input)) {
                sm->number_count++;
                printf("Number %d recorded.\n", sm->number_count);
            } else {
                sm->current_state = ERROR;
            }
            break;

        case READING_WORDS:
            if (strcmp(input, "process") == 0) {
                sm->current_state = PROCESSING;
            } else {
                sm->word_count++;
                printf("Word %d recorded.\n", sm->word_count);
            }
            break;

        case PROCESSING:
            if (sm->current_state == READING_NUMBERS) {
                sm->total_numbers += sm->number_count;
                printf("Processed %d numbers. Total: %d\n", sm->number_count, sm->total_numbers);
                sm->number_count = 0;
            } else {
                sm->total_words += sm->word_count;
                printf("Processed %d words. Total: %d\n", sm->word_count, sm->total_words);
                sm->word_count = 0;
            }
            sm->current_state = INIT;
            break;

        case DONE:
            break;

        case ERROR:
            break;
    }
}

int main() {
    struct StateMachine sm;
    init_state_machine(&sm);

    char input[1024];
    int continue_processing = 1;

    printf("State Machine Processor\n");
    printf("Enter 'quit' to exit.\n");

    while (continue_processing && sm.current_state != ERROR) {
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }

        size_t len = strlen(input);
        if (len > 0 && input[len - 1] == '\n') {
            input[len - 1] = '\0';
        }

        if (strcmp(input, "quit") == 0) {
            continue_processing = 0;
            sm.current_state = DONE;
        } else {
            process_input(&sm, input);
        }

        if (sm.current_state == ERROR) {
            printf("Invalid input or state transition error.\n");
            continue_processing = 0;
        }
    }

    printf("Final totals - Numbers: %d, Words: %d\n", sm.total_numbers, sm.total_words);
    return 0;
}