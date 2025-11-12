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
    int total_chars;
    int longest_word_len;
};

struct TextAnalyzer {
    struct TextProcessor processor;
    int unique_words;
    int most_frequent_count;
};

void init_text_processor(struct TextProcessor *tp) {
    tp->word_count = 0;
    tp->total_words = 0;
    tp->total_chars = 0;
    tp->longest_word_len = 0;
}

void init_text_analyzer(struct TextAnalyzer *ta) {
    init_text_processor(&ta->processor);
    ta->unique_words = 0;
    ta->most_frequent_count = 0;
}

int is_word_char(char c) {
    return isalnum((unsigned char)c) || c == '\'' || c == '-';
}

int add_word(struct TextProcessor *tp, const char *word) {
    if (tp->word_count >= MAX_WORDS || word[0] == '\0') {
        return 0;
    }
    
    for (int i = 0; i < tp->word_count; i++) {
        if (strcmp(tp->words[i].word, word) == 0) {
            tp->words[i].count++;
            return 1;
        }
    }
    
    size_t len = strlen(word);
    if (len >= MAX_WORD_LEN) {
        return 0;
    }
    
    strncpy(tp->words[tp->word_count].word, word, MAX_WORD_LEN - 1);
    tp->words[tp->word_count].word[MAX_WORD_LEN - 1] = '\0';
    tp->words[tp->word_count].count = 1;
    tp->words[tp->word_count].length = (int)len;
    tp->words[tp->word_count].is_capitalized = isupper((unsigned char)word[0]) ? 1 : 0;
    
    tp->word_count++;
    return 1;
}

void process_text(struct TextProcessor *tp, const char *text) {
    if (text == NULL) return;
    
    char buffer[MAX_LINE_LEN];
    size_t text_len = strlen(text);
    if (text_len >= MAX_LINE_LEN - 1) return;
    
    strncpy(buffer, text, MAX_LINE_LEN - 1);
    buffer[MAX_LINE_LEN - 1] = '\0';
    
    char *ptr = buffer;
    char word[MAX_WORD_LEN];
    int word_pos = 0;
    
    while (*ptr != '\0') {
        if (is_word_char(*ptr)) {
            if (word_pos < MAX_WORD_LEN - 1) {
                word[word_pos++] = *ptr;
            }
        } else {
            if (word_pos > 0) {
                word[word_pos] = '\0';
                if (add_word(tp, word)) {
                    tp->total_words++;
                    tp->total_chars += word_pos;
                    if (word_pos > tp->longest_word_len) {
                        tp->longest_word_len = word_pos;
                    }
                }
                word_pos = 0;
            }
        }
        ptr++;
    }
    
    if (word_pos > 0) {
        word[word_pos] = '\0';
        if (add_word(tp, word)) {
            tp->total_words++;
            tp->total_chars += word_pos;
            if (word_pos > tp->longest_word_len) {
                tp->longest_word_len = word_pos;
            }
        }
    }
}

void analyze_text(struct TextAnalyzer *ta) {
    ta->unique_words = ta->processor.word_count;
    ta->most_frequent_count = 0;
    
    for (int i = 0; i < ta->processor.word_count; i++) {
        if (ta->processor.words[i].count > ta->most_frequent_count) {
            ta->most_frequent_count = ta->processor.words[i].count;
        }
    }
}

void print_analysis(const struct TextAnalyzer *ta) {
    printf("Text Analysis Results:\n");
    printf("Total words: %d\n", ta->processor.total_words);
    printf("Unique words: %d\n", ta->unique_words);
    printf("Total characters: %d\n", ta->processor.total_chars);
    printf("Longest word length: %d\n", ta->processor.longest_word_len);
    printf("Most frequent word count: %d\n", ta->most_frequent_count);
    
    printf("\nWord frequency (top 5):\n");
    int printed = 0;
    for (int count = ta->most_frequent_count; count > 0 && printed < 5; count--) {
        for (int i = 0; i < ta->processor.word_count && printed < 5; i++) {
            if (ta->processor.words[i].count == count) {
                printf("'%s': %d times\n", ta->processor.words[i].word, count);
                printed++;
            }
        }
    }
}

int main(void) {
    struct TextAnaly