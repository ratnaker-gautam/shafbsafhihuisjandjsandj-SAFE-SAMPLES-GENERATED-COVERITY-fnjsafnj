//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: heap_stack_mix ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdint.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
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
    
    char line[MAX_LINE_LENGTH];
    *count = 0;
    
    while (fgets(line, sizeof(line), file) && *count < MAX_ENTRIES) {
        if (strlen(line) >= sizeof(line) - 1) continue;
        
        char timestamp[32] = {0};
        char level[16] = {0};
        char message[256] = {0};
        
        int parsed = sscanf(line, "%31s %15s %255[^\n]", timestamp, level, message);
        if (parsed != 3) continue;
        
        if (!is_valid_timestamp(timestamp)) continue;
        
        int severity = parse_log_level(level);
        if (severity == -1) continue;
        
        struct LogEntry* entry = malloc(sizeof(struct LogEntry));
        if (!entry) {
            fclose(file);
            return 0;
        }
        
        strncpy(entry->timestamp, timestamp, sizeof(entry->timestamp) - 1);
        strncpy(entry->level, level, sizeof(entry->level) - 1);
        strncpy(entry->message, message, sizeof(entry->message) - 1);
        entry->severity = severity;
        
        entries[*count] = entry;
        (*count)++;
    }
    
    fclose(file);
    return 1;
}

void analyze_logs(struct LogEntry** entries, int count) {
    int severity_count[5] = {0};
    char* severity_names[] = {"DEBUG", "INFO", "WARNING", "ERROR", "CRITICAL"};
    
    for (int i = 0; i < count; i++) {
        if (entries[i]->severity >= 0 && entries[i]->severity < 5) {
            severity_count[entries[i]->severity]++;
        }
    }
    
    printf("Log Analysis Summary:\n");
    printf("====================\n");
    printf("Total entries: %d\n", count);
    for (int i = 0; i < 5; i++) {
        printf("%-8s: %d entries\n", severity_names[i], severity_count[i]);
    }
    
    if (count > 0) {
        printf("\nRecent entries:\n");
        int display_count = count > 5 ? 5 : count;
        for (int i = count - display_count; i < count; i++) {
            printf("%s %-8s %s\n", entries[i]->timestamp, entries[i]->level, entries[i]->message);
        }
    }
}

void cleanup_entries(struct LogEntry** entries, int count) {
    for (int i = 0; i < count; i++) {
        free(entries[i]);
    }
}

int main() {
    char filename[MAX_FILENAME];
    struct LogEntry* entries[MAX_ENTRIES];
    int entry_count = 0;
    
    printf("Enter log file name: ");
    if (!fgets(filename, sizeof(filename), stdin)) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(filename);
    if (len > 0 && filename[len - 1] == '\n') {
        filename[len - 1] = '\0';
    }
    
    if (strlen(filename) == 0) {
        printf("Invalid filename\n");
        return 1;
    }
    
    if (!read_log_file(filename, entries, &entry_count)) {
        printf("Error reading log file: %s\n", filename);
        return 1;
    }
    
    if (entry_count == 0) {
        printf("No valid log entries found\n");
        return 0;
    }
    
    analyze_logs(entries, entry_count);
    cleanup_entries(entries