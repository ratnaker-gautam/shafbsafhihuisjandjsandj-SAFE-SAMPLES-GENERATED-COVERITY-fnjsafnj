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
    
    char *timestamp_start = ptr;
    while (ptr < end && !isspace(*ptr)) ptr++;
    if (ptr - timestamp_start >= 20) return 0;
    strncpy(entry->timestamp, timestamp_start, ptr - timestamp_start);
    entry->timestamp[ptr - timestamp_start] = '\0';
    
    while (ptr < end && isspace(*ptr)) ptr++;
    if (ptr >= end) return 0;
    
    char *level_start = ptr;
    while (ptr < end && !isspace(*ptr)) ptr++;
    if (ptr - level_start >= 10) return 0;
    strncpy(entry->level, level_start, ptr - level_start);
    entry->level[ptr - level_start] = '\0';
    
    while (ptr < end && isspace(*ptr)) ptr++;
    if (ptr >= end) return 0;
    
    char *msg_start = ptr;
    char *msg_end = end;
    while (msg_end > msg_start && isspace(*(msg_end - 1))) msg_end--;
    
    size_t msg_len = msg_end - msg_start;
    if (msg_len >= MAX_LINE_LEN - 30) return 0;
    strncpy(entry->message, msg_start, msg_len);
    entry->message[msg_len] = '\0';
    
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
        
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
            len--;
        }
        
        if (len == 0) break;
        
        if (parse_log_line(line, entries + entry_count)) {
            entry_count++;
        } else {
            printf("Invalid log format. Skipping: %s\n", line);
        }
    }
    
    printf("\nParsed %d log entries:\n", entry_count);
    printf("====================\n");
    
    int error_count = 0;
    int warning_count = 0;
    int info_count = 0;
    
    struct LogEntry *current = entries;
    struct LogEntry *end = entries + entry_count;
    
    while (current < end) {
        printf("Time: %s | Level: %s | Message: %s\n", 
               current->timestamp, current->level, current->message);
        
        char *level_ptr = current->level;
        if (strcmp(level_ptr, "ERROR") == 0) {
            error_count++;
        } else if (strcmp(level_ptr, "WARNING") == 0) {
            warning_count++;
        } else if (strcmp(level_ptr, "INFO") == 0) {
            info_count++;
        }
        
        current++;
    }
    
    printf("\nSummary:\n");
    printf("ERROR entries: %d\n", error_count);
    printf("WARNING entries: %d\n", warning_count);
    printf("INFO entries: %d\n", info_count);
    printf("Total entries: %d\n", entry_count);
    
    return 0;
}