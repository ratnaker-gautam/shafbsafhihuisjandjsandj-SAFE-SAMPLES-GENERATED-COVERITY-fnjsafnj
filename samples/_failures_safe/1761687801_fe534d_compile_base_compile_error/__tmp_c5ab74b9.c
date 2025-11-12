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
        memset(analyzer->words[i].word, 0, MAX_WORD_LEN);
    }
}

int is_word_char(char c) {
    return isalpha((unsigned char)c) || c == '\'' || c == '-';
}

void normalize_word(char *dest, const char *src, size_t len) {
    if (len >= MAX_WORD_LEN) {
        len = MAX_WORD_LEN - 1;
    }
    
    for (size_t i = 0; i < len; i++) {
        dest[i] = tolower((unsigned char)src[i]);
    }
    dest[len] = '\0';
}

int find_word_index(struct TextAnalyzer *analyzer, const char *word) {
    for (int i = 0; i < analyzer->word_count; i++) {
        if (strcmp(analyzer->words[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

void add_word(struct TextAnalyzer *analyzer, const char *word, int length, int capitalized) {
    if (length <= 0 || length >= MAX_WORD_LEN || analyzer->word_count >= MAX_WORDS) {
        return;
    }
    
    char normalized[MAX_WORD_LEN];
    normalize_word(normalized, word, length);
    
    int index = find_word_index(analyzer, normalized);
    if (index >= 0) {
        analyzer->words[index].count++;
        if (capitalized && !analyzer->words[index].is_capitalized) {
            analyzer->words[index].is_capitalized = 1;
        }
    } else {
        struct WordStats *ws = &analyzer->words[analyzer->word_count];
        strncpy(ws->word, normalized, MAX_WORD_LEN - 1);
        ws->word[MAX_WORD_LEN - 1] = '\0';
        ws->count = 1;
        ws->length = length;
        ws->is_capitalized = capitalized;
        analyzer->word_count++;
    }
    
    analyzer->total_words++;
    
    if (length > analyzer->longest_word_length) {
        analyzer->longest_word_length = length;
    }
    if (length < analyzer->shortest_word_length) {
        analyzer->shortest_word_length = length;
    }
}

void process_text(struct TextAnalyzer *analyzer, const char *text) {
    if (!text) return;
    
    const char *p = text;
    while (*p) {
        while (*p && !is_word_char(*p)) {
            p++;
        }
        
        if (!*p) break;
        
        const char *start = p;
        int capitalized = isupper((unsigned char)*start) ? 1 : 0;
        
        while (*p && is_word_char(*p)) {
            p++;
        }
        
        size_t length = p - start;
        if (length > 0 && length < MAX_WORD_LEN) {
            add_word(analyzer, start, length, capitalized);
        }
    }
}

int compare_word_stats(const void *a, const void *b) {
    const struct WordStats *wa = (const struct WordStats *)a;
    const struct WordStats *wb = (const struct WordStats *)b;
    
    if (wa->count != wb->count) {
        return wb->count - wa->count;
    }
    return strcmp(wa->word, wb->word);
}

void print_analysis(struct TextAnalyzer *analyzer) {
    if (analyzer->total_words == 0) {
        printf("No words found in text.\n");
        return;
    }
    
    qsort(analyzer->words, analyzer->word_count, sizeof(struct WordStats), compare_word_stats);
    
    printf("Text Analysis Results:\n");
    printf("Total words: %d\n", analyzer->total_words);
    printf("Unique words: %d\n", analyzer->word_count);
    printf("Longest word length: %d\n", analyzer->longest_word_length);
    printf("Shortest word length: %d\n", analyzer->shortest_word_length);
    printf("\nTop 10 most frequent words:\n");
    
    int limit = analyzer->word_count < 10 ? analyzer->word_count : 10;
    for (int i = 0; i < limit; i++) {
        struct WordStats *ws = &analyzer->words[i];
        printf("%2d. %-20s (count: %3d, length: %2d%s)\n",