//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_LINES 1000

struct LogEntry {
    char timestamp[32];
    char level[16];
    char message[MAX_LINE_LENGTH - 48];
};

int parse_log_line(char *line, struct LogEntry *entry) {
    if (line == NULL || entry == NULL) return 0;
    
    char *ptr = line;
    char *end = ptr + strlen(line);
    
    while (ptr < end && isspace(*ptr)) ptr++;
    
    char *timestamp_start = ptr;
    while (ptr < end && *ptr != ' ' && *ptr != '\t') ptr++;
    if (ptr - timestamp_start >= 32) return 0;
    strncpy(entry->timestamp, timestamp_start, ptr - timestamp_start);
    entry->timestamp[ptr - timestamp_start] = '\0';
    
    while (ptr < end && isspace(*ptr)) ptr++;
    
    char *level_start = ptr;
    while (ptr < end && *ptr != ' ' && *ptr != '\t') ptr++;
    if (ptr - level_start >= 16) return 0;
    strncpy(entry->level, level_start, ptr - level_start);
    entry->level[ptr - level_start] = '\0';
    
    while (ptr < end && isspace(*ptr)) ptr++;
    
    char *message_start = ptr;
    size_t message_len = end - message_start;
    if (message_len >= MAX_LINE_LENGTH - 48) return 0;
    strncpy(entry->message, message_start, message_len);
    entry->message[message_len] = '\0';
    
    return 1;
}

int main(void) {
    struct LogEntry logs[MAX_LINES];
    int line_count = 0;
    char buffer[MAX_LINE_LENGTH];
    
    int error_count = 0;
    int warning_count = 0;
    int info_count = 0;
    
    while (line_count < MAX_LINES && fgets(buffer, MAX_LINE_LENGTH, stdin) != NULL) {
        if (strlen(buffer) == 0) continue;
        
        buffer[strcspn(buffer, "\n")] = '\0';
        
        struct LogEntry *current = logs + line_count;
        
        if (parse_log_line(buffer, current)) {
            char *level_ptr = current->level;
            
            if (strcmp(level_ptr, "ERROR") == 0) {
                error_count++;
            } else if (strcmp(level_ptr, "WARNING") == 0) {
                warning_count++;
            } else if (strcmp(level_ptr, "INFO") == 0) {
                info_count++;
            }
            
            line_count++;
        }
    }
    
    if (ferror(stdin)) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    printf("Log Analysis Summary:\n");
    printf("Total lines processed: %d\n", line_count);
    printf("ERROR entries: %d\n", error_count);
    printf("WARNING entries: %d\n", warning_count);
    printf("INFO entries: %d\n", info_count);
    
    if (line_count > 0) {
        printf("\nFirst 5 log entries:\n");
        struct LogEntry *ptr = logs;
        struct LogEntry *end = logs + (line_count < 5 ? line_count : 5);
        
        while (ptr < end) {
            printf("[%s] %s: %s\n", ptr->timestamp, ptr->level, ptr->message);
            ptr++;
        }
    }
    
    return 0;
}