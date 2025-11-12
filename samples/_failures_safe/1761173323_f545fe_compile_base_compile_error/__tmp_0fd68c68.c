//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORD_LEN 64
#define MAX_WORDS 1000
#define MAX_LINE_LEN 256

struct WordStats {
    char word[MAX_WORD_LEN];
    int count;
    int length;
    int is_capitalized;
};

struct TextProcessor {
    struct WordStats words[MAX_WORDS];
    int word_count;
    int total_characters;
    int total_words;
    int unique_words;
};

struct TextAnalyzer {
    struct TextProcessor processor;
    char most_frequent_word[MAX_WORD_LEN];
    int max_frequency;
    double average_word_length;
};

void initialize_processor(struct TextProcessor *proc) {
    proc->word_count = 0;
    proc->total_characters = 0;
    proc->total_words = 0;
    proc->unique_words = 0;
}

void initialize_analyzer(struct TextAnalyzer *analyzer) {
    initialize_processor(&analyzer->processor);
    memset(analyzer->most_frequent_word, 0, MAX_WORD_LEN);
    analyzer->max_frequency = 0;
    analyzer->average_word_length = 0.0;
}

int is_valid_char(char c) {
    return isalpha(c) || c == ' ' || c == '\n' || c == '\t' || c == '\0';
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

int find_word_index(struct TextProcessor *proc, const char *word) {
    for (int i = 0; i < proc->word_count; i++) {
        if (strcmp(proc->words[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

int process_word(struct TextProcessor *proc, const char *word) {
    if (word == NULL || strlen(word) == 0 || strlen(word) >= MAX_WORD_LEN) {
        return 0;
    }
    
    char clean_word[MAX_WORD_LEN];
    size_t clean_len = 0;
    
    for (size_t i = 0; word[i] != '\0' && clean_len < MAX_WORD_LEN - 1; i++) {
        if (isalpha(word[i])) {
            clean_word[clean_len++] = tolower(word[i]);
        }
    }
    clean_word[clean_len] = '\0';
    
    if (clean_len == 0) return 0;
    
    int index = find_word_index(proc, clean_word);
    
    if (index == -1) {
        if (proc->word_count >= MAX_WORDS) return 0;
        
        index = proc->word_count;
        if (!safe_strncpy(proc->words[index].word, clean_word, MAX_WORD_LEN)) {
            return 0;
        }
        
        proc->words[index].count = 1;
        proc->words[index].length = (int)clean_len;
        proc->words[index].is_capitalized = isupper(word[0]) ? 1 : 0;
        proc->word_count++;
        proc->unique_words++;
    } else {
        proc->words[index].count++;
    }
    
    proc->total_words++;
    proc->total_characters += (int)clean_len;
    
    return 1;
}

int process_text(struct TextProcessor *proc, const char *text) {
    if (text == NULL) return 0;
    
    char buffer[MAX_LINE_LEN];
    size_t text_len = strlen(text);
    
    if (text_len >= MAX_LINE_LEN * 10) return 0;
    
    size_t buf_pos = 0;
    
    for (size_t i = 0; i <= text_len; i++) {
        if (!is_valid_char(text[i])) return 0;
        
        if (text[i] == ' ' || text[i] == '\n' || text[i] == '\t' || text[i] == '\0') {
            if (buf_pos > 0) {
                buffer[buf_pos] = '\0';
                if (!process_word(proc, buffer)) return 0;
                buf_pos = 0;
            }
        } else {
            if (buf_pos < MAX_LINE_LEN - 1) {
                buffer[buf_pos++] = text[i];
            } else {
                return 0;
            }
        }
    }
    
    return 1;
}

void analyze_text(struct TextAnalyzer *analyzer) {
    if (analyzer->processor.total_words == 0) return;
    
    analyzer->average_word_length = (double)analyzer->processor.total_characters / 
                                   (double)analyzer->processor.total_words;
    
    analyzer->max_frequency = 0;
    
    for (int i = 0; i < analyzer->processor.word_count; i++) {
        if (analyzer->processor.words[i].count > analyzer->max_frequency) {
            analyzer->max_frequency = analyzer->processor.words[i].count;
            safe_strncpy(analyzer->most_frequent_word, 
                        analyzer->processor.words[i].word, MAX_WORD_LEN);
        }
    }