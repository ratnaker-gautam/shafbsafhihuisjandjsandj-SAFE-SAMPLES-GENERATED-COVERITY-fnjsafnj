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
};

void init_analyzer(struct TextAnalyzer *analyzer) {
    analyzer->word_count = 0;
    analyzer->total_chars = 0;
    analyzer->total_words = 0;
    analyzer->unique_words = 0;
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
    
    char buffer[MAX_TEXT_LEN];
    size_t text_len = strlen(text);
    if (text_len >= MAX_TEXT_LEN) text_len = MAX_TEXT_LEN - 1;
    
    strncpy(buffer, text, text_len);
    buffer[text_len] = '\0';
    
    char *ptr = buffer;
    while (*ptr) {
        while (*ptr && !is_word_char(*ptr)) ptr++;
        if (!*ptr) break;
        
        char *word_start = ptr;
        while (*ptr && is_word_char(*ptr)) ptr++;
        
        char temp_word[MAX_WORD_LEN];
        size_t word_len = ptr - word_start;
        if (word_len >= MAX_WORD_LEN) word_len = MAX_WORD_LEN - 1;
        
        strncpy(temp_word, word_start, word_len);
        temp_word[word_len] = '\0';
        
        int is_cap = isupper((unsigned char)temp_word[0]) ? 1 : 0;
        for (size_t i = 0; i < word_len; i++) {
            temp_word[i] = (char)tolower((unsigned char)temp_word[i]);
        }
        
        if (word_len > 0) {
            add_word(analyzer, temp_word, is_cap);
            analyzer->total_words++;
            analyzer->total_chars += (int)word_len;
        }
    }
}

void print_analysis(struct TextAnalyzer *analyzer) {
    printf("Text Analysis Results:\n");
    printf("Total words: %d\n", analyzer->total_words);
    printf("Total characters in words: %d\n", analyzer->total_chars);
    printf("Unique words: %d\n", analyzer->unique_words);
    printf("\nWord frequency:\n");
    
    for (int i = 0; i < analyzer->word_count; i++) {
        struct WordStats *ws = &analyzer->words[i];
        printf("'%s': %d occurrences", ws->word, ws->count);
        if (ws->is_capitalized) {
            printf(" (capitalized)");
        }
        printf("\n");
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
    print_