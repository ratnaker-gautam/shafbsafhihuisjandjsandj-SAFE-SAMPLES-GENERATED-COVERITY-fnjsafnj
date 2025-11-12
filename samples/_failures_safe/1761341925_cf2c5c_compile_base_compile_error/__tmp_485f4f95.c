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
    if (strlen(line) >= MAX_LINE_LENGTH - 1) return 0;
    strncpy(temp_line, line, MAX_LINE_LENGTH - 1);
    temp_line[MAX_LINE_LENGTH - 1] = '\0';
    
    char* timestamp_start = strchr(temp_line, '[');
    if (timestamp_start == NULL) return 0;
    
    char* timestamp_end = strchr(timestamp_start, ']');
    if (timestamp_end == NULL) return 0;
    
    size_t timestamp_len = timestamp_end - timestamp_start - 1;
    if (timestamp_len >= sizeof(entry->timestamp)) return 0;
    strncpy(entry->timestamp, timestamp_start + 1, timestamp_len);
    entry->timestamp[timestamp_len] = '\0';
    
    char* level_start = timestamp_end + 1;
    while (*level_start == ' ') level_start++;
    
    char* level_end = strchr(level_start, ':');
    if (level_end == NULL) return 0;
    
    size_t level_len = level_end - level_start;
    if (level_len >= sizeof(entry->level)) return 0;
    strncpy(entry->level, level_start, level_len);
    entry->level[level_len] = '\0';
    
    char* message_start = level_end + 1;
    while (*message_start == ' ') message_start++;
    
    size_t message_len = strlen(message_start);
    if (message_len >= sizeof(entry->message)) return 0;
    strncpy(entry->message, message_start, sizeof(entry->message) - 1);
    entry->message[sizeof(entry->message) - 1] = '\0';
    
    char* newline = strchr(entry->message, '\n');
    if (newline != NULL) *newline = '\0';
    
    return 1;
}

int count_log_level(const struct LogEntry* entries, int count, const char* level) {
    int level_count = 0;
    for (int i = 0; i < count; i++) {
        if (strcmp(entries[i].level, level) == 0) {
            level_count++;
        }
    }
    return level_count;
}

void display_summary(const struct LogEntry* entries, int count) {
    printf("Log Analysis Summary:\n");
    printf("Total entries: %d\n", count);
    printf("INFO entries: %d\n", count_log_level(entries, count, "INFO"));
    printf("WARNING entries: %d\n", count_log_level(entries, count, "WARNING"));
    printf("ERROR entries: %d\n", count_log_level(entries, count, "ERROR"));
    printf("DEBUG entries: %d\n", count_log_level(entries, count, "DEBUG"));
}

void display_entries(const struct LogEntry* entries, int count, const char* level_filter) {
    printf("\nLog Entries");
    if (level_filter != NULL) {
        printf(" (Level: %s)", level_filter);
    }
    printf(":\n");
    
    int displayed = 0;
    for (int i = 0; i < count; i++) {
        if (level_filter == NULL || strcmp(entries[i].level, level_filter) == 0) {
            printf("[%s] %s: %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
            displayed++;
        }
    }
    
    if (displayed == 0) {
        printf("No entries found.\n");
    }
}

int main(void) {
    struct LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    
    printf("Log Analyzer - Enter log lines (format: [timestamp] LEVEL: message)\n");
    printf("Enter 'END' on a separate line to finish input.\n");
    printf("Enter 'SUMMARY' to show analysis.\n");
    printf("Enter 'SHOW LEVEL' to filter by level.\n\n");
    
    char input_line[MAX_LINE_LENGTH];
    
    while (entry_count < MAX_ENTRIES) {
        printf("> ");
        
        if (fgets(input_line, sizeof(input_line), stdin) == NULL) {
            break;
        }
        
        input_line[strcspn(input_line, "\n")] = '\0';
        
        if (strlen(input_line) == 0) {
            continue;
        }
        
        if (strcmp(input_line, "END") == 0) {
            break;
        }
        
        if (strcmp(input_line, "SUMMARY") == 0) {
            display_summary(entries, entry_count);
            continue;
        }
        
        if (strncmp(input_line, "SHOW ", 5) == 0) {
            const char* level = input_line + 5;
            if (strlen(level) > 0) {
                display_entries(entries, entry_count, level);
            } else {
                printf("Please specify a level (INFO, WARNING, ERROR