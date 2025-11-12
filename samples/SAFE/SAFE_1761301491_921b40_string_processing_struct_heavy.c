//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORDS 100
#define MAX_WORD_LEN 50
#define MAX_TEXT_LEN 1000

struct WordStats {
    char word[MAX_WORD_LEN];
    int count;
    int length;
    int is_capitalized;
};

struct TextProcessor {
    struct WordStats words[MAX_WORDS];
    int word_count;
    char processed_text[MAX_TEXT_LEN];
    int total_chars;
    int total_words;
};

void init_text_processor(struct TextProcessor *tp) {
    tp->word_count = 0;
    tp->processed_text[0] = '\0';
    tp->total_chars = 0;
    tp->total_words = 0;
    for (int i = 0; i < MAX_WORDS; i++) {
        tp->words[i].word[0] = '\0';
        tp->words[i].count = 0;
        tp->words[i].length = 0;
        tp->words[i].is_capitalized = 0;
    }
}

int is_word_char(char c) {
    return isalpha((unsigned char)c) || c == '\'' || c == '-';
}

int add_word(struct TextProcessor *tp, const char *word, int len, int capitalized) {
    if (tp->word_count >= MAX_WORDS || len >= MAX_WORD_LEN || len <= 0) {
        return 0;
    }

    for (int i = 0; i < tp->word_count; i++) {
        if (strncmp(tp->words[i].word, word, len) == 0 && tp->words[i].word[len] == '\0') {
            tp->words[i].count++;
            return 1;
        }
    }

    if (tp->word_count < MAX_WORDS) {
        strncpy(tp->words[tp->word_count].word, word, len);
        tp->words[tp->word_count].word[len] = '\0';
        tp->words[tp->word_count].count = 1;
        tp->words[tp->word_count].length = len;
        tp->words[tp->word_count].is_capitalized = capitalized;
        tp->word_count++;
        return 1;
    }
    return 0;
}

void process_text(struct TextProcessor *tp, const char *input) {
    if (input == NULL) {
        return;
    }

    int input_len = strlen(input);
    if (input_len >= MAX_TEXT_LEN) {
        return;
    }

    strncpy(tp->processed_text, input, MAX_TEXT_LEN - 1);
    tp->processed_text[MAX_TEXT_LEN - 1] = '\0';
    tp->total_chars = input_len;

    int i = 0;
    while (i < input_len && tp->total_words < MAX_WORDS) {
        while (i < input_len && !is_word_char(input[i])) {
            i++;
        }

        if (i >= input_len) {
            break;
        }

        int start = i;
        int word_len = 0;
        int capitalized = isupper((unsigned char)input[i]);

        while (i < input_len && is_word_char(input[i]) && word_len < MAX_WORD_LEN - 1) {
            i++;
            word_len++;
        }

        if (word_len > 0 && word_len < MAX_WORD_LEN) {
            char temp_word[MAX_WORD_LEN];
            strncpy(temp_word, &input[start], word_len);
            temp_word[word_len] = '\0';
            if (add_word(tp, temp_word, word_len, capitalized)) {
                tp->total_words++;
            }
        }
    }
}

void print_stats(const struct TextProcessor *tp) {
    printf("Text Statistics:\n");
    printf("Total characters: %d\n", tp->total_chars);
    printf("Total unique words: %d\n", tp->word_count);
    printf("Total words processed: %d\n", tp->total_words);
    printf("\nWord frequency:\n");

    for (int i = 0; i < tp->word_count; i++) {
        printf("'%s': %d occurrences (length: %d, %s)\n",
               tp->words[i].word,
               tp->words[i].count,
               tp->words[i].length,
               tp->words[i].is_capitalized ? "capitalized" : "lowercase");
    }
}

int main() {
    struct TextProcessor processor;
    char input_text[MAX_TEXT_LEN];

    init_text_processor(&processor);

    printf("Enter text to analyze (max %d characters): ", MAX_TEXT_LEN - 1);
    if (fgets(input_text, sizeof(input_text), stdin) == NULL) {
        printf("Error reading input.\n");
        return 1;
    }

    size_t len = strlen(input_text);
    if (len > 0 && input_text[len - 1] == '\n') {
        input_text[len - 1] = '\0';
    }

    if (strlen(input_text) == 0) {
        printf("No input provided.\n");
        return 1;
    }

    process_text(&processor, input_text);
    print_stats(&processor);

    return 0;
}