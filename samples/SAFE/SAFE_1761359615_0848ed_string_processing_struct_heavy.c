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

int is_word_char(char c) {
    return isalpha((unsigned char)c) || c == '\'' || c == '-';
}

int add_word(struct TextProcessor *tp, const char *word, int len) {
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
    tp->words[tp->word_count].is_capitalized = isupper((unsigned char)word[0]);
    tp->word_count++;
    return 1;
}

int process_text(struct TextProcessor *tp, const char *text) {
    if (text == NULL) {
        return 0;
    }

    size_t text_len = strlen(text);
    if (text_len == 0 || text_len >= MAX_TEXT_LEN) {
        return 0;
    }

    strncpy(tp->processed_text, text, MAX_TEXT_LEN - 1);
    tp->processed_text[MAX_TEXT_LEN - 1] = '\0';
    tp->total_chars = (int)text_len;

    int in_word = 0;
    char current_word[MAX_WORD_LEN];
    int word_len = 0;
    memset(current_word, 0, sizeof(current_word));

    for (size_t i = 0; i <= text_len; i++) {
        char c = text[i];
        if (is_word_char(c) && word_len < MAX_WORD_LEN - 1) {
            if (!in_word) {
                in_word = 1;
                tp->total_words++;
            }
            current_word[word_len++] = c;
        } else {
            if (in_word) {
                current_word[word_len] = '\0';
                if (!add_word(tp, current_word, word_len)) {
                    return 0;
                }
                word_len = 0;
                in_word = 0;
                memset(current_word, 0, sizeof(current_word));
            }
        }
    }
    return 1;
}

void print_stats(const struct TextProcessor *tp) {
    printf("Text Statistics:\n");
    printf("Total characters: %d\n", tp->total_chars);
    printf("Total words: %d\n", tp->total_words);
    printf("Unique words: %d\n", tp->word_count);
    printf("\nWord frequency:\n");

    for (int i = 0; i < tp->word_count; i++) {
        printf("'%s': %d occurrences", tp->words[i].word, tp->words[i].count);
        if (tp->words[i].is_capitalized) {
            printf(" (capitalized)");
        }
        printf("\n");
    }
}

int main() {
    struct TextProcessor processor;
    init_text_processor(&processor);

    char input_text[MAX_TEXT_LEN];
    printf("Enter text to analyze (max %d characters):\n", MAX_TEXT_LEN - 1);
    
    if (fgets(input_text, sizeof(input_text), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }

    size_t len = strlen(input_text);
    if (len > 0 && input_text[len - 1] == '\n') {
        input_text[len - 1] = '\0';
    }

    if (strlen(input_text) == 0) {
        fprintf(stderr, "Empty input text\n");
        return 1;
    }

    if (!process_text(&processor, input_text)) {
        fprintf(stderr, "Error processing text\n");
        return 1;
    }

    print_stats(&processor);
    return 0;
}