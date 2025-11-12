//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: log_analyzer
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

int parse_log_line(const char* line, struct LogEntry* entry) {
    if (line == NULL || entry == NULL) return 0;
    
    char temp_line[MAX_LINE_LENGTH];
    if (strlen(line) >= MAX_LINE_LENGTH) return 0;
    strcpy(temp_line, line);
    
    char* timestamp = strtok(temp_line, " ");
    if (timestamp == NULL) return 0;
    if (strlen(timestamp) >= sizeof(entry->timestamp)) return 0;
    strcpy(entry->timestamp, timestamp);
    
    char* level = strtok(NULL, " ");
    if (level == NULL) return 0;
    if (strlen(level) >= sizeof(entry->level)) return 0;
    strcpy(entry->level, level);
    
    char* message = strtok(NULL, "\n");
    if (message == NULL) return 0;
    if (strlen(message) >= sizeof(entry->message)) return 0;
    strcpy(entry->message, message);
    
    return 1;
}

int count_log_level(const struct LogEntry* entries, int count, const char* level) {
    if (entries == NULL || level == NULL || count <= 0) return 0;
    
    int level_count = 0;
    for (int i = 0; i < count; i++) {
        if (strcmp(entries[i].level, level) == 0) {
            level_count++;
        }
    }
    return level_count;
}

void print_summary(const struct LogEntry* entries, int count) {
    if (entries == NULL || count <= 0) return;
    
    printf("Log Analysis Summary:\n");
    printf("Total entries: %d\n", count);
    printf("INFO entries: %d\n", count_log_level(entries, count, "INFO"));
    printf("WARNING entries: %d\n", count_log_level(entries, count, "WARNING"));
    printf("ERROR entries: %d\n", count_log_level(entries, count, "ERROR"));
    printf("DEBUG entries: %d\n", count_log_level(entries, count, "DEBUG"));
}

int read_log_file(struct LogEntry* entries, int max_entries) {
    if (entries == NULL || max_entries <= 0) return 0;
    
    char line[MAX_LINE_LENGTH];
    int entry_count = 0;
    
    printf("Enter log entries (format: TIMESTAMP LEVEL MESSAGE):\n");
    printf("Enter 'END' on a new line to finish input.\n");
    
    while (entry_count < max_entries) {
        if (fgets(line, sizeof(line), stdin) == NULL) break;
        
        line[strcspn(line, "\n")] = 0;
        
        if (strcmp(line, "END") == 0) break;
        
        if (strlen(line) == 0) continue;
        
        if (parse_log_line(line, &entries[entry_count])) {
            entry_count++;
        } else {
            printf("Invalid log format: %s\n", line);
        }
    }
    
    return entry_count;
}

void display_entries_by_level(const struct LogEntry* entries, int count, const char* level) {
    if (entries == NULL || level == NULL || count <= 0) return;
    
    printf("\n%s entries:\n", level);
    int found = 0;
    for (int i = 0; i < count; i++) {
        if (strcmp(entries[i].level, level) == 0) {
            printf("%s %s %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
            found = 1;
        }
    }
    if (!found) {
        printf("No %s entries found.\n", level);
    }
}

int main(void) {
    struct LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    
    printf("Log Analyzer\n");
    printf("============\n\n");
    
    entry_count = read_log_file(entries, MAX_ENTRIES);
    
    if (entry_count == 0) {
        printf("No valid log entries to analyze.\n");
        return 1;
    }
    
    print_summary(entries, entry_count);
    
    printf("\nFilter by level (INFO/WARNING/ERROR/DEBUG): ");
    char level_filter[16];
    if (fgets(level_filter, sizeof(level_filter), stdin) != NULL) {
        level_filter[strcspn(level_filter, "\n")] = 0;
        for (char* p = level_filter; *p; p++) {
            *p = toupper((unsigned char)*p);
        }
        display_entries_by_level(entries, entry_count, level_filter);
    }
    
    return 0;
}