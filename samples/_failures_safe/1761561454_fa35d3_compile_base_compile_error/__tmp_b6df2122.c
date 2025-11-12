//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_LINE_LEN 256
#define MAX_ENTRIES 1000
#define TIMESTAMP_LEN 20
#define LEVEL_LEN 10

typedef struct {
    char timestamp[TIMESTAMP_LEN];
    char level[LEVEL_LEN];
    char message[MAX_LINE_LEN - TIMESTAMP_LEN - LEVEL_LEN - 3];
} LogEntry;

#define VALID_LEVEL(level) (strcmp(level, "INFO") == 0 || strcmp(level, "WARN") == 0 || strcmp(level, "ERROR") == 0)

#define IS_VALID_TIMESTAMP(ts) (strlen(ts) == 19 && ts[4] == '-' && ts[7] == '-' && ts[10] == ' ' && ts[13] == ':' && ts[16] == ':')

int parse_log_entry(const char* line, LogEntry* entry) {
    if (line == NULL || entry == NULL) return 0;
    
    char temp_line[MAX_LINE_LEN];
    if (strlen(line) >= MAX_LINE_LEN - 1) return 0;
    strncpy(temp_line, line, MAX_LINE_LEN - 1);
    temp_line[MAX_LINE_LEN - 1] = '\0';
    
    if (strlen(temp_line) < TIMESTAMP_LEN + LEVEL_LEN + 3) return 0;
    
    strncpy(entry->timestamp, temp_line, TIMESTAMP_LEN - 1);
    entry->timestamp[TIMESTAMP_LEN - 1] = '\0';
    
    if (!IS_VALID_TIMESTAMP(entry->timestamp)) return 0;
    
    const char* level_start = temp_line + TIMESTAMP_LEN + 1;
    if (level_start - temp_line >= (int)strlen(temp_line)) return 0;
    
    char* bracket = strchr(level_start, ']');
    if (bracket == NULL) return 0;
    
    size_t level_len = bracket - level_start;
    if (level_len >= LEVEL_LEN - 1) return 0;
    
    strncpy(entry->level, level_start, level_len);
    entry->level[level_len] = '\0';
    
    if (!VALID_LEVEL(entry->level)) return 0;
    
    const char* message_start = bracket + 2;
    if (message_start - temp_line >= (int)strlen(temp_line)) return 0;
    
    size_t message_len = strlen(message_start);
    if (message_len >= sizeof(entry->message) - 1) return 0;
    
    strncpy(entry->message, message_start, sizeof(entry->message) - 1);
    entry->message[sizeof(entry->message) - 1] = '\0';
    
    return 1;
}

#define COUNT_LEVEL(entries, count, target_level, counter_var) \
    do { \
        for (size_t i = 0; i < count; i++) { \
            if (strcmp(entries[i].level, target_level) == 0) { \
                counter_var++; \
            } \
        } \
    } while(0)

void analyze_logs(LogEntry entries[], size_t count) {
    if (entries == NULL || count == 0 || count > MAX_ENTRIES) return;
    
    size_t info_count = 0, warn_count = 0, error_count = 0;
    
    COUNT_LEVEL(entries, count, "INFO", info_count);
    COUNT_LEVEL(entries, count, "WARN", warn_count);
    COUNT_LEVEL(entries, count, "ERROR", error_count);
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %zu\n", count);
    printf("INFO: %zu\n", info_count);
    printf("WARN: %zu\n", warn_count);
    printf("ERROR: %zu\n", error_count);
    
    if (count > 0) {
        printf("\nFirst timestamp: %s\n", entries[0].timestamp);
        printf("Last timestamp: %s\n", entries[count - 1].timestamp);
    }
}

int main(void) {
    LogEntry entries[MAX_ENTRIES];
    size_t entry_count = 0;
    
    printf("Enter log entries (format: YYYY-MM-DD HH:MM:SS [LEVEL] message)\n");
    printf("Enter 'END' on a separate line to finish input\n");
    
    char line[MAX_LINE_LEN];
    
    while (entry_count < MAX_ENTRIES) {
        if (fgets(line, sizeof(line), stdin) == NULL) {
            break;
        }
        
        line[strcspn(line, "\n")] = '\0';
        
        if (strlen(line) == 0) {
            continue;
        }
        
        if (strcmp(line, "END") == 0) {
            break;
        }
        
        LogEntry entry;
        if (parse_log_entry(line, &entry)) {
            entries[entry_count] = entry;
            entry_count++;
            printf("Added: %s [%s] %s\n", entry.timestamp, entry.level, entry.message);
        } else {
            printf("Invalid log format: %s\n", line);
        }
    }
    
    if (entry_count > 0) {
        analyze_logs(entries, entry_count);
    } else {