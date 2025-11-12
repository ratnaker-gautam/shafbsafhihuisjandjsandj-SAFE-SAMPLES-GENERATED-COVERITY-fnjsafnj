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

struct TextProcessor {
    struct WordStats words[MAX_WORDS];
    int word_count;
    char processed_text[MAX_TEXT_LEN];
    int total_chars;
    int total_words;
};

void init_text_processor(struct TextProcessor *tp) {
    tp->word_count = 0;
    tp->processed_text[0] = '\0';
    tp->total_chars = 0;
    tp->total_words = 0;
    for (int i = 0; i < MAX_WORDS; i++) {
        tp->words[i].word[0] = '\0';
        tp->words[i].count = 0;
        tp->words[i].length = 0;
        tp->words[i].is_capitalized = 0;
    }
}

int is_word_char(char c) {
    return isalpha((unsigned char)c) || c == '\'' || c == '-';
}

int find_word_index(struct TextProcessor *tp, const char *word) {
    for (int i = 0; i < tp->word_count; i++) {
        if (strcmp(tp->words[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

int process_text(struct TextProcessor *tp, const char *input) {
    if (input == NULL || strlen(input) >= MAX_TEXT_LEN) {
        return 0;
    }
    
    char buffer[MAX_TEXT_LEN];
    strncpy(buffer, input, MAX_TEXT_LEN - 1);
    buffer[MAX_TEXT_LEN - 1] = '\0';
    
    char *pos = buffer;
    int in_word = 0;
    char current_word[MAX_WORD_LEN];
    int word_len = 0;
    
    while (*pos && tp->word_count < MAX_WORDS) {
        if (is_word_char(*pos)) {
            if (!in_word) {
                in_word = 1;
                word_len = 0;
            }
            if (word_len < MAX_WORD_LEN - 1) {
                current_word[word_len++] = *pos;
            }
        } else {
            if (in_word && word_len > 0) {
                current_word[word_len] = '\0';
                
                int found_index = find_word_index(tp, current_word);
                if (found_index == -1) {
                    if (tp->word_count < MAX_WORDS) {
                        strncpy(tp->words[tp->word_count].word, current_word, MAX_WORD_LEN - 1);
                        tp->words[tp->word_count].word[MAX_WORD_LEN - 1] = '\0';
                        tp->words[tp->word_count].count = 1;
                        tp->words[tp->word_count].length = word_len;
                        tp->words[tp->word_count].is_capitalized = isupper((unsigned char)current_word[0]) ? 1 : 0;
                        tp->word_count++;
                    }
                } else {
                    tp->words[found_index].count++;
                }
                tp->total_words++;
            }
            in_word = 0;
            word_len = 0;
        }
        pos++;
    }
    
    if (in_word && word_len > 0 && tp->word_count < MAX_WORDS) {
        current_word[word_len] = '\0';
        int found_index = find_word_index(tp, current_word);
        if (found_index == -1) {
            strncpy(tp->words[tp->word_count].word, current_word, MAX_WORD_LEN - 1);
            tp->words[tp->word_count].word[MAX_WORD_LEN - 1] = '\0';
            tp->words[tp->word_count].count = 1;
            tp->words[tp->word_count].length = word_len;
            tp->words[tp->word_count].is_capitalized = isupper((unsigned char)current_word[0]) ? 1 : 0;
            tp->word_count++;
        } else {
            tp->words[found_index].count++;
        }
        tp->total_words++;
    }
    
    strncpy(tp->processed_text, buffer, MAX_TEXT_LEN - 1);
    tp->processed_text[MAX_TEXT_LEN - 1] = '\0';
    tp->total_chars = strlen(tp->processed_text);
    
    return 1;
}

void print_statistics(struct TextProcessor *tp) {
    printf("Text Statistics:\n");
    printf("Total characters: %d\n", tp->total_chars);
    printf("Total words: %d\n", tp->total_words);
    printf("Unique words: %d\n", tp->word_count);
    printf("\nWord frequency:\n");
    
    for (int i = 0; i < tp->word_count; i++) {
        printf("%-20s: %d occurrences (length: %d, %s)\n", 
               tp->words[i].word, 
               tp->words[i].count,
               tp->words[i].length,
               tp->words[i].is_capitalized ? "capitalized" : "lowercase");
    }
}

int main() {
    struct TextProcessor processor;