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
    char message[256];
};

int parse_log_line(const char* line, struct LogEntry* entry) {
    if (line == NULL || entry == NULL) return 0;
    
    char temp_line[MAX_LINE_LENGTH];
    if (strlen(line) >= MAX_LINE_LENGTH - 1) return 0;
    strcpy(temp_line, line);
    
    char* timestamp = strtok(temp_line, " ");
    char* level = strtok(NULL, " ");
    char* message = strtok(NULL, "\n");
    
    if (timestamp == NULL || level == NULL || message == NULL) return 0;
    
    if (strlen(timestamp) >= 31) return 0;
    if (strlen(level) >= 15) return 0;
    if (strlen(message) >= 255) return 0;
    
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
        
        if (strlen(line) > 0) {
            struct LogEntry entry;
            if (parse_log_line(line, &entry)) {
                entries[count] = entry;
                count++;
            }
        }
    }
    
    fclose(file);
    return count;
}

void analyze_log_levels(struct LogEntry entries[], int count) {
    if (entries == NULL || count <= 0) return;
    
    int error_count = 0;
    int warning_count = 0;
    int info_count = 0;
    
    for (int i = 0; i < count; i++) {
        if (strcmp(entries[i].level, "ERROR") == 0) {
            error_count++;
        } else if (strcmp(entries[i].level, "WARNING") == 0) {
            warning_count++;
        } else if (strcmp(entries[i].level, "INFO") == 0) {
            info_count++;
        }
    }
    
    printf("Log Level Analysis:\n");
    printf("ERROR: %d entries\n", error_count);
    printf("WARNING: %d entries\n", warning_count);
    printf("INFO: %d entries\n", info_count);
    printf("Total: %d entries\n", count);
}

void search_log_messages(struct LogEntry entries[], int count, const char* keyword) {
    if (entries == NULL || count <= 0 || keyword == NULL) return;
    
    printf("Search results for '%s':\n", keyword);
    int found = 0;
    
    for (int i = 0; i < count; i++) {
        if (strstr(entries[i].message, keyword) != NULL) {
            printf("[%s] %s: %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
            found = 1;
        }
    }
    
    if (!found) {
        printf("No entries found containing '%s'\n", keyword);
    }
}

void display_recent_entries(struct LogEntry entries[], int count, int num_entries) {
    if (entries == NULL || count <= 0 || num_entries <= 0) return;
    
    if (num_entries > count) num_entries = count;
    
    printf("Most recent %d entries:\n", num_entries);
    int start = count - num_entries;
    if (start < 0) start = 0;
    
    for (int i = start; i < count; i++) {
        printf("[%s] %s: %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
    }
}

int main() {
    struct LogEntry entries[MAX_ENTRIES];
    char filename[256];
    char keyword[256];
    int choice;
    
    printf("Log Analyzer\n");
    printf("Enter log filename: ");
    if (fgets(filename, sizeof(filename), stdin) == NULL) {
        printf("Error reading filename\n");
        return 1;
    }
    filename[strcspn(filename, "\n")] = '\0';
    
    int entry_count = read_log_file(filename, entries, MAX_ENTRIES);
    if (entry_count == 0) {
        printf("Error: Could not read log file or file is empty\n");
        return 1;
    }
    
    printf("Successfully loaded %d log entries\n\n", entry_count);
    
    while (1) {
        printf("1. Analyze log levels\n");
        printf("2. Search messages\n");
        printf("3. Show recent entries\n");
        printf("4. Exit\n");
        printf("Choose option: ");
        
        if (scanf("%d", &choice) != 1) {
            printf("Invalid