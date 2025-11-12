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
    while (ptr < end && *ptr != ' ') ptr++;
    if (ptr >= end) return 0;
    
    size_t timestamp_len = ptr - timestamp_start;
    if (timestamp_len >= sizeof(entry->timestamp)) return 0;
    strncpy(entry->timestamp, timestamp_start, timestamp_len);
    entry->timestamp[timestamp_len] = '\0';
    
    while (ptr < end && isspace(*ptr)) ptr++;
    if (ptr >= end) return 0;
    
    char *level_start = ptr;
    while (ptr < end && *ptr != ' ') ptr++;
    if (ptr >= end) return 0;
    
    size_t level_len = ptr - level_start;
    if (level_len >= sizeof(entry->level)) return 0;
    strncpy(entry->level, level_start, level_len);
    entry->level[level_len] = '\0';
    
    while (ptr < end && isspace(*ptr)) ptr++;
    if (ptr >= end) return 0;
    
    char *msg_start = ptr;
    char *msg_end = end - 1;
    while (msg_end > msg_start && isspace(*msg_end)) msg_end--;
    
    size_t msg_len = msg_end - msg_start + 1;
    if (msg_len >= sizeof(entry->message)) return 0;
    strncpy(entry->message, msg_start, msg_len);
    entry->message[msg_len] = '\0';
    
    return 1;
}

int count_log_level(struct LogEntry *entries, int count, const char *level) {
    int total = 0;
    struct LogEntry *end = entries + count;
    
    for (struct LogEntry *ptr = entries; ptr < end; ptr++) {
        if (strcmp(ptr->level, level) == 0) {
            total++;
        }
    }
    return total;
}

void print_summary(struct LogEntry *entries, int count) {
    printf("Log Analysis Summary:\n");
    printf("Total entries: %d\n", count);
    printf("INFO entries: %d\n", count_log_level(entries, count, "INFO"));
    printf("WARN entries: %d\n", count_log_level(entries, count, "WARN"));
    printf("ERROR entries: %d\n", count_log_level(entries, count, "ERROR"));
    printf("DEBUG entries: %d\n", count_log_level(entries, count, "DEBUG"));
}

int main(void) {
    struct LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    char buffer[MAX_LINE_LEN];
    
    printf("Enter log entries (one per line, format: TIMESTAMP LEVEL MESSAGE):\n");
    printf("Enter empty line to finish.\n");
    
    while (entry_count < MAX_ENTRIES) {
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            break;
        }
        
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        }
        
        if (buffer[0] == '\0') {
            break;
        }
        
        if (parse_log_line(buffer, entries + entry_count)) {
            entry_count++;
        } else {
            printf("Invalid log format. Skipping line.\n");
        }
    }
    
    if (entry_count > 0) {
        print_summary(entries, entry_count);
        
        printf("\nRecent entries:\n");
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