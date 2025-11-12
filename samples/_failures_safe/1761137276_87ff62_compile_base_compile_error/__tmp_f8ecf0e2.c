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
    int total_chars;
    int total_words;
    int unique_words;
};

struct TextAnalyzer {
    struct TextProcessor processor;
    char input_buffer[MAX_LINE_LEN];
    int line_count;
};

void init_text_analyzer(struct TextAnalyzer *analyzer) {
    analyzer->processor.word_count = 0;
    analyzer->processor.total_chars = 0;
    analyzer->processor.total_words = 0;
    analyzer->processor.unique_words = 0;
    analyzer->line_count = 0;
    memset(analyzer->input_buffer, 0, sizeof(analyzer->input_buffer));
    for (int i = 0; i < MAX_WORDS; i++) {
        memset(analyzer->processor.words[i].word, 0, MAX_WORD_LEN);
        analyzer->processor.words[i].count = 0;
        analyzer->processor.words[i].length = 0;
        analyzer->processor.words[i].is_capitalized = 0;
    }
}

int is_valid_char(char c) {
    return isalpha(c) || c == ' ' || c == '\n' || c == '\t';
}

int find_word_index(struct TextProcessor *proc, const char *word) {
    for (int i = 0; i < proc->word_count; i++) {
        if (strcmp(proc->words[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

void add_word(struct TextProcessor *proc, const char *word, int len) {
    if (len <= 0 || len >= MAX_WORD_LEN || proc->word_count >= MAX_WORDS) {
        return;
    }
    
    int index = find_word_index(proc, word);
    if (index >= 0) {
        proc->words[index].count++;
        return;
    }
    
    strncpy(proc->words[proc->word_count].word, word, MAX_WORD_LEN - 1);
    proc->words[proc->word_count].word[MAX_WORD_LEN - 1] = '\0';
    proc->words[proc->word_count].count = 1;
    proc->words[proc->word_count].length = len;
    proc->words[proc->word_count].is_capitalized = isupper(word[0]) ? 1 : 0;
    proc->word_count++;
    proc->unique_words++;
}

void process_line(struct TextAnalyzer *analyzer, const char *line) {
    if (line == NULL || strlen(line) >= MAX_LINE_LEN) {
        return;
    }
    
    char buffer[MAX_LINE_LEN];
    strncpy(buffer, line, MAX_LINE_LEN - 1);
    buffer[MAX_LINE_LEN - 1] = '\0';
    
    char *token = strtok(buffer, " \t\n");
    while (token != NULL) {
        int len = strlen(token);
        if (len > 0 && len < MAX_WORD_LEN) {
            char clean_word[MAX_WORD_LEN];
            int clean_len = 0;
            
            for (int i = 0; i < len && clean_len < MAX_WORD_LEN - 1; i++) {
                if (isalpha(token[i])) {
                    clean_word[clean_len++] = token[i];
                }
            }
            clean_word[clean_len] = '\0';
            
            if (clean_len > 0) {
                add_word(&analyzer->processor, clean_word, clean_len);
                analyzer->processor.total_words++;
                analyzer->processor.total_chars += clean_len;
            }
        }
        token = strtok(NULL, " \t\n");
    }
    analyzer->line_count++;
}

void print_analysis(const struct TextAnalyzer *analyzer) {
    printf("Text Analysis Results:\n");
    printf("Total lines processed: %d\n", analyzer->line_count);
    printf("Total words: %d\n", analyzer->processor.total_words);
    printf("Unique words: %d\n", analyzer->processor.unique_words);
    printf("Total characters in words: %d\n", analyzer->processor.total_chars);
    printf("\nWord frequency (top 10):\n");
    
    struct WordStats sorted[MAX_WORDS];
    int count = analyzer->processor.word_count;
    if (count > MAX_WORDS) count = MAX_WORDS;
    
    for (int i = 0; i < count; i++) {
        sorted[i] = analyzer->processor.words[i];
    }
    
    for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j++) {
            if (sorted[j].count > sorted[i].count) {
                struct WordStats temp = sorted[i];
                sorted[i] = sorted[j];
                sorted[j] = temp;
            }
        }
    }
    
    int display_count = count < 10 ? count : 10;
    for (int i = 0; i < display_count; i++) {
        printf("%-15s: %d occurrences\n",