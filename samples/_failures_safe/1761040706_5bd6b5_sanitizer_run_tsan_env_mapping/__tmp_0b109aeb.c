//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: pointer_arithmetic ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024

int main(void) {
    char input[MAX_INPUT_LEN];
    char *p = input;
    char *end = input + MAX_INPUT_LEN - 1;
    int c;
    size_t len = 0;

    printf("Enter a string (max %d chars): ", MAX_INPUT_LEN - 1);
    
    while ((c = getchar()) != '\n' && c != EOF) {
        if (p < end) {
            *p++ = (char)c;
            len++;
        } else {
            printf("Input too long. Truncating.\n");
            break;
        }
    }
    *p = '\0';

    if (len == 0) {
        printf("No input provided.\n");
        return 1;
    }

    p = input;
    char *q = input;
    while (*p) {
        if (isalnum((unsigned char)*p)) {
            *q++ = *p;
        }
        p++;
    }
    *q = '\0';

    len = strlen(input);
    if (len == 0) {
        printf("No alphanumeric characters found.\n");
        return 1;
    }

    p = input;
    char *r = input + len - 1;
    while (p < r) {
        char temp = *p;
        *p = *r;
        *r = temp;
        p++;
        r--;
    }

    printf("Processed string: %s\n", input);

    p = input;
    int word_count = 0;
    int in_word = 0;
    while (*p) {
        if (isalnum((unsigned char)*p)) {
            if (!in_word) {
                word_count++;
                in_word = 1;
            }
        } else {
            in_word = 0;
        }
        p++;
    }

    printf("Word count: %d\n", word_count);

    return 0;
}