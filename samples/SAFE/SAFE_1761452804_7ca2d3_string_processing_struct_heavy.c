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
    if (index == -1) {
        index = tp->word_count;
        strncpy(tp->words[index].word, word, MAX_WORD_LEN - 1);
        tp->words[index].word[MAX_WORD_LEN - 1] = '\0';
        tp->words[index].count = 1;
        tp->words[index].first_position = position;
        tp->words[index].last_position = position;
        tp->word_count++;
    } else {
        tp->words[index].count++;
        tp->words[index].last_position = position;
    }
    return index;
}

int process_text(struct TextProcessor *tp, const char *input) {
    if (input == NULL || strlen(input) >= MAX_TEXT_LEN) {
        return -1;
    }
    
    strncpy(tp->processed_text, input, MAX_TEXT_LEN - 1);
    tp->processed_text[MAX_TEXT_LEN - 1] = '\0';
    tp->text_length = strlen(tp->processed_text);
    
    char buffer[MAX_WORD_LEN];
    int buf_pos = 0;
    int word_position = 0;
    
    for (int i = 0; i <= tp->text_length; i++) {
        char c = tp->processed_text[i];
        
        if (is_word_char(c) && buf_pos < MAX_WORD_LEN - 1) {
            buffer[buf_pos++] = tolower((unsigned char)c);
        } else if (buf_pos > 0) {
            buffer[buf_pos] = '\0';
            if (add_word(tp, buffer, word_position) != -1) {
                word_position++;
                tp->total_words++;
            }
            buf_pos = 0;
        }
        
        if (c == '\0') break;
    }
    
    return 0;
}

void print_word_stats(const struct TextProcessor *tp) {
    printf("Total words processed: %d\n", tp->total_words);
    printf("Unique words: %d\n", tp->word_count);
    printf("\nWord statistics:\n");
    printf("%-20s %-8s %-15s %-15s\n", "Word", "Count", "First Pos", "Last Pos");
    printf("------------------------------------------------------------\n");
    
    for (int i = 0; i < tp->word_count; i++) {
        printf("%-20s %-8d %-15d %-15d\n", 
               tp->words[i].word, 
               tp->words[i].count, 
               tp->words[i].first_position, 
               tp->words[i].last_position);
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