//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

#define MAX_LINE_LEN 1024
#define MAX_ENTRIES 1000

typedef struct {
    char timestamp[32];
    char level[16];
    char message[256];
} LogEntry;

int parse_log_line(char *line, LogEntry *entry) {
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
    if (msg_len >= sizeof(entry->message)) return 0;
    strncpy(entry->message, ptr, msg_len);
    entry->message[msg_len] = '\0';
    
    char *newline = strchr(entry->message, '\n');
    if (newline != NULL) *newline = '\0';
    
    return 1;
}

int count_log_level(LogEntry *entries, int count, const char *level) {
    if (entries == NULL || level == NULL || count <= 0) return 0;
    
    int total = 0;
    LogEntry *end = entries + count;
    for (LogEntry *ptr = entries; ptr < end; ptr++) {
        if (strcmp(ptr->level, level) == 0) {
            total++;
        }
    }
    return total;
}

int main(void) {
    LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    
    char buffer[MAX_LINE_LEN];
    while (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        if (entry_count >= MAX_ENTRIES) break;
        
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
    
    int info_count = count_log_level(entries, entry_count, "INFO");
    int warn_count = count_log_level(entries, entry_count, "WARN");
    int error_count = count_log_level(entries, entry_count, "ERROR");
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", entry_count);
    printf("INFO: %d\n", info_count);
    printf("WARN: %d\n", warn_count);
    printf("ERROR: %d\n", error_count);
    
    LogEntry *ptr = entries;
    LogEntry *end = entries + entry_count;
    printf("\nFirst 5 entries:\n");
    for (int i = 0; i < 5 && ptr < end; i++, ptr++) {
        printf("%s [%s] %s\n", ptr->timestamp, ptr->level, ptr->message);
    }
    
    return 0;
}