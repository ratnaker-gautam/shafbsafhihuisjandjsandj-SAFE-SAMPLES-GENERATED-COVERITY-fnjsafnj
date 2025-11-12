//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORDS 100
#define MAX_WORD_LEN 50
#define MAX_TEXT_LEN 1000

struct WordInfo {
    char word[MAX_WORD_LEN];
    int count;
    int length;
    int is_capitalized;
};

struct TextProcessor {
    struct WordInfo words[MAX_WORDS];
    int word_count;
    char original_text[MAX_TEXT_LEN];
    int total_chars;
};

void init_text_processor(struct TextProcessor *tp) {
    tp->word_count = 0;
    tp->original_text[0] = '\0';
    tp->total_chars = 0;
    for (int i = 0; i < MAX_WORDS; i++) {
        tp->words[i].word[0] = '\0';
        tp->words[i].count = 0;
        tp->words[i].length = 0;
        tp->words[i].is_capitalized = 0;
    }
}

int safe_strncpy(char *dest, const char *src, size_t n) {
    if (dest == NULL || src == NULL || n == 0) return 0;
    size_t i;
    for (i = 0; i < n - 1 && src[i] != '\0'; i++) {
        dest[i] = src[i];
    }
    dest[i] = '\0';
    return 1;
}

int is_word_char(char c) {
    return isalpha((unsigned char)c) || c == '\'' || c == '-';
}

int process_text(struct TextProcessor *tp, const char *text) {
    if (tp == NULL || text == NULL) return 0;
    
    if (!safe_strncpy(tp->original_text, text, MAX_TEXT_LEN)) return 0;
    tp->total_chars = strlen(tp->original_text);
    
    const char *ptr = text;
    int in_word = 0;
    char current_word[MAX_WORD_LEN];
    int word_len = 0;
    
    while (*ptr != '\0' && tp->word_count < MAX_WORDS) {
        if (is_word_char(*ptr)) {
            if (!in_word) {
                in_word = 1;
                word_len = 0;
            }
            if (word_len < MAX_WORD_LEN - 1) {
                current_word[word_len++] = *ptr;
            }
        } else {
            if (in_word && word_len > 0) {
                current_word[word_len] = '\0';
                
                int found = 0;
                for (int i = 0; i < tp->word_count; i++) {
                    if (strcmp(tp->words[i].word, current_word) == 0) {
                        tp->words[i].count++;
                        found = 1;
                        break;
                    }
                }
                
                if (!found) {
                    if (!safe_strncpy(tp->words[tp->word_count].word, current_word, MAX_WORD_LEN)) {
                        return 0;
                    }
                    tp->words[tp->word_count].count = 1;
                    tp->words[tp->word_count].length = word_len;
                    tp->words[tp->word_count].is_capitalized = isupper((unsigned char)current_word[0]);
                    tp->word_count++;
                }
                
                in_word = 0;
                word_len = 0;
            }
        }
        ptr++;
    }
    
    if (in_word && word_len > 0 && tp->word_count < MAX_WORDS) {
        current_word[word_len] = '\0';
        int found = 0;
        for (int i = 0; i < tp->word_count; i++) {
            if (strcmp(tp->words[i].word, current_word) == 0) {
                tp->words[i].count++;
                found = 1;
                break;
            }
        }
        if (!found) {
            if (!safe_strncpy(tp->words[tp->word_count].word, current_word, MAX_WORD_LEN)) {
                return 0;
            }
            tp->words[tp->word_count].count = 1;
            tp->words[tp->word_count].length = word_len;
            tp->words[tp->word_count].is_capitalized = isupper((unsigned char)current_word[0]);
            tp->word_count++;
        }
    }
    
    return 1;
}

void print_word_stats(const struct TextProcessor *tp) {
    if (tp == NULL) return;
    
    printf("Text analysis results:\n");
    printf("Total characters: %d\n", tp->total_chars);
    printf("Unique words: %d\n", tp->word_count);
    printf("\nWord frequency:\n");
    
    for (int i = 0; i < tp->word_count; i++) {
        printf("'%s': %d occurrences, length: %d, %s\n",
               tp->words[i].word,
               tp->words[i].count,
               tp->words[i].length,
               tp->words[i].is_capitalized ? "capitalized" : "lowercase");
    }
}

int main(void) {
    struct TextProcessor processor;
    init_text_processor(&processor);
    
    char input_text[MAX_TEXT_LEN];
    printf("Enter text to analyze (max %d characters): ", MAX_TEXT_LEN - 1);
    
    if (fgets(input_text, MAX_TEXT_L