//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 1000

struct LogEntry {
    char timestamp[20];
    char level[10];
    char message[MAX_LINE_LENGTH];
};

int parse_log_line(const char* line, struct LogEntry* entry) {
    if (line == NULL || entry == NULL) return 0;
    
    char temp_line[MAX_LINE_LENGTH];
    if (strlen(line) >= MAX_LINE_LENGTH) return 0;
    strcpy(temp_line, line);
    
    char* timestamp = strtok(temp_line, " ");
    if (timestamp == NULL) return 0;
    if (strlen(timestamp) >= 19) return 0;
    strcpy(entry->timestamp, timestamp);
    
    char* level = strtok(NULL, " ");
    if (level == NULL) return 0;
    if (strlen(level) >= 9) return 0;
    strcpy(entry->level, level);
    
    char* message = strtok(NULL, "\n");
    if (message == NULL) return 0;
    if (strlen(message) >= MAX_LINE_LENGTH) return 0;
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

void print_statistics(const struct LogEntry* entries, int count) {
    if (entries == NULL || count <= 0) return;
    
    printf("Log Statistics:\n");
    printf("Total entries: %d\n", count);
    printf("INFO entries: %d\n", count_log_level(entries, count, "INFO"));
    printf("WARN entries: %d\n", count_log_level(entries, count, "WARN"));
    printf("ERROR entries: %d\n", count_log_level(entries, count, "ERROR"));
    printf("DEBUG entries: %d\n", count_log_level(entries, count, "DEBUG"));
}

int read_log_file(const char* filename, struct LogEntry* entries, int max_entries) {
    if (filename == NULL || entries == NULL || max_entries <= 0) return 0;
    
    FILE* file = fopen(filename, "r");
    if (file == NULL) return 0;
    
    char line[MAX_LINE_LENGTH];
    int entry_count = 0;
    
    while (fgets(line, sizeof(line), file) != NULL && entry_count < max_entries) {
        line[strcspn(line, "\n")] = '\0';
        
        if (strlen(line) > 0) {
            struct LogEntry entry;
            if (parse_log_line(line, &entry)) {
                entries[entry_count] = entry;
                entry_count++;
            }
        }
    }
    
    fclose(file);
    return entry_count;
}

int main(void) {
    struct LogEntry entries[MAX_ENTRIES];
    char filename[256];
    
    printf("Enter log file name: ");
    if (fgets(filename, sizeof(filename), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    filename[strcspn(filename, "\n")] = '\0';
    
    if (strlen(filename) == 0) {
        printf("Invalid filename\n");
        return 1;
    }
    
    int entry_count = read_log_file(filename, entries, MAX_ENTRIES);
    
    if (entry_count == 0) {
        printf("No log entries found or error reading file\n");
        return 1;
    }
    
    printf("\nFirst 5 log entries:\n");
    int display_count = (entry_count < 5) ? entry_count : 5;
    for (int i = 0; i < display_count; i++) {
        printf("%s [%s] %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
    }
    
    printf("\n");
    print_statistics(entries, entry_count);
    
    return 0;
}