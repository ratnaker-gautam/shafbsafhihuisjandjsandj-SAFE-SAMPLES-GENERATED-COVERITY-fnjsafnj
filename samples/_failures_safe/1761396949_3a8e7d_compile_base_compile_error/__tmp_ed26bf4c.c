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
    if (analyzer == NULL) return;
    analyzer->word_count = 0;
    analyzer->total_words = 0;
    analyzer->longest_word_length = 0;
    analyzer->shortest_word_length = MAX_WORD_LEN;
    for (int i = 0; i < MAX_WORDS; i++) {
        analyzer->words[i].count = 0;
        analyzer->words[i].length = 0;
        analyzer->words[i].is_capitalized = 0;
        memset(analyzer->words[i].word, 0, MAX_WORD_LEN);
    }
}

int is_word_char(char c) {
    return isalpha((unsigned char)c) || c == '\'' || c == '-';
}

void normalize_word(char *dest, const char *src, size_t max_len) {
    if (dest == NULL || src == NULL || max_len == 0) return;
    
    size_t i = 0;
    while (src[i] != '\0' && i < max_len - 1) {
        dest[i] = tolower((unsigned char)src[i]);
        i++;
    }
    dest[i] = '\0';
}

int find_word_index(struct TextAnalyzer *analyzer, const char *word) {
    if (analyzer == NULL || word == NULL) return -1;
    
    for (int i = 0; i < analyzer->word_count; i++) {
        if (strcmp(analyzer->words[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

void add_word(struct TextAnalyzer *analyzer, const char *original_word) {
    if (analyzer == NULL || original_word == NULL || analyzer->word_count >= MAX_WORDS) return;
    
    char normalized[MAX_WORD_LEN];
    normalize_word(normalized, original_word, MAX_WORD_LEN);
    
    if (strlen(normalized) == 0) return;
    
    int index = find_word_index(analyzer, normalized);
    
    if (index == -1) {
        index = analyzer->word_count;
        strncpy(analyzer->words[index].word, normalized, MAX_WORD_LEN - 1);
        analyzer->words[index].word[MAX_WORD_LEN - 1] = '\0';
        analyzer->words[index].length = strlen(normalized);
        analyzer->words[index].is_capitalized = isupper((unsigned char)original_word[0]) ? 1 : 0;
        analyzer->word_count++;
    }
    
    analyzer->words[index].count++;
    analyzer->total_words++;
    
    int len = analyzer->words[index].length;
    if (len > analyzer->longest_word_length) {
        analyzer->longest_word_length = len;
    }
    if (len < analyzer->shortest_word_length) {
        analyzer->shortest_word_length = len;
    }
}

void process_text(struct TextAnalyzer *analyzer, const char *text) {
    if (analyzer == NULL || text == NULL) return;
    
    char buffer[MAX_WORD_LEN];
    size_t buf_index = 0;
    size_t text_len = strlen(text);
    
    for (size_t i = 0; i <= text_len; i++) {
        if (i < text_len && is_word_char(text[i])) {
            if (buf_index < MAX_WORD_LEN - 1) {
                buffer[buf_index++] = text[i];
            }
        } else {
            if (buf_index > 0) {
                buffer[buf_index] = '\0';
                add_word(analyzer, buffer);
                buf_index = 0;
            }
        }
    }
}

int compare_word_count(const void *a, const void *b) {
    const struct WordStats *wa = (const struct WordStats *)a;
    const struct WordStats *wb = (const struct WordStats *)b;
    if (wb->count > wa->count) return 1;
    if (wb->count < wa->count) return -1;
    return 0;
}

void print_analysis(struct TextAnalyzer *analyzer) {
    if (analyzer == NULL) return;
    
    printf("Text Analysis Results:\n");
    printf("Total unique words: %d\n", analyzer->word_count);
    printf("Total word occurrences: %d\n", analyzer->total_words);
    printf("Longest word length: %d\n", analyzer->longest_word_length);
    printf("Shortest word length: %d\n", analyzer->shortest_word_length);
    printf("\n");
    
    qsort(analyzer->words, analyzer->word_count, sizeof(struct WordStats), compare_word_count);
    
    printf("Top 10 most frequent words:\n");
    printf("%-20s %-8s %-8s %s\n", "Word", "Count", "Length", "Capitalized");
    printf("------------------------------------------------------------\n");
    
    int limit = analyzer->word_count < 10 ? analyzer->