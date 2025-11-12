//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: struct_heavy ; Variation: string_processing
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
    int first_position;
    int last_position;
};

struct TextProcessor {
    struct WordStats words[MAX_WORDS];
    int word_count;
    char processed_text[MAX_TEXT_LEN];
    int text_length;
    int total_words;
};

struct AnalysisResult {
    int unique_words;
    int total_occurrences;
    char most_frequent_word[MAX_WORD_LEN];
    int max_frequency;
    double avg_word_length;
};

void init_text_processor(struct TextProcessor *tp) {
    tp->word_count = 0;
    tp->text_length = 0;
    tp->total_words = 0;
    memset(tp->processed_text, 0, sizeof(tp->processed_text));
    for (int i = 0; i < MAX_WORDS; i++) {
        memset(tp->words[i].word, 0, sizeof(tp->words[i].word));
        tp->words[i].count = 0;
        tp->words[i].first_position = -1;
        tp->words[i].last_position = -1;
    }
}

int is_word_char(char c) {
    return isalnum((unsigned char)c) || c == '\'' || c == '-';
}

int find_word_index(struct TextProcessor *tp, const char *word) {
    for (int i = 0; i < tp->word_count; i++) {
        if (strcmp(tp->words[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

int add_word(struct TextProcessor *tp, const char *word, int position) {
    if (tp->word_count >= MAX_WORDS || strlen(word) >= MAX_WORD_LEN) {
        return 0;
    }
    
    int index = find_word_index(tp, word);
    if (index == -1) {
        index = tp->word_count;
        strncpy(tp->words[index].word, word, MAX_WORD_LEN - 1);
        tp->words[index].word[MAX_WORD_LEN - 1] = '\0';
        tp->words[index].count = 1;
        tp->words[index].first_position = position;
        tp->words[index].last_position = position;
        tp->word_count++;
    } else {
        tp->words[index].count++;
        tp->words[index].last_position = position;
    }
    return 1;
}

void process_text(struct TextProcessor *tp, const char *input) {
    if (input == NULL || strlen(input) >= MAX_TEXT_LEN) {
        return;
    }
    
    strncpy(tp->processed_text, input, MAX_TEXT_LEN - 1);
    tp->processed_text[MAX_TEXT_LEN - 1] = '\0';
    tp->text_length = strlen(tp->processed_text);
    
    int pos = 0;
    int word_start = -1;
    char current_word[MAX_WORD_LEN];
    
    for (int i = 0; i <= tp->text_length; i++) {
        if (i < tp->text_length && is_word_char(tp->processed_text[i])) {
            if (word_start == -1) {
                word_start = i;
            }
        } else {
            if (word_start != -1) {
                int word_len = i - word_start;
                if (word_len < MAX_WORD_LEN) {
                    strncpy(current_word, &tp->processed_text[word_start], word_len);
                    current_word[word_len] = '\0';
                    
                    for (int j = 0; j < word_len; j++) {
                        current_word[j] = tolower((unsigned char)current_word[j]);
                    }
                    
                    if (add_word(tp, current_word, pos)) {
                        pos++;
                        tp->total_words++;
                    }
                }
                word_start = -1;
            }
        }
    }
}

void analyze_text(struct TextProcessor *tp, struct AnalysisResult *result) {
    if (tp->word_count == 0) {
        result->unique_words = 0;
        result->total_occurrences = 0;
        strcpy(result->most_frequent_word, "");
        result->max_frequency = 0;
        result->avg_word_length = 0.0;
        return;
    }
    
    result->unique_words = tp->word_count;
    result->total_occurrences = tp->total_words;
    
    result->max_frequency = 0;
    strcpy(result->most_frequent_word, "");
    
    double total_length = 0.0;
    
    for (int i = 0; i < tp->word_count; i++) {
        if (tp->words[i].count > result->max_frequency) {
            result->max_frequency = tp->words[i].count;
            strncpy(result->most_frequent_word, tp->words[i].word, MAX_WORD_LEN - 1);
            result->most_frequent_word[MAX_WORD_LEN - 1] = '\0';
        }
        total_length += strlen(tp->words[i].word) * tp->words[i].count;
    }
    
    result->avg_word_length = total_length / tp->total_words;
}

void print_analysis(struct AnalysisResult *result) {
    printf("Text Analysis Results