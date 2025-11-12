//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: pointer_arithmetic ; Variation: log_analyzer
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
    if (!line || !entry) return 0;
    
    char *ptr = line;
    while (*ptr && isspace(*ptr)) ptr++;
    if (!*ptr) return 0;
    
    char *timestamp_end = strchr(ptr, ' ');
    if (!timestamp_end || timestamp_end - ptr >= 32) return 0;
    strncpy(entry->timestamp, ptr, timestamp_end - ptr);
    entry->timestamp[timestamp_end - ptr] = '\0';
    
    ptr = timestamp_end + 1;
    while (*ptr && isspace(*ptr)) ptr++;
    if (!*ptr) return 0;
    
    char *level_end = strchr(ptr, ' ');
    if (!level_end || level_end - ptr >= 16) return 0;
    strncpy(entry->level, ptr, level_end - ptr);
    entry->level[level_end - ptr] = '\0';
    
    ptr = level_end + 1;
    while (*ptr && isspace(*ptr)) ptr++;
    if (!*ptr) return 0;
    
    size_t msg_len = strlen(ptr);
    if (msg_len >= 256) return 0;
    strcpy(entry->message, ptr);
    
    char *newline = strchr(entry->message, '\n');
    if (newline) *newline = '\0';
    
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
    
    printf("Enter log entries (format: TIMESTAMP LEVEL MESSAGE):\n");
    printf("Enter empty line to finish.\n");
    
    char line[MAX_LINE_LEN];
    while (entry_count < MAX_ENTRIES) {
        if (!fgets(line, sizeof(line), stdin)) {
            break;
        }
        
        if (line[0] == '\n') {
            break;
        }
        
        if (parse_log_line(line, &entries[entry_count])) {
            entry_count++;
        } else {
            printf("Invalid log format. Skipping line.\n");
        }
    }
    
    if (entry_count == 0) {
        printf("No valid log entries found.\n");
        return 1;
    }
    
    print_summary(entries, entry_count);
    
    return 0;
}