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
    if (ptr - ts_start >= 20 || ptr >= end) return 0;
    strncpy(entry->timestamp, ts_start, ptr - ts_start);
    entry->timestamp[ptr - ts_start] = '\0';
    
    ptr++;
    if (ptr >= end) return 0;
    
    char *level_start = ptr;
    while (ptr < end && *ptr != ' ') ptr++;
    if (ptr - level_start >= 10 || ptr >= end) return 0;
    strncpy(entry->level, level_start, ptr - level_start);
    entry->level[ptr - level_start] = '\0';
    
    ptr++;
    if (ptr >= end) return 0;
    
    char *msg_start = ptr;
    while (ptr < end && *ptr != '\n' && *ptr != '\r') ptr++;
    size_t msg_len = ptr - msg_start;
    if (msg_len >= MAX_LINE_LEN - 30) msg_len = MAX_LINE_LEN - 31;
    strncpy(entry->message, msg_start, msg_len);
    entry->message[msg_len] = '\0';
    
    return 1;
}

int count_level_occurrences(struct LogEntry *entries, int count, const char *level) {
    int occurrences = 0;
    struct LogEntry *end = entries + count;
    for (struct LogEntry *e = entries; e < end; e++) {
        if (strcmp(e->level, level) == 0) {
            occurrences++;
        }
    }
    return occurrences;
}

void print_summary(struct LogEntry *entries, int count) {
    printf("Log Analysis Summary:\n");
    printf("Total entries: %d\n", count);
    printf("INFO entries: %d\n", count_level_occurrences(entries, count, "INFO"));
    printf("WARN entries: %d\n", count_level_occurrences(entries, count, "WARN"));
    printf("ERROR entries: %d\n", count_level_occurrences(entries, count, "ERROR"));
    printf("DEBUG entries: %d\n", count_level_occurrences(entries, count, "DEBUG"));
}

int main(void) {
    struct LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    
    printf("Enter log entries (format: TIMESTAMP LEVEL MESSAGE):\n");
    printf("Press Ctrl+D (Unix) or Ctrl+Z (Windows) to finish.\n");
    
    char line[MAX_LINE_LEN];
    while (entry_count < MAX_ENTRIES && fgets(line, sizeof(line), stdin) != NULL) {
        if (strlen(line) == 0 || line[0] == '\n') continue;
        
        struct LogEntry *current = entries + entry_count;
        if (parse_log_line(line, current)) {
            entry_count++;
        } else {
            printf("Invalid log format. Expected: TIMESTAMP LEVEL MESSAGE\n");
        }
    }
    
    if (ferror(stdin)) {
        fprintf(stderr, "Error reading input.\n");
        return 1;
    }
    
    if (entry_count == 0) {
        printf("No valid log entries found.\n");
        return 0;
    }
    
    print_summary(entries, entry_count);
    
    printf("\nLast 5 entries:\n");
    int start = (entry_count > 5) ? entry_count - 5 : 0;
    struct LogEntry *first = entries + start;
    struct LogEntry *last = entries + entry_count;
    for (struct LogEntry *e = first; e < last; e++) {
        printf("%s [%s] %s\n", e->timestamp, e->level, e->message);
    }
    
    return 0;
}