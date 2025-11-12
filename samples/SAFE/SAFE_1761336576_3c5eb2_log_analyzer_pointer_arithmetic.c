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
    
    ptr++;
    char *level_start = ptr;
    while (*ptr && *ptr != ' ') ptr++;
    if (*ptr == '\0') return 0;
    size_t level_len = ptr - level_start;
    if (level_len >= sizeof(entry->level)) return 0;
    strncpy(entry->level, level_start, level_len);
    entry->level[level_len] = '\0';
    
    ptr++;
    char *message_start = ptr;
    while (*ptr && *ptr != '\n' && *ptr != '\r') ptr++;
    size_t message_len = ptr - message_start;
    if (message_len >= sizeof(entry->message)) return 0;
    strncpy(entry->message, message_start, message_len);
    entry->message[message_len] = '\0';
    
    return 1;
}

int count_log_level(struct LogEntry *entries, int count, const char *level) {
    if (entries == NULL || level == NULL) return 0;
    
    int level_count = 0;
    struct LogEntry *current = entries;
    struct LogEntry *end = entries + count;
    
    while (current < end) {
        if (strcmp(current->level, level) == 0) {
            level_count++;
        }
        current++;
    }
    return level_count;
}

int main(void) {
    struct LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    
    printf("Enter log entries (format: TIMESTAMP LEVEL MESSAGE):\n");
    printf("Enter 'END' on a new line to finish input.\n");
    
    char line[MAX_LINE_LEN];
    while (entry_count < MAX_ENTRIES) {
        if (fgets(line, sizeof(line), stdin) == NULL) {
            break;
        }
        
        if (strncmp(line, "END", 3) == 0 && (line[3] == '\n' || line[3] == '\r' || line[3] == '\0')) {
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
    
    printf("\nLog Analysis Results:\n");
    printf("Total entries: %d\n", entry_count);
    
    const char *levels[] = {"INFO", "WARN", "ERROR", "DEBUG"};
    int level_counts[4] = {0};
    
    for (int i = 0; i < 4; i++) {
        level_counts[i] = count_log_level(entries, entry_count, levels[i]);
    }
    
    for (int i = 0; i < 4; i++) {
        printf("%s: %d (%.1f%%)\n", levels[i], level_counts[i], 
               entry_count > 0 ? (level_counts[i] * 100.0) / entry_count : 0.0);
    }
    
    printf("\nRecent entries:\n");
    int display_count = entry_count > 5 ? 5 : entry_count;
    struct LogEntry *display_ptr = entries + (entry_count - display_count);
    
    for (int i = 0; i < display_count; i++) {
        printf("%s [%s] %s\n", display_ptr->timestamp, display_ptr->level, display_ptr->message);
        display_ptr++;
    }
    
    return 0;
}