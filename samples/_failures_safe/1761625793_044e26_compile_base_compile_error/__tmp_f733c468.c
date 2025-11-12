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
    int first_position;
    int last_position;
};

struct TextProcessor {
    struct WordStats words[MAX_WORDS];
    int word_count;
    char processed_text[MAX_TEXT_LEN];
    int text_length;
    int total_words;
};

void init_text_processor(struct TextProcessor *tp) {
    tp->word_count = 0;
    tp->text_length = 0;
    tp->total_words = 0;
    memset(tp->processed_text, 0, sizeof(tp->processed_text));
    for (int i = 0; i < MAX_WORDS; i++) {
        memset(tp->words[i].word, 0, sizeof(tp->words[i].word));
        tp->words[i].count = 0;
        tp->words[i].first_position = -1;
        tp->words[i].last_position = -1;
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

int add_word(struct TextProcessor *tp, const char *word, int position) {
    if (tp->word_count >= MAX_WORDS || strlen(word) >= MAX_WORD_LEN) {
        return -1;
    }
    
    int index = find_word_index(tp, word);
    if (index >= 0) {
        tp->words[index].count++;
        tp->words[index].last_position = position;
        return index;
    }
    
    strncpy(tp->words[tp->word_count].word, word, MAX_WORD_LEN - 1);
    tp->words[tp->word_count].word[MAX_WORD_LEN - 1] = '\0';
    tp->words[tp->word_count].count = 1;
    tp->words[tp->word_count].first_position = position;
    tp->words[tp->word_count].last_position = position;
    tp->word_count++;
    return tp->word_count - 1;
}

int process_text(struct TextProcessor *tp, const char *text) {
    if (text == NULL || strlen(text) >= MAX_TEXT_LEN) {
        return -1;
    }
    
    strncpy(tp->processed_text, text, MAX_TEXT_LEN - 1);
    tp->processed_text[MAX_TEXT_LEN - 1] = '\0';
    tp->text_length = strlen(tp->processed_text);
    
    int pos = 0;
    int word_start = -1;
    int word_count = 0;
    
    while (pos < tp->text_length && word_count < MAX_WORDS) {
        if (is_word_char(tp->processed_text[pos])) {
            if (word_start == -1) {
                word_start = pos;
            }
        } else {
            if (word_start != -1) {
                char word[MAX_WORD_LEN];
                int word_len = pos - word_start;
                if (word_len < MAX_WORD_LEN) {
                    strncpy(word, &tp->processed_text[word_start], word_len);
                    word[word_len] = '\0';
                    
                    for (int i = 0; i < word_len; i++) {
                        word[i] = tolower((unsigned char)word[i]);
                    }
                    
                    if (add_word(tp, word, word_count) >= 0) {
                        word_count++;
                    }
                }
                word_start = -1;
            }
        }
        pos++;
    }
    
    if (word_start != -1 && word_count < MAX_WORDS) {
        char word[MAX_WORD_LEN];
        int word_len = tp->text_length - word_start;
        if (word_len < MAX_WORD_LEN) {
            strncpy(word, &tp->processed_text[word_start], word_len);
            word[word_len] = '\0';
            
            for (int i = 0; i < word_len; i++) {
                word[i] = tolower((unsigned char)word[i]);
            }
            
            if (add_word(tp, word, word_count) >= 0) {
                word_count++;
            }
        }
    }
    
    tp->total_words = word_count;
    return word_count;
}

void print_word_stats(const struct TextProcessor *tp) {
    printf("Word Statistics:\n");
    printf("Total unique words: %d\n", tp->word_count);
    printf("Total words processed: %d\n", tp->total_words);
    printf("\n");
    
    for (int i = 0; i < tp->word_count; i++) {
        printf("Word: %-20s Count: %-4d First: %-4d Last: %-4d\n",
               tp->words[i].word, tp->words[i].count,
               tp->words[i].first_position, tp->words[i].last_position);
    }
}

int main(void) {
    struct TextProcessor processor;
    init_text_processor(&processor);
    
    char input