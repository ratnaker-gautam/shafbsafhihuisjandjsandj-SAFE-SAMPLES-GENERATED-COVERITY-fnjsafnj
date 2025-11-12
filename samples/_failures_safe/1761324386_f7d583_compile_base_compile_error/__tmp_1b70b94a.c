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
    int first_position;
    int last_position;
};

struct TextProcessor {
    struct WordStats words[MAX_WORDS];
    int word_count;
    char processed_text[MAX_TEXT_LEN];
    int total_words;
    int total_chars;
};

struct TextAnalyzer {
    struct TextProcessor processor;
    int unique_words;
    int longest_word_len;
    int shortest_word_len;
};

void init_text_processor(struct TextProcessor *tp) {
    tp->word_count = 0;
    tp->total_words = 0;
    tp->total_chars = 0;
    tp->processed_text[0] = '\0';
    for (int i = 0; i < MAX_WORDS; i++) {
        tp->words[i].word[0] = '\0';
        tp->words[i].count = 0;
        tp->words[i].first_position = -1;
        tp->words[i].last_position = -1;
    }
}

void init_text_analyzer(struct TextAnalyzer *ta) {
    init_text_processor(&ta->processor);
    ta->unique_words = 0;
    ta->longest_word_len = 0;
    ta->shortest_word_len = MAX_WORD_LEN;
}

int is_valid_char(char c) {
    return isalnum((unsigned char)c) || c == ' ' || c == '\t' || c == '\n' || c == '.' || c == ',' || c == '!' || c == '?';
}

int sanitize_input(char *input, char *output, int max_len) {
    if (input == NULL || output == NULL || max_len <= 0) return 0;
    
    int j = 0;
    for (int i = 0; input[i] != '\0' && j < max_len - 1; i++) {
        if (is_valid_char(input[i])) {
            output[j++] = input[i];
        }
    }
    output[j] = '\0';
    return 1;
}

int extract_word(const char *text, int pos, char *word) {
    if (text == NULL || word == NULL || pos < 0) return 0;
    
    int i = pos;
    while (text[i] != '\0' && !isalnum((unsigned char)text[i])) {
        i++;
    }
    
    int start = i;
    int j = 0;
    while (text[i] != '\0' && isalnum((unsigned char)text[i]) && j < MAX_WORD_LEN - 1) {
        word[j++] = tolower((unsigned char)text[i]);
        i++;
    }
    word[j] = '\0';
    
    return (j > 0) ? i : -1;
}

int find_word_index(struct TextProcessor *tp, const char *word) {
    if (tp == NULL || word == NULL) return -1;
    
    for (int i = 0; i < tp->word_count; i++) {
        if (strcmp(tp->words[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

int process_text(struct TextProcessor *tp, const char *input) {
    if (tp == NULL || input == NULL) return 0;
    
    char sanitized[MAX_TEXT_LEN];
    if (!sanitize_input(input, sanitized, MAX_TEXT_LEN)) return 0;
    
    strncpy(tp->processed_text, sanitized, MAX_TEXT_LEN - 1);
    tp->processed_text[MAX_TEXT_LEN - 1] = '\0';
    
    int pos = 0;
    char word[MAX_WORD_LEN];
    
    while (pos >= 0 && pos < MAX_TEXT_LEN && tp->word_count < MAX_WORDS) {
        int new_pos = extract_word(tp->processed_text, pos, word);
        if (new_pos == -1) break;
        
        if (strlen(word) > 0) {
            tp->total_words++;
            tp->total_chars += (int)strlen(word);
            
            int idx = find_word_index(tp, word);
            if (idx == -1) {
                idx = tp->word_count;
                strncpy(tp->words[idx].word, word, MAX_WORD_LEN - 1);
                tp->words[idx].word[MAX_WORD_LEN - 1] = '\0';
                tp->words[idx].count = 1;
                tp->words[idx].first_position = pos;
                tp->words[idx].last_position = pos;
                tp->word_count++;
            } else {
                tp->words[idx].count++;
                tp->words[idx].last_position = pos;
            }
        }
        pos = new_pos;
    }
    
    return 1;
}

void analyze_text(struct TextAnalyzer *ta, const char *input) {
    if (ta == NULL || input == NULL) return;
    
    init_text_analyzer(ta);
    if (!process_text(&ta->processor, input)) return;
    
    ta->unique_words = ta->processor.word_count;
    
    for (int i = 0; i < ta->processor.word_count; i++) {
        int len = (int)strlen(ta->processor.words[i