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
    if (line == NULL || entry == NULL) return 0;
    
    char *ptr = line;
    while (*ptr && isspace(*ptr)) ptr++;
    if (*ptr == '\0') return 0;

    char *timestamp_end = strchr(ptr, ' ');
    if (timestamp_end == NULL) return 0;
    size_t timestamp_len = timestamp_end - ptr;
    if (timestamp_len >= sizeof(entry->timestamp)) return 0;
    strncpy(entry->timestamp, ptr, timestamp_len);
    entry->timestamp[timestamp_len] = '\0';

    ptr = timestamp_end + 1;
    while (*ptr && isspace(*ptr)) ptr++;
    if (*ptr == '\0') return 0;

    char *level_end = strchr(ptr, ' ');
    if (level_end == NULL) return 0;
    size_t level_len = level_end - ptr;
    if (level_len >= sizeof(entry->level)) return 0;
    strncpy(entry->level, ptr, level_len);
    entry->level[level_len] = '\0';

    ptr = level_end + 1;
    while (*ptr && isspace(*ptr)) ptr++;
    if (*ptr == '\0') return 0;

    size_t msg_len = strlen(ptr);
    if (msg_len > 0 && ptr[msg_len - 1] == '\n') {
        ptr[msg_len - 1] = '\0';
        msg_len--;
    }
    if (msg_len >= sizeof(entry->message)) return 0;
    strncpy(entry->message, ptr, msg_len);
    entry->message[msg_len] = '\0';

    return 1;
}

int count_log_level(struct LogEntry *entries, int count, const char *level) {
    int total = 0;
    struct LogEntry *current = entries;
    struct LogEntry *end = entries + count;
    
    while (current < end) {
        if (strcmp(current->level, level) == 0) {
            total++;
        }
        current++;
    }
    return total;
}

void print_entries_by_level(struct LogEntry *entries, int count, const char *level) {
    struct LogEntry *current = entries;
    struct LogEntry *end = entries + count;
    
    printf("Entries with level '%s':\n", level);
    while (current < end) {
        if (strcmp(current->level, level) == 0) {
            printf("%s %s %s\n", current->timestamp, current->level, current->message);
        }
        current++;
    }
}

int main(void) {
    struct LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    char line[MAX_LINE_LEN];
    
    printf("Enter log entries (format: TIMESTAMP LEVEL MESSAGE). Empty line to finish:\n");
    
    while (entry_count < MAX_ENTRIES) {
        if (fgets(line, sizeof(line), stdin) == NULL) break;
        
        if (line[0] == '\n') break;
        
        if (parse_log_line(line, &entries[entry_count])) {
            entry_count++;
        } else {
            printf("Invalid log format. Skipping line.\n");
        }
    }
    
    if (entry_count == 0) {
        printf("No valid log entries provided.\n");
        return 1;
    }
    
    printf("\nLog Analysis Results:\n");
    printf("Total entries: %d\n", entry_count);
    
    const char *levels[] = {"INFO", "WARN", "ERROR", "DEBUG"};
    int level_count = sizeof(levels) / sizeof(levels[0]);
    
    for (int i = 0; i < level_count; i++) {
        int count = count_log_level(entries, entry_count, levels[i]);
        printf("%s: %d\n", levels[i], count);
    }
    
    printf("\n");
    print_entries_by_level(entries, entry_count, "ERROR");
    
    return 0;
}