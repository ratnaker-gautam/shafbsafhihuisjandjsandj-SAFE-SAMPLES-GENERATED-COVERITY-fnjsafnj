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
    int longest_word_length;
    int shortest_word_length;
};

struct TextAnalyzer {
    struct TextProcessor processor;
    char input_buffer[MAX_LINE_LEN];
    int line_count;
};

void initialize_processor(struct TextProcessor *proc) {
    proc->word_count = 0;
    proc->total_words = 0;
    proc->longest_word_length = 0;
    proc->shortest_word_length = MAX_WORD_LEN;
}

void initialize_analyzer(struct TextAnalyzer *analyzer) {
    initialize_processor(&analyzer->processor);
    analyzer->line_count = 0;
    memset(analyzer->input_buffer, 0, sizeof(analyzer->input_buffer));
}

int is_valid_char(char c) {
    return isalpha(c) || c == ' ' || c == '\t' || c == '\n' || c == '\0';
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

void add_word(struct TextProcessor *proc, const char *word) {
    if (proc->word_count >= MAX_WORDS) return;
    
    int index = find_word_index(proc, word);
    if (index >= 0) {
        proc->words[index].count++;
        return;
    }
    
    struct WordStats *ws = &proc->words[proc->word_count];
    if (safe_strncpy(ws->word, word, MAX_WORD_LEN)) {
        ws->count = 1;
        ws->length = strlen(word);
        ws->is_capitalized = isupper(word[0]) ? 1 : 0;
        
        if (ws->length > proc->longest_word_length) {
            proc->longest_word_length = ws->length;
        }
        if (ws->length < proc->shortest_word_length) {
            proc->shortest_word_length = ws->length;
        }
        
        proc->word_count++;
    }
    proc->total_words++;
}

void process_line(struct TextAnalyzer *analyzer, const char *line) {
    char buffer[MAX_LINE_LEN];
    if (!safe_strncpy(buffer, line, sizeof(buffer))) return;
    
    char *token = strtok(buffer, " \t\n");
    while (token != NULL) {
        char clean_word[MAX_WORD_LEN];
        int j = 0;
        
        for (int i = 0; token[i] != '\0' && j < MAX_WORD_LEN - 1; i++) {
            if (isalpha(token[i])) {
                clean_word[j++] = tolower(token[i]);
            }
        }
        clean_word[j] = '\0';
        
        if (j > 0) {
            add_word(&analyzer->processor, clean_word);
        }
        
        token = strtok(NULL, " \t\n");
    }
}

void display_results(struct TextAnalyzer *analyzer) {
    printf("Text Analysis Results:\n");
    printf("Total unique words: %d\n", analyzer->processor.word_count);
    printf("Total words: %d\n", analyzer->processor.total_words);
    printf("Longest word length: %d\n", analyzer->processor.longest_word_length);
    printf("Shortest word length: %d\n", analyzer->processor.shortest_word_length);
    printf("Lines processed: %d\n", analyzer->line_count);
    printf("\nWord frequencies:\n");
    
    for (int i = 0; i < analyzer->processor.word_count; i++) {
        printf("%s: %d\n", analyzer->processor.words[i].word, 
               analyzer->processor.words[i].count);
    }
}

int main(void) {
    struct TextAnalyzer analyzer;
    initialize_analyzer(&analyzer);
    
    printf("Enter text lines (empty line to finish):\n");
    
    while (fgets(analyzer.input_buffer, sizeof(analyzer.input_buffer), stdin) != NULL) {
        for (size_t i = 0; i < sizeof(analyzer.input_buffer) && analyzer.input_buffer[i] != '\0'; i++) {
            if (!is_valid_char(analyzer.input_buffer[i])) {
                fprintf(stderr, "Invalid input character detected\n");
                return 1;
            }
        }
        
        if (analyzer.input_buffer[0] == '\n') break;
        
        process_line(&analyzer, analyzer.input_buffer);
        analyzer.line_count++;
        
        if (analyzer.line_count > 100)