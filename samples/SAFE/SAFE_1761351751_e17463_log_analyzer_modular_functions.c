//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: log_analyzer
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
};

int parse_log_line(const char* line, struct LogEntry* entry) {
    if (line == NULL || entry == NULL) return 0;
    
    char temp_line[MAX_LINE_LENGTH];
    if (strlen(line) >= MAX_LINE_LENGTH) return 0;
    strcpy(temp_line, line);
    
    char* timestamp = strtok(temp_line, " ");
    if (timestamp == NULL) return 0;
    
    char* level = strtok(NULL, " ");
    if (level == NULL) return 0;
    
    char* message = strtok(NULL, "\n");
    if (message == NULL) return 0;
    
    if (strlen(timestamp) >= sizeof(entry->timestamp) ||
        strlen(level) >= sizeof(entry->level) ||
        strlen(message) >= sizeof(entry->message)) {
        return 0;
    }
    
    strcpy(entry->timestamp, timestamp);
    strcpy(entry->level, level);
    strcpy(entry->message, message);
    
    return 1;
}

int count_log_level(const struct LogEntry* entries, int count, const char* level) {
    if (entries == NULL || level == NULL || count <= 0 || count > MAX_ENTRIES) return 0;
    
    int level_count = 0;
    for (int i = 0; i < count; i++) {
        if (strcmp(entries[i].level, level) == 0) {
            level_count++;
        }
    }
    return level_count;
}

void print_log_summary(const struct LogEntry* entries, int count) {
    if (entries == NULL || count <= 0 || count > MAX_ENTRIES) return;
    
    printf("Log Analysis Summary:\n");
    printf("Total entries: %d\n", count);
    
    const char* levels[] = {"INFO", "WARNING", "ERROR", "DEBUG"};
    int num_levels = sizeof(levels) / sizeof(levels[0]);
    
    for (int i = 0; i < num_levels; i++) {
        int count_level = count_log_level(entries, count, levels[i]);
        printf("%s: %d\n", levels[i], count_level);
    }
}

int read_log_file(struct LogEntry* entries, int* entry_count) {
    if (entries == NULL || entry_count == NULL) return 0;
    
    printf("Enter log entries (one per line, format: TIMESTAMP LEVEL MESSAGE):\n");
    printf("Enter 'END' on a separate line to finish.\n");
    
    char line[MAX_LINE_LENGTH];
    int count = 0;
    
    while (count < MAX_ENTRIES) {
        if (fgets(line, sizeof(line), stdin) == NULL) break;
        
        line[strcspn(line, "\n")] = 0;
        
        if (strcmp(line, "END") == 0) break;
        
        if (strlen(line) == 0) continue;
        
        struct LogEntry entry;
        if (parse_log_line(line, &entry)) {
            entries[count] = entry;
            count++;
        } else {
            printf("Invalid log format: %s\n", line);
        }
    }
    
    *entry_count = count;
    return 1;
}

int main(void) {
    struct LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    
    if (!read_log_file(entries, &entry_count)) {
        printf("Error reading log entries.\n");
        return 1;
    }
    
    if (entry_count == 0) {
        printf("No valid log entries found.\n");
        return 0;
    }
    
    print_log_summary(entries, entry_count);
    
    return 0;
}