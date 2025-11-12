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

void init_text_analyzer(struct TextAnalyzer *analyzer) {
    memset(analyzer, 0, sizeof(*analyzer));
    analyzer->processor.word_count = 0;
    analyzer->processor.total_words = 0;
    analyzer->processor.unique_words = 0;
    analyzer->processor.longest_word_length = 0;
    analyzer->line_count = 0;
}

int is_valid_char(char c) {
    return isalnum((unsigned char)c) || c == '\'' || c == '-';
}

int find_word_index(struct TextProcessor *proc, const char *word) {
    for (int i = 0; i < proc->word_count; i++) {
        if (strcmp(proc->words[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

int add_word(struct TextProcessor *proc, const char *word, int length) {
    if (proc->word_count >= MAX_WORDS || length >= MAX_WORD_LEN) {
        return -1;
    }
    
    int index = find_word_index(proc, word);
    if (index >= 0) {
        proc->words[index].count++;
        return index;
    }
    
    if (proc->word_count < MAX_WORDS) {
        struct WordStats *ws = &proc->words[proc->word_count];
        strncpy(ws->word, word, MAX_WORD_LEN - 1);
        ws->word[MAX_WORD_LEN - 1] = '\0';
        ws->count = 1;
        ws->length = length;
        ws->is_capitalized = isupper((unsigned char)word[0]);
        proc->word_count++;
        proc->unique_words++;
        if (length > proc->longest_word_length) {
            proc->longest_word_length = length;
        }
        return proc->word_count - 1;
    }
    return -1;
}

void process_line(struct TextAnalyzer *analyzer, const char *line) {
    if (!line || strlen(line) >= MAX_LINE_LEN) {
        return;
    }
    
    char buffer[MAX_LINE_LEN];
    strncpy(buffer, line, MAX_LINE_LEN - 1);
    buffer[MAX_LINE_LEN - 1] = '\0';
    
    char *ptr = buffer;
    while (*ptr) {
        while (*ptr && !is_valid_char(*ptr)) {
            ptr++;
        }
        
        if (!*ptr) break;
        
        char *word_start = ptr;
        while (*ptr && is_valid_char(*ptr)) {
            ptr++;
        }
        
        int word_len = ptr - word_start;
        if (word_len > 0 && word_len < MAX_WORD_LEN) {
            char word[MAX_WORD_LEN];
            strncpy(word, word_start, word_len);
            word[word_len] = '\0';
            
            if (add_word(&analyzer->processor, word, word_len) >= 0) {
                analyzer->processor.total_words++;
            }
        }
        
        if (!*ptr) break;
        ptr++;
    }
}

void print_statistics(struct TextAnalyzer *analyzer) {
    printf("Text Analysis Results:\n");
    printf("Total words: %d\n", analyzer->processor.total_words);
    printf("Unique words: %d\n", analyzer->processor.unique_words);
    printf("Longest word length: %d\n", analyzer->processor.longest_word_length);
    printf("Lines processed: %d\n", analyzer->line_count);
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
    struct TextAnalyzer analyzer;
    init_text_analyzer(&analyzer);
    
    printf("Enter text lines (empty line to finish):\n");
    
    while (fgets(analyzer.input_buffer, MAX_LINE_LEN, stdin)) {
        size_t len = strlen(analyzer.input_buffer);
        if (len > 0 && analyzer.input_buffer[len - 1] == '\n') {
            analyzer