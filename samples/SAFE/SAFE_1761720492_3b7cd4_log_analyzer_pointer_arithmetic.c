//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 1000

struct LogEntry {
    char timestamp[32];
    char level[16];
    char message[MAX_LINE_LENGTH];
};

int parse_log_line(char *line, struct LogEntry *entry) {
    if (line == NULL || entry == NULL) return 0;
    
    char *ptr = line;
    char *end = ptr + strlen(line);
    
    while (ptr < end && isspace(*ptr)) ptr++;
    if (ptr >= end) return 0;
    
    char *timestamp_start = ptr;
    while (ptr < end && !isspace(*ptr)) ptr++;
    if (ptr >= end) return 0;
    size_t timestamp_len = ptr - timestamp_start;
    if (timestamp_len >= sizeof(entry->timestamp)) return 0;
    strncpy(entry->timestamp, timestamp_start, timestamp_len);
    entry->timestamp[timestamp_len] = '\0';
    
    while (ptr < end && isspace(*ptr)) ptr++;
    if (ptr >= end) return 0;
    
    char *level_start = ptr;
    while (ptr < end && !isspace(*ptr)) ptr++;
    if (ptr >= end) return 0;
    size_t level_len = ptr - level_start;
    if (level_len >= sizeof(entry->level)) return 0;
    strncpy(entry->level, level_start, level_len);
    entry->level[level_len] = '\0';
    
    while (ptr < end && isspace(*ptr)) ptr++;
    if (ptr >= end) return 0;
    
    char *message_start = ptr;
    while (ptr < end && *ptr != '\n' && *ptr != '\r') ptr++;
    size_t message_len = ptr - message_start;
    if (message_len >= sizeof(entry->message)) return 0;
    strncpy(entry->message, message_start, message_len);
    entry->message[message_len] = '\0';
    
    return 1;
}

int main(void) {
    struct LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    char line[MAX_LINE_LENGTH];
    
    while (fgets(line, sizeof(line), stdin) != NULL && entry_count < MAX_ENTRIES) {
        if (strlen(line) >= sizeof(line) - 1) {
            continue;
        }
        
        struct LogEntry *current = entries + entry_count;
        if (parse_log_line(line, current)) {
            entry_count++;
        }
    }
    
    if (ferror(stdin)) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    int error_count = 0;
    int warning_count = 0;
    int info_count = 0;
    
    struct LogEntry *entry_ptr = entries;
    struct LogEntry *end_ptr = entries + entry_count;
    
    while (entry_ptr < end_ptr) {
        if (strcmp(entry_ptr->level, "ERROR") == 0) {
            error_count++;
        } else if (strcmp(entry_ptr->level, "WARNING") == 0) {
            warning_count++;
        } else if (strcmp(entry_ptr->level, "INFO") == 0) {
            info_count++;
        }
        entry_ptr++;
    }
    
    printf("Log Analysis Summary:\n");
    printf("Total entries: %d\n", entry_count);
    printf("ERROR entries: %d\n", error_count);
    printf("WARNING entries: %d\n", warning_count);
    printf("INFO entries: %d\n", info_count);
    
    if (entry_count > 0) {
        printf("\nFirst 5 entries:\n");
        struct LogEntry *display_ptr = entries;
        int display_count = entry_count > 5 ? 5 : entry_count;
        
        for (int i = 0; i < display_count; i++) {
            printf("[%s] %s: %s\n", display_ptr->timestamp, display_ptr->level, display_ptr->message);
            display_ptr++;
        }
    }
    
    return 0;
}