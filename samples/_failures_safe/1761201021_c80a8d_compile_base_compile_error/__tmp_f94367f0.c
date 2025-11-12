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

void init_text_processor(struct TextProcessor *tp) {
    tp->word_count = 0;
    tp->total_words = 0;
    tp->unique_words = 0;
    tp->processed_text[0] = '\0';
    for (int i = 0; i < MAX_WORDS; i++) {
        tp->words[i].word[0] = '\0';
        tp->words[i].count = 0;
        tp->words[i].first_position = -1;
        tp->words[i].last_position = -1;
    }
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

int find_word_index(struct TextProcessor *tp, const char *word) {
    for (int i = 0; i < tp->word_count; i++) {
        if (strcmp(tp->words[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

int add_word(struct TextProcessor *tp, const char *word, int position) {
    if (tp->word_count >= MAX_WORDS) return 0;
    if (strlen(word) >= MAX_WORD_LEN) return 0;
    
    int index = find_word_index(tp, word);
    if (index == -1) {
        index = tp->word_count;
        if (!safe_strncpy(tp->words[index].word, word, MAX_WORD_LEN)) return 0;
        tp->words[index].count = 1;
        tp->words[index].first_position = position;
        tp->words[index].last_position = position;
        tp->word_count++;
        tp->unique_words++;
    } else {
        tp->words[index].count++;
        tp->words[index].last_position = position;
    }
    tp->total_words++;
    return 1;
}

int process_text(struct TextProcessor *tp, const char *text) {
    if (text == NULL) return 0;
    
    char buffer[MAX_TEXT_LEN];
    if (!safe_strncpy(buffer, text, MAX_TEXT_LEN)) return 0;
    
    char *ptr = buffer;
    int position = 0;
    int in_word = 0;
    char current_word[MAX_WORD_LEN];
    int word_len = 0;
    
    for (size_t i = 0; i < strlen(buffer) && buffer[i] != '\0'; i++) {
        if (!is_valid_char(buffer[i])) continue;
        
        if (isalnum((unsigned char)buffer[i])) {
            if (!in_word) {
                in_word = 1;
                word_len = 0;
            }
            if (word_len < MAX_WORD_LEN - 1) {
                current_word[word_len++] = tolower((unsigned char)buffer[i]);
            }
        } else if (in_word) {
            current_word[word_len] = '\0';
            if (word_len > 0) {
                if (!add_word(tp, current_word, position)) return 0;
                position++;
            }
            in_word = 0;
            word_len = 0;
        }
    }
    
    if (in_word && word_len > 0) {
        current_word[word_len] = '\0';
        if (!add_word(tp, current_word, position)) return 0;
    }
    
    if (!safe_strncpy(tp->processed_text, buffer, MAX_TEXT_LEN)) return 0;
    return 1;
}

void print_statistics(struct TextProcessor *tp) {
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

int main() {
    struct TextProcessor processor;
    init_text_processor(&processor);
    
    char input_text[MAX_TEXT_LEN];
    printf("Enter text to analyze (max %d characters):\n", MAX_TEXT_LEN - 1);
    
    if (fgets(input_text, sizeof(input_text), stdin) == NULL) {
        printf("Error reading input