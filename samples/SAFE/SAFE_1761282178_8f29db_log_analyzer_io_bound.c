//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
    strncpy(temp_line, line, MAX_LINE_LENGTH - 1);
    temp_line[MAX_LINE_LENGTH - 1] = '\0';
    
    char* timestamp = strtok(temp_line, " ");
    char* level = strtok(NULL, " ");
    char* message = strtok(NULL, "\n");
    
    if (timestamp == NULL || level == NULL || message == NULL) return 0;
    
    if (strlen(timestamp) >= sizeof(entry->timestamp)) return 0;
    if (strlen(level) >= sizeof(entry->level)) return 0;
    if (strlen(message) >= sizeof(entry->message)) return 0;
    
    strcpy(entry->timestamp, timestamp);
    strcpy(entry->level, level);
    strcpy(entry->message, message);
    
    return 1;
}

int is_valid_level(const char* level) {
    const char* valid_levels[] = {"INFO", "WARN", "ERROR", "DEBUG"};
    for (int i = 0; i < 4; i++) {
        if (strcmp(level, valid_levels[i]) == 0) return 1;
    }
    return 0;
}

void analyze_logs(struct LogEntry* entries, int count) {
    int level_counts[4] = {0};
    const char* level_names[] = {"INFO", "WARN", "ERROR", "DEBUG"};
    
    for (int i = 0; i < count; i++) {
        for (int j = 0; j < 4; j++) {
            if (strcmp(entries[i].level, level_names[j]) == 0) {
                level_counts[j]++;
                break;
            }
        }
    }
    
    printf("\nLog Analysis Results:\n");
    printf("====================\n");
    for (int i = 0; i < 4; i++) {
        printf("%s: %d entries\n", level_names[i], level_counts[i]);
    }
    
    printf("\nRecent ERROR entries:\n");
    printf("====================\n");
    int error_count = 0;
    for (int i = count - 1; i >= 0 && error_count < 5; i--) {
        if (strcmp(entries[i].level, "ERROR") == 0) {
            printf("%s %s %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
            error_count++;
        }
    }
    if (error_count == 0) {
        printf("No ERROR entries found.\n");
    }
}

int main() {
    struct LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    char filename[256];
    
    printf("Enter log file name: ");
    if (fgets(filename, sizeof(filename), stdin) == NULL) {
        fprintf(stderr, "Error reading filename\n");
        return 1;
    }
    
    size_t len = strlen(filename);
    if (len > 0 && filename[len - 1] == '\n') {
        filename[len - 1] = '\0';
    }
    
    if (strlen(filename) == 0) {
        fprintf(stderr, "Empty filename\n");
        return 1;
    }
    
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Cannot open file: %s\n", filename);
        return 1;
    }
    
    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), file) != NULL && entry_count < MAX_ENTRIES) {
        line[MAX_LINE_LENGTH - 1] = '\0';
        
        struct LogEntry entry;
        if (parse_log_line(line, &entry)) {
            if (is_valid_level(entry.level)) {
                entries[entry_count] = entry;
                entry_count++;
            }
        }
    }
    
    if (ferror(file)) {
        fprintf(stderr, "Error reading file\n");
        fclose(file);
        return 1;
    }
    
    fclose(file);
    
    if (entry_count == 0) {
        printf("No valid log entries found.\n");
        return 0;
    }
    
    printf("Successfully parsed %d log entries.\n", entry_count);
    analyze_logs(entries, entry_count);
    
    return 0;
}