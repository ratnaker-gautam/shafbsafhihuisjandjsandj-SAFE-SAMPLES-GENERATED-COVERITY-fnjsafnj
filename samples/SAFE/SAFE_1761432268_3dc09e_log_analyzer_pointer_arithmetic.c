//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 256
#define MAX_LINES 1000

struct LogEntry {
    char timestamp[20];
    char level[10];
    char message[MAX_LINE_LENGTH - 30];
};

int parse_log_line(char *line, struct LogEntry *entry) {
    if (line == NULL || entry == NULL) return 0;
    
    char *ptr = line;
    char *end = ptr + strlen(line);
    
    while (ptr < end && isspace(*ptr)) ptr++;
    if (ptr >= end) return 0;
    
    char *timestamp_start = ptr;
    while (ptr < end && !isspace(*ptr) && (ptr - timestamp_start) < 19) ptr++;
    if (ptr >= end || (ptr - timestamp_start) >= 19) return 0;
    strncpy(entry->timestamp, timestamp_start, ptr - timestamp_start);
    entry->timestamp[ptr - timestamp_start] = '\0';
    
    while (ptr < end && isspace(*ptr)) ptr++;
    if (ptr >= end) return 0;
    
    char *level_start = ptr;
    while (ptr < end && !isspace(*ptr) && (ptr - level_start) < 9) ptr++;
    if (ptr >= end || (ptr - level_start) >= 9) return 0;
    strncpy(entry->level, level_start, ptr - level_start);
    entry->level[ptr - level_start] = '\0';
    
    while (ptr < end && isspace(*ptr)) ptr++;
    if (ptr >= end) return 0;
    
    char *message_start = ptr;
    size_t message_len = end - message_start;
    if (message_len >= MAX_LINE_LENGTH - 30) message_len = MAX_LINE_LENGTH - 31;
    strncpy(entry->message, message_start, message_len);
    entry->message[message_len] = '\0';
    
    return 1;
}

int main(void) {
    struct LogEntry logs[MAX_LINES];
    int log_count = 0;
    char line[MAX_LINE_LENGTH];
    
    printf("Enter log entries (empty line to finish):\n");
    
    while (log_count < MAX_LINES) {
        if (fgets(line, sizeof(line), stdin) == NULL) break;
        
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') line[len - 1] = '\0';
        
        if (strlen(line) == 0) break;
        
        if (parse_log_line(line, logs + log_count)) {
            log_count++;
        } else {
            printf("Invalid log format: %s\n", line);
        }
    }
    
    if (log_count == 0) {
        printf("No valid log entries found.\n");
        return 0;
    }
    
    int error_count = 0;
    int warning_count = 0;
    int info_count = 0;
    
    for (int i = 0; i < log_count; i++) {
        struct LogEntry *entry = logs + i;
        
        if (strcmp(entry->level, "ERROR") == 0) {
            error_count++;
        } else if (strcmp(entry->level, "WARNING") == 0) {
            warning_count++;
        } else if (strcmp(entry->level, "INFO") == 0) {
            info_count++;
        }
    }
    
    printf("\nLog Analysis Summary:\n");
    printf("Total entries: %d\n", log_count);
    printf("ERROR entries: %d\n", error_count);
    printf("WARNING entries: %d\n", warning_count);
    printf("INFO entries: %d\n", info_count);
    
    if (error_count > 0) {
        printf("\nERROR entries:\n");
        for (int i = 0; i < log_count; i++) {
            struct LogEntry *entry = logs + i;
            if (strcmp(entry->level, "ERROR") == 0) {
                printf("%s %s %s\n", entry->timestamp, entry->level, entry->message);
            }
        }
    }
    
    return 0;
}