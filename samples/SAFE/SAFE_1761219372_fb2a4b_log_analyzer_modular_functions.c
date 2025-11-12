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

int read_log_file(const char* filename, struct LogEntry entries[], int max_entries) {
    if (filename == NULL || entries == NULL || max_entries <= 0) return 0;
    
    FILE* file = fopen(filename, "r");
    if (file == NULL) return 0;
    
    char line[MAX_LINE_LENGTH];
    int count = 0;
    
    while (fgets(line, sizeof(line), file) != NULL && count < max_entries) {
        line[strcspn(line, "\n")] = '\0';
        if (parse_log_line(line, &entries[count])) {
            count++;
        }
    }
    
    fclose(file);
    return count;
}

void count_log_levels(const struct LogEntry entries[], int count, int level_counts[]) {
    if (entries == NULL || level_counts == NULL || count <= 0) return;
    
    for (int i = 0; i < 4; i++) level_counts[i] = 0;
    
    for (int i = 0; i < count; i++) {
        if (strcmp(entries[i].level, "INFO") == 0) level_counts[0]++;
        else if (strcmp(entries[i].level, "WARN") == 0) level_counts[1]++;
        else if (strcmp(entries[i].level, "ERROR") == 0) level_counts[2]++;
        else level_counts[3]++;
    }
}

void display_statistics(const struct LogEntry entries[], int count) {
    if (entries == NULL || count <= 0) {
        printf("No log entries to analyze.\n");
        return;
    }
    
    int level_counts[4] = {0};
    count_log_levels(entries, count, level_counts);
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", count);
    printf("INFO: %d\n", level_counts[0]);
    printf("WARN: %d\n", level_counts[1]);
    printf("ERROR: %d\n", level_counts[2]);
    printf("OTHER: %d\n", level_counts[3]);
    
    if (count > 0) {
        printf("\nFirst 5 entries:\n");
        int display_count = (count < 5) ? count : 5;
        for (int i = 0; i < display_count; i++) {
            printf("%d. [%s] %s: %s\n", i+1, entries[i].timestamp, entries[i].level, entries[i].message);
        }
    }
}

int main(void) {
    struct LogEntry entries[MAX_ENTRIES];
    char filename[256];
    
    printf("Enter log file name: ");
    if (fgets(filename, sizeof(filename), stdin) == NULL) {
        printf("Error reading input.\n");
        return 1;
    }
    
    filename[strcspn(filename, "\n")] = '\0';
    
    if (strlen(filename) == 0) {
        printf("No filename provided.\n");
        return 1;
    }
    
    int entry_count = read_log_file(filename, entries, MAX_ENTRIES);
    
    if (entry_count == 0) {
        printf("Could not read log file or file is empty.\n");
        return 1;
    }
    
    display_statistics(entries, entry_count);
    
    return 0;
}