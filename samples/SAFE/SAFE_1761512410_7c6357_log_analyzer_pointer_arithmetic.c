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

void print_entries_by_level(struct LogEntry *entries, int count, const char *level) {
    struct LogEntry *end = entries + count;
    for (struct LogEntry *entry = entries; entry < end; entry++) {
        if (strcmp(entry->level, level) == 0) {
            printf("%s %s %s\n", entry->timestamp, entry->level, entry->message);
        }
    }
}

int main(void) {
    struct LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    
    char line[MAX_LINE_LEN];
    while (fgets(line, sizeof(line), stdin) != NULL && entry_count < MAX_ENTRIES) {
        if (parse_log_line(line, entries + entry_count)) {
            entry_count++;
        }
    }
    
    if (entry_count == 0) {
        printf("No valid log entries found.\n");
        return 1;
    }
    
    printf("Log Analysis Summary:\n");
    printf("Total entries: %d\n", entry_count);
    
    const char *levels[] = {"INFO", "WARN", "ERROR", "DEBUG"};
    int level_count = sizeof(levels) / sizeof(levels[0]);
    
    for (int i = 0; i < level_count; i++) {
        int count = count_level_occurrences(entries, entry_count, levels[i]);
        printf("%s: %d\n", levels[i], count);
    }
    
    printf("\nERROR entries:\n");
    print_entries_by_level(entries, entry_count, "ERROR");
    
    printf("\nWARN entries:\n");
    print_entries_by_level(entries, entry_count, "WARN");
    
    return 0;
}