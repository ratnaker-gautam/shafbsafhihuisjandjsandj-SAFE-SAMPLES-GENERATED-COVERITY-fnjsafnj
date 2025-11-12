//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORDS 100
#define MAX_WORD_LEN 50
#define MAX_TEXT_LEN 1000

struct WordStats {
    char word[MAX_WORD_LEN];
    int count;
    int length;
    int is_capitalized;
};

struct TextAnalyzer {
    struct WordStats words[MAX_WORDS];
    int word_count;
    int total_chars;
    int total_words;
    char processed_text[MAX_TEXT_LEN];
};

void init_analyzer(struct TextAnalyzer *analyzer) {
    analyzer->word_count = 0;
    analyzer->total_chars = 0;
    analyzer->total_words = 0;
    analyzer->processed_text[0] = '\0';
    for (int i = 0; i < MAX_WORDS; i++) {
        analyzer->words[i].word[0] = '\0';
        analyzer->words[i].count = 0;
        analyzer->words[i].length = 0;
        analyzer->words[i].is_capitalized = 0;
    }
}

int is_word_char(char c) {
    return isalpha((unsigned char)c) || c == '\'' || c == '-';
}

int add_word(struct TextAnalyzer *analyzer, const char *word, int len) {
    if (analyzer->word_count >= MAX_WORDS || len >= MAX_WORD_LEN) {
        return 0;
    }
    
    for (int i = 0; i < analyzer->word_count; i++) {
        if (strncmp(analyzer->words[i].word, word, len) == 0 && 
            analyzer->words[i].word[len] == '\0') {
            analyzer->words[i].count++;
            return 1;
        }
    }
    
    if (len < MAX_WORD_LEN) {
        strncpy(analyzer->words[analyzer->word_count].word, word, len);
        analyzer->words[analyzer->word_count].word[len] = '\0';
        analyzer->words[analyzer->word_count].count = 1;
        analyzer->words[analyzer->word_count].length = len;
        analyzer->words[analyzer->word_count].is_capitalized = isupper((unsigned char)word[0]);
        analyzer->word_count++;
        return 1;
    }
    
    return 0;
}

void process_text(struct TextAnalyzer *analyzer, const char *text) {
    if (text == NULL) {
        return;
    }
    
    size_t text_len = strlen(text);
    if (text_len >= MAX_TEXT_LEN) {
        return;
    }
    
    strncpy(analyzer->processed_text, text, MAX_TEXT_LEN - 1);
    analyzer->processed_text[MAX_TEXT_LEN - 1] = '\0';
    
    char buffer[MAX_WORD_LEN];
    int buf_pos = 0;
    int in_word = 0;
    
    for (size_t i = 0; i < text_len; i++) {
        char c = text[i];
        analyzer->total_chars++;
        
        if (is_word_char(c)) {
            if (buf_pos < MAX_WORD_LEN - 1) {
                buffer[buf_pos++] = c;
            }
            in_word = 1;
        } else {
            if (in_word && buf_pos > 0) {
                buffer[buf_pos] = '\0';
                add_word(analyzer, buffer, buf_pos);
                analyzer->total_words++;
                buf_pos = 0;
            }
            in_word = 0;
        }
    }
    
    if (in_word && buf_pos > 0) {
        buffer[buf_pos] = '\0';
        add_word(analyzer, buffer, buf_pos);
        analyzer->total_words++;
    }
}

void print_analysis(const struct TextAnalyzer *analyzer) {
    printf("Text Analysis Results:\n");
    printf("Total characters: %d\n", analyzer->total_chars);
    printf("Total words: %d\n", analyzer->total_words);
    printf("Unique words: %d\n", analyzer->word_count);
    printf("\nWord frequency:\n");
    
    for (int i = 0; i < analyzer->word_count; i++) {
        printf("'%s': %d occurrences (length: %d, %s)\n",
               analyzer->words[i].word,
               analyzer->words[i].count,
               analyzer->words[i].length,
               analyzer->words[i].is_capitalized ? "capitalized" : "lowercase");
    }
}

int main() {
    struct TextAnalyzer analyzer;
    char input_text[MAX_TEXT_LEN];
    
    init_analyzer(&analyzer);
    
    printf("Enter text to analyze (max %d characters):\n", MAX_TEXT_LEN - 1);
    
    if (fgets(input_text, sizeof(input_text), stdin) == NULL) {
        printf("Error reading input.\n");
        return 1;
    }
    
    size_t len = strlen(input_text);
    if (len > 0 && input_text[len - 1] == '\n') {
        input_text[len - 1] = '\0';
    }
    
    if (strlen(input_text) == 0) {
        printf("No input provided.\n");
        return 1;
    }
    
    process_text(&analyzer, input_text);
    print_analysis(&analyzer);
    
    return 0;
}