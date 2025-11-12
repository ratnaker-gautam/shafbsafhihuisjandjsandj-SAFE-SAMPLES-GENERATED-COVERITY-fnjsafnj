//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LEN 256
#define MAX_ENTRIES 100

struct LogEntry {
    char timestamp[20];
    char level[10];
    char message[MAX_LINE_LEN - 30];
};

int parse_log_line(char *line, struct LogEntry *entry) {
    if (line == NULL || entry == NULL) return 0;
    
    char *ptr = line;
    while (*ptr && isspace(*ptr)) ptr++;
    if (*ptr == '\0') return 0;
    
    char *timestamp_end = strchr(ptr, ' ');
    if (timestamp_end == NULL || timestamp_end - ptr >= 19) return 0;
    strncpy(entry->timestamp, ptr, timestamp_end - ptr);
    entry->timestamp[timestamp_end - ptr] = '\0';
    
    ptr = timestamp_end + 1;
    while (*ptr && isspace(*ptr)) ptr++;
    
    char *level_end = strchr(ptr, ' ');
    if (level_end == NULL || level_end - ptr >= 9) return 0;
    strncpy(entry->level, ptr, level_end - ptr);
    entry->level[level_end - ptr] = '\0';
    
    ptr = level_end + 1;
    while (*ptr && isspace(*ptr)) ptr++;
    
    size_t msg_len = strlen(ptr);
    if (msg_len >= MAX_LINE_LEN - 30) return 0;
    strcpy(entry->message, ptr);
    
    char *newline = strchr(entry->message, '\n');
    if (newline != NULL) *newline = '\0';
    
    return 1;
}

int main(void) {
    struct LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    char line[MAX_LINE_LEN];
    
    printf("Enter log entries (empty line to finish):\n");
    
    while (entry_count < MAX_ENTRIES && fgets(line, sizeof(line), stdin) != NULL) {
        if (line[0] == '\n') break;
        
        if (parse_log_line(line, entries + entry_count)) {
            entry_count++;
        } else {
            printf("Invalid log format. Use: TIMESTAMP LEVEL MESSAGE\n");
        }
    }
    
    printf("\nLog Analysis Results:\n");
    printf("====================\n");
    
    int error_count = 0;
    int warning_count = 0;
    int info_count = 0;
    
    for (int i = 0; i < entry_count; i++) {
        struct LogEntry *entry = entries + i;
        
        if (strcmp(entry->level, "ERROR") == 0) {
            error_count++;
        } else if (strcmp(entry->level, "WARNING") == 0) {
            warning_count++;
        } else if (strcmp(entry->level, "INFO") == 0) {
            info_count++;
        }
    }
    
    printf("Total entries: %d\n", entry_count);
    printf("ERROR entries: %d\n", error_count);
    printf("WARNING entries: %d\n", warning_count);
    printf("INFO entries: %d\n", info_count);
    
    if (error_count > 0) {
        printf("\nERROR entries:\n");
        for (int i = 0; i < entry_count; i++) {
            struct LogEntry *entry = entries + i;
            if (strcmp(entry->level, "ERROR") == 0) {
                printf("%s: %s\n", entry->timestamp, entry->message);
            }
        }
    }
    
    return 0;
}