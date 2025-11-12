//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 256
#define MAX_WORD_LEN 64
#define IS_WHITESPACE(c) ((c) == ' ' || (c) == '\t' || (c) == '\n')
#define IS_TERMINATOR(c) ((c) == '.' || (c) == '!' || (c) == '?')

typedef struct {
    char text[MAX_WORD_LEN];
    size_t length;
} Word;

size_t safe_strlen(const char *str, size_t max_len) {
    size_t len = 0;
    while (len < max_len && str[len] != '\0') {
        len++;
    }
    return len;
}

int parse_words(const char *input, Word *words, size_t max_words) {
    size_t input_len = safe_strlen(input, MAX_INPUT_LEN);
    if (input_len == 0 || input_len >= MAX_INPUT_LEN) {
        return -1;
    }

    size_t word_count = 0;
    size_t pos = 0;
    size_t word_start = 0;
    int in_word = 0;

    while (pos < input_len && word_count < max_words) {
        if (IS_WHITESPACE(input[pos]) || IS_TERMINATOR(input[pos])) {
            if (in_word) {
                size_t word_len = pos - word_start;
                if (word_len > 0 && word_len < MAX_WORD_LEN) {
                    memcpy(words[word_count].text, &input[word_start], word_len);
                    words[word_count].text[word_len] = '\0';
                    words[word_count].length = word_len;
                    word_count++;
                }
                in_word = 0;
            }
        } else {
            if (!in_word) {
                word_start = pos;
                in_word = 1;
            }
        }
        pos++;
    }

    if (in_word && word_count < max_words) {
        size_t word_len = pos - word_start;
        if (word_len > 0 && word_len < MAX_WORD_LEN) {
            memcpy(words[word_count].text, &input[word_start], word_len);
            words[word_count].text[word_len] = '\0';
            words[word_count].length = word_len;
            word_count++;
        }
    }

    return (int)word_count;
}

void capitalize_words(Word *words, int count) {
    for (int i = 0; i < count; i++) {
        if (words[i].length > 0 && islower(words[i].text[0])) {
            words[i].text[0] = (char)toupper(words[i].text[0]);
        }
    }
}

void reverse_words(Word *words, int count) {
    for (int i = 0; i < count / 2; i++) {
        Word temp = words[i];
        words[i] = words[count - i - 1];
        words[count - i - 1] = temp;
    }
}

int main(void) {
    char input[MAX_INPUT_LEN];
    Word words[MAX_WORDS];

    printf("Enter text (max %d characters): ", MAX_INPUT_LEN - 1);
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }

    size_t input_len = safe_strlen(input, sizeof(input));
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
    }

    int word_count = parse_words(input, words, MAX_WORDS);
    if (word_count <= 0) {
        printf("No words found in input\n");
        return EXIT_SUCCESS;
    }

    printf("Original words: ");
    for (int i = 0; i < word_count; i++) {
        printf("%s ", words[i].text);
    }
    printf("\n");

    capitalize_words(words, word_count);
    printf("Capitalized: ");
    for (int i = 0; i < word_count; i++) {
        printf("%s ", words[i].text);
    }
    printf("\n");

    reverse_words(words, word_count);
    printf("Reversed: ");
    for (int i = 0; i < word_count; i++) {
        printf("%s ", words[i].text);
    }
    printf("\n");

    return EXIT_SUCCESS;
}