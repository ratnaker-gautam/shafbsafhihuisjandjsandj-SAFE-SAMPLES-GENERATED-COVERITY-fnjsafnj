//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORD_LEN 64
#define MAX_WORDS 1000
#define MAX_TEXT_LEN 10000

struct WordStats {
    char word[MAX_WORD_LEN];
    int count;
    int length;
    double frequency;
};

struct TextAnalyzer {
    struct WordStats words[MAX_WORDS];
    int word_count;
    int total_words;
    char processed_text[MAX_TEXT_LEN];
};

struct AnalysisResult {
    struct TextAnalyzer analyzer;
    int unique_words;
    int longest_word_length;
    char most_frequent_word[MAX_WORD_LEN];
};

void initialize_analyzer(struct TextAnalyzer *analyzer) {
    analyzer->word_count = 0;
    analyzer->total_words = 0;
    memset(analyzer->processed_text, 0, sizeof(analyzer->processed_text));
    for (int i = 0; i < MAX_WORDS; i++) {
        memset(analyzer->words[i].word, 0, sizeof(analyzer->words[i].word));
        analyzer->words[i].count = 0;
        analyzer->words[i].length = 0;
        analyzer->words[i].frequency = 0.0;
    }
}

int is_valid_char(char c) {
    return isalnum((unsigned char)c) || c == ' ' || c == '\n' || c == '\t';
}

int safe_strncpy(char *dest, const char *src, size_t n) {
    if (dest == NULL || src == NULL || n == 0) return 0;
    size_t i;
    for (i = 0; i < n - 1 && src[i] != '\0'; i++) {
        dest[i] = src[i];
    }
    dest[i] = '\0';
    return 1;
}

void preprocess_text(struct TextAnalyzer *analyzer, const char *input) {
    if (input == NULL) return;
    
    size_t input_len = strlen(input);
    if (input_len >= MAX_TEXT_LEN) input_len = MAX_TEXT_LEN - 1;
    
    size_t j = 0;
    for (size_t i = 0; i < input_len && j < MAX_TEXT_LEN - 1; i++) {
        if (is_valid_char(input[i])) {
            analyzer->processed_text[j++] = tolower((unsigned char)input[i]);
        } else if (j > 0 && analyzer->processed_text[j-1] != ' ') {
            analyzer->processed_text[j++] = ' ';
        }
    }
    
    while (j > 0 && analyzer->processed_text[j-1] == ' ') {
        j--;
    }
    analyzer->processed_text[j] = '\0';
}

int find_word_index(struct TextAnalyzer *analyzer, const char *word) {
    for (int i = 0; i < analyzer->word_count; i++) {
        if (strcmp(analyzer->words[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

int add_word(struct TextAnalyzer *analyzer, const char *word) {
    if (analyzer->word_count >= MAX_WORDS) return 0;
    
    int word_len = strlen(word);
    if (word_len == 0 || word_len >= MAX_WORD_LEN) return 0;
    
    int index = find_word_index(analyzer, word);
    if (index >= 0) {
        analyzer->words[index].count++;
        return 1;
    }
    
    if (!safe_strncpy(analyzer->words[analyzer->word_count].word, word, MAX_WORD_LEN)) {
        return 0;
    }
    
    analyzer->words[analyzer->word_count].count = 1;
    analyzer->words[analyzer->word_count].length = word_len;
    analyzer->word_count++;
    return 1;
}

void analyze_words(struct TextAnalyzer *analyzer) {
    char *text = analyzer->processed_text;
    char word[MAX_WORD_LEN];
    int word_pos = 0;
    
    analyzer->total_words = 0;
    
    for (size_t i = 0; text[i] != '\0' && analyzer->total_words < MAX_WORDS; i++) {
        if (text[i] == ' ') {
            if (word_pos > 0) {
                word[word_pos] = '\0';
                if (add_word(analyzer, word)) {
                    analyzer->total_words++;
                }
                word_pos = 0;
            }
        } else {
            if (word_pos < MAX_WORD_LEN - 1) {
                word[word_pos++] = text[i];
            }
        }
    }
    
    if (word_pos > 0) {
        word[word_pos] = '\0';
        if (add_word(analyzer, word)) {
            analyzer->total_words++;
        }
    }
    
    for (int i = 0; i < analyzer->word_count; i++) {
        if (analyzer->total_words > 0) {
            analyzer->words[i].frequency = (double)analyzer->words[i].count / analyzer->total_words;
        }
    }
}

void compute_results(struct TextAnalyzer *analyzer, struct AnalysisResult *result) {
    result->analyzer = *analyzer;
    result->unique_words = analyzer->word_count;
    result->longest_word_length = 0;
    
    int max_count = 0