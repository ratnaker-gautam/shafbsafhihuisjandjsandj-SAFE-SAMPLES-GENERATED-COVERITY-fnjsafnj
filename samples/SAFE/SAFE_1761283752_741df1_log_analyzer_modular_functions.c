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
    
    char temp[MAX_LINE_LENGTH];
    if (strlen(line) >= MAX_LINE_LENGTH) return 0;
    strcpy(temp, line);
    
    char* timestamp = strtok(temp, " ");
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
    if (entries == NULL || level == NULL || count <= 0 || count > MAX_ENTRIES) return 0;
    
    int level_count = 0;
    for (int i = 0; i < count; i++) {
        if (strcmp(entries[i].level, level) == 0) {
            level_count++;
        }
    }
    return level_count;
}

void print_summary(const struct LogEntry* entries, int count) {
    if (entries == NULL || count <= 0 || count > MAX_ENTRIES) return;
    
    printf("Log Analysis Summary:\n");
    printf("Total entries: %d\n", count);
    printf("INFO entries: %d\n", count_log_level(entries, count, "INFO"));
    printf("WARN entries: %d\n", count_log_level(entries, count, "WARN"));
    printf("ERROR entries: %d\n", count_log_level(entries, count, "ERROR"));
    printf("DEBUG entries: %d\n", count_log_level(entries, count, "DEBUG"));
}

int read_log_file(const char* filename, struct LogEntry* entries, int* count) {
    if (filename == NULL || entries == NULL || count == NULL) return 0;
    
    FILE* file = fopen(filename, "r");
    if (file == NULL) return 0;
    
    char line[MAX_LINE_LENGTH];
    *count = 0;
    
    while (fgets(line, sizeof(line), file) != NULL && *count < MAX_ENTRIES) {
        line[strcspn(line, "\n")] = '\0';
        
        if (strlen(line) > 0) {
            if (parse_log_line(line, &entries[*count])) {
                (*count)++;
            }
        }
    }
    
    fclose(file);
    return 1;
}

int main(void) {
    struct LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    
    printf("Enter log filename: ");
    char filename[256];
    if (fgets(filename, sizeof(filename), stdin) == NULL) {
        printf("Error reading filename\n");
        return 1;
    }
    
    filename[strcspn(filename, "\n")] = '\0';
    
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
        return 1;
    }
    
    print_summary(entries, entry_count);
    
    printf("\nFirst 5 log entries:\n");
    int display_count = (entry_count < 5) ? entry_count : 5;
    for (int i = 0; i < display_count; i++) {
        printf("%s %s %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
    }
    
    return 0;
}