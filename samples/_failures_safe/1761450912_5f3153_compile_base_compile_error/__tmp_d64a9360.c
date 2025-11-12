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
    int unique_words;
    int longest_word_length;
};

void init_analyzer(struct TextAnalyzer *analyzer) {
    analyzer->word_count = 0;
    analyzer->total_words = 0;
    analyzer->unique_words = 0;
    analyzer->longest_word_length = 0;
    for (int i = 0; i < MAX_WORDS; i++) {
        analyzer->words[i].word[0] = '\0';
        analyzer->words[i].count = 0;
        analyzer->words[i].length = 0;
        analyzer->words[i].is_capitalized = 0;
    }
}

int is_word_char(char c) {
    return isalnum((unsigned char)c) || c == '\'' || c == '-';
}

int find_word_index(struct TextAnalyzer *analyzer, const char *word) {
    for (int i = 0; i < analyzer->word_count; i++) {
        if (strcmp(analyzer->words[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

int add_word(struct TextAnalyzer *analyzer, const char *word, int length, int capitalized) {
    if (analyzer->word_count >= MAX_WORDS || length <= 0 || length >= MAX_WORD_LEN) {
        return -1;
    }

    int index = find_word_index(analyzer, word);
    if (index >= 0) {
        analyzer->words[index].count++;
        return index;
    }

    if (analyzer->word_count < MAX_WORDS) {
        strncpy(analyzer->words[analyzer->word_count].word, word, MAX_WORD_LEN - 1);
        analyzer->words[analyzer->word_count].word[MAX_WORD_LEN - 1] = '\0';
        analyzer->words[analyzer->word_count].count = 1;
        analyzer->words[analyzer->word_count].length = length;
        analyzer->words[analyzer->word_count].is_capitalized = capitalized;
        analyzer->word_count++;
        analyzer->unique_words++;
        if (length > analyzer->longest_word_length) {
            analyzer->longest_word_length = length;
        }
        return analyzer->word_count - 1;
    }
    return -1;
}

void process_text(struct TextAnalyzer *analyzer, const char *text) {
    if (text == NULL) return;

    char buffer[MAX_WORD_LEN];
    int buf_index = 0;
    int in_word = 0;
    int word_start_cap = 0;

    for (int i = 0; text[i] != '\0'; i++) {
        if (is_word_char(text[i])) {
            if (!in_word) {
                in_word = 1;
                buf_index = 0;
                word_start_cap = isupper((unsigned char)text[i]) ? 1 : 0;
            }
            if (buf_index < MAX_WORD_LEN - 1) {
                buffer[buf_index++] = tolower((unsigned char)text[i]);
            }
        } else {
            if (in_word && buf_index > 0) {
                buffer[buf_index] = '\0';
                add_word(analyzer, buffer, buf_index, word_start_cap);
                analyzer->total_words++;
                in_word = 0;
            }
        }
    }

    if (in_word && buf_index > 0) {
        buffer[buf_index] = '\0';
        add_word(analyzer, buffer, buf_index, word_start_cap);
        analyzer->total_words++;
    }
}

void print_analysis(struct TextAnalyzer *analyzer) {
    printf("Text Analysis Results:\n");
    printf("Total words: %d\n", analyzer->total_words);
    printf("Unique words: %d\n", analyzer->unique_words);
    printf("Longest word length: %d\n\n", analyzer->longest_word_length);
    
    printf("Word frequencies:\n");
    for (int i = 0; i < analyzer->word_count; i++) {
        printf("%-20s: %d (length: %d, %s)\n", 
               analyzer->words[i].word,
               analyzer->words[i].count,
               analyzer->words[i].length,
               analyzer->words[i].is_capitalized ? "capitalized" : "lowercase");
    }
}

int compare_word_count(const void *a, const void *b) {
    const struct WordStats *wa = (const struct WordStats *)a;
    const struct WordStats *wb = (const struct WordStats *)b;
    return wb->count - wa->count;
}

void sort_by_frequency(struct TextAnalyzer *analyzer) {
    qsort(analyzer->words, analyzer->word_count, sizeof(struct WordStats), compare_word_count);
}

int main(void) {
    struct TextAnalyzer analyzer;
    init_analyzer(&analyzer);

    char input_buffer[4096];
    printf("Enter text to analyze (max 4095 characters):\n");
    
    if (fgets(input_buffer, sizeof(input_buffer