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

struct TextProcessor {
    struct WordStats words[MAX_WORDS];
    int word_count;
    int total_words;
    char processed_text[MAX_TEXT_LEN];
    int text_length;
};

struct AnalysisResult {
    struct WordStats most_frequent;
    struct WordStats longest_word;
    int unique_words;
    double avg_word_length;
};

void init_text_processor(struct TextProcessor *tp) {
    tp->word_count = 0;
    tp->total_words = 0;
    tp->text_length = 0;
    tp->processed_text[0] = '\0';
    for (int i = 0; i < MAX_WORDS; i++) {
        tp->words[i].word[0] = '\0';
        tp->words[i].count = 0;
        tp->words[i].length = 0;
        tp->words[i].frequency = 0.0;
    }
}

int is_word_char(char c) {
    return isalnum((unsigned char)c) || c == '\'' || c == '-';
}

int process_text(struct TextProcessor *tp, const char *input) {
    if (input == NULL || strlen(input) >= MAX_TEXT_LEN) {
        return 0;
    }
    
    strncpy(tp->processed_text, input, MAX_TEXT_LEN - 1);
    tp->processed_text[MAX_TEXT_LEN - 1] = '\0';
    tp->text_length = strlen(tp->processed_text);
    
    char temp[MAX_TEXT_LEN];
    strncpy(temp, tp->processed_text, MAX_TEXT_LEN - 1);
    temp[MAX_TEXT_LEN - 1] = '\0';
    
    char *token = strtok(temp, " \t\n\r.,;:!?\"()[]{}");
    while (token != NULL && tp->word_count < MAX_WORDS) {
        int len = strlen(token);
        if (len > 0 && len < MAX_WORD_LEN) {
            int found = 0;
            for (int i = 0; i < tp->word_count; i++) {
                if (strcasecmp(tp->words[i].word, token) == 0) {
                    tp->words[i].count++;
                    found = 1;
                    break;
                }
            }
            
            if (!found) {
                strncpy(tp->words[tp->word_count].word, token, MAX_WORD_LEN - 1);
                tp->words[tp->word_count].word[MAX_WORD_LEN - 1] = '\0';
                tp->words[tp->word_count].count = 1;
                tp->words[tp->word_count].length = len;
                tp->word_count++;
            }
            tp->total_words++;
        }
        token = strtok(NULL, " \t\n\r.,;:!?\"()[]{}");
    }
    
    for (int i = 0; i < tp->word_count; i++) {
        if (tp->total_words > 0) {
            tp->words[i].frequency = (double)tp->words[i].count / tp->total_words;
        }
    }
    
    return 1;
}

void analyze_text(struct TextProcessor *tp, struct AnalysisResult *result) {
    if (tp->word_count == 0) {
        result->unique_words = 0;
        result->avg_word_length = 0.0;
        result->most_frequent.count = 0;
        result->longest_word.length = 0;
        return;
    }
    
    result->unique_words = tp->word_count;
    
    double total_length = 0.0;
    for (int i = 0; i < tp->word_count; i++) {
        total_length += tp->words[i].length * tp->words[i].count;
    }
    result->avg_word_length = total_length / tp->total_words;
    
    int max_freq_index = 0;
    int max_length_index = 0;
    
    for (int i = 1; i < tp->word_count; i++) {
        if (tp->words[i].count > tp->words[max_freq_index].count) {
            max_freq_index = i;
        }
        if (tp->words[i].length > tp->words[max_length_index].length) {
            max_length_index = i;
        }
    }
    
    result->most_frequent = tp->words[max_freq_index];
    result->longest_word = tp->words[max_length_index];
}

void print_analysis(struct AnalysisResult *result) {
    printf("Text Analysis Results:\n");
    printf("Unique words: %d\n", result->unique_words);
    printf("Average word length: %.2f\n", result->avg_word_length);
    
    if (result->most_frequent.count > 0) {
        printf("Most frequent word: '%s' (appears %d times, %.2f%%)\n",
               result->most_frequent.word, result->most_frequent.count,
               result->most_frequent.frequency * 100);
    }
    
    if (result->longest_word.length > 0) {
        printf("Longest word: '%s' (%d characters)\n",
               result->longest_word.word,