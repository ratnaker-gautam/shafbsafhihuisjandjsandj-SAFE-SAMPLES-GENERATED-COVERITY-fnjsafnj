//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: pointer_arithmetic ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 1000

struct LogEntry {
    char timestamp[32];
    char level[16];
    char message[256];
};

int parse_log_line(char *line, struct LogEntry *entry) {
    if (line == NULL || entry == NULL) return 0;
    
    char *ptr = line;
    char *end = ptr + strlen(line);
    
    while (ptr < end && isspace(*ptr)) ptr++;
    if (ptr >= end) return 0;
    
    char *timestamp_start = ptr;
    while (ptr < end && *ptr != ' ') ptr++;
    if (ptr >= end) return 0;
    size_t timestamp_len = ptr - timestamp_start;
    if (timestamp_len >= sizeof(entry->timestamp)) return 0;
    memcpy(entry->timestamp, timestamp_start, timestamp_len);
    entry->timestamp[timestamp_len] = '\0';
    
    while (ptr < end && isspace(*ptr)) ptr++;
    if (ptr >= end) return 0;
    
    char *level_start = ptr;
    while (ptr < end && *ptr != ' ') ptr++;
    if (ptr >= end) return 0;
    size_t level_len = ptr - level_start;
    if (level_len >= sizeof(entry->level)) return 0;
    memcpy(entry->level, level_start, level_len);
    entry->level[level_len] = '\0';
    
    while (ptr < end && isspace(*ptr)) ptr++;
    if (ptr >= end) return 0;
    
    char *message_start = ptr;
    while (ptr < end && *ptr != '\n' && *ptr != '\r') ptr++;
    size_t message_len = ptr - message_start;
    if (message_len >= sizeof(entry->message)) return 0;
    memcpy(entry->message, message_start, message_len);
    entry->message[message_len] = '\0';
    
    return 1;
}

int main(void) {
    struct LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    int error_count = 0;
    int info_count = 0;
    int warning_count = 0;
    
    char buffer[MAX_LINE_LENGTH];
    
    while (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        if (entry_count >= MAX_ENTRIES) {
            fprintf(stderr, "Maximum entries reached\n");
            break;
        }
        
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        }
        
        if (parse_log_line(buffer, entries + entry_count)) {
            char *level_ptr = entries[entry_count].level;
            if (strcmp(level_ptr, "ERROR") == 0) {
                error_count++;
            } else if (strcmp(level_ptr, "WARNING") == 0) {
                warning_count++;
            } else if (strcmp(level_ptr, "INFO") == 0) {
                info_count++;
            }
            entry_count++;
        }
    }
    
    if (ferror(stdin)) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    printf("Log Analysis Summary:\n");
    printf("Total entries: %d\n", entry_count);
    printf("INFO: %d\n", info_count);
    printf("WARNING: %d\n", warning_count);
    printf("ERROR: %d\n", error_count);
    
    printf("\nRecent entries:\n");
    int display_count = (entry_count > 5) ? 5 : entry_count;
    struct LogEntry *current = entries;
    struct LogEntry *end_ptr = entries + display_count;
    
    while (current < end_ptr) {
        printf("[%s] %s: %s\n", current->timestamp, current->level, current->message);
        current++;
    }
    
    return EXIT_SUCCESS;
}