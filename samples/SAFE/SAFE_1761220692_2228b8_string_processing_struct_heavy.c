//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORD_LEN 64
#define MAX_WORDS 100
#define MAX_TEXT_LEN 1024

struct WordStats {
    char word[MAX_WORD_LEN];
    size_t count;
    size_t length;
    int is_capitalized;
};

struct TextAnalyzer {
    struct WordStats words[MAX_WORDS];
    size_t word_count;
    size_t total_chars;
    size_t total_words;
    char processed_text[MAX_TEXT_LEN];
};

void init_analyzer(struct TextAnalyzer *analyzer) {
    analyzer->word_count = 0;
    analyzer->total_chars = 0;
    analyzer->total_words = 0;
    analyzer->processed_text[0] = '\0';
    for (size_t i = 0; i < MAX_WORDS; i++) {
        analyzer->words[i].word[0] = '\0';
        analyzer->words[i].count = 0;
        analyzer->words[i].length = 0;
        analyzer->words[i].is_capitalized = 0;
    }
}

int is_word_char(char c) {
    return isalnum((unsigned char)c) || c == '\'' || c == '-';
}

int add_word(struct TextAnalyzer *analyzer, const char *word, size_t len) {
    if (len == 0 || len >= MAX_WORD_LEN || analyzer->word_count >= MAX_WORDS) {
        return 0;
    }

    for (size_t i = 0; i < analyzer->word_count; i++) {
        if (strncmp(analyzer->words[i].word, word, len) == 0 && 
            analyzer->words[i].word[len] == '\0') {
            analyzer->words[i].count++;
            return 1;
        }
    }

    struct WordStats *ws = &analyzer->words[analyzer->word_count];
    strncpy(ws->word, word, len);
    ws->word[len] = '\0';
    ws->count = 1;
    ws->length = len;
    ws->is_capitalized = isupper((unsigned char)word[0]) ? 1 : 0;
    analyzer->word_count++;
    return 1;
}

void process_text(struct TextAnalyzer *analyzer, const char *text) {
    if (text == NULL) return;
    
    size_t text_len = strlen(text);
    if (text_len >= MAX_TEXT_LEN) return;
    
    strncpy(analyzer->processed_text, text, MAX_TEXT_LEN - 1);
    analyzer->processed_text[MAX_TEXT_LEN - 1] = '\0';
    
    char buffer[MAX_WORD_LEN];
    size_t buf_pos = 0;
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
    printf("Total characters: %zu\n", analyzer->total_chars);
    printf("Total words: %zu\n", analyzer->total_words);
    printf("Unique words: %zu\n", analyzer->word_count);
    printf("\nWord Statistics:\n");
    
    for (size_t i = 0; i < analyzer->word_count; i++) {
        const struct WordStats *ws = &analyzer->words[i];
        printf("'%s': length=%zu, count=%zu, %s\n", 
               ws->word, ws->length, ws->count,
               ws->is_capitalized ? "capitalized" : "lowercase");
    }
}

int main(void) {
    struct TextAnalyzer analyzer;
    init_analyzer(&analyzer);
    
    char input_text[MAX_TEXT_LEN];
    printf("Enter text to analyze (max %d characters):\n", MAX_TEXT_LEN - 1);
    
    if (fgets(input_text, sizeof(input_text), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input_text);
    if (len > 0 && input_text[len - 1] == '\n') {
        input_text[len - 1] = '\0';
    }
    
    if (strlen(input_text) == 0) {
        printf("No text entered.\n");
        return 1;
    }
    
    process_text(&analyzer, input_text);
    print_analysis(&analyzer);
    
    return 0;
}