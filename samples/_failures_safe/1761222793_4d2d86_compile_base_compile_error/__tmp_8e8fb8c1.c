//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORD_LEN 64
#define MAX_WORDS 100
#define MAX_TEXT_LEN 1024

struct WordStats {
    char word[MAX_WORD_LEN];
    int count;
    int length;
    int is_capitalized;
};

struct TextProcessor {
    struct WordStats words[MAX_WORDS];
    int word_count;
    int total_chars;
    int total_words;
    char processed_text[MAX_TEXT_LEN];
};

void init_text_processor(struct TextProcessor *tp) {
    tp->word_count = 0;
    tp->total_chars = 0;
    tp->total_words = 0;
    memset(tp->processed_text, 0, sizeof(tp->processed_text));
    for (int i = 0; i < MAX_WORDS; i++) {
        memset(tp->words[i].word, 0, sizeof(tp->words[i].word));
        tp->words[i].count = 0;
        tp->words[i].length = 0;
        tp->words[i].is_capitalized = 0;
    }
}

int is_valid_char(char c) {
    return isalpha(c) || c == ' ' || c == '\0';
}

int add_word(struct TextProcessor *tp, const char *word, int len, int capitalized) {
    if (tp->word_count >= MAX_WORDS || len <= 0 || len >= MAX_WORD_LEN) {
        return 0;
    }

    for (int i = 0; i < tp->word_count; i++) {
        if (strncmp(tp->words[i].word, word, len) == 0 && tp->words[i].word[len] == '\0') {
            tp->words[i].count++;
            return 1;
        }
    }

    strncpy(tp->words[tp->word_count].word, word, len);
    tp->words[tp->word_count].word[len] = '\0';
    tp->words[tp->word_count].count = 1;
    tp->words[tp->word_count].length = len;
    tp->words[tp->word_count].is_capitalized = capitalized;
    tp->word_count++;
    return 1;
}

int process_text(struct TextProcessor *tp, const char *input) {
    if (input == NULL || strlen(input) >= MAX_TEXT_LEN) {
        return 0;
    }

    strncpy(tp->processed_text, input, MAX_TEXT_LEN - 1);
    tp->processed_text[MAX_TEXT_LEN - 1] = '\0';

    char buffer[MAX_WORD_LEN];
    int buf_pos = 0;
    int in_word = 0;
    int capitalized = 0;

    for (int i = 0; tp->processed_text[i] != '\0'; i++) {
        if (!is_valid_char(tp->processed_text[i])) {
            return 0;
        }

        if (isalpha(tp->processed_text[i])) {
            if (!in_word) {
                capitalized = isupper(tp->processed_text[i]) ? 1 : 0;
                in_word = 1;
            }
            if (buf_pos < MAX_WORD_LEN - 1) {
                buffer[buf_pos++] = tolower(tp->processed_text[i]);
            }
        } else if (tp->processed_text[i] == ' ') {
            if (in_word && buf_pos > 0) {
                buffer[buf_pos] = '\0';
                if (!add_word(tp, buffer, buf_pos, capitalized)) {
                    return 0;
                }
                tp->total_words++;
                tp->total_chars += buf_pos;
                buf_pos = 0;
                in_word = 0;
                capitalized = 0;
            }
        }
    }

    if (in_word && buf_pos > 0) {
        buffer[buf_pos] = '\0';
        if (!add_word(tp, buffer, buf_pos, capitalized)) {
            return 0;
        }
        tp->total_words++;
        tp->total_chars += buf_pos;
    }

    return 1;
}

void print_stats(const struct TextProcessor *tp) {
    printf("Text analysis results:\n");
    printf("Total words: %d\n", tp->total_words);
    printf("Total characters: %d\n", tp->total_chars);
    printf("Unique words: %d\n", tp->word_count);
    printf("\nWord frequency:\n");

    for (int i = 0; i < tp->word_count; i++) {
        printf("'%s': %d occurrence(s), length: %d, %s\n",
               tp->words[i].word,
               tp->words[i].count,
               tp->words[i].length,
               tp->words[i].is_capitalized ? "capitalized" : "lowercase");
    }
}

int main(void) {
    struct TextProcessor processor;
    char input_text[MAX_TEXT_LEN];

    init_text_processor(&processor);

    printf("Enter text to analyze (letters and spaces only): ");
    if (fgets(input_text, sizeof(input_text), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }

    size_t len = strlen(input_text);
    if (len > 0 && input_text[len - 1] == '\n') {
        input_text[len - 1] = '\0';
    }

    if (strlen(input_text) == 0