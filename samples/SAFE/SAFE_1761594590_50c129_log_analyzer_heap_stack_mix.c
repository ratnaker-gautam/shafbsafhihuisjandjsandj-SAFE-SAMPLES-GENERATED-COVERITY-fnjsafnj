//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdint.h>
#include <ctype.h>

#define MAX_LINE_LEN 1024
#define MAX_ENTRIES 1000
#define MAX_FILENAME 256

struct LogEntry {
    char timestamp[32];
    char level[16];
    char message[256];
    int severity;
};

int parse_log_level(const char* level) {
    if (strcmp(level, "DEBUG") == 0) return 0;
    if (strcmp(level, "INFO") == 0) return 1;
    if (strcmp(level, "WARNING") == 0) return 2;
    if (strcmp(level, "ERROR") == 0) return 3;
    if (strcmp(level, "CRITICAL") == 0) return 4;
    return -1;
}

int is_valid_timestamp(const char* timestamp) {
    if (strlen(timestamp) != 19) return 0;
    for (int i = 0; i < 19; i++) {
        if (i == 4 || i == 7) {
            if (timestamp[i] != '-') return 0;
        } else if (i == 10) {
            if (timestamp[i] != ' ') return 0;
        } else if (i == 13 || i == 16) {
            if (timestamp[i] != ':') return 0;
        } else {
            if (!isdigit(timestamp[i])) return 0;
        }
    }
    return 1;
}

int read_log_file(const char* filename, struct LogEntry** entries, int* count) {
    FILE* file = fopen(filename, "r");
    if (!file) return 0;
    
    char line[MAX_LINE_LEN];
    *count = 0;
    *entries = malloc(MAX_ENTRIES * sizeof(struct LogEntry));
    if (!*entries) {
        fclose(file);
        return 0;
    }
    
    while (fgets(line, sizeof(line), file) && *count < MAX_ENTRIES) {
        char timestamp[32] = {0};
        char level[16] = {0};
        char message[256] = {0};
        
        if (sscanf(line, "%31[^|]|%15[^|]|%255[^\n]", timestamp, level, message) == 3) {
            if (is_valid_timestamp(timestamp) && parse_log_level(level) >= 0) {
                struct LogEntry* entry = &(*entries)[*count];
                strncpy(entry->timestamp, timestamp, sizeof(entry->timestamp)-1);
                strncpy(entry->level, level, sizeof(entry->level)-1);
                strncpy(entry->message, message, sizeof(entry->message)-1);
                entry->severity = parse_log_level(level);
                (*count)++;
            }
        }
    }
    
    fclose(file);
    return 1;
}

void analyze_logs(struct LogEntry* entries, int count) {
    int level_counts[5] = {0};
    char levels[5][16] = {"DEBUG", "INFO", "WARNING", "ERROR", "CRITICAL"};
    
    for (int i = 0; i < count; i++) {
        if (entries[i].severity >= 0 && entries[i].severity < 5) {
            level_counts[entries[i].severity]++;
        }
    }
    
    printf("Log Analysis Summary:\n");
    printf("====================\n");
    printf("Total entries: %d\n", count);
    for (int i = 0; i < 5; i++) {
        printf("%-8s: %d\n", levels[i], level_counts[i]);
    }
    
    if (count > 0) {
        printf("\nRecent entries:\n");
        int show_count = count > 5 ? 5 : count;
        for (int i = count - show_count; i < count; i++) {
            printf("%s | %s | %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
        }
    }
}

int main() {
    char filename[MAX_FILENAME];
    printf("Enter log file name: ");
    
    if (!fgets(filename, sizeof(filename), stdin)) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(filename);
    if (len > 0 && filename[len-1] == '\n') {
        filename[len-1] = '\0';
    }
    
    if (strlen(filename) == 0) {
        printf("Invalid filename\n");
        return 1;
    }
    
    struct LogEntry* entries = NULL;
    int count = 0;
    
    if (!read_log_file(filename, &entries, &count)) {
        printf("Error reading log file or no valid entries found\n");
        if (entries) free(entries);
        return 1;
    }
    
    if (count == 0) {
        printf("No valid log entries found\n");
        free(entries);
        return 1;
    }
    
    analyze_logs(entries, count);
    
    free(entries);
    return 0;
}