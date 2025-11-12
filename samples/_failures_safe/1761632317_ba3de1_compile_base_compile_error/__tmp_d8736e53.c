//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORD_LEN 64
#define MAX_WORDS 1000

struct WordStats {
    char word[MAX_WORD_LEN];
    int count;
    int length;
    int is_capitalized;
};

struct TextAnalyzer {
    struct WordStats words[MAX_WORDS];
    int word_count;
    int total_words;
    int longest_word_length;
    int shortest_word_length;
};

void init_analyzer(struct TextAnalyzer *analyzer) {
    analyzer->word_count = 0;
    analyzer->total_words = 0;
    analyzer->longest_word_length = 0;
    analyzer->shortest_word_length = MAX_WORD_LEN;
    for (int i = 0; i < MAX_WORDS; i++) {
        analyzer->words[i].count = 0;
        analyzer->words[i].length = 0;
        analyzer->words[i].is_capitalized = 0;
        analyzer->words[i].word[0] = '\0';
    }
}

int is_word_char(char c) {
    return isalpha((unsigned char)c) || c == '\'' || c == '-';
}

void add_word(struct TextAnalyzer *analyzer, const char *word, int length) {
    if (length <= 0 || length >= MAX_WORD_LEN || analyzer->word_count >= MAX_WORDS) {
        return;
    }

    for (int i = 0; i < analyzer->word_count; i++) {
        if (strncmp(analyzer->words[i].word, word, length) == 0 && 
            analyzer->words[i].word[length] == '\0') {
            analyzer->words[i].count++;
            return;
        }
    }

    if (analyzer->word_count < MAX_WORDS) {
        struct WordStats *ws = &analyzer->words[analyzer->word_count];
        strncpy(ws->word, word, length);
        ws->word[length] = '\0';
        ws->count = 1;
        ws->length = length;
        ws->is_capitalized = isupper((unsigned char)word[0]) ? 1 : 0;
        analyzer->word_count++;
    }
}

void process_text(struct TextAnalyzer *analyzer, const char *text) {
    if (text == NULL) return;

    int length = strlen(text);
    if (length <= 0) return;

    char buffer[MAX_WORD_LEN];
    int buf_pos = 0;
    int in_word = 0;

    for (int i = 0; i < length && buf_pos < MAX_WORD_LEN - 1; i++) {
        if (is_word_char(text[i])) {
            buffer[buf_pos++] = text[i];
            in_word = 1;
        } else {
            if (in_word && buf_pos > 0) {
                buffer[buf_pos] = '\0';
                add_word(analyzer, buffer, buf_pos);
                analyzer->total_words++;
                buf_pos = 0;
                in_word = 0;
            }
        }
    }

    if (in_word && buf_pos > 0) {
        buffer[buf_pos] = '\0';
        add_word(analyzer, buffer, buf_pos);
        analyzer->total_words++;
    }
}

void calculate_stats(struct TextAnalyzer *analyzer) {
    analyzer->longest_word_length = 0;
    analyzer->shortest_word_length = MAX_WORD_LEN;

    for (int i = 0; i < analyzer->word_count; i++) {
        int len = analyzer->words[i].length;
        if (len > analyzer->longest_word_length) {
            analyzer->longest_word_length = len;
        }
        if (len < analyzer->shortest_word_length && len > 0) {
            analyzer->shortest_word_length = len;
        }
    }

    if (analyzer->shortest_word_length == MAX_WORD_LEN) {
        analyzer->shortest_word_length = 0;
    }
}

void print_analysis(const struct TextAnalyzer *analyzer) {
    printf("Text Analysis Results:\n");
    printf("Total words: %d\n", analyzer->total_words);
    printf("Unique words: %d\n", analyzer->word_count);
    printf("Longest word length: %d\n", analyzer->longest_word_length);
    printf("Shortest word length: %d\n", analyzer->shortest_word_length);
    printf("\nWord frequency:\n");

    for (int i = 0; i < analyzer->word_count; i++) {
        if (analyzer->words[i].count > 0) {
            printf("'%s': %d occurrence(s)\n", 
                   analyzer->words[i].word, 
                   analyzer->words[i].count);
        }
    }
}

int main(void) {
    struct TextAnalyzer analyzer;
    char input_text[4096];

    init_analyzer(&analyzer);

    printf("Enter text to analyze (max 4095 characters):\n");
    
    if (fgets(input_text, sizeof(input_text), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }

    size_t len = strlen(input_text);
    if (len > 0 && input_text[len - 1] == '\n') {
        input_text[len - 1] = '\0';
    }

    if (strlen(input_text) == 0) {
        printf("No text provided for analysis