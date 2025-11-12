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

struct TextProcessor {
    struct WordStats words[MAX_WORDS];
    int word_count;
    int total_chars;
    int total_words;
    char processed_text[MAX_TEXT_LEN];
};

void init_text_processor(struct TextProcessor *tp) {
    tp->word_count = 0;
    tp->total_chars = 0;
    tp->total_words = 0;
    tp->processed_text[0] = '\0';
    for (int i = 0; i < MAX_WORDS; i++) {
        tp->words[i].word[0] = '\0';
        tp->words[i].count = 0;
        tp->words[i].length = 0;
        tp->words[i].is_capitalized = 0;
    }
}

int is_word_char(char c) {
    return isalnum((unsigned char)c) || c == '\'' || c == '-';
}

int find_word_index(struct TextProcessor *tp, const char *word) {
    for (int i = 0; i < tp->word_count; i++) {
        if (strcmp(tp->words[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

int add_word(struct TextProcessor *tp, const char *word, int capitalized) {
    if (tp->word_count >= MAX_WORDS) return -1;
    
    int idx = find_word_index(tp, word);
    if (idx >= 0) {
        tp->words[idx].count++;
        return idx;
    }
    
    int len = strlen(word);
    if (len >= MAX_WORD_LEN) return -1;
    
    strcpy(tp->words[tp->word_count].word, word);
    tp->words[tp->word_count].count = 1;
    tp->words[tp->word_count].length = len;
    tp->words[tp->word_count].is_capitalized = capitalized;
    tp->word_count++;
    return tp->word_count - 1;
}

int process_text(struct TextProcessor *tp, const char *text) {
    if (text == NULL) return -1;
    
    int text_len = strlen(text);
    if (text_len >= MAX_TEXT_LEN) return -1;
    
    strcpy(tp->processed_text, text);
    tp->total_chars = text_len;
    
    int i = 0;
    while (i < text_len) {
        while (i < text_len && !is_word_char(text[i])) i++;
        if (i >= text_len) break;
        
        int start = i;
        while (i < text_len && is_word_char(text[i])) i++;
        
        int word_len = i - start;
        if (word_len >= MAX_WORD_LEN) continue;
        
        char word[MAX_WORD_LEN];
        strncpy(word, text + start, word_len);
        word[word_len] = '\0';
        
        int capitalized = isupper((unsigned char)word[0]) ? 1 : 0;
        add_word(tp, word, capitalized);
        tp->total_words++;
    }
    
    return 0;
}

void print_statistics(struct TextProcessor *tp) {
    printf("Text Statistics:\n");
    printf("Total characters: %d\n", tp->total_chars);
    printf("Total words: %d\n", tp->total_words);
    printf("Unique words: %d\n", tp->word_count);
    printf("\nWord frequency:\n");
    
    for (int i = 0; i < tp->word_count; i++) {
        printf("%-20s: %d (length: %d, %s)\n", 
               tp->words[i].word, 
               tp->words[i].count,
               tp->words[i].length,
               tp->words[i].is_capitalized ? "capitalized" : "lowercase");
    }
}

int main() {
    struct TextProcessor processor;
    init_text_processor(&processor);
    
    char input_text[MAX_TEXT_LEN];
    printf("Enter text to analyze (max %d characters):\n", MAX_TEXT_LEN - 1);
    
    if (fgets(input_text, sizeof(input_text), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input_text);
    if (len > 0 && input_text[len - 1] == '\n') {
        input_text[len - 1] = '\0';
    }
    
    if (process_text(&processor, input_text) != 0) {
        printf("Error processing text\n");
        return 1;
    }
    
    print_statistics(&processor);
    
    return 0;
}