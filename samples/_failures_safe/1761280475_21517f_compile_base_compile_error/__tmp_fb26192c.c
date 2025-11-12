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
    int total_chars;
    int total_words;
    int unique_words;
};

void init_analyzer(struct TextAnalyzer *analyzer) {
    analyzer->word_count = 0;
    analyzer->total_chars = 0;
    analyzer->total_words = 0;
    analyzer->unique_words = 0;
}

int is_word_char(char c) {
    return isalpha((unsigned char)c) || c == '\'' || c == '-';
}

void to_lower_case(char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

int find_word_index(struct TextAnalyzer *analyzer, const char *word) {
    for (int i = 0; i < analyzer->word_count; i++) {
        if (strcmp(analyzer->words[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

void add_word(struct TextAnalyzer *analyzer, const char *word, int is_capitalized) {
    if (analyzer->word_count >= MAX_WORDS) return;
    
    char normalized[MAX_WORD_LEN];
    strncpy(normalized, word, MAX_WORD_LEN - 1);
    normalized[MAX_WORD_LEN - 1] = '\0';
    to_lower_case(normalized);
    
    int index = find_word_index(analyzer, normalized);
    
    if (index == -1) {
        struct WordStats *ws = &analyzer->words[analyzer->word_count];
        strncpy(ws->word, normalized, MAX_WORD_LEN - 1);
        ws->word[MAX_WORD_LEN - 1] = '\0';
        ws->count = 1;
        ws->length = strlen(normalized);
        ws->is_capitalized = is_capitalized;
        analyzer->word_count++;
        analyzer->unique_words++;
    } else {
        analyzer->words[index].count++;
        if (is_capitalized) {
            analyzer->words[index].is_capitalized = 1;
        }
    }
}

void process_text(struct TextAnalyzer *analyzer, const char *text) {
    if (text == NULL) return;
    
    char buffer[MAX_WORD_LEN];
    int buf_index = 0;
    int in_word = 0;
    
    for (int i = 0; text[i] != '\0'; i++) {
        if (is_word_char(text[i])) {
            if (buf_index < MAX_WORD_LEN - 1) {
                buffer[buf_index++] = text[i];
            }
            in_word = 1;
        } else {
            if (in_word && buf_index > 0) {
                buffer[buf_index] = '\0';
                int is_cap = isupper((unsigned char)buffer[0]) ? 1 : 0;
                add_word(analyzer, buffer, is_cap);
                analyzer->total_words++;
                analyzer->total_chars += buf_index;
                buf_index = 0;
            }
            in_word = 0;
        }
    }
    
    if (in_word && buf_index > 0) {
        buffer[buf_index] = '\0';
        int is_cap = isupper((unsigned char)buffer[0]) ? 1 : 0;
        add_word(analyzer, buffer, is_cap);
        analyzer->total_words++;
        analyzer->total_chars += buf_index;
    }
}

int compare_word_count(const void *a, const void *b) {
    const struct WordStats *wa = (const struct WordStats *)a;
    const struct WordStats *wb = (const struct WordStats *)b;
    return wb->count - wa->count;
}

void print_analysis(struct TextAnalyzer *analyzer) {
    printf("Text Analysis Results:\n");
    printf("Total words: %d\n", analyzer->total_words);
    printf("Unique words: %d\n", analyzer->unique_words);
    printf("Total characters in words: %d\n", analyzer->total_chars);
    printf("\nWord frequency (top 10):\n");
    
    struct WordStats sorted[MAX_WORDS];
    if (analyzer->word_count > 0) {
        memcpy(sorted, analyzer->words, sizeof(struct WordStats) * analyzer->word_count);
        qsort(sorted, analyzer->word_count, sizeof(struct WordStats), compare_word_count);
        
        int limit = analyzer->word_count < 10 ? analyzer->word_count : 10;
        for (int i = 0; i < limit; i++) {
            printf("%d. '%s' - count: %d, length: %d%s\n", 
                   i + 1, sorted[i].word, sorted[i].count, sorted[i].length,
                   sorted[i].is_capitalized ? " (capitalized)" : "");
        }
    }
}

int main(void) {
    struct TextAnalyzer analyzer;
    init_analyzer(&analyzer);
    
    char input_buffer[4096];
    printf("Enter text to analyze (max 409