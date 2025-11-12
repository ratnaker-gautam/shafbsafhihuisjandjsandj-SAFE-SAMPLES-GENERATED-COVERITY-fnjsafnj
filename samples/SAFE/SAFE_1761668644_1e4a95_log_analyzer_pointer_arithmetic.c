//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 1000

struct LogEntry {
    char timestamp[32];
    char level[16];
    char message[MAX_LINE_LENGTH];
};

int parse_log_line(char *line, struct LogEntry *entry) {
    if (line == NULL || entry == NULL) return 0;
    
    char *ptr = line;
    while (*ptr && isspace(*ptr)) ptr++;
    
    char *timestamp_end = strchr(ptr, ' ');
    if (timestamp_end == NULL || timestamp_end - ptr >= 32) return 0;
    strncpy(entry->timestamp, ptr, timestamp_end - ptr);
    entry->timestamp[timestamp_end - ptr] = '\0';
    
    ptr = timestamp_end + 1;
    while (*ptr && isspace(*ptr)) ptr++;
    
    char *level_end = strchr(ptr, ' ');
    if (level_end == NULL || level_end - ptr >= 16) return 0;
    strncpy(entry->level, ptr, level_end - ptr);
    entry->level[level_end - ptr] = '\0';
    
    ptr = level_end + 1;
    while (*ptr && isspace(*ptr)) ptr++;
    
    size_t msg_len = strlen(ptr);
    if (msg_len >= MAX_LINE_LENGTH) return 0;
    strcpy(entry->message, ptr);
    
    char *newline = strchr(entry->message, '\n');
    if (newline != NULL) *newline = '\0';
    
    return 1;
}

int main(void) {
    struct LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    char line[MAX_LINE_LENGTH];
    
    while (fgets(line, sizeof(line), stdin) != NULL && entry_count < MAX_ENTRIES) {
        if (strlen(line) > 0) {
            if (parse_log_line(line, entries + entry_count)) {
                entry_count++;
            }
        }
    }
    
    if (ferror(stdin)) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    int error_count = 0;
    int warning_count = 0;
    int info_count = 0;
    
    struct LogEntry *current = entries;
    struct LogEntry *end = entries + entry_count;
    
    while (current < end) {
        if (strcmp(current->level, "ERROR") == 0) {
            error_count++;
        } else if (strcmp(current->level, "WARNING") == 0) {
            warning_count++;
        } else if (strcmp(current->level, "INFO") == 0) {
            info_count++;
        }
        current++;
    }
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", entry_count);
    printf("ERROR entries: %d\n", error_count);
    printf("WARNING entries: %d\n", warning_count);
    printf("INFO entries: %d\n", info_count);
    
    if (entry_count > 0) {
        printf("\nLast 5 entries:\n");
        int start = entry_count > 5 ? entry_count - 5 : 0;
        current = entries + start;
        while (current < end) {
            printf("%s [%s] %s\n", current->timestamp, current->level, current->message);
            current++;
        }
    }
    
    return 0;
}