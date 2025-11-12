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

int parse_log_entry(const char* line, LogEntry* entry) {
    if (!line || !entry) return 0;
    
    char temp[MAX_LINE_LEN];
    if (strlen(line) >= MAX_LINE_LEN - 1) return 0;
    strncpy(temp, line, MAX_LINE_LEN - 1);
    temp[MAX_LINE_LEN - 1] = '\0';
    
    char* timestamp = strtok(temp, " ");
    char* level = strtok(NULL, " ");
    char* message = strtok(NULL, "");
    
    if (!timestamp || !level || !message) return 0;
    
    if (strlen(timestamp) >= TIMESTAMP_LEN - 1) return 0;
    if (strlen(level) >= LEVEL_LEN - 1) return 0;
    if (strlen(message) >= sizeof(entry->message) - 1) return 0;
    
    if (!VALID_LEVEL(level)) return 0;
    
    strncpy(entry->timestamp, timestamp, TIMESTAMP_LEN - 1);
    entry->timestamp[TIMESTAMP_LEN - 1] = '\0';
    strncpy(entry->level, level, LEVEL_LEN - 1);
    entry->level[LEVEL_LEN - 1] = '\0';
    strncpy(entry->message, message, sizeof(entry->message) - 1);
    entry->message[sizeof(entry->message) - 1] = '\0';
    
    return 1;
}

#define PRINT_STATS(level, count, total) printf("%s: %d (%.1f%%)\n", level, count, total > 0 ? (count * 100.0 / total) : 0.0)

int main(void) {
    LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    char line[MAX_LINE_LEN];
    
    printf("Enter log entries (empty line to finish):\n");
    
    while (entry_count < MAX_ENTRIES && fgets(line, sizeof(line), stdin)) {
        if (line[0] == '\n' || line[0] == '\0') break;
        
        line[strcspn(line, "\n")] = '\0';
        
        if (strlen(line) == 0) continue;
        
        LogEntry entry;
        if (parse_log_entry(line, &entry)) {
            entries[entry_count] = entry;
            entry_count++;
        } else {
            printf("Invalid log format: %s\n", line);
        }
    }
    
    if (entry_count == 0) {
        printf("No valid log entries found.\n");
        return 1;
    }
    
    int info_count = 0, warn_count = 0, error_count = 0;
    
    for (int i = 0; i < entry_count; i++) {
        if (strcmp(entries[i].level, "INFO") == 0) info_count++;
        else if (strcmp(entries[i].level, "WARN") == 0) warn_count++;
        else if (strcmp(entries[i].level, "ERROR") == 0) error_count++;
    }
    
    printf("\nLog Analysis Results:\n");
    printf("Total entries: %d\n", entry_count);
    PRINT_STATS("INFO", info_count, entry_count);
    PRINT_STATS("WARN", warn_count, entry_count);
    PRINT_STATS("ERROR", error_count, entry_count);
    
    printf("\nRecent entries:\n");
    int display_count = entry_count > 5 ? 5 : entry_count;
    for (int i = entry_count - display_count; i < entry_count; i++) {
        printf("%s [%s] %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
    }
    
    return 0;
}