//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

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
    
    for (struct LogEntry *current = entries; current < end; current++) {
        if (strcmp(current->level, level) == 0) {
            occurrences++;
        }
    }
    return occurrences;
}

int main(void) {
    struct LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    
    char line[MAX_LINE_LEN];
    
    while (fgets(line, sizeof(line), stdin)) {
        if (entry_count >= MAX_ENTRIES) {
            fprintf(stderr, "Maximum entries reached\n");
            break;
        }
        
        if (parse_log_line(line, entries + entry_count)) {
            entry_count++;
        }
    }
    
    if (ferror(stdin)) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", entry_count);
    
    const char *levels[] = {"INFO", "WARNING", "ERROR", "DEBUG"};
    int level_count = sizeof(levels) / sizeof(levels[0]);
    
    for (int i = 0; i < level_count; i++) {
        int count = count_level_occurrences(entries, entry_count, levels[i]);
        printf("%s: %d\n", levels[i], count);
    }
    
    if (entry_count > 0) {
        printf("\nLast 5 entries:\n");
        int start = entry_count > 5 ? entry_count - 5 : 0;
        struct LogEntry *ptr = entries + start;
        struct LogEntry *end = entries + entry_count;
        
        while (ptr < end) {
            printf("%s [%s] %s\n", ptr->timestamp, ptr->level, ptr->message);
            ptr++;
        }
    }
    
    return 0;
}