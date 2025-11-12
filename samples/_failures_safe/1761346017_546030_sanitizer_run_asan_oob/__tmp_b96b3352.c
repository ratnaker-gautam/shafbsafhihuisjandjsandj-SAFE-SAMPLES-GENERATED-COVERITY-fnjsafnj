//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

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
    char *end = ptr + strlen(line);
    
    while (ptr < end && isspace(*ptr)) ptr++;
    if (ptr >= end) return 0;
    
    char *ts_start = ptr;
    while (ptr < end && !isspace(*ptr)) ptr++;
    if (ptr - ts_start >= 32) return 0;
    strncpy(entry->timestamp, ts_start, ptr - ts_start);
    entry->timestamp[ptr - ts_start] = '\0';
    
    while (ptr < end && isspace(*ptr)) ptr++;
    if (ptr >= end) return 0;
    
    char *level_start = ptr;
    while (ptr < end && !isspace(*ptr)) ptr++;
    if (ptr - level_start >= 16) return 0;
    strncpy(entry->level, level_start, ptr - level_start);
    entry->level[ptr - level_start] = '\0';
    
    while (ptr < end && isspace(*ptr)) ptr++;
    if (ptr >= end) return 0;
    
    char *msg_start = ptr;
    while (ptr < end && *ptr != '\n' && *ptr != '\r') ptr++;
    if (ptr - msg_start >= 256) return 0;
    strncpy(entry->message, msg_start, ptr - msg_start);
    entry->message[ptr - msg_start] = '\0';
    
    return 1;
}

int count_level_occurrences(struct LogEntry *entries, int count, const char *level) {
    int occurrences = 0;
    struct LogEntry *end = entries + count;
    
    for (struct LogEntry *entry = entries; entry < end; entry++) {
        if (strcmp(entry->level, level) == 0) {
            occurrences++;
        }
    }
    return occurrences;
}

void print_summary(struct LogEntry *entries, int count) {
    printf("Log Analysis Summary:\n");
    printf("Total entries: %d\n", count);
    
    const char *levels[] = {"INFO", "WARNING", "ERROR", "DEBUG"};
    int level_count = sizeof(levels) / sizeof(levels[0]);
    
    for (int i = 0; i < level_count; i++) {
        int count = count_level_occurrences(entries, count, levels[i]);
        printf("%s: %d\n", levels[i], count);
    }
}

int main(void) {
    struct LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    char buffer[MAX_LINE_LEN];
    
    printf("Enter log entries (format: TIMESTAMP LEVEL MESSAGE):\n");
    printf("Enter empty line to finish.\n");
    
    while (entry_count < MAX_ENTRIES) {
        if (fgets(buffer, MAX_LINE_LEN, stdin) == NULL) {
            break;
        }
        
        if (buffer[0] == '\n' || buffer[0] == '\0') {
            break;
        }
        
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        }
        
        if (parse_log_line(buffer, entries + entry_count)) {
            entry_count++;
        } else {
            printf("Invalid log format. Skipping line.\n");
        }
    }
    
    if (entry_count > 0) {
        print_summary(entries, entry_count);
        
        printf("\nLast 5 entries:\n");
        int start = (entry_count > 5) ? entry_count - 5 : 0;
        struct LogEntry *ptr = entries + start;
        struct LogEntry *end = entries + entry_count;
        
        while (ptr < end) {
            printf("%s [%s] %s\n", ptr->timestamp, ptr->level, ptr->message);
            ptr++;
        }
    } else {
        printf("No valid log entries found.\n");
    }
    
    return 0;
}