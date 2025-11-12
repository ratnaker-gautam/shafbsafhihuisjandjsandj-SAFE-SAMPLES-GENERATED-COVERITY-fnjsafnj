//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: macros ; Variation: log_analyzer
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
    char message[MAX_LINE_LEN - TIMESTAMP_LEN - 3];
} LogEntry;

int parse_log_line(const char* line, LogEntry* entry) {
    if (!line || !entry) return 0;
    
    size_t len = strlen(line);
    if (len < TIMESTAMP_LEN + 3 || len >= MAX_LINE_LEN) return 0;
    
    if (line[TIMESTAMP_LEN-1] != ' ' || line[TIMESTAMP_LEN] != '[') return 0;
    if (line[TIMESTAMP_LEN+2] != ']' || line[TIMESTAMP_LEN+3] != ' ') return 0;
    
    if (!IS_VALID_LEVEL(line[TIMESTAMP_LEN+1])) return 0;
    
    memcpy(entry->timestamp, line, TIMESTAMP_LEN - 1);
    entry->timestamp[TIMESTAMP_LEN - 1] = '\0';
    
    entry->level = line[TIMESTAMP_LEN+1];
    
    size_t msg_start = TIMESTAMP_LEN + 4;
    size_t msg_len = len - msg_start;
    if (msg_len >= sizeof(entry->message)) return 0;
    
    memcpy(entry->message, line + msg_start, msg_len);
    entry->message[msg_len] = '\0';
    
    return 1;
}

int read_log_file(const char* filename, LogEntry entries[], int max_entries) {
    if (!filename || !entries || max_entries <= 0) return 0;
    
    FILE* file = fopen(filename, "r");
    if (!file) return 0;
    
    char line[MAX_LINE_LEN];
    int count = 0;
    
    while (fgets(line, sizeof(line), file) && count < max_entries) {
        line[strcspn(line, "\n")] = '\0';
        
        if (strlen(line) == 0) continue;
        
        LogEntry entry;
        if (parse_log_line(line, &entry)) {
            entries[count] = entry;
            count++;
        }
    }
    
    fclose(file);
    return count;
}

void generate_summary(const LogEntry entries[], int count, int level_counts[]) {
    if (!entries || !level_counts || count <= 0) return;
    
    for (int i = 0; i < LOG_LEVELS; i++) {
        level_counts[i] = 0;
    }
    
    for (int i = 0; i < count; i++) {
        int idx = LEVEL_TO_INDEX(entries[i].level);
        if (idx >= 0 && idx < LOG_LEVELS) {
            level_counts[idx]++;
        }
    }
}

void print_summary(const int level_counts[]) {
    if (!level_counts) return;
    
    printf("Log Summary:\n");
    printf("Errors:   %d\n", level_counts[LOG_LEVEL_ERROR]);
    printf("Warnings: %d\n", level_counts[LOG_LEVEL_WARN]);
    printf("Info:     %d\n", level_counts[LOG_LEVEL_INFO]);
    printf("Debug:    %d\n", level_counts[LOG_LEVEL_DEBUG]);
    printf("Total:    %d\n", level_counts[0] + level_counts[1] + level_counts[2] + level_counts[3]);
}

int main(void) {
    char filename[256];
    printf("Enter log filename: ");
    
    if (!fgets(filename, sizeof(filename), stdin)) {
        fprintf(stderr, "Error reading filename\n");
        return 1;
    }
    
    filename[strcspn(filename, "\n")] = '\0';
    
    if (strlen(filename) == 0) {
        fprintf(stderr, "Empty filename\n");
        return 1;
    }
    
    LogEntry entries[MAX_ENTRIES];
    int count = read_log_file(filename, entries, MAX_ENTRIES);
    
    if (count <= 0) {
        fprintf(stderr, "No valid log entries found or file error\n");
        return 1;
    }
    
    int level_counts[LOG_LEVELS];
    generate_summary(entries, count, level_counts);
    print_summary(level_counts);
    
    printf("\nFirst 5 entries:\n");
    int display_count = (count < 5) ? count : 5;
    for (int i = 0; i < display_count; i++) {
        printf("%s [%c] %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
    }
    
    return 0;
}