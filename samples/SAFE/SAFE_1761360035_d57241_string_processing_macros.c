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

int parse_sentence(const char *input, Word *words, size_t max_words) {
    if (input == NULL || words == NULL || max_words == 0) {
        return -1;
    }

    size_t input_len = safe_strlen(input, MAX_INPUT_LEN);
    if (input_len == 0 || input_len >= MAX_INPUT_LEN) {
        return -1;
    }

    size_t word_count = 0;
    size_t word_pos = 0;
    int in_word = 0;

    for (size_t i = 0; i < input_len && word_count < max_words; i++) {
        char c = input[i];
        
        if (IS_WHITESPACE(c)) {
            if (in_word && word_pos > 0) {
                words[word_count].text[word_pos] = '\0';
                words[word_count].length = word_pos;
                word_count++;
                word_pos = 0;
                in_word = 0;
            }
        } else if (IS_TERMINATOR(c)) {
            if (in_word && word_pos > 0) {
                words[word_count].text[word_pos] = '\0';
                words[word_count].length = word_pos;
                word_count++;
            }
            break;
        } else if (isalpha((unsigned char)c)) {
            if (word_pos < MAX_WORD_LEN - 1) {
                words[word_count].text[word_pos++] = tolower((unsigned char)c);
                in_word = 1;
            }
        }
    }

    if (in_word && word_pos > 0 && word_count < max_words) {
        words[word_count].text[word_pos] = '\0';
        words[word_count].length = word_pos;
        word_count++;
    }

    return (int)word_count;
}

void reverse_words(Word *words, int count) {
    if (words == NULL || count <= 0) {
        return;
    }

    for (int i = 0; i < count / 2; i++) {
        Word temp = words[i];
        words[i] = words[count - i - 1];
        words[count - i - 1] = temp;
    }
}

void print_words(const Word *words, int count) {
    if (words == NULL || count <= 0) {
        return;
    }

    for (int i = 0; i < count; i++) {
        if (i > 0) {
            putchar(' ');
        }
        for (size_t j = 0; j < words[i].length; j++) {
            putchar(words[i].text[j]);
        }
    }
    putchar('.');
    putchar('\n');
}

int main(void) {
    char input[MAX_INPUT_LEN];
    Word words[MAX_WORDS];
    
    printf("Enter a sentence (end with . ! or ?): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }

    int word_count = parse_sentence(input, words, MAX_WORDS);
    if (word_count <= 0) {
        fprintf(stderr, "No valid words found or input error\n");
        return EXIT_FAILURE;
    }

    printf("Original: ");
    print_words(words, word_count);

    reverse_words(words, word_count);

    printf("Reversed: ");
    print_words(words, word_count);

    return EXIT_SUCCESS;
}