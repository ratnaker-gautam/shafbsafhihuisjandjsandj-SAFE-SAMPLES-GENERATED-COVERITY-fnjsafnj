//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

#define MAX_LINE_LEN 1024
#define MAX_ENTRIES 1000

typedef struct {
    char timestamp[32];
    char level[16];
    char message[256];
} LogEntry;

int parse_log_line(char *line, LogEntry *entry) {
    if (line == NULL || entry == NULL) return 0;
    
    char *ptr = line;
    while (*ptr && isspace(*ptr)) ptr++;
    if (*ptr == '\0') return 0;
    
    char *timestamp_end = strchr(ptr, ' ');
    if (timestamp_end == NULL) return 0;
    size_t timestamp_len = timestamp_end - ptr;
    if (timestamp_len >= sizeof(entry->timestamp)) return 0;
    strncpy(entry->timestamp, ptr, timestamp_len);
    entry->timestamp[timestamp_len] = '\0';
    
    ptr = timestamp_end + 1;
    while (*ptr && isspace(*ptr)) ptr++;
    if (*ptr == '\0') return 0;
    
    char *level_end = strchr(ptr, ' ');
    if (level_end == NULL) return 0;
    size_t level_len = level_end - ptr;
    if (level_len >= sizeof(entry->level)) return 0;
    strncpy(entry->level, ptr, level_len);
    entry->level[level_len] = '\0';
    
    ptr = level_end + 1;
    while (*ptr && isspace(*ptr)) ptr++;
    if (*ptr == '\0') return 0;
    
    size_t msg_len = strlen(ptr);
    if (msg_len >= sizeof(entry->message)) return 0;
    strncpy(entry->message, ptr, msg_len);
    entry->message[msg_len] = '\0';
    
    char *newline = strchr(entry->message, '\n');
    if (newline != NULL) *newline = '\0';
    
    return 1;
}

void count_log_levels(LogEntry *entries, int count) {
    if (entries == NULL || count <= 0) return;
    
    int info_count = 0, warn_count = 0, error_count = 0, other_count = 0;
    
    LogEntry *current = entries;
    LogEntry *end = entries + count;
    
    while (current < end) {
        char *level_ptr = current->level;
        if (strcmp(level_ptr, "INFO") == 0) {
            info_count++;
        } else if (strcmp(level_ptr, "WARN") == 0) {
            warn_count++;
        } else if (strcmp(level_ptr, "ERROR") == 0) {
            error_count++;
        } else {
            other_count++;
        }
        current++;
    }
    
    printf("Log Level Summary:\n");
    printf("INFO: %d\n", info_count);
    printf("WARN: %d\n", warn_count);
    printf("ERROR: %d\n", error_count);
    printf("OTHER: %d\n", other_count);
}

int main(void) {
    LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    
    printf("Enter log entries (format: TIMESTAMP LEVEL MESSAGE):\n");
    printf("Enter empty line to finish.\n");
    
    char buffer[MAX_LINE_LEN];
    
    while (entry_count < MAX_ENTRIES) {
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) break;
        
        if (buffer[0] == '\n') break;
        
        if (parse_log_line(buffer, &entries[entry_count])) {
            entry_count++;
        } else {
            printf("Invalid log format. Skipping line.\n");
        }
    }
    
    if (entry_count == 0) {
        printf("No valid log entries found.\n");
        return 1;
    }
    
    printf("\nParsed %d log entries:\n", entry_count);
    
    LogEntry *entry_ptr = entries;
    for (int i = 0; i < entry_count; i++) {
        printf("Entry %d: [%s] %s - %s\n", i + 1, 
               entry_ptr->timestamp, entry_ptr->level, entry_ptr->message);
        entry_ptr++;
    }
    
    printf("\n");
    count_log_levels(entries, entry_count);
    
    return 0;
}