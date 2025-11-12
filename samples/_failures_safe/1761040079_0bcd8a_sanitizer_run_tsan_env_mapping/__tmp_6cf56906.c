//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: struct_heavy ; Variation: string_processing
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
        tp->words[i].first_position = -1;
        tp->words[i].last_position = -1;
    }
}

int is_word_char(char c) {
    return isalnum((unsigned char)c) || c == '_' || c == '-';
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
    
    strcpy(tp->words[tp->word_count].word, word);
    tp->words[tp->word_count].count = 1;
    tp->words[tp->word_count].first_position = position;
    tp->words[tp->word_count].last_position = position;
    tp->word_count++;
    return tp->word_count - 1;
}

int process_text(struct TextProcessor *tp, const char *input) {
    if (input == NULL || strlen(input) >= MAX_TEXT_LEN) {
        return -1;
    }
    
    strcpy(tp->processed_text, input);
    tp->total_chars = strlen(input);
    
    char buffer[MAX_WORD_LEN];
    int buf_pos = 0;
    int word_position = 0;
    
    for (int i = 0; i <= tp->total_chars; i++) {
        char c = input[i];
        
        if (is_word_char(c) && buf_pos < MAX_WORD_LEN - 1) {
            buffer[buf_pos++] = tolower((unsigned char)c);
        } else {
            if (buf_pos > 0) {
                buffer[buf_pos] = '\0';
                if (add_word(tp, buffer, word_position) >= 0) {
                    tp->total_words++;
                    word_position++;
                }
                buf_pos = 0;
            }
        }
    }
    
    return 0;
}

void print_word_stats(struct TextProcessor *tp) {
    printf("Text Statistics:\n");
    printf("Total characters: %d\n", tp->total_chars);
    printf("Total words: %d\n", tp->total_words);
    printf("Unique words: %d\n", tp->word_count);
    printf("\nWord Details:\n");
    
    for (int i = 0; i < tp->word_count; i++) {
        printf("Word: %-20s Count: %-4d First: %-4d Last: %-4d\n",
               tp->words[i].word, tp->words[i].count,
               tp->words[i].first_position, tp->words[i].last_position);
    }
}

int main(void) {
    struct TextProcessor processor;
    init_text_processor(&processor);
    
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
    
    if (process_text(&processor, input_text) != 0) {
        fprintf(stderr, "Error processing text\n");
        return 1;
    }
    
    print_word_stats(&processor);
    
    return 0;
}