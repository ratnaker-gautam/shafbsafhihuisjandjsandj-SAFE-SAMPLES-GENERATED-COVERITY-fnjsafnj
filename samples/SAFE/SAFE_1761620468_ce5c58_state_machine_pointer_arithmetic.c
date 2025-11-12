//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum State { START, IN_WORD, IN_NUMBER, IN_OPERATOR, ERROR, END };

int main(void) {
    char input[256];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return EXIT_FAILURE;
    }
    char *p = input;
    enum State current = START;
    int word_count = 0;
    int number_count = 0;
    int operator_count = 0;
    while (*p != '\0' && *p != '\n') {
        switch (current) {
            case START:
                if (isalpha(*p)) {
                    current = IN_WORD;
                    word_count++;
                } else if (isdigit(*p)) {
                    current = IN_NUMBER;
                    number_count++;
                } else if (strchr("+-*/", *p) != NULL) {
                    current = IN_OPERATOR;
                    operator_count++;
                } else if (!isspace(*p)) {
                    current = ERROR;
                }
                break;
            case IN_WORD:
                if (!isalnum(*p)) {
                    if (strchr("+-*/", *p) != NULL) {
                        current = IN_OPERATOR;
                        operator_count++;
                    } else if (isdigit(*p)) {
                        current = IN_NUMBER;
                        number_count++;
                    } else if (isspace(*p)) {
                        current = START;
                    } else {
                        current = ERROR;
                    }
                }
                break;
            case IN_NUMBER:
                if (!isdigit(*p)) {
                    if (isalpha(*p)) {
                        current = IN_WORD;
                        word_count++;
                    } else if (strchr("+-*/", *p) != NULL) {
                        current = IN_OPERATOR;
                        operator_count++;
                    } else if (isspace(*p)) {
                        current = START;
                    } else {
                        current = ERROR;
                    }
                }
                break;
            case IN_OPERATOR:
                if (isalpha(*p)) {
                    current = IN_WORD;
                    word_count++;
                } else if (isdigit(*p)) {
                    current = IN_NUMBER;
                    number_count++;
                } else if (strchr("+-*/", *p) != NULL) {
                    operator_count++;
                } else if (isspace(*p)) {
                    current = START;
                } else {
                    current = ERROR;
                }
                break;
            case ERROR:
                if (isspace(*p)) {
                    current = START;
                }
                break;
            case END:
                break;
        }
        p++;
    }
    printf("Words: %d\nNumbers: %d\nOperators: %d\n", word_count, number_count, operator_count);
    return EXIT_SUCCESS;
}