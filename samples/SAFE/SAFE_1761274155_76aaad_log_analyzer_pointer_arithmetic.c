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
    if (end - ptr < 8) return 0;
    
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
    
    printf("Enter log lines (empty line to finish):\n");
    
    while (entry_count < MAX_ENTRIES && fgets(buffer, MAX_LINE_LEN, stdin) != NULL) {
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        }
        
        if (buffer[0] == '\0') break;
        
        if (parse_log_line(buffer, entries + entry_count)) {
            entry_count++;
        } else {
            printf("Invalid log format: %s\n", buffer);
        }
    }
    
    if (entry_count == 0) {
        printf("No valid log entries found.\n");
        return 1;
    }
    
    printf("\nLog Analysis Results:\n");
    printf("Total entries: %d\n", entry_count);
    
    const char *levels[] = {"INFO", "WARN", "ERROR", "DEBUG"};
    for (int i = 0; i < 4; i++) {
        int count = count_log_level(entries, entry_count, levels[i]);
        printf("%s: %d\n", levels[i], count);
    }
    
    printf("\nRecent entries:\n");
    int show_count = entry_count > 5 ? 5 : entry_count;
    struct LogEntry *start = entries + (entry_count - show_count);
    struct LogEntry *end = entries + entry_count;
    
    for (struct LogEntry *e = start; e < end; e++) {
        printf("%s [%s] %s\n", e->timestamp, e->level, e->message);
    }
    
    return 0;
}