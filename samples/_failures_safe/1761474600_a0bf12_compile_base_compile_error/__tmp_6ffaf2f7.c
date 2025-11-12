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
    struct TextAnalyzer analyzer;
    int unique_words;
    char most_frequent_word[MAX_WORD_LEN];
    int max_count;
    double avg_word_length;
};

void init_text_analyzer(struct TextAnalyzer *ta) {
    ta->word_count = 0;
    ta->total_words = 0;
    ta->text_length = 0;
    ta->text[0] = '\0';
    for (int i = 0; i < MAX_WORDS; i++) {
        ta->words[i].word[0] = '\0';
        ta->words[i].count = 0;
        ta->words[i].length = 0;
        ta->words[i].frequency = 0.0;
    }
}

int is_valid_char(char c) {
    return isalnum((unsigned char)c) || c == ' ' || c == '\t' || c == '\n' || c == '.' || c == ',' || c == '!' || c == '?';
}

int sanitize_text(struct TextAnalyzer *ta, const char *input) {
    if (input == NULL) return 0;
    
    size_t input_len = strlen(input);
    if (input_len >= MAX_TEXT_LEN) return 0;
    
    int j = 0;
    for (size_t i = 0; i < input_len; i++) {
        if (is_valid_char(input[i])) {
            ta->text[j++] = input[i];
        }
    }
    ta->text[j] = '\0';
    ta->text_length = j;
    return 1;
}

int find_word_index(struct TextAnalyzer *ta, const char *word) {
    for (int i = 0; i < ta->word_count; i++) {
        if (strcmp(ta->words[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

int add_word(struct TextAnalyzer *ta, const char *word) {
    if (ta->word_count >= MAX_WORDS) return 0;
    
    int word_len = strlen(word);
    if (word_len >= MAX_WORD_LEN || word_len == 0) return 0;
    
    int index = find_word_index(ta, word);
    if (index >= 0) {
        ta->words[index].count++;
        return 1;
    }
    
    strncpy(ta->words[ta->word_count].word, word, MAX_WORD_LEN - 1);
    ta->words[ta->word_count].word[MAX_WORD_LEN - 1] = '\0';
    ta->words[ta->word_count].count = 1;
    ta->words[ta->word_count].length = word_len;
    ta->word_count++;
    return 1;
}

int extract_words(struct TextAnalyzer *ta) {
    char buffer[MAX_WORD_LEN];
    int buf_index = 0;
    int in_word = 0;
    
    for (int i = 0; i < ta->text_length; i++) {
        char c = ta->text[i];
        
        if (isalnum((unsigned char)c)) {
            if (buf_index < MAX_WORD_LEN - 1) {
                buffer[buf_index++] = tolower((unsigned char)c);
            }
            in_word = 1;
        } else {
            if (in_word) {
                buffer[buf_index] = '\0';
                if (buf_index > 0) {
                    if (!add_word(ta, buffer)) return 0;
                    ta->total_words++;
                }
                buf_index = 0;
                in_word = 0;
            }
        }
    }
    
    if (in_word && buf_index > 0) {
        buffer[buf_index] = '\0';
        if (!add_word(ta, buffer)) return 0;
        ta->total_words++;
    }
    
    return 1;
}

void calculate_frequencies(struct TextAnalyzer *ta) {
    for (int i = 0; i < ta->word_count; i++) {
        if (ta->total_words > 0) {
            ta->words[i].frequency = (double)ta->words[i].count / ta->total_words;
        }
    }
}

void analyze_text(struct AnalysisResult *result, struct TextAnalyzer *ta) {
    result->analyzer = *ta;
    result->unique_words = ta->word_count;
    result->max_count = 0;
    result->avg_word_length = 0.0;
    
    strcpy(result->most_frequent_word, "");
    
    int total_length = 0;
    for (int i = 0; i < ta->word_count; i++) {
        total_length += ta->words[i].length * ta->words[i].count;
        
        if (ta->words[i].count > result->max_count) {
            result->max_count = ta->words[i].count;
            strncpy(result->most_frequent_word, ta->words[i].word