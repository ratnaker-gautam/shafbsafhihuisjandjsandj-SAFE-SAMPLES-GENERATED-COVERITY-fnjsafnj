//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 1000

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
    
    char line[MAX_LINE_LENGTH];
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
        
        if (sscanf(line, "%31s %15s %255[^\n]", timestamp, level, message) == 3) {
            if (is_valid_timestamp(timestamp) && parse_log_level(level) >= 0) {
                struct LogEntry* entry = &(*entries)[*count];
                strncpy(entry->timestamp, timestamp, sizeof(entry->timestamp) - 1);
                entry->timestamp[sizeof(entry->timestamp) - 1] = '\0';
                strncpy(entry->level, level, sizeof(entry->level) - 1);
                entry->level[sizeof(entry->level) - 1] = '\0';
                strncpy(entry->message, message, sizeof(entry->message) - 1);
                entry->message[sizeof(entry->message) - 1] = '\0';
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
    char earliest[32] = "9999-99-99 99:99:99";
    char latest[32] = "0000-00-00 00:00:00";
    
    for (int i = 0; i < count; i++) {
        if (entries[i].severity >= 0 && entries[i].severity < 5) {
            level_counts[entries[i].severity]++;
        }
        
        if (strcmp(entries[i].timestamp, earliest) < 0) {
            strncpy(earliest, entries[i].timestamp, sizeof(earliest) - 1);
            earliest[sizeof(earliest) - 1] = '\0';
        }
        if (strcmp(entries[i].timestamp, latest) > 0) {
            strncpy(latest, entries[i].timestamp, sizeof(latest) - 1);
            latest[sizeof(latest) - 1] = '\0';
        }
    }
    
    printf("Log Analysis Summary:\n");
    printf("Total entries: %d\n", count);
    printf("Time range: %s to %s\n", earliest, latest);
    printf("DEBUG: %d\n", level_counts[0]);
    printf("INFO: %d\n", level_counts[1]);
    printf("WARNING: %d\n", level_counts[2]);
    printf("ERROR: %d\n", level_counts[3]);
    printf("CRITICAL: %d\n", level_counts[4]);
}

void find_recent_logs(struct LogEntry* entries, int count) {
    if (count == 0) return;
    
    printf("\nRecent log entries (last 5):\n");
    int start = count > 5 ? count - 5 : 0;
    for (int i = start; i < count; i++) {
        printf("%s [%s] %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
    }
}

int main() {
    char filename[256];
    printf("Enter log file name: ");
    if (scanf("%255s", filename) != 1) {
        printf("Invalid input\n");