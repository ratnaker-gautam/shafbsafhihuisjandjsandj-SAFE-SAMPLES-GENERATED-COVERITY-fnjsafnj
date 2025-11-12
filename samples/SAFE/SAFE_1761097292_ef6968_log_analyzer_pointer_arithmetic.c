//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: pointer_arithmetic ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LEN 1024
#define MAX_ENTRIES 1000

struct LogEntry {
    char timestamp[32];
    char level[16];
    char message[256];
};

int parse_log_line(char *line, struct LogEntry *entry) {
    if (!line || !entry) return 0;
    
    char *ptr = line;
    while (*ptr && isspace(*ptr)) ptr++;
    if (!*ptr) return 0;
    
    char *timestamp_start = ptr;
    while (*ptr && !isspace(*ptr)) ptr++;
    if (!*ptr) return 0;
    size_t timestamp_len = ptr - timestamp_start;
    if (timestamp_len >= sizeof(entry->timestamp)) return 0;
    memcpy(entry->timestamp, timestamp_start, timestamp_len);
    entry->timestamp[timestamp_len] = '\0';
    
    while (*ptr && isspace(*ptr)) ptr++;
    if (!*ptr) return 0;
    
    char *level_start = ptr;
    while (*ptr && !isspace(*ptr)) ptr++;
    if (!*ptr) return 0;
    size_t level_len = ptr - level_start;
    if (level_len >= sizeof(entry->level)) return 0;
    memcpy(entry->level, level_start, level_len);
    entry->level[level_len] = '\0';
    
    while (*ptr && isspace(*ptr)) ptr++;
    if (!*ptr) return 0;
    
    char *message_start = ptr;
    while (*ptr && *ptr != '\n' && *ptr != '\r') ptr++;
    size_t message_len = ptr - message_start;
    if (message_len >= sizeof(entry->message)) return 0;
    memcpy(entry->message, message_start, message_len);
    entry->message[message_len] = '\0';
    
    return 1;
}

int main(void) {
    struct LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    char buffer[MAX_LINE_LEN];
    
    printf("Enter log entries (timestamp level message). Empty line to finish:\n");
    
    while (entry_count < MAX_ENTRIES) {
        if (!fgets(buffer, sizeof(buffer), stdin)) break;
        
        char *ptr = buffer;
        while (*ptr && isspace(*ptr)) ptr++;
        if (!*ptr || *ptr == '\n' || *ptr == '\r') break;
        
        if (!parse_log_line(buffer, &entries[entry_count])) {
            printf("Invalid log format. Skipping.\n");
            continue;
        }
        
        entry_count++;
    }
    
    printf("\nLog Analysis:\n");
    printf("Total entries: %d\n", entry_count);
    
    int error_count = 0;
    int warning_count = 0;
    int info_count = 0;
    
    for (int i = 0; i < entry_count; i++) {
        char *level_ptr = entries[i].level;
        if (strcmp(level_ptr, "ERROR") == 0) {
            error_count++;
        } else if (strcmp(level_ptr, "WARNING") == 0) {
            warning_count++;
        } else if (strcmp(level_ptr, "INFO") == 0) {
            info_count++;
        }
    }
    
    printf("ERROR entries: %d\n", error_count);
    printf("WARNING entries: %d\n", warning_count);
    printf("INFO entries: %d\n", info_count);
    
    if (error_count > 0) {
        printf("\nERROR entries:\n");
        for (int i = 0; i < entry_count; i++) {
            char *level_ptr = entries[i].level;
            if (strcmp(level_ptr, "ERROR") == 0) {
                printf("%s %s %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
            }
        }
    }
    
    return 0;
}