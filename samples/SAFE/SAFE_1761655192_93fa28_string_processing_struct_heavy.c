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

struct TextProcessor init_processor(void) {
    struct TextProcessor tp;
    tp.word_count = 0;
    tp.total_words = 0;
    tp.unique_words = 0;
    tp.longest_word_length = 0;
    return tp;
}

int is_valid_char(char c) {
    return isalpha(c) || c == '\'' || c == '-';
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
    if (tp->word_count >= MAX_WORDS) {
        return 0;
    }
    
    int index = find_word_index(tp, word);
    if (index >= 0) {
        tp->words[index].count++;
        return 1;
    }
    
    size_t len = strlen(word);
    if (len >= MAX_WORD_LEN) {
        return 0;
    }
    
    strcpy(tp->words[tp->word_count].word, word);
    tp->words[tp->word_count].count = 1;
    tp->words[tp->word_count].length = (int)len;
    tp->words[tp->word_count].is_capitalized = isupper(word[0]) ? 1 : 0;
    
    if (len > tp->longest_word_length) {
        tp->longest_word_length = (int)len;
    }
    
    tp->word_count++;
    tp->unique_words++;
    return 1;
}

void process_text(struct TextProcessor *tp, const char *text) {
    char buffer[MAX_LINE_LEN];
    char word[MAX_WORD_LEN];
    int buf_pos = 0;
    int word_pos = 0;
    
    while (*text && buf_pos < MAX_LINE_LEN - 1) {
        char c = *text++;
        
        if (is_valid_char(c)) {
            if (word_pos < MAX_WORD_LEN - 1) {
                word[word_pos++] = tolower(c);
            }
        } else {
            if (word_pos > 0) {
                word[word_pos] = '\0';
                if (add_word(tp, word)) {
                    tp->total_words++;
                }
                word_pos = 0;
            }
        }
        
        buffer[buf_pos++] = c;
    }
    
    if (word_pos > 0) {
        word[word_pos] = '\0';
        if (add_word(tp, word)) {
            tp->total_words++;
        }
    }
}

void print_statistics(struct TextProcessor *tp) {
    printf("Text Statistics:\n");
    printf("Total words: %d\n", tp->total_words);
    printf("Unique words: %d\n", tp->unique_words);
    printf("Longest word length: %d\n", tp->longest_word_length);
    printf("\nWord frequency (top 10):\n");
    
    for (int i = 0; i < (tp->word_count < 10 ? tp->word_count : 10); i++) {
        int max_index = i;
        for (int j = i + 1; j < tp->word_count; j++) {
            if (tp->words[j].count > tp->words[max_index].count) {
                max_index = j;
            }
        }
        
        if (max_index != i) {
            struct WordStats temp = tp->words[i];
            tp->words[i] = tp->words[max_index];
            tp->words[max_index] = temp;
        }
        
        printf("%-15s: %d\n", tp->words[i].word, tp->words[i].count);
    }
}

int main(void) {
    struct TextProcessor processor = init_processor();
    char input_text[] = "The quick brown fox jumps over the lazy dog. The dog was not amused by the fox's antics. Quick thinking saved the day!";
    
    process_text(&processor, input_text);
    print_statistics(&processor);
    
    return 0;
}