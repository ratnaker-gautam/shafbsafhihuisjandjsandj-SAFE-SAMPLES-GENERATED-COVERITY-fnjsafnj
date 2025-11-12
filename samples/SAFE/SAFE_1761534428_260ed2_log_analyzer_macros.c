//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_LINE_LEN 1024
#define MAX_ENTRIES 1000
#define TIMESTAMP_LEN 20
#define LEVEL_LEN 16

typedef struct {
    char timestamp[TIMESTAMP_LEN];
    char level[LEVEL_LEN];
    char message[MAX_LINE_LEN - TIMESTAMP_LEN - LEVEL_LEN - 3];
} LogEntry;

#define VALID_LEVEL(level) (strcmp(level, "INFO") == 0 || strcmp(level, "WARNING") == 0 || strcmp(level, "ERROR") == 0)

int parse_timestamp(const char* str, struct tm* tm) {
    return sscanf(str, "%4d-%2d-%2d %2d:%2d:%2d",
                  &tm->tm_year, &tm->tm_mon, &tm->tm_mday,
                  &tm->tm_hour, &tm->tm_min, &tm->tm_sec) == 6;
}

int is_valid_log_line(const char* line) {
    if (!line || strlen(line) < TIMESTAMP_LEN + LEVEL_LEN + 3) return 0;
    
    struct tm tm = {0};
    if (!parse_timestamp(line, &tm)) return 0;
    
    if (line[TIMESTAMP_LEN] != ' ') return 0;
    
    const char* level_start = line + TIMESTAMP_LEN + 1;
    char level[LEVEL_LEN] = {0};
    size_t level_len = 0;
    
    while (level_len < LEVEL_LEN - 1 && level_start[level_len] && 
           level_start[level_len] != ' ' && isalpha(level_start[level_len])) {
        level[level_len] = level_start[level_len];
        level_len++;
    }
    
    if (level_len == 0 || !VALID_LEVEL(level)) return 0;
    
    const char* message_start = level_start + level_len;
    if (*message_start != ' ') return 0;
    
    message_start++;
    if (*message_start == '\0') return 0;
    
    return 1;
}

int parse_log_line(const char* line, LogEntry* entry) {
    if (!is_valid_log_line(line)) return 0;
    
    strncpy(entry->timestamp, line, TIMESTAMP_LEN - 1);
    entry->timestamp[TIMESTAMP_LEN - 1] = '\0';
    
    const char* level_start = line + TIMESTAMP_LEN + 1;
    size_t level_len = 0;
    while (level_len < LEVEL_LEN - 1 && level_start[level_len] && 
           level_start[level_len] != ' ') {
        entry->level[level_len] = level_start[level_len];
        level_len++;
    }
    entry->level[level_len] = '\0';
    
    const char* message_start = level_start + level_len + 1;
    strncpy(entry->message, message_start, sizeof(entry->message) - 1);
    entry->message[sizeof(entry->message) - 1] = '\0';
    
    return 1;
}

int main(void) {
    LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    int info_count = 0, warning_count = 0, error_count = 0;
    
    char line[MAX_LINE_LEN];
    
    printf("Enter log entries (empty line to finish):\n");
    
    while (entry_count < MAX_ENTRIES && fgets(line, sizeof(line), stdin)) {
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }
        
        if (line[0] == '\0') break;
        
        if (!is_valid_log_line(line)) {
            printf("Invalid log format: %s\n", line);
            continue;
        }
        
        if (parse_log_line(line, &entries[entry_count])) {
            if (strcmp(entries[entry_count].level, "INFO") == 0) info_count++;
            else if (strcmp(entries[entry_count].level, "WARNING") == 0) warning_count++;
            else if (strcmp(entries[entry_count].level, "ERROR") == 0) error_count++;
            
            entry_count++;
        }
    }
    
    printf("\nLog Analysis Summary:\n");
    printf("Total entries: %d\n", entry_count);
    printf("INFO: %d\n", info_count);
    printf("WARNING: %d\n", warning_count);
    printf("ERROR: %d\n", error_count);
    
    if (entry_count > 0) {
        printf("\nRecent entries:\n");
        int start = (entry_count > 5) ? entry_count - 5 : 0;
        for (int i = start; i < entry_count; i++) {
            printf("%s [%s] %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
        }
    }
    
    return 0;
}