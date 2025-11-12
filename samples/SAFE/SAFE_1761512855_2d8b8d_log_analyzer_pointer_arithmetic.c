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
    int line_count = 0;
    
    while (fgets(line, sizeof(line), stdin) != NULL) {
        line_count++;
        if (line_count > 10000) {
            fprintf(stderr, "Too many input lines\n");
            return 1;
        }
        
        if (entry_count >= MAX_ENTRIES) {
            fprintf(stderr, "Too many log entries\n");
            return 1;
        }
        
        struct LogEntry *current = entries + entry_count;
        if (parse_log_line(line, current)) {
            entry_count++;
        }
    }
    
    if (ferror(stdin)) {
        fprintf(stderr, "Input error\n");
        return 1;
    }
    
    int error_count = 0;
    int warning_count = 0;
    int info_count = 0;
    
    struct LogEntry *entry_ptr = entries;
    for (int i = 0; i < entry_count; i++, entry_ptr++) {
        if (strcmp(entry_ptr->level, "ERROR") == 0) {
            error_count++;
        } else if (strcmp(entry_ptr->level, "WARNING") == 0) {
            warning_count++;
        } else if (strcmp(entry_ptr->level, "INFO") == 0) {
            info_count++;
        }
    }
    
    printf("Log Analysis Summary:\n");
    printf("Total entries: %d\n", entry_count);
    printf("ERROR entries: %d\n", error_count);
    printf("WARNING entries: %d\n", warning_count);
    printf("INFO entries: %d\n", info_count);
    
    if (error_count > 0) {
        printf("\nERROR entries:\n");
        entry_ptr = entries;
        for (int i = 0; i < entry_count; i++, entry_ptr++) {
            if (strcmp(entry_ptr->level, "ERROR") == 0) {
                printf("%s %s %s\n", entry_ptr->timestamp, entry_ptr->level, entry_ptr->message);
            }
        }
    }
    
    return 0;
}