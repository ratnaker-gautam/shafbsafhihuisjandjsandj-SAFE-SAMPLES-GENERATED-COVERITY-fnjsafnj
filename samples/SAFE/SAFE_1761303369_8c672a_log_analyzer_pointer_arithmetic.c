//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LEN 256
#define MAX_ENTRIES 1000

struct LogEntry {
    char timestamp[20];
    char level[10];
    char message[MAX_LINE_LEN - 30];
};

int parse_log_line(char *line, struct LogEntry *entry) {
    if (line == NULL || entry == NULL) return 0;
    
    char *ptr = line;
    char *end = ptr + strlen(line);
    
    if (end - ptr < 25) return 0;
    
    char *ts_end = ptr + 19;
    if (*ts_end != ' ') return 0;
    memcpy(entry->timestamp, ptr, 19);
    entry->timestamp[19] = '\0';
    
    ptr = ts_end + 1;
    if (end - ptr < 6) return 0;
    
    char *level_end = ptr;
    while (level_end < end && *level_end != ' ') level_end++;
    if (level_end - ptr >= 10) return 0;
    memcpy(entry->level, ptr, level_end - ptr);
    entry->level[level_end - ptr] = '\0';
    
    ptr = level_end + 1;
    if (ptr >= end) return 0;
    
    size_t msg_len = end - ptr;
    if (msg_len >= MAX_LINE_LEN - 30) msg_len = MAX_LINE_LEN - 31;
    memcpy(entry->message, ptr, msg_len);
    entry->message[msg_len] = '\0';
    
    return 1;
}

int count_log_level(struct LogEntry *entries, int count, const char *level) {
    int total = 0;
    struct LogEntry *end = entries + count;
    
    for (struct LogEntry *e = entries; e < end; e++) {
        if (strcmp(e->level, level) == 0) {
            total++;
        }
    }
    return total;
}

int main(void) {
    struct LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    char buffer[MAX_LINE_LEN];
    
    while (entry_count < MAX_ENTRIES && fgets(buffer, MAX_LINE_LEN, stdin) != NULL) {
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        }
        
        if (parse_log_line(buffer, entries + entry_count)) {
            entry_count++;
        }
    }
    
    if (ferror(stdin)) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", entry_count);
    
    const char *levels[] = {"INFO", "WARN", "ERROR", "DEBUG"};
    int level_count = sizeof(levels) / sizeof(levels[0]);
    
    for (int i = 0; i < level_count; i++) {
        int count = count_log_level(entries, entry_count, levels[i]);
        printf("%s: %d\n", levels[i], count);
    }
    
    if (entry_count > 0) {
        printf("\nFirst entry: %s %s %s\n", 
               entries[0].timestamp, entries[0].level, entries[0].message);
        printf("Last entry: %s %s %s\n", 
               entries[entry_count - 1].timestamp, 
               entries[entry_count - 1].level, 
               entries[entry_count - 1].message);
    }
    
    return 0;
}