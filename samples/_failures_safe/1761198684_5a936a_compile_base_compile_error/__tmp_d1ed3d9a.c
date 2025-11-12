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

int parse_sentence(const char *input, Word words[MAX_WORDS], size_t *word_count) {
    size_t input_len = safe_strlen(input, MAX_INPUT_LEN);
    if (input_len == 0 || input_len >= MAX_INPUT_LEN) {
        return 0;
    }

    *word_count = 0;
    size_t pos = 0;
    size_t word_start = 0;
    int in_word = 0;

    while (pos < input_len && *word_count < MAX_WORDS) {
        if (IS_WHITESPACE(input[pos])) {
            if (in_word) {
                size_t word_len = pos - word_start;
                if (word_len > 0 && word_len < MAX_WORD_LEN) {
                    strncpy(words[*word_count].text, &input[word_start], word_len);
                    words[*word_count].text[word_len] = '\0';
                    words[*word_count].length = word_len;
                    (*word_count)++;
                }
                in_word = 0;
            }
        } else if (IS_TERMINATOR(input[pos])) {
            if (in_word) {
                size_t word_len = pos - word_start;
                if (word_len > 0 && word_len < MAX_WORD_LEN) {
                    strncpy(words[*word_count].text, &input[word_start], word_len);
                    words[*word_count].text[word_len] = '\0';
                    words[*word_count].length = word_len;
                    (*word_count)++;
                }
            }
            break;
        } else {
            if (!in_word) {
                word_start = pos;
                in_word = 1;
            }
        }
        pos++;
    }

    if (in_word && *word_count < MAX_WORDS) {
        size_t word_len = pos - word_start;
        if (word_len > 0 && word_len < MAX_WORD_LEN) {
            strncpy(words[*word_count].text, &input[word_start], word_len);
            words[*word_count].text[word_len] = '\0';
            words[*word_count].length = word_len;
            (*word_count)++;
        }
    }

    return (*word_count > 0);
}

void capitalize_words(Word words[MAX_WORDS], size_t word_count) {
    for (size_t i = 0; i < word_count; i++) {
        if (words[i].length > 0 && islower(words[i].text[0])) {
            words[i].text[0] = toupper(words[i].text[0]);
        }
    }
}

void reverse_words(Word words[MAX_WORDS], size_t word_count) {
    for (size_t i = 0; i < word_count / 2; i++) {
        Word temp = words[i];
        words[i] = words[word_count - 1 - i];
        words[word_count - 1 - i] = temp;
    }
}

int build_output(const Word words[MAX_WORDS], size_t word_count, char *output, size_t output_size) {
    if (output_size < 2) return 0;
    
    size_t pos = 0;
    for (size_t i = 0; i < word_count; i++) {
        if (pos + words[i].length + 2 >= output_size) {
            return 0;
        }
        if (i > 0) {
            output[pos++] = ' ';
        }
        strncpy(&output[pos], words[i].text, words[i].length);
        pos += words[i].length;
    }
    output[pos++] = '.';
    output[pos] = '\0';
    return 1;
}

int main(void) {
    char input[MAX_INPUT_LEN];
    Word words[MAX_WORDS];
    size_t word_count;
    char output[MAX_INPUT_LEN];

    printf("Enter a sentence (end with . ! or ?): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }

    if (!parse_sentence(input, words, &word_count)) {
        fprintf(stderr, "Invalid input or no words found\n");
        return 1;
    }

    printf("Original: ");
    if (build_output(words, word_count, output, sizeof(output))) {
        printf("%s\n", output);
    }

    capitalize_words(words, word_count);
    printf("Capitalized: ");
    if (build_output(words, word_count, output, sizeof(output))) {
        printf("%s\n", output);
    }

    reverse_words(words, word_count);
    printf("Reversed: ");
    if (build_output(words, word_count, output, sizeof(output))) {