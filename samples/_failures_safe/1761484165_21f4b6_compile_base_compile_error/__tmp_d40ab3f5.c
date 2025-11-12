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
    char text[MAX_TEXT_LEN];
    int text_length;
};

struct AnalysisResult {
    struct WordStats most_frequent;
    struct WordStats longest_word;
    int unique_words;
    double avg_word_length;
};

void init_text_analyzer(struct TextAnalyzer *analyzer) {
    analyzer->word_count = 0;
    analyzer->total_words = 0;
    analyzer->text_length = 0;
    memset(analyzer->text, 0, MAX_TEXT_LEN);
    for (int i = 0; i < MAX_WORDS; i++) {
        memset(analyzer->words[i].word, 0, MAX_WORD_LEN);
        analyzer->words[i].count = 0;
        analyzer->words[i].length = 0;
        analyzer->words[i].frequency = 0.0;
    }
}

int is_word_char(char c) {
    return isalnum((unsigned char)c) || c == '\'' || c == '-';
}

int safe_strncpy(char *dest, const char *src, size_t n) {
    if (n == 0) return 0;
    size_t i;
    for (i = 0; i < n - 1 && src[i] != '\0'; i++) {
        dest[i] = src[i];
    }
    dest[i] = '\0';
    return i;
}

int find_word_index(struct TextAnalyzer *analyzer, const char *word) {
    for (int i = 0; i < analyzer->word_count; i++) {
        if (strcmp(analyzer->words[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

int add_word(struct TextAnalyzer *analyzer, const char *word, int length) {
    if (length <= 0 || length >= MAX_WORD_LEN || analyzer->word_count >= MAX_WORDS) {
        return -1;
    }
    
    int index = find_word_index(analyzer, word);
    if (index >= 0) {
        analyzer->words[index].count++;
        return index;
    }
    
    if (safe_strncpy(analyzer->words[analyzer->word_count].word, word, MAX_WORD_LEN) == 0) {
        return -1;
    }
    analyzer->words[analyzer->word_count].count = 1;
    analyzer->words[analyzer->word_count].length = length;
    analyzer->word_count++;
    return analyzer->word_count - 1;
}

int process_text(struct TextAnalyzer *analyzer, const char *input) {
    if (input == NULL) return 0;
    
    size_t input_len = strlen(input);
    if (input_len >= MAX_TEXT_LEN - 1) return 0;
    
    safe_strncpy(analyzer->text, input, MAX_TEXT_LEN);
    analyzer->text_length = (int)input_len;
    
    char buffer[MAX_WORD_LEN];
    int buf_pos = 0;
    int in_word = 0;
    
    for (int i = 0; i <= analyzer->text_length; i++) {
        char c = analyzer->text[i];
        
        if (is_word_char(c) && buf_pos < MAX_WORD_LEN - 1) {
            buffer[buf_pos++] = tolower((unsigned char)c);
            in_word = 1;
        } else if (in_word) {
            buffer[buf_pos] = '\0';
            if (buf_pos > 0) {
                add_word(analyzer, buffer, buf_pos);
                analyzer->total_words++;
            }
            buf_pos = 0;
            in_word = 0;
        }
    }
    
    for (int i = 0; i < analyzer->word_count; i++) {
        if (analyzer->total_words > 0) {
            analyzer->words[i].frequency = (double)analyzer->words[i].count / analyzer->total_words;
        }
    }
    
    return analyzer->word_count;
}

void analyze_results(struct TextAnalyzer *analyzer, struct AnalysisResult *result) {
    if (analyzer->word_count == 0) {
        memset(result, 0, sizeof(struct AnalysisResult));
        return;
    }
    
    result->most_frequent = analyzer->words[0];
    result->longest_word = analyzer->words[0];
    result->unique_words = analyzer->word_count;
    
    int total_length = 0;
    
    for (int i = 0; i < analyzer->word_count; i++) {
        total_length += analyzer->words[i].length * analyzer->words[i].count;
        
        if (analyzer->words[i].count > result->most_frequent.count) {
            result->most_frequent = analyzer->words[i];
        }
        
        if (analyzer->words[i].length > result->longest_word.length) {
            result->longest_word = analyzer->words[i];
        }
    }
    
    if (analyzer->total_words > 0) {
        result->avg_word_length = (double)total_length / analyzer->total