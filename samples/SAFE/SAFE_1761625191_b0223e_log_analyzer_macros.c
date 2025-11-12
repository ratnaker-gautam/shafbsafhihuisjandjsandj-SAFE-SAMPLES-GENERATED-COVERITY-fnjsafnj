//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LEN 1024
#define MAX_ENTRIES 1000
#define TIMESTAMP_LEN 20
#define LEVEL_LEN 16

typedef struct {
    char timestamp[TIMESTAMP_LEN];
    char level[LEVEL_LEN];
    char message[MAX_LINE_LEN - TIMESTAMP_LEN - LEVEL_LEN];
} LogEntry;

#define IS_VALID_LEVEL(l) (strcmp(l, "INFO") == 0 || strcmp(l, "WARNING") == 0 || strcmp(l, "ERROR") == 0)

int parse_log_entry(const char* line, LogEntry* entry) {
    if (!line || !entry) return 0;
    
    char timestamp[TIMESTAMP_LEN];
    char level[LEVEL_LEN];
    
    int scanned = sscanf(line, "%19s %15s", timestamp, level);
    if (scanned != 2) return 0;
    
    if (!IS_VALID_LEVEL(level)) return 0;
    
    const char* message_start = line;
    for (int i = 0; i < 2; i++) {
        while (*message_start && !isspace(*message_start)) message_start++;
        while (*message_start && isspace(*message_start)) message_start++;
    }
    
    if (strlen(message_start) >= sizeof(entry->message)) return 0;
    
    strncpy(entry->timestamp, timestamp, sizeof(entry->timestamp) - 1);
    entry->timestamp[sizeof(entry->timestamp) - 1] = '\0';
    strncpy(entry->level, level, sizeof(entry->level) - 1);
    entry->level[sizeof(entry->level) - 1] = '\0';
    strncpy(entry->message, message_start, sizeof(entry->message) - 1);
    entry->message[sizeof(entry->message) - 1] = '\0';
    
    return 1;
}

#define COUNT_LEVEL(entry_ptr, l) do { \
    if (strcmp((entry_ptr)->level, l) == 0) { \
        counts[level_index]++; \
        break; \
    } \
    level_index++; \
} while (0)

void analyze_logs(LogEntry* entries, int count) {
    int counts[3] = {0};
    
    for (int i = 0; i < count; i++) {
        int level_index = 0;
        COUNT_LEVEL(&entries[i], "INFO");
        COUNT_LEVEL(&entries[i], "WARNING");
        COUNT_LEVEL(&entries[i], "ERROR");
    }
    
    printf("Log Analysis Results:\n");
    printf("INFO entries: %d\n", counts[0]);
    printf("WARNING entries: %d\n", counts[1]);
    printf("ERROR entries: %d\n", counts[2]);
    
    int total = counts[0] + counts[1] + counts[2];
    if (total > 0) {
        printf("ERROR percentage: %.1f%%\n", (counts[2] * 100.0) / total);
    }
}

int main() {
    LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    
    printf("Enter log entries (format: TIMESTAMP LEVEL MESSAGE)\n");
    printf("Supported levels: INFO, WARNING, ERROR\n");
    printf("Enter 'END' on a separate line to finish input\n");
    
    char line[MAX_LINE_LEN];
    
    while (entry_count < MAX_ENTRIES && fgets(line, sizeof(line), stdin)) {
        line[strcspn(line, "\n")] = '\0';
        
        if (strcmp(line, "END") == 0) {
            break;
        }
        
        if (strlen(line) == 0) {
            continue;
        }
        
        LogEntry entry;
        if (parse_log_entry(line, &entry)) {
            entries[entry_count] = entry;
            entry_count++;
        } else {
            printf("Invalid log entry format: %s\n", line);
        }
    }
    
    if (entry_count == 0) {
        printf("No valid log entries to analyze.\n");
        return 1;
    }
    
    analyze_logs(entries, entry_count);
    
    return 0;
}