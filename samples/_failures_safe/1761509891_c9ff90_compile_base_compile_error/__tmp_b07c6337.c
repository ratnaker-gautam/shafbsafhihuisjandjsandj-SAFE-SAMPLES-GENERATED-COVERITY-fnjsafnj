//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

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
    strncpy(temp_line, line, MAX_LINE_LENGTH - 1);
    temp_line[MAX_LINE_LENGTH - 1] = '\0';
    
    char* timestamp_start = strchr(temp_line, '[');
    char* timestamp_end = strchr(temp_line, ']');
    if (timestamp_start == NULL || timestamp_end == NULL) return 0;
    if (timestamp_end - timestamp_start - 1 >= 31) return 0;
    
    strncpy(entry->timestamp, timestamp_start + 1, timestamp_end - timestamp_start - 1);
    entry->timestamp[timestamp_end - timestamp_start - 1] = '\0';
    
    char* level_start = timestamp_end + 1;
    while (*level_start == ' ') level_start++;
    
    char* level_end = strchr(level_start, ':');
    if (level_end == NULL) return 0;
    if (level_end - level_start >= 15) return 0;
    
    strncpy(entry->level, level_start, level_end - level_start);
    entry->level[level_end - level_start] = '\0';
    
    char* message_start = level_end + 1;
    while (*message_start == ' ') message_start++;
    
    if (strlen(message_start) >= 255) return 0;
    strncpy(entry->message, message_start, 255);
    entry->message[255] = '\0';
    
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

void analyze_log_levels(struct LogEntry entries[], int count) {
    if (entries == NULL || count <= 0) return;
    
    int info_count = 0, warning_count = 0, error_count = 0, other_count = 0;
    
    for (int i = 0; i < count; i++) {
        if (strcmp(entries[i].level, "INFO") == 0) {
            info_count++;
        } else if (strcmp(entries[i].level, "WARNING") == 0) {
            warning_count++;
        } else if (strcmp(entries[i].level, "ERROR") == 0) {
            error_count++;
        } else {
            other_count++;
        }
    }
    
    printf("Log Level Analysis:\n");
    printf("INFO: %d entries\n", info_count);
    printf("WARNING: %d entries\n", warning_count);
    printf("ERROR: %d entries\n", error_count);
    printf("OTHER: %d entries\n", other_count);
}

void find_recent_entries(struct LogEntry entries[], int count, int max_display) {
    if (entries == NULL || count <= 0 || max_display <= 0) return;
    
    printf("\nMost Recent %d Entries:\n", max_display);
    
    int start = count - max_display;
    if (start < 0) start = 0;
    
    for (int i = start; i < count; i++) {
        printf("[%s] %s: %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
    }
}

void search_messages(struct LogEntry entries[], int count, const char* keyword) {
    if (entries == NULL || count <= 0 || keyword == NULL) return;
    
    printf("\nEntries containing '%s':\n", keyword);
    int found = 0;
    
    for (int i = 0; i < count; i++) {
        if (strstr(entries[i].message, keyword) != NULL) {
            printf("[%s] %s: %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
            found++;
        }
    }
    
    if (found == 0) {
        printf("No entries found containing '%s'\n", keyword);
    }
}

int main(void) {
    struct LogEntry entries[MAX_ENTRIES];
    char filename[256];
    char keyword[256];
    int choice;
    
    printf("Log File Analyzer\n");
    printf("Enter log file name: ");
    if (fgets(filename, sizeof(filename), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    filename[strcspn(filename, "\n")