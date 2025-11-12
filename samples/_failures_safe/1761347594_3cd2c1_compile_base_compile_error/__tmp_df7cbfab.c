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
    int first_position;
    int last_position;
};

struct TextProcessor {
    struct WordStats words[MAX_WORDS];
    int word_count;
    char processed_text[MAX_TEXT_LEN];
    int total_words;
    int unique_words;
};

struct TextProcessor init_processor(void) {
    struct TextProcessor tp;
    tp.word_count = 0;
    tp.total_words = 0;
    tp.unique_words = 0;
    tp.processed_text[0] = '\0';
    for (int i = 0; i < MAX_WORDS; i++) {
        tp.words[i].word[0] = '\0';
        tp.words[i].count = 0;
        tp.words[i].first_position = -1;
        tp.words[i].last_position = -1;
    }
    return tp;
}

int is_valid_char(char c) {
    return isalnum((unsigned char)c) || c == ' ' || c == '\t' || c == '\n';
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

void normalize_word(char *word) {
    if (word == NULL || word[0] == '\0') return;
    for (int i = 0; word[i] != '\0'; i++) {
        word[i] = tolower((unsigned char)word[i]);
    }
}

int find_word_index(struct TextProcessor *tp, const char *word) {
    if (tp == NULL || word == NULL) return -1;
    for (int i = 0; i < tp->word_count; i++) {
        if (strcmp(tp->words[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

int add_word(struct TextProcessor *tp, const char *word, int position) {
    if (tp == NULL || word == NULL || position < 0) return 0;
    if (tp->word_count >= MAX_WORDS) return 0;
    
    int index = find_word_index(tp, word);
    if (index >= 0) {
        tp->words[index].count++;
        tp->words[index].last_position = position;
        return 1;
    }
    
    if (safe_strncpy(tp->words[tp->word_count].word, word, MAX_WORD_LEN)) {
        tp->words[tp->word_count].count = 1;
        tp->words[tp->word_count].first_position = position;
        tp->words[tp->word_count].last_position = position;
        tp->word_count++;
        tp->unique_words++;
        return 1;
    }
    return 0;
}

int process_text(struct TextProcessor *tp, const char *text) {
    if (tp == NULL || text == NULL) return 0;
    
    char buffer[MAX_TEXT_LEN];
    if (!safe_strncpy(buffer, text, MAX_TEXT_LEN)) return 0;
    
    char *token = strtok(buffer, " \t\n");
    int position = 0;
    
    while (token != NULL && position < MAX_WORDS) {
        int valid = 1;
        for (int i = 0; token[i] != '\0'; i++) {
            if (!is_valid_char(token[i])) {
                valid = 0;
                break;
            }
        }
        
        if (valid && strlen(token) > 0) {
            normalize_word(token);
            if (add_word(tp, token, position)) {
                position++;
            }
        }
        
        token = strtok(NULL, " \t\n");
    }
    
    tp->total_words = position;
    if (safe_strncpy(tp->processed_text, text, MAX_TEXT_LEN)) {
        return 1;
    }
    return 0;
}

void print_statistics(struct TextProcessor *tp) {
    if (tp == NULL) return;
    
    printf("Text Analysis Results:\n");
    printf("Total words: %d\n", tp->total_words);
    printf("Unique words: %d\n", tp->unique_words);
    printf("\nWord Statistics:\n");
    
    for (int i = 0; i < tp->word_count; i++) {
        printf("Word: %-15s Count: %-3d First: %-3d Last: %-3d\n",
               tp->words[i].word,
               tp->words[i].count,
               tp->words[i].first_position,
               tp->words[i].last_position);
    }
}

int main(void) {
    struct TextProcessor processor = init_processor();
    char input_text[MAX_TEXT_LEN];
    
    printf("Enter text to analyze (max %d characters):\n", MAX_TEXT_LEN - 1);
    
    if (fgets(input_text, MAX_TEXT_LEN, stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;