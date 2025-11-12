//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: pointer_arithmetic ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 1000

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
    char *message_end = end - 1;
    while (message_end > message_start && isspace(*message_end)) message_end--;
    size_t message_len = message_end - message_start + 1;
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
    char line[MAX_LINE_LENGTH];
    
    printf("Enter log entries (empty line to finish):\n");
    
    while (entry_count < MAX_ENTRIES) {
        if (fgets(line, sizeof(line), stdin) == NULL) break;
        
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }
        
        if (line[0] == '\0') break;
        
        if (parse_log_line(line, entries + entry_count)) {
            entry_count++;
        } else {
            printf("Invalid log format: %s\n", line);
        }
    }
    
    if (entry_count > 0) {
        print_summary(entries, entry_count);
        
        printf("\nRecent entries:\n");
        int start = (entry_count > 5) ? entry_count - 5 : 0;
        for (int i = start; i < entry_count; i++) {
            printf("%s [%s] %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
        }
    } else {
        printf("No valid log entries found.\n");
    }
    
    return 0;
}