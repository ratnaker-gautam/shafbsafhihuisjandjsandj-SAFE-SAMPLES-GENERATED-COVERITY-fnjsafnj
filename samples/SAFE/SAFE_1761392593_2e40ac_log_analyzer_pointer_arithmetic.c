//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LEN 256
#define MAX_ENTRIES 1000

struct LogEntry {
    char timestamp[20];
    char level[10];
    char message[MAX_LINE_LEN - 30];
};

int parse_log_line(char *line, struct LogEntry *entry) {
    if (line == NULL || entry == NULL) return 0;
    
    char *ptr = line;
    char *end = ptr + strlen(line);
    
    while (ptr < end && isspace(*ptr)) ptr++;
    if (ptr >= end) return 0;
    
    char *ts_start = ptr;
    while (ptr < end && *ptr != ' ') ptr++;
    if (ptr - ts_start >= 20) return 0;
    strncpy(entry->timestamp, ts_start, ptr - ts_start);
    entry->timestamp[ptr - ts_start] = '\0';
    
    while (ptr < end && isspace(*ptr)) ptr++;
    if (ptr >= end) return 0;
    
    char *level_start = ptr;
    while (ptr < end && *ptr != ' ') ptr++;
    if (ptr - level_start >= 10) return 0;
    strncpy(entry->level, level_start, ptr - level_start);
    entry->level[ptr - level_start] = '\0';
    
    while (ptr < end && isspace(*ptr)) ptr++;
    if (ptr >= end) return 0;
    
    char *msg_start = ptr;
    size_t msg_len = end - msg_start;
    if (msg_len >= MAX_LINE_LEN - 30) msg_len = MAX_LINE_LEN - 31;
    strncpy(entry->message, msg_start, msg_len);
    entry->message[msg_len] = '\0';
    
    return 1;
}

int main(void) {
    struct LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    char line[MAX_LINE_LEN];
    
    printf("Enter log entries (empty line to finish):\n");
    
    while (entry_count < MAX_ENTRIES) {
        if (fgets(line, sizeof(line), stdin) == NULL) break;
        
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') line[len - 1] = '\0';
        
        if (line[0] == '\0') break;
        
        if (parse_log_line(line, entries + entry_count)) {
            entry_count++;
        } else {
            printf("Invalid log format. Use: TIMESTAMP LEVEL MESSAGE\n");
        }
    }
    
    printf("\nLog Analysis Results:\n");
    printf("====================\n");
    
    int error_count = 0;
    int warning_count = 0;
    int info_count = 0;
    
    for (int i = 0; i < entry_count; i++) {
        struct LogEntry *entry = entries + i;
        
        if (strcmp(entry->level, "ERROR") == 0) {
            error_count++;
        } else if (strcmp(entry->level, "WARNING") == 0) {
            warning_count++;
        } else if (strcmp(entry->level, "INFO") == 0) {
            info_count++;
        }
    }
    
    printf("Total entries: %d\n", entry_count);
    printf("ERROR entries: %d\n", error_count);
    printf("WARNING entries: %d\n", warning_count);
    printf("INFO entries: %d\n", info_count);
    
    if (error_count > 0) {
        printf("\nERROR entries:\n");
        for (int i = 0; i < entry_count; i++) {
            struct LogEntry *entry = entries + i;
            if (strcmp(entry->level, "ERROR") == 0) {
                printf("%s: %s\n", entry->timestamp, entry->message);
            }
        }
    }
    
    return 0;
}