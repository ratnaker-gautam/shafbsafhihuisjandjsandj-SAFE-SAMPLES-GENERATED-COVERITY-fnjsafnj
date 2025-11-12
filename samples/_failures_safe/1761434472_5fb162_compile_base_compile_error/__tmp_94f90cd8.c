//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

struct TextStats {
    size_t char_count;
    size_t word_count;
    size_t line_count;
    size_t sentence_count;
    size_t longest_word_length;
};

struct WordInfo {
    char word[64];
    size_t count;
};

struct TextAnalysis {
    struct TextStats stats;
    struct WordInfo *word_freq;
    size_t unique_words;
    size_t word_freq_capacity;
};

void init_text_analysis(struct TextAnalysis *analysis) {
    memset(&analysis->stats, 0, sizeof(struct TextStats));
    analysis->word_freq = NULL;
    analysis->unique_words = 0;
    analysis->word_freq_capacity = 0;
}

void cleanup_text_analysis(struct TextAnalysis *analysis) {
    if (analysis->word_freq != NULL) {
        free(analysis->word_freq);
        analysis->word_freq = NULL;
    }
}

int is_word_char(char c) {
    return isalnum((unsigned char)c) || c == '_' || c == '-';
}

int add_word_to_freq(struct TextAnalysis *analysis, const char *word, size_t len) {
    if (len == 0 || len >= 64) return 0;
    
    for (size_t i = 0; i < analysis->unique_words; i++) {
        if (strncmp(analysis->word_freq[i].word, word, len) == 0 && 
            analysis->word_freq[i].word[len] == '\0') {
            analysis->word_freq[i].count++;
            return 1;
        }
    }
    
    if (analysis->unique_words >= analysis->word_freq_capacity) {
        size_t new_capacity = analysis->word_freq_capacity == 0 ? 16 : analysis->word_freq_capacity * 2;
        struct WordInfo *new_freq = realloc(analysis->word_freq, new_capacity * sizeof(struct WordInfo));
        if (new_freq == NULL) return 0;
        analysis->word_freq = new_freq;
        analysis->word_freq_capacity = new_capacity;
    }
    
    strncpy(analysis->word_freq[analysis->unique_words].word, word, len);
    analysis->word_freq[analysis->unique_words].word[len] = '\0';
    analysis->word_freq[analysis->unique_words].count = 1;
    analysis->unique_words++;
    return 1;
}

void analyze_text(struct TextAnalysis *analysis, const char *text) {
    if (text == NULL) return;
    
    size_t len = strlen(text);
    if (len == 0) return;
    
    int in_word = 0;
    int in_sentence = 0;
    size_t word_start = 0;
    size_t current_word_len = 0;
    
    for (size_t i = 0; i < len; i++) {
        analysis->stats.char_count++;
        
        if (text[i] == '\n') {
            analysis->stats.line_count++;
        }
        
        if (is_word_char(text[i])) {
            if (!in_word) {
                in_word = 1;
                word_start = i;
                current_word_len = 0;
            }
            current_word_len++;
        } else {
            if (in_word) {
                analysis->stats.word_count++;
                if (current_word_len > analysis->stats.longest_word_length) {
                    analysis->stats.longest_word_length = current_word_len;
                }
                add_word_to_freq(analysis, &text[word_start], current_word_len);
                in_word = 0;
            }
        }
        
        if (text[i] == '.' || text[i] == '!' || text[i] == '?') {
            if (in_sentence) {
                analysis->stats.sentence_count++;
                in_sentence = 0;
            }
        } else if (is_word_char(text[i])) {
            in_sentence = 1;
        }
    }
    
    if (in_word) {
        analysis->stats.word_count++;
        if (current_word_len > analysis->stats.longest_word_length) {
            analysis->stats.longest_word_length = current_word_len;
        }
        add_word_to_freq(analysis, &text[word_start], current_word_len);
    }
    
    if (in_sentence) {
        analysis->stats.sentence_count++;
    }
}

int compare_word_freq(const void *a, const void *b) {
    const struct WordInfo *wa = (const struct WordInfo *)a;
    const struct WordInfo *wb = (const struct WordInfo *)b;
    if (wa->count > wb->count) return -1;
    if (wa->count < wb->count) return 1;
    return strcmp(wa->word, wb->word);
}

void print_analysis(const struct TextAnalysis *analysis) {
    printf("Text Statistics:\n");
    printf("Characters: %zu\n", analysis->stats.char_count);
    printf("Words: %zu\n", analysis->stats.word_count);
    printf("Lines: %zu\n", analysis->stats.line_count);
    printf("Sentences: %zu\n", analysis->stats.sentence_count);
    printf("Longest word: %zu characters\n", analysis->stats.longest_word_length);
    
    printf("\nTop 10 most frequent words:\n");
    size_t limit = analysis->unique_words > 10 ? 10 : analysis->unique_words;
    for (size