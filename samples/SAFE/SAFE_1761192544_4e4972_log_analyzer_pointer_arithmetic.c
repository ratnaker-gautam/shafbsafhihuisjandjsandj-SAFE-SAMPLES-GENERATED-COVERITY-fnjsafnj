//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX_LINE_LEN 1024
#define MAX_ENTRIES 1000

struct LogEntry {
    char timestamp[32];
    char level[16];
    char message[256];
};

int parse_log_line(char *line, struct LogEntry *entry) {
    if (line == NULL || entry == NULL) return 0;
    
    char *ptr = line;
    while (*ptr && isspace(*ptr)) ptr++;
    if (*ptr == '\0') return 0;

    char *timestamp_start = ptr;
    while (*ptr && *ptr != ' ') ptr++;
    if (*ptr == '\0') return 0;
    size_t timestamp_len = ptr - timestamp_start;
    if (timestamp_len >= sizeof(entry->timestamp)) return 0;
    strncpy(entry->timestamp, timestamp_start, timestamp_len);
    entry->timestamp[timestamp_len] = '\0';

    while (*ptr && isspace(*ptr)) ptr++;
    if (*ptr == '\0') return 0;

    char *level_start = ptr;
    while (*ptr && *ptr != ' ') ptr++;
    if (*ptr == '\0') return 0;
    size_t level_len = ptr - level_start;
    if (level_len >= sizeof(entry->level)) return 0;
    strncpy(entry->level, level_start, level_len);
    entry->level[level_len] = '\0';

    while (*ptr && isspace(*ptr)) ptr++;
    if (*ptr == '\0') return 0;

    char *message_start = ptr;
    while (*ptr && *ptr != '\n' && *ptr != '\r') ptr++;
    size_t message_len = ptr - message_start;
    if (message_len >= sizeof(entry->message)) return 0;
    strncpy(entry->message, message_start, message_len);
    entry->message[message_len] = '\0';

    return 1;
}

int main(void) {
    struct LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    char line[MAX_LINE_LEN];
    
    printf("Enter log entries (format: TIMESTAMP LEVEL MESSAGE):\n");
    printf("Enter empty line to finish.\n");
    
    while (entry_count < MAX_ENTRIES) {
        if (fgets(line, sizeof(line), stdin) == NULL) break;
        
        char *ptr = line;
        while (*ptr && isspace(*ptr)) ptr++;
        if (*ptr == '\0' || *ptr == '\n' || *ptr == '\r') break;
        
        if (parse_log_line(line, entries + entry_count)) {
            entry_count++;
        } else {
            printf("Invalid log format. Skipping line.\n");
        }
    }
    
    if (entry_count == 0) {
        printf("No valid log entries found.\n");
        return 0;
    }
    
    int info_count = 0;
    int warning_count = 0;
    int error_count = 0;
    
    struct LogEntry *current = entries;
    struct LogEntry *end = entries + entry_count;
    
    while (current < end) {
        if (strcmp(current->level, "INFO") == 0) {
            info_count++;
        } else if (strcmp(current->level, "WARNING") == 0) {
            warning_count++;
        } else if (strcmp(current->level, "ERROR") == 0) {
            error_count++;
        }
        current++;
    }
    
    printf("\nLog Analysis Summary:\n");
    printf("Total entries: %d\n", entry_count);
    printf("INFO: %d\n", info_count);
    printf("WARNING: %d\n", warning_count);
    printf("ERROR: %d\n", error_count);
    
    printf("\nRecent ERROR entries:\n");
    current = entries;
    int error_shown = 0;
    while (current < end && error_shown < 5) {
        if (strcmp(current->level, "ERROR") == 0) {
            printf("%s: %s\n", current->timestamp, current->message);
            error_shown++;
        }
        current++;
    }
    
    if (error_shown == 0) {
        printf("No ERROR entries found.\n");
    }
    
    return 0;
}