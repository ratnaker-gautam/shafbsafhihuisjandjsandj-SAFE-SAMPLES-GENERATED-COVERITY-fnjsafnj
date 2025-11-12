//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: struct_heavy ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

struct TextStatistics {
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
    struct TextStatistics stats;
    struct WordInfo *word_freq;
    size_t unique_words;
    size_t word_capacity;
};

void init_text_analysis(struct TextAnalysis *analysis) {
    memset(&analysis->stats, 0, sizeof(struct TextStatistics));
    analysis->word_freq = NULL;
    analysis->unique_words = 0;
    analysis->word_capacity = 0;
}

void cleanup_text_analysis(struct TextAnalysis *analysis) {
    if (analysis->word_freq != NULL) {
        free(analysis->word_freq);
        analysis->word_freq = NULL;
    }
    analysis->unique_words = 0;
    analysis->word_capacity = 0;
}

int is_word_char(char c) {
    return isalnum((unsigned char)c) || c == '\'' || c == '-';
}

int add_word_to_analysis(struct TextAnalysis *analysis, const char *word, size_t length) {
    if (length == 0 || length >= 64) return 0;
    
    char normalized[64];
    for (size_t i = 0; i < length; i++) {
        normalized[i] = tolower((unsigned char)word[i]);
    }
    normalized[length] = '\0';
    
    for (size_t i = 0; i < analysis->unique_words; i++) {
        if (strcmp(analysis->word_freq[i].word, normalized) == 0) {
            analysis->word_freq[i].count++;
            return 1;
        }
    }
    
    if (analysis->unique_words >= analysis->word_capacity) {
        size_t new_capacity = analysis->word_capacity == 0 ? 16 : analysis->word_capacity * 2;
        struct WordInfo *new_freq = realloc(analysis->word_freq, new_capacity * sizeof(struct WordInfo));
        if (new_freq == NULL) return 0;
        analysis->word_freq = new_freq;
        analysis->word_capacity = new_capacity;
    }
    
    strncpy(analysis->word_freq[analysis->unique_words].word, normalized, 63);
    analysis->word_freq[analysis->unique_words].word[63] = '\0';
    analysis->word_freq[analysis->unique_words].count = 1;
    analysis->unique_words++;
    return 1;
}

void analyze_text(struct TextAnalysis *analysis, const char *text) {
    if (text == NULL) return;
    
    size_t len = strlen(text);
    if (len == 0) return;
    
    int in_word = 0;
    size_t word_start = 0;
    size_t current_word_length = 0;
    
    for (size_t i = 0; i <= len; i++) {
        char c = text[i];
        analysis->stats.char_count++;
        
        if (c == '\n' || c == '\0') {
            analysis->stats.line_count++;
        }
        
        if (c == '.' || c == '!' || c == '?') {
            analysis->stats.sentence_count++;
        }
        
        if (is_word_char(c)) {
            if (!in_word) {
                in_word = 1;
                word_start = i;
                current_word_length = 0;
            }
            current_word_length++;
        } else {
            if (in_word) {
                if (current_word_length > analysis->stats.longest_word_length) {
                    analysis->stats.longest_word_length = current_word_length;
                }
                if (current_word_length > 0) {
                    add_word_to_analysis(analysis, text + word_start, current_word_length);
                    analysis->stats.word_count++;
                }
                in_word = 0;
                current_word_length = 0;
            }
        }
    }
}

void print_statistics(const struct TextAnalysis *analysis) {
    printf("Text Statistics:\n");
    printf("Characters: %zu\n", analysis->stats.char_count);
    printf("Words: %zu\n", analysis->stats.word_count);
    printf("Lines: %zu\n", analysis->stats.line_count);
    printf("Sentences: %zu\n", analysis->stats.sentence_count);
    printf("Longest word: %zu characters\n", analysis->stats.longest_word_length);
    printf("Unique words: %zu\n", analysis->unique_words);
    
    printf("\nWord frequencies (top 10):\n");
    for (size_t i = 0; i < analysis->unique_words && i < 10; i++) {
        printf("%s: %zu\n", analysis->word_freq[i].word, analysis->word_freq[i].count);
    }
}

int main(void) {
    struct TextAnalysis analysis;
    init_text_analysis(&analysis);
    
    char input_buffer[4096];
    printf("Enter text to analyze (max 4095 characters):\n");
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        printf("Error reading input.\n");
        cleanup_text_analysis(&analysis);
        return 1;
    }
    
    size_t input_len = strlen(input_buffer);
    if (input_len > 0 && input_buffer[input_len - 1] == '\n')