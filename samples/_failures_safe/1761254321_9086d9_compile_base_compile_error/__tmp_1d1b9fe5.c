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
    return isalnum((unsigned char)c) || c == '\'' || c == '-';
}

void add_word_to_freq(struct TextAnalysis *analysis, const char *word) {
    if (word == NULL || strlen(word) == 0) return;
    
    for (size_t i = 0; i < analysis->unique_words; i++) {
        if (strcmp(analysis->word_freq[i].word, word) == 0) {
            analysis->word_freq[i].count++;
            return;
        }
    }
    
    if (analysis->unique_words >= analysis->word_freq_capacity) {
        size_t new_capacity = analysis->word_freq_capacity == 0 ? 16 : analysis->word_freq_capacity * 2;
        struct WordInfo *new_freq = realloc(analysis->word_freq, new_capacity * sizeof(struct WordInfo));
        if (new_freq == NULL) return;
        analysis->word_freq = new_freq;
        analysis->word_freq_capacity = new_capacity;
    }
    
    strncpy(analysis->word_freq[analysis->unique_words].word, word, 63);
    analysis->word_freq[analysis->unique_words].word[63] = '\0';
    analysis->word_freq[analysis->unique_words].count = 1;
    analysis->unique_words++;
}

void analyze_text(struct TextAnalysis *analysis, const char *text) {
    if (text == NULL) return;
    
    size_t len = strlen(text);
    if (len == 0) return;
    
    init_text_analysis(analysis);
    
    int in_word = 0;
    int in_sentence = 0;
    char current_word[64] = {0};
    size_t word_pos = 0;
    
    for (size_t i = 0; i < len; i++) {
        char c = text[i];
        analysis->stats.char_count++;
        
        if (is_word_char(c)) {
            if (!in_word) {
                in_word = 1;
                analysis->stats.word_count++;
                word_pos = 0;
            }
            if (word_pos < 63) {
                current_word[word_pos++] = tolower((unsigned char)c);
            }
        } else {
            if (in_word) {
                current_word[word_pos] = '\0';
                add_word_to_freq(analysis, current_word);
                if (word_pos > analysis->stats.longest_word_length) {
                    analysis->stats.longest_word_length = word_pos;
                }
                in_word = 0;
                word_pos = 0;
            }
            
            if (c == '.' || c == '!' || c == '?') {
                if (in_sentence) {
                    analysis->stats.sentence_count++;
                    in_sentence = 0;
                }
            } else if (isalpha((unsigned char)c)) {
                in_sentence = 1;
            }
        }
        
        if (c == '\n') {
            analysis->stats.line_count++;
        }
    }
    
    if (in_word) {
        current_word[word_pos] = '\0';
        add_word_to_freq(analysis, current_word);
        if (word_pos > analysis->stats.longest_word_length) {
            analysis->stats.longest_word_length = word_pos;
        }
    }
    
    if (in_sentence) {
        analysis->stats.sentence_count++;
    }
    
    if (analysis->stats.line_count == 0 && len > 0) {
        analysis->stats.line_count = 1;
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
    
    printf("\nTop 10 most frequent words