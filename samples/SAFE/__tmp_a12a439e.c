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
    int is_capitalized;
};

struct TextAnalyzer {
    struct WordStats words[MAX_WORDS];
    int word_count;
    int total_chars;
    int total_words;
    int unique_words;
    char processed_text[MAX_TEXT_LEN];
};

void init_analyzer(struct TextAnalyzer *analyzer) {
    analyzer->word_count = 0;
    analyzer->total_chars = 0;
    analyzer->total_words = 0;
    analyzer->unique_words = 0;
    analyzer->processed_text[0] = '\0';
    for (int i = 0; i < MAX_WORDS; i++) {
        analyzer->words[i].word[0] = '\0';
        analyzer->words[i].count = 0;
        analyzer->words[i].length = 0;
        analyzer->words[i].is_capitalized = 0;
    }
}

int is_word_char(char c) {
    return isalnum((unsigned char)c) || c == '\'' || c == '-';
}

int find_word_index(struct TextAnalyzer *analyzer, const char *word) {
    for (int i = 0; i < analyzer->word_count; i++) {
        if (strcmp(analyzer->words[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

void add_word(struct TextAnalyzer *analyzer, const char *word, int is_capitalized) {
    if (analyzer->word_count >= MAX_WORDS) return;
    
    int index = find_word_index(analyzer, word);
    if (index >= 0) {
        analyzer->words[index].count++;
        return;
    }
    
    struct WordStats *ws = &analyzer->words[analyzer->word_count];
    size_t len = strlen(word);
    if (len >= MAX_WORD_LEN) len = MAX_WORD_LEN - 1;
    
    strncpy(ws->word, word, len);
    ws->word[len] = '\0';
    ws->count = 1;
    ws->length = (int)len;
    ws->is_capitalized = is_capitalized;
    analyzer->word_count++;
    analyzer->unique_words++;
}

void process_text(struct TextAnalyzer *analyzer, const char *text) {
    if (text == NULL) return;
    
    size_t text_len = strlen(text);
    if (text_len >= MAX_TEXT_LEN) text_len = MAX_TEXT_LEN - 1;
    
    strncpy(analyzer->processed_text, text, text_len);
    analyzer->processed_text[text_len] = '\0';
    analyzer->total_chars = (int)text_len;
    
    char buffer[MAX_WORD_LEN];
    int buf_pos = 0;
    int in_word = 0;
    int word_start = 1;
    
    for (size_t i = 0; i <= text_len; i++) {
        char c = text[i];
        
        if (is_word_char(c) && buf_pos < MAX_WORD_LEN - 1) {
            buffer[buf_pos++] = c;
            in_word = 1;
        } else {
            if (in_word) {
                buffer[buf_pos] = '\0';
                if (buf_pos > 0) {
                    int is_cap = isupper((unsigned char)buffer[0]) ? 1 : 0;
                    add_word(analyzer, buffer, is_cap);
                    analyzer->total_words++;
                }
                buf_pos = 0;
                in_word = 0;
            }
        }
        word_start = (c == '\0' || isspace((unsigned char)c)) ? 1 : 0;
    }
}

void print_analysis(struct TextAnalyzer *analyzer) {
    printf("Text Analysis Results:\n");
    printf("Total characters: %d\n", analyzer->total_chars);
    printf("Total words: %d\n", analyzer->total_words);
    printf("Unique words: %d\n", analyzer->unique_words);
    printf("\nWord frequency:\n");
    
    for (int i = 0; i < analyzer->word_count; i++) {
        struct WordStats *ws = &analyzer->words[i];
        printf("'%s': %d occurrences, length: %d, %s\n", 
               ws->word, ws->count, ws->length, 
               ws->is_capitalized ? "capitalized" : "not capitalized");
    }
}

int main() {
    struct TextAnalyzer analyzer;
    init_analyzer(&analyzer);
    
    char input_text[MAX_TEXT_LEN];
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