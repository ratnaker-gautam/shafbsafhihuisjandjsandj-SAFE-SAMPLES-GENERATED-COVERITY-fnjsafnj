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
    
    if (line[TIMESTAMP_LEN-1] != ' ' || line[TIMESTAMP_LEN] != '[') return 0;
    if (!IS_VALID_LEVEL(line[TIMESTAMP_LEN+1])) return 0;
    if (line[TIMESTAMP_LEN+2] != ']' || line[TIMESTAMP_LEN+3] != ' ') return 0;
    
    memcpy(entry->timestamp, line, TIMESTAMP_LEN-1);
    entry->timestamp[TIMESTAMP_LEN-1] = '\0';
    
    for (int i = 0; i < TIMESTAMP_LEN-1; i++) {
        if (!isdigit(entry->timestamp[i]) && entry->timestamp[i] != '-' && 
            entry->timestamp[i] != ':' && entry->timestamp[i] != ' ') return 0;
    }
    
    entry->level = line[TIMESTAMP_LEN+1];
    
    size_t msg_start = TIMESTAMP_LEN + 4;
    size_t msg_len = len - msg_start;
    if (msg_len <= 0 || msg_len >= sizeof(entry->message)) return 0;
    
    memcpy(entry->message, line + msg_start, msg_len);
    entry->message[msg_len] = '\0';
    
    return 1;
}

int main(void) {
    LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    int level_counts[LOG_LEVELS] = {0};
    char line[MAX_LINE_LEN];
    
    printf("Enter log entries (format: YYYY-MM-DD HH:MM:SS [LEVEL] message)\\n");
    printf("Enter empty line to finish.\\n");
    
    while (entry_count < MAX_ENTRIES) {
        if (!fgets(line, sizeof(line), stdin)) break;
        
        size_t len = strlen(line);
        if (len > 0 && line[len-1] == '\\n') {
            line[len-1] = '\\0';
            len--;
        }
        
        if (len == 0) break;
        
        if (len >= MAX_LINE_LEN) {
            printf("Line too long, skipping.\\n");
            continue;
        }
        
        LogEntry entry;
        if (parse_log_line(line, &entry)) {
            entries[entry_count] = entry;
            level_counts[LEVEL_TO_INDEX(entry.level)]++;
            entry_count++;
        } else {
            printf("Invalid log format, skipping.\\n");
        }
    }
    
    if (entry_count == 0) {
        printf("No valid log entries found.\\n");
        return 0;
    }
    
    printf("\\nLog Analysis Summary:\\n");
    printf("Total entries: %d\\n", entry_count);
    printf("Error: %d\\n", level_counts[LOG_LEVEL_ERROR]);
    printf("Warning: %d\\n", level_counts[LOG_LEVEL_WARN]);
    printf("Info: %d\\n", level_counts[LOG_LEVEL_INFO]);
    printf("Debug: %d\\n", level_counts[LOG_LEVEL_DEBUG]);
    
    printf("\\nLast 5 entries:\\n");
    int start = (entry_count > 5) ? entry_count - 5 : 0;
    for (int i = start; i < entry_count; i++) {
        printf("%s [%c] %s\\n", entries[i].timestamp, entries[i].level, entries[i].message);
    }
    
    int max_level = 0;
    for (int i = 1; i < LOG_LEVELS; i++) {
        if (level_counts[i] > level_counts[max_level]) {
            max_level = i;
        }
    }
    
    const char* level_names[] = {"Error", "Warning", "Info", "Debug"};
    printf("\\nMost frequent level: %s (%d occurrences)\\n", 
           level_names[max_level], level_counts[max_level]);
    
    return 0;
}