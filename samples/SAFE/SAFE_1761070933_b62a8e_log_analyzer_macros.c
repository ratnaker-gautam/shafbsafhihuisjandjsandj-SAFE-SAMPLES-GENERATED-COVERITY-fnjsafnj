//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: macros ; Variation: log_analyzer
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
    
    memcpy(entry->timestamp, line, TIMESTAMP_LEN - 1);
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
    if (msg_len >= sizeof(entry->message)) return 0;
    
    strncpy(entry->message, msg_start, sizeof(entry->message) - 1);
    entry->message[sizeof(entry->message) - 1] = '\0';
    
    return 1;
}

int main(void) {
    LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    int level_counts[LOG_LEVELS] = {0};
    char line[MAX_LINE_LEN];
    
    printf("Enter log entries (format: YYYY-MM-DD HH:MM:SS [LEVEL] message):\n");
    printf("Enter 'END' on a separate line to finish input.\n");
    
    while (entry_count < MAX_ENTRIES) {
        if (!fgets(line, sizeof(line), stdin)) break;
        
        line[strcspn(line, "\n")] = '\0';
        
        if (strcmp(line, "END") == 0) break;
        
        if (strlen(line) == 0) continue;
        
        LogEntry entry;
        if (parse_log_line(line, &entry)) {
            entries[entry_count] = entry;
            level_counts[LEVEL_TO_INDEX(entry.level)]++;
            entry_count++;
        } else {
            printf("Invalid log format: %s\n", line);
        }
    }
    
    if (entry_count == 0) {
        printf("No valid log entries found.\n");
        return 0;
    }
    
    printf("\nLog Analysis Summary:\n");
    printf("Total entries: %d\n", entry_count);
    printf("Error: %d\n", level_counts[LOG_LEVEL_ERROR]);
    printf("Warning: %d\n", level_counts[LOG_LEVEL_WARN]);
    printf("Info: %d\n", level_counts[LOG_LEVEL_INFO]);
    printf("Debug: %d\n", level_counts[LOG_LEVEL_DEBUG]);
    
    printf("\nRecent entries (last 5):\n");
    int start = (entry_count > 5) ? entry_count - 5 : 0;
    for (int i = start; i < entry_count; i++) {
        printf("%s [%c] %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
    }
    
    return 0;
}