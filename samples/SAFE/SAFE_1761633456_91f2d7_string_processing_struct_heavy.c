//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORD_LEN 64
#define MAX_WORDS 1000

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

void add_word(struct TextAnalyzer *analyzer, const char *word, int length) {
    if (length <= 0 || length >= MAX_WORD_LEN) return;
    
    int index = find_word_index(analyzer, word);
    if (index >= 0) {
        analyzer->words[index].count++;
        return;
    }
    
    if (analyzer->word_count >= MAX_WORDS) return;
    
    struct WordStats *ws = &analyzer->words[analyzer->word_count];
    strncpy(ws->word, word, MAX_WORD_LEN - 1);
    ws->word[MAX_WORD_LEN - 1] = '\0';
    ws->count = 1;
    ws->length = length;
    ws->is_capitalized = isupper((unsigned char)word[0]) ? 1 : 0;
    
    analyzer->word_count++;
    analyzer->unique_words++;
}

void process_text(struct TextAnalyzer *analyzer, const char *text) {
    if (!text) return;
    
    char buffer[MAX_WORD_LEN];
    int buf_pos = 0;
    int text_len = strlen(text);
    
    for (int i = 0; i <= text_len; i++) {
        char c = text[i];
        
        if (is_word_char(c) && buf_pos < MAX_WORD_LEN - 1) {
            buffer[buf_pos++] = c;
        } else {
            if (buf_pos > 0) {
                buffer[buf_pos] = '\0';
                add_word(analyzer, buffer, buf_pos);
                analyzer->total_words++;
                analyzer->total_chars += buf_pos;
                buf_pos = 0;
            }
        }
    }
}

int compare_word_count(const void *a, const void *b) {
    const struct WordStats *wa = (const struct WordStats *)a;
    const struct WordStats *wb = (const struct WordStats *)b;
    return wb->count - wa->count;
}

void print_analysis(struct TextAnalyzer *analyzer) {
    printf("Text Analysis Results:\n");
    printf("Total words: %d\n", analyzer->total_words);
    printf("Total characters: %d\n", analyzer->total_chars);
    printf("Unique words: %d\n", analyzer->unique_words);
    printf("Average word length: %.2f\n", 
           analyzer->total_words > 0 ? (double)analyzer->total_chars / analyzer->total_words : 0.0);
    
    printf("\nTop 10 most frequent words:\n");
    struct WordStats sorted[MAX_WORDS];
    if (analyzer->word_count > 0) {
        memcpy(sorted, analyzer->words, sizeof(struct WordStats) * analyzer->word_count);
        qsort(sorted, analyzer->word_count, sizeof(struct WordStats), compare_word_count);
        
        int limit = analyzer->word_count < 10 ? analyzer->word_count : 10;
        for (int i = 0; i < limit; i++) {
            printf("%d. '%s' (count: %d, length: %d, %s)\n", 
                   i + 1, sorted[i].word, sorted[i].count, sorted[i].length,
                   sorted[i].is_capitalized ? "capitalized" : "lowercase");
        }
    }
}

int main() {
    struct TextAnalyzer analyzer;
    init_analyzer(&analyzer);
    
    char input_buffer[4096];
    printf("Enter text to analyze (max 4095 characters):\n");
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin)) {
        size_t len = strlen(input_buffer);
        if (len > 0 && input_buffer[len - 1] == '\n') {
            input_buffer[len - 1] = '\0';
        }
        
        if (strlen(input_buffer) > 0) {
            process_text(&analyzer, input_buffer);
            print_analysis(&analyzer);
        } else {
            printf("No text entered.\n");
        }
    } else {
        printf("Error reading input.\n");
    }
    
    return 0;
}