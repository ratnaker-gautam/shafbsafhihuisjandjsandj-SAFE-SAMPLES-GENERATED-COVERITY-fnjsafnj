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
    int total_words;
    int unique_words;
    int longest_word_length;
};

struct TextAnalyzer {
    struct TextProcessor processor;
    char input_buffer[MAX_LINE_LEN];
    int line_count;
};

void initialize_processor(struct TextProcessor *proc) {
    proc->word_count = 0;
    proc->total_words = 0;
    proc->unique_words = 0;
    proc->longest_word_length = 0;
    for (int i = 0; i < MAX_WORDS; i++) {
        proc->words[i].word[0] = '\0';
        proc->words[i].count = 0;
        proc->words[i].length = 0;
        proc->words[i].is_capitalized = 0;
    }
}

void initialize_analyzer(struct TextAnalyzer *analyzer) {
    initialize_processor(&analyzer->processor);
    analyzer->input_buffer[0] = '\0';
    analyzer->line_count = 0;
}

int is_word_character(char c) {
    return isalpha((unsigned char)c) || c == '\'' || c == '-';
}

int find_word_index(struct TextProcessor *proc, const char *word) {
    for (int i = 0; i < proc->word_count; i++) {
        if (strcmp(proc->words[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

int add_word(struct TextProcessor *proc, const char *word) {
    if (proc->word_count >= MAX_WORDS) {
        return 0;
    }
    
    int len = strlen(word);
    if (len >= MAX_WORD_LEN || len == 0) {
        return 0;
    }
    
    int index = find_word_index(proc, word);
    if (index >= 0) {
        proc->words[index].count++;
        return 1;
    }
    
    strncpy(proc->words[proc->word_count].word, word, MAX_WORD_LEN - 1);
    proc->words[proc->word_count].word[MAX_WORD_LEN - 1] = '\0';
    proc->words[proc->word_count].count = 1;
    proc->words[proc->word_count].length = len;
    proc->words[proc->word_count].is_capitalized = isupper((unsigned char)word[0]) ? 1 : 0;
    
    if (len > proc->longest_word_length) {
        proc->longest_word_length = len;
    }
    
    proc->word_count++;
    proc->unique_words++;
    return 1;
}

void process_line(struct TextAnalyzer *analyzer, const char *line) {
    char buffer[MAX_LINE_LEN];
    strncpy(buffer, line, MAX_LINE_LEN - 1);
    buffer[MAX_LINE_LEN - 1] = '\0';
    
    char *ptr = buffer;
    char word[MAX_WORD_LEN];
    int word_pos = 0;
    
    while (*ptr) {
        if (is_word_character(*ptr)) {
            if (word_pos < MAX_WORD_LEN - 1) {
                word[word_pos++] = *ptr;
            }
        } else {
            if (word_pos > 0) {
                word[word_pos] = '\0';
                add_word(&analyzer->processor, word);
                analyzer->processor.total_words++;
                word_pos = 0;
            }
        }
        ptr++;
    }
    
    if (word_pos > 0) {
        word[word_pos] = '\0';
        add_word(&analyzer->processor, word);
        analyzer->processor.total_words++;
    }
    
    analyzer->line_count++;
}

void print_statistics(struct TextAnalyzer *analyzer) {
    printf("Text Analysis Results:\n");
    printf("Total lines: %d\n", analyzer->line_count);
    printf("Total words: %d\n", analyzer->processor.total_words);
    printf("Unique words: %d\n", analyzer->processor.unique_words);
    printf("Longest word length: %d\n", analyzer->processor.longest_word_length);
    printf("\nWord frequency (top 10):\n");
    
    for (int i = 0; i < (analyzer->processor.word_count < 10 ? analyzer->processor.word_count : 10); i++) {
        int max_index = i;
        for (int j = i + 1; j < analyzer->processor.word_count; j++) {
            if (analyzer->processor.words[j].count > analyzer->processor.words[max_index].count) {
                max_index = j;
            }
        }
        
        if (max_index != i) {
            struct WordStats temp = analyzer->processor.words[i];
            analyzer->processor.words[i] = analyzer->processor.words[max_index];
            analyzer->processor.words[max_index] = temp;
        }
        
        printf("%-20s: %d\n", analyzer->processor.words[i].word, analyzer->processor.words[i].count);
    }
}

int main(void) {
    struct