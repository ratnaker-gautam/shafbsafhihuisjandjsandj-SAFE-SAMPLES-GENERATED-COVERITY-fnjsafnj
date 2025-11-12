//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_LINE_LEN 1024
#define MAX_ENTRIES 1000
#define TIMESTAMP_LEN 20
#define LOG_LEVELS 4

#define LOG_LEVEL_ERROR 0
#define LOG_LEVEL_WARN 1
#define LOG_LEVEL_INFO 2
#define LOG_LEVEL_DEBUG 3

#define IS_VALID_LEVEL(c) ((c) == 'E' || (c) == 'W' || (c) == 'I' || (c) == 'D')
#define LEVEL_TO_INDEX(c) ((c) == 'E' ? 0 : (c) == 'W' ? 1 : (c) == 'I' ? 2 : 3)

typedef struct {
    char timestamp[TIMESTAMP_LEN];
    char level;
    char message[MAX_LINE_LEN - TIMESTAMP_LEN - 4];
} LogEntry;

int parse_log_line(const char* line, LogEntry* entry) {
    if (!line || !entry) return 0;
    
    size_t len = strlen(line);
    if (len < TIMESTAMP_LEN + 3 || len >= MAX_LINE_LEN) return 0;
    
    if (line[TIMESTAMP_LEN-1] != ' ' || line[TIMESTAMP_LEN] != '[') return 0;
    if (line[TIMESTAMP_LEN+2] != ']' || line[TIMESTAMP_LEN+3] != ' ') return 0;
    
    if (!IS_VALID_LEVEL(line[TIMESTAMP_LEN+1])) return 0;
    
    strncpy(entry->timestamp, line, TIMESTAMP_LEN-1);
    entry->timestamp[TIMESTAMP_LEN-1] = '\0';
    
    entry->level = line[TIMESTAMP_LEN+1];
    
    size_t msg_start = TIMESTAMP_LEN + 4;
    if (msg_start >= len) return 0;
    
    strncpy(entry->message, line + msg_start, MAX_LINE_LEN - TIMESTAMP_LEN - 4);
    entry->message[MAX_LINE_LEN - TIMESTAMP_LEN - 5] = '\0';
    
    return 1;
}

int read_log_file(const char* filename, LogEntry* entries, int* count) {
    if (!filename || !entries || !count) return 0;
    
    FILE* file = fopen(filename, "r");
    if (!file) return 0;
    
    char line[MAX_LINE_LEN];
    *count = 0;
    
    while (fgets(line, sizeof(line), file) && *count < MAX_ENTRIES) {
        if (line[0] == '\0') continue;
        
        size_t len = strlen(line);
        if (len > 0 && line[len-1] == '\n') {
            line[len-1] = '\0';
        }
        
        if (parse_log_line(line, &entries[*count])) {
            (*count)++;
        }
    }
    
    fclose(file);
    return 1;
}

void generate_summary(const LogEntry* entries, int count) {
    if (!entries || count <= 0) return;
    
    int level_counts[LOG_LEVELS] = {0};
    
    for (int i = 0; i < count; i++) {
        int idx = LEVEL_TO_INDEX(entries[i].level);
        if (idx >= 0 && idx < LOG_LEVELS) {
            level_counts[idx]++;
        }
    }
    
    printf("Log Analysis Summary:\n");
    printf("Total entries: %d\n", count);
    printf("Errors: %d\n", level_counts[LOG_LEVEL_ERROR]);
    printf("Warnings: %d\n", level_counts[LOG_LEVEL_WARN]);
    printf("Info: %d\n", level_counts[LOG_LEVEL_INFO]);
    printf("Debug: %d\n", level_counts[LOG_LEVEL_DEBUG]);
}

int main(void) {
    LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    
    printf("Enter log filename: ");
    char filename[256];
    
    if (!fgets(filename, sizeof(filename), stdin)) {
        fprintf(stderr, "Error reading filename\n");
        return 1;
    }
    
    size_t len = strlen(filename);
    if (len > 0 && filename[len-1] == '\n') {
        filename[len-1] = '\0';
    }
    
    if (strlen(filename) == 0) {
        fprintf(stderr, "Empty filename\n");
        return 1;
    }
    
    if (!read_log_file(filename, entries, &entry_count)) {
        fprintf(stderr, "Error reading log file\n");
        return 1;
    }
    
    if (entry_count == 0) {
        printf("No valid log entries found\n");
        return 0;
    }
    
    generate_summary(entries, entry_count);
    
    printf("\nSample entries:\n");
    int samples = (entry_count < 3) ? entry_count : 3;
    for (int i = 0; i < samples; i++) {
        printf("%s [%c] %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
    }
    
    return 0;
}