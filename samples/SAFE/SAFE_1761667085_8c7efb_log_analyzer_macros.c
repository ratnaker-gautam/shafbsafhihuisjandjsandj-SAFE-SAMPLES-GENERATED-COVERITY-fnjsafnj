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
    char message[MAX_LINE_LEN - TIMESTAMP_LEN - 3];
} LogEntry;

int parse_timestamp(const char* str, char* timestamp) {
    if (strlen(str) < TIMESTAMP_LEN - 1) return 0;
    for (int i = 0; i < TIMESTAMP_LEN - 1; i++) {
        if (i == 4 || i == 7) {
            if (str[i] != '-') return 0;
        } else if (i == 10) {
            if (str[i] != ' ') return 0;
        } else if (i == 13 || i == 16) {
            if (str[i] != ':') return 0;
        } else {
            if (!isdigit(str[i])) return 0;
        }
    }
    strncpy(timestamp, str, TIMESTAMP_LEN - 1);
    timestamp[TIMESTAMP_LEN - 1] = '\0';
    return 1;
}

int parse_log_entry(const char* line, LogEntry* entry) {
    if (strlen(line) < TIMESTAMP_LEN + 3) return 0;
    
    if (!parse_timestamp(line, entry->timestamp)) return 0;
    
    if (line[TIMESTAMP_LEN] != ' ' || line[TIMESTAMP_LEN + 1] != '[') return 0;
    
    char level_char = line[TIMESTAMP_LEN + 2];
    if (!IS_VALID_LEVEL(level_char)) return 0;
    entry->level = level_char;
    
    if (line[TIMESTAMP_LEN + 3] != ']' || line[TIMESTAMP_LEN + 4] != ' ') return 0;
    
    const char* message_start = line + TIMESTAMP_LEN + 5;
    size_t message_len = strlen(message_start);
    if (message_len >= sizeof(entry->message)) return 0;
    
    strncpy(entry->message, message_start, sizeof(entry->message) - 1);
    entry->message[sizeof(entry->message) - 1] = '\0';
    
    return 1;
}

int main(void) {
    LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    int level_counts[LOG_LEVELS] = {0};
    char line[MAX_LINE_LEN];
    
    printf("Enter log entries (empty line to finish):\n");
    printf("Format: YYYY-MM-DD HH:MM:SS [L] message\n");
    printf("Where L is E(rror), W(arning), I(nfo), or D(ebug)\n\n");
    
    while (entry_count < MAX_ENTRIES) {
        if (fgets(line, sizeof(line), stdin) == NULL) break;
        
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }
        
        if (strlen(line) == 0) break;
        
        if (parse_log_entry(line, &entries[entry_count])) {
            int level_idx = LEVEL_TO_INDEX(entries[entry_count].level);
            level_counts[level_idx]++;
            entry_count++;
        } else {
            printf("Invalid log entry format. Skipping: %s\n", line);
        }
    }
    
    printf("\nLog Analysis Summary:\n");
    printf("====================\n");
    printf("Total entries: %d\n", entry_count);
    printf("Error entries: %d\n", level_counts[LOG_LEVEL_ERROR]);
    printf("Warning entries: %d\n", level_counts[LOG_LEVEL_WARN]);
    printf("Info entries: %d\n", level_counts[LOG_LEVEL_INFO]);
    printf("Debug entries: %d\n", level_counts[LOG_LEVEL_DEBUG]);
    
    if (entry_count > 0) {
        printf("\nRecent entries:\n");
        int start = entry_count > 5 ? entry_count - 5 : 0;
        for (int i = start; i < entry_count; i++) {
            printf("%s [%c] %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
        }
    }
    
    return 0;
}