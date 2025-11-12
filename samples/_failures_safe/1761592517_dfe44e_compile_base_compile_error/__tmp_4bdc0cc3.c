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
    double frequency;
};

struct TextAnalyzer {
    struct WordStats words[MAX_WORDS];
    int word_count;
    int total_words;
    char filename[MAX_LINE_LEN];
};

struct TextProcessor {
    struct TextAnalyzer analyzer;
    int case_sensitive;
    int min_word_length;
};

void initialize_processor(struct TextProcessor *proc, const char *filename) {
    if (proc == NULL || filename == NULL) return;
    
    strncpy(proc->analyzer.filename, filename, MAX_LINE_LEN - 1);
    proc->analyzer.filename[MAX_LINE_LEN - 1] = '\0';
    proc->analyzer.word_count = 0;
    proc->analyzer.total_words = 0;
    proc->case_sensitive = 0;
    proc->min_word_length = 1;
    
    for (int i = 0; i < MAX_WORDS; i++) {
        proc->analyzer.words[i].word[0] = '\0';
        proc->analyzer.words[i].count = 0;
        proc->analyzer.words[i].length = 0;
        proc->analyzer.words[i].frequency = 0.0;
    }
}

int is_valid_char(char c) {
    return isalnum((unsigned char)c) || c == '\'' || c == '-';
}

void normalize_word(char *dest, const char *src, int case_sensitive) {
    if (dest == NULL || src == NULL) return;
    
    int i = 0;
    while (src[i] != '\0' && i < MAX_WORD_LEN - 1) {
        if (case_sensitive) {
            dest[i] = src[i];
        } else {
            dest[i] = tolower((unsigned char)src[i]);
        }
        i++;
    }
    dest[i] = '\0';
}

int find_word_index(struct TextAnalyzer *analyzer, const char *word) {
    if (analyzer == NULL || word == NULL) return -1;
    
    for (int i = 0; i < analyzer->word_count; i++) {
        if (strcmp(analyzer->words[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

void add_word(struct TextProcessor *proc, const char *raw_word) {
    if (proc == NULL || raw_word == NULL) return;
    
    char normalized[MAX_WORD_LEN];
    normalize_word(normalized, raw_word, proc->case_sensitive);
    
    int len = strlen(normalized);
    if (len < proc->min_word_length || len >= MAX_WORD_LEN) return;
    
    int index = find_word_index(&proc->analyzer, normalized);
    
    if (index >= 0) {
        proc->analyzer.words[index].count++;
    } else if (proc->analyzer.word_count < MAX_WORDS) {
        strncpy(proc->analyzer.words[proc->analyzer.word_count].word, normalized, MAX_WORD_LEN - 1);
        proc->analyzer.words[proc->analyzer.word_count].word[MAX_WORD_LEN - 1] = '\0';
        proc->analyzer.words[proc->analyzer.word_count].count = 1;
        proc->analyzer.words[proc->analyzer.word_count].length = len;
        proc->analyzer.word_count++;
    }
    proc->analyzer.total_words++;
}

void process_text(struct TextProcessor *proc, const char *text) {
    if (proc == NULL || text == NULL) return;
    
    char buffer[MAX_LINE_LEN];
    char word[MAX_WORD_LEN];
    int text_len = strlen(text);
    
    if (text_len >= MAX_LINE_LEN) return;
    
    strncpy(buffer, text, MAX_LINE_LEN - 1);
    buffer[MAX_LINE_LEN - 1] = '\0';
    
    int i = 0;
    int word_start = -1;
    
    while (i <= text_len) {
        if (i < text_len && is_valid_char(buffer[i])) {
            if (word_start == -1) {
                word_start = i;
            }
        } else {
            if (word_start != -1) {
                int word_len = i - word_start;
                if (word_len > 0 && word_len < MAX_WORD_LEN) {
                    strncpy(word, &buffer[word_start], word_len);
                    word[word_len] = '\0';
                    add_word(proc, word);
                }
                word_start = -1;
            }
        }
        i++;
    }
}

void calculate_frequencies(struct TextProcessor *proc) {
    if (proc == NULL || proc->analyzer.total_words == 0) return;
    
    for (int i = 0; i < proc->analyzer.word_count; i++) {
        proc->analyzer.words[i].frequency = (double)proc->analyzer.words[i].count / proc->analyzer.total_words * 100.0;
    }
}

int compare_word_count(const void *a, const void *b) {
    const struct WordStats *wa = (const struct WordStats *)