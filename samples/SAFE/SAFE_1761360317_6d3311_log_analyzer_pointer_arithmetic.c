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

void print_summary(struct LogEntry *entries, int count) {
    if (entries == NULL || count <= 0) return;
    
    printf("Log Analysis Summary:\n");
    printf("Total entries: %d\n", count);
    
    const char *levels[] = {"INFO", "WARN", "ERROR", "DEBUG"};
    int level_count = sizeof(levels) / sizeof(levels[0]);
    
    for (int i = 0; i < level_count; i++) {
        int cnt = count_log_level(entries, count, levels[i]);
        printf("%s: %d\n", levels[i], cnt);
    }
}

int main(void) {
    struct LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    
    printf("Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL message)\n");
    printf("Enter 'END' on a separate line to finish input\n");
    
    char buffer[MAX_LINE_LEN];
    
    while (entry_count < MAX_ENTRIES) {
        if (fgets(buffer, MAX_LINE_LEN, stdin) == NULL) break;
        
        buffer[strcspn(buffer, "\n")] = '\0';
        
        if (strcmp(buffer, "END") == 0) break;
        
        if (strlen(buffer) == 0) continue;
        
        struct LogEntry *current = entries + entry_count;
        if (parse_log_line(buffer, current)) {
            entry_count++;
        } else {
            printf("Invalid log format. Expected: YYYY-MM-DD HH:MM:SS LEVEL message\n");
        }
    }
    
    if (entry_count > 0) {
        print_summary(entries, entry_count);
        
        printf("\nLast 5 entries:\n");
        int start = (entry_count > 5) ? entry_count - 5 : 0;
        struct LogEntry *ptr = entries + start;
        struct LogEntry *end = entries + entry_count;
        
        while (ptr < end) {
            printf("%s %s %s\n", ptr->timestamp, ptr->level, ptr->message);
            ptr++;
        }
    } else {
        printf("No valid log entries processed.\n");
    }
    
    return 0;
}