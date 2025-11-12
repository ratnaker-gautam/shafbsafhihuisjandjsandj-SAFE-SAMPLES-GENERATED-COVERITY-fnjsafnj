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

int count_log_level(struct LogEntry *entries, int count, const char *level) {
    if (entries == NULL || level == NULL || count <= 0) return 0;
    
    int total = 0;
    struct LogEntry *end = entries + count;
    for (struct LogEntry *ptr = entries; ptr < end; ptr++) {
        if (strcmp(ptr->level, level) == 0) {
            total++;
        }
    }
    return total;
}

int main(void) {
    struct LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    
    char buffer[MAX_LINE_LEN];
    while (fgets(buffer, sizeof(buffer), stdin) != NULL && entry_count < MAX_ENTRIES) {
        if (parse_log_line(buffer, entries + entry_count)) {
            entry_count++;
        }
    }
    
    if (ferror(stdin)) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    if (entry_count == 0) {
        printf("No valid log entries found\n");
        return 0;
    }
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", entry_count);
    
    const char *levels[] = {"INFO", "WARN", "ERROR", "DEBUG"};
    int level_count = sizeof(levels) / sizeof(levels[0]);
    
    for (int i = 0; i < level_count; i++) {
        int count = count_log_level(entries, entry_count, levels[i]);
        printf("%s: %d\n", levels[i], count);
    }
    
    struct LogEntry *oldest = entries;
    struct LogEntry *newest = entries;
    struct LogEntry *end_ptr = entries + entry_count;
    
    for (struct LogEntry *ptr = entries + 1; ptr < end_ptr; ptr++) {
        if (strcmp(ptr->timestamp, oldest->timestamp) < 0) {
            oldest = ptr;
        }
        if (strcmp(ptr->timestamp, newest->timestamp) > 0) {
            newest = ptr;
        }
    }
    
    printf("Oldest entry: %s %s %s\n", oldest->timestamp, oldest->level, oldest->message);
    printf("Newest entry: %s %s %s\n", newest->timestamp, newest->level, newest->message);
    
    return 0;
}