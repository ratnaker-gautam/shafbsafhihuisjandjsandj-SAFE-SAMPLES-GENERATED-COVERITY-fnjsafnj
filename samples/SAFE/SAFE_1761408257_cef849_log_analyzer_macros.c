//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_LINE_LEN 1024
#define MAX_ENTRIES 1000
#define TIMESTAMP_LEN 20
#define LOG_LEVELS 4

#define LOG_LEVEL_ERROR 0
#define LOG_LEVEL_WARN 1
#define LOG_LEVEL_INFO 2
#define LOG_LEVEL_DEBUG 3

#define IS_VALID_LEVEL(c) ((c) == 'E' || (c) == 'W' || (c) == 'I' || (c) == 'D')
#define LEVEL_TO_INDEX(c) ((c) == 'E' ? 0 : (c) == 'W' ? 1 : (c) == 'I' ? 2 : 3)

typedef struct {
    char timestamp[TIMESTAMP_LEN];
    char level;
    char message[MAX_LINE_LEN - TIMESTAMP_LEN - 4];
} LogEntry;

int parse_log_line(const char* line, LogEntry* entry) {
    if (!line || !entry) return 0;
    
    size_t len = strlen(line);
    if (len < TIMESTAMP_LEN + 3 || len >= MAX_LINE_LEN) return 0;
    
    if (line[TIMESTAMP_LEN - 1] != ' ' || line[TIMESTAMP_LEN] != '[') return 0;
    
    char level_char = line[TIMESTAMP_LEN + 1];
    if (!IS_VALID_LEVEL(level_char)) return 0;
    
    if (line[TIMESTAMP_LEN + 2] != ']' || line[TIMESTAMP_LEN + 3] != ' ') return 0;
    
    strncpy(entry->timestamp, line, TIMESTAMP_LEN - 1);
    entry->timestamp[TIMESTAMP_LEN - 1] = '\0';
    
    for (int i = 0; i < TIMESTAMP_LEN - 1; i++) {
        if (!isdigit(entry->timestamp[i]) && entry->timestamp[i] != '-' && 
            entry->timestamp[i] != ':' && entry->timestamp[i] != '.') {
            return 0;
        }
    }
    
    entry->level = level_char;
    
    const char* msg_start = line + TIMESTAMP_LEN + 4;
    size_t msg_len = len - (TIMESTAMP_LEN + 4);
    if (msg_len > 0) {
        strncpy(entry->message, msg_start, msg_len);
        entry->message[msg_len] = '\0';
    } else {
        entry->message[0] = '\0';
    }
    
    return 1;
}

int main(void) {
    LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    int level_counts[LOG_LEVELS] = {0};
    char line[MAX_LINE_LEN];
    
    printf("Log Analyzer - Enter log lines (format: YYYY-MM-DD HH:MM:SS [LEVEL] message)\\n");
    printf("Enter 'END' on a separate line to finish input.\\n");
    
    while (entry_count < MAX_ENTRIES) {
        if (!fgets(line, sizeof(line), stdin)) {
            break;
        }
        
        line[strcspn(line, "\\n")] = 0;
        
        if (strcmp(line, "END") == 0) {
            break;
        }
        
        if (strlen(line) == 0) {
            continue;
        }
        
        LogEntry entry;
        if (parse_log_line(line, &entry)) {
            entries[entry_count] = entry;
            int level_idx = LEVEL_TO_INDEX(entry.level);
            level_counts[level_idx]++;
            entry_count++;
        } else {
            printf("Invalid log format: %s\\n", line);
        }
    }
    
    printf("\\n=== Log Analysis Results ===\\n");
    printf("Total entries processed: %d\\n", entry_count);
    printf("Error entries: %d\\n", level_counts[LOG_LEVEL_ERROR]);
    printf("Warning entries: %d\\n", level_counts[LOG_LEVEL_WARN]);
    printf("Info entries: %d\\n", level_counts[LOG_LEVEL_INFO]);
    printf("Debug entries: %d\\n", level_counts[LOG_LEVEL_DEBUG]);
    
    if (entry_count > 0) {
        printf("\\n=== Recent Entries ===\\n");
        int display_count = entry_count > 5 ? 5 : entry_count;
        for (int i = entry_count - display_count; i < entry_count; i++) {
            printf("%s [%c] %s\\n", entries[i].timestamp, entries[i].level, entries[i].message);
        }
    }
    
    return 0;
}