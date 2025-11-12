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
    char input_buffer[MAX_LINE_LEN];
    int line_count;
};

void init_text_processor(struct TextProcessor *tp) {
    tp->word_count = 0;
    tp->total_characters = 0;
    tp->total_words = 0;
    tp->unique_words = 0;
}

void init_text_analyzer(struct TextAnalyzer *ta) {
    init_text_processor(&ta->processor);
    ta->line_count = 0;
    memset(ta->input_buffer, 0, sizeof(ta->input_buffer));
}

int is_valid_char(char c) {
    return isalpha((unsigned char)c) || c == ' ' || c == '\n' || c == '\t';
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

int find_word_index(struct TextProcessor *tp, const char *word) {
    for (int i = 0; i < tp->word_count; i++) {
        if (strcmp(tp->words[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

int add_word(struct TextProcessor *tp, const char *word) {
    if (tp->word_count >= MAX_WORDS) return 0;
    
    int index = find_word_index(tp, word);
    if (index >= 0) {
        tp->words[index].count++;
        return 1;
    }
    
    if (!safe_strncpy(tp->words[tp->word_count].word, word, MAX_WORD_LEN)) {
        return 0;
    }
    
    tp->words[tp->word_count].count = 1;
    tp->words[tp->word_count].length = strlen(word);
    tp->words[tp->word_count].is_capitalized = isupper((unsigned char)word[0]);
    
    tp->word_count++;
    tp->unique_words++;
    return 1;
}

void process_text(struct TextAnalyzer *ta, const char *text) {
    if (text == NULL) return;
    
    char buffer[MAX_LINE_LEN];
    if (!safe_strncpy(buffer, text, sizeof(buffer))) return;
    
    char *token = strtok(buffer, " \t\n");
    while (token != NULL && ta->processor.total_words < MAX_WORDS) {
        int valid = 1;
        for (int i = 0; token[i] != '\0'; i++) {
            if (!isalpha((unsigned char)token[i])) {
                valid = 0;
                break;
            }
        }
        
        if (valid && strlen(token) < MAX_WORD_LEN) {
            add_word(&ta->processor, token);
            ta->processor.total_words++;
            ta->processor.total_characters += strlen(token);
        }
        
        token = strtok(NULL, " \t\n");
    }
}

void display_statistics(struct TextAnalyzer *ta) {
    printf("Text Analysis Results:\n");
    printf("Total words: %d\n", ta->processor.total_words);
    printf("Total characters: %d\n", ta->processor.total_characters);
    printf("Unique words: %d\n", ta->processor.unique_words);
    printf("\nWord frequency:\n");
    
    for (int i = 0; i < ta->processor.word_count; i++) {
        printf("%s: %d (length: %d, %s)\n", 
               ta->processor.words[i].word,
               ta->processor.words[i].count,
               ta->processor.words[i].length,
               ta->processor.words[i].is_capitalized ? "capitalized" : "lowercase");
    }
}

int main(void) {
    struct TextAnalyzer analyzer;
    init_text_analyzer(&analyzer);
    
    printf("Enter text (max %d characters per line, empty line to finish):\n", MAX_LINE_LEN - 1);
    
    while (fgets(analyzer.input_buffer, sizeof(analyzer.input_buffer), stdin) != NULL) {
        if (analyzer.input_buffer[0] == '\n') break;
        
        int valid_line = 1;
        for (int i = 0; analyzer.input_buffer[i] != '\0' && analyzer.input_buffer[i] != '\n'; i++) {
            if (!is_valid_char(analyzer.input_buffer[i])) {
                valid_line = 0;
                break;
            }
        }
        
        if (valid_line) {
            process_text(&analyzer, analyzer.input_buffer);
            analyzer.line_count++;
        } else {
            printf("Invalid characters detected. Skipping line.\n");
        }