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
    size_t longest_word;
};

struct WordInfo {
    char word[64];
    size_t count;
};

struct TextAnalysis {
    struct TextStats stats;
    struct WordInfo *word_freq;
    size_t word_freq_size;
    size_t word_freq_capacity;
};

void init_text_analysis(struct TextAnalysis *analysis) {
    memset(&analysis->stats, 0, sizeof(struct TextStats));
    analysis->word_freq = NULL;
    analysis->word_freq_size = 0;
    analysis->word_freq_capacity = 0;
}

void cleanup_text_analysis(struct TextAnalysis *analysis) {
    if (analysis->word_freq != NULL) {
        free(analysis->word_freq);
    }
}

int is_word_char(char c) {
    return isalnum((unsigned char)c) || c == '\'' || c == '-';
}

void add_word_to_freq(struct TextAnalysis *analysis, const char *word) {
    if (word[0] == '\0') return;
    
    for (size_t i = 0; i < analysis->word_freq_size; i++) {
        if (strcmp(analysis->word_freq[i].word, word) == 0) {
            analysis->word_freq[i].count++;
            return;
        }
    }
    
    if (analysis->word_freq_size >= analysis->word_freq_capacity) {
        size_t new_capacity = analysis->word_freq_capacity == 0 ? 16 : analysis->word_freq_capacity * 2;
        struct WordInfo *new_freq = realloc(analysis->word_freq, new_capacity * sizeof(struct WordInfo));
        if (new_freq == NULL) return;
        analysis->word_freq = new_freq;
        analysis->word_freq_capacity = new_capacity;
    }
    
    strncpy(analysis->word_freq[analysis->word_freq_size].word, word, 63);
    analysis->word_freq[analysis->word_freq_size].word[63] = '\0';
    analysis->word_freq[analysis->word_freq_size].count = 1;
    analysis->word_freq_size++;
}

void analyze_text(struct TextAnalysis *analysis, const char *text) {
    if (text == NULL) return;
    
    size_t len = strlen(text);
    if (len == 0) return;
    
    char word_buf[64];
    size_t word_len = 0;
    int in_word = 0;
    
    for (size_t i = 0; i < len; i++) {
        char c = text[i];
        analysis->stats.char_count++;
        
        if (c == '\n') {
            analysis->stats.line_count++;
        }
        
        if (c == '.' || c == '!' || c == '?') {
            analysis->stats.sentence_count++;
        }
        
        if (is_word_char(c)) {
            if (!in_word) {
                in_word = 1;
                analysis->stats.word_count++;
                word_len = 0;
            }
            if (word_len < 63) {
                word_buf[word_len++] = tolower((unsigned char)c);
            }
        } else {
            if (in_word) {
                in_word = 0;
                word_buf[word_len] = '\0';
                if (word_len > analysis->stats.longest_word) {
                    analysis->stats.longest_word = word_len;
                }
                add_word_to_freq(analysis, word_buf);
            }
        }
    }
    
    if (in_word) {
        word_buf[word_len] = '\0';
        if (word_len > analysis->stats.longest_word) {
            analysis->stats.longest_word = word_len;
        }
        add_word_to_freq(analysis, word_buf);
    }
    
    if (analysis->stats.line_count == 0 && len > 0) {
        analysis->stats.line_count = 1;
    }
}

void print_analysis(const struct TextAnalysis *analysis) {
    printf("Text Statistics:\n");
    printf("Characters: %zu\n", analysis->stats.char_count);
    printf("Words: %zu\n", analysis->stats.word_count);
    printf("Lines: %zu\n", analysis->stats.line_count);
    printf("Sentences: %zu\n", analysis->stats.sentence_count);
    printf("Longest word: %zu characters\n", analysis->stats.longest_word);
    
    printf("\nTop 5 most frequent words:\n");
    for (size_t i = 0; i < analysis->word_freq_size && i < 5; i++) {
        printf("%s: %zu\n", analysis->word_freq[i].word, analysis->word_freq[i].count);
    }
}

int main(void) {
    char input_buffer[4096];
    struct TextAnalysis analysis;
    
    init_text_analysis(&analysis);
    
    printf("Enter text to analyze (max 4095 characters):\n");
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        printf("Error reading input.\n");
        cleanup_text_analysis(&analysis);
        return 1;
    }
    
    size_t input_len = strlen(input_buffer);
    if (input_len > 0 && input_buffer[input_len - 1]