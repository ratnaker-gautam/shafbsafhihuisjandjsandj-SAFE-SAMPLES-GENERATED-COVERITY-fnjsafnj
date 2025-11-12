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
    char *end = line + strlen(line);
    
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
    if (msg_len >= MAX_LINE_LEN - 30) return 0;
    memcpy(entry->message, ptr, msg_len);
    entry->message[msg_len] = '\0';
    
    return 1;
}

int is_valid_level(const char *level) {
    const char *valid[] = {"INFO", "WARN", "ERROR", "DEBUG"};
    for (int i = 0; i < 4; i++) {
        if (strcmp(level, valid[i]) == 0) return 1;
    }
    return 0;
}

int main(void) {
    struct LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    char line[MAX_LINE_LEN];
    
    printf("Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL message)\\n");
    printf("Enter empty line to finish.\\n");
    
    while (entry_count < MAX_ENTRIES) {
        if (fgets(line, sizeof(line), stdin) == NULL) break;
        
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\\n') {
            line[len - 1] = '\\0';
            len--;
        }
        
        if (len == 0) break;
        
        if (len >= MAX_LINE_LEN - 1) {
            printf("Line too long, skipping.\\n");
            continue;
        }
        
        struct LogEntry *current = entries + entry_count;
        if (!parse_log_line(line, current)) {
            printf("Invalid log format, skipping.\\n");
            continue;
        }
        
        if (!is_valid_level(current->level)) {
            printf("Invalid log level, skipping.\\n");
            continue;
        }
        
        entry_count++;
    }
    
    printf("\\nLog Analysis Results:\\n");
    printf("Total entries: %d\\n", entry_count);
    
    int level_counts[4] = {0};
    const char *levels[] = {"INFO", "WARN", "ERROR", "DEBUG"};
    
    for (int i = 0; i < entry_count; i++) {
        struct LogEntry *entry = entries + i;
        for (int j = 0; j < 4; j++) {
            if (strcmp(entry->level, levels[j]) == 0) {
                level_counts[j]++;
                break;
            }
        }
    }
    
    printf("\\nBreakdown by level:\\n");
    for (int i = 0; i < 4; i++) {
        printf("%s: %d\\n", levels[i], level_counts[i]);
    }
    
    if (entry_count > 0) {
        printf("\\nFirst 5 entries:\\n");
        int show_count = entry_count < 5 ? entry_count : 5;
        for (int i = 0; i < show_count; i++) {
            struct LogEntry *entry = entries + i;
            printf("[%s] %s: %s\\n", entry->timestamp, entry->level, entry->message);
        }
    }
    
    return 0;
}