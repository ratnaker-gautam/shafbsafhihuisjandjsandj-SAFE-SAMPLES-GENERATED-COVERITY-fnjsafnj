//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: log_analyzer
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

int main(void) {
    struct LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    int error_count = 0;
    int warning_count = 0;
    int info_count = 0;
    
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
        fprintf(stderr, "Filename cannot be empty\n");
        return 1;
    }
    
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Cannot open file: %s\n", filename);
        return 1;
    }
    
    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), file) != NULL && entry_count < MAX_ENTRIES) {
        size_t line_len = strlen(line);
        if (line_len > 0 && line[line_len - 1] == '\n') {
            line[line_len - 1] = '\0';
        }
        
        if (line_len > 1) {
            struct LogEntry entry;
            if (parse_log_line(line, &entry)) {
                entries[entry_count] = entry;
                entry_count++;
                
                if (strcmp(entry.level, "ERROR") == 0) {
                    error_count++;
                } else if (strcmp(entry.level, "WARNING") == 0) {
                    warning_count++;
                } else if (strcmp(entry.level, "INFO") == 0) {
                    info_count++;
                }
            }
        }
    }
    
    if (ferror(file)) {
        fprintf(stderr, "Error reading file\n");
        fclose(file);
        return 1;
    }
    
    fclose(file);
    
    printf("\nLog Analysis Results:\n");
    printf("Total entries: %d\n", entry_count);
    printf("ERROR entries: %d\n", error_count);
    printf("WARNING entries: %d\n", warning_count);
    printf("INFO entries: %d\n", info_count);
    
    if (entry_count > 0) {
        printf("\nSample entries:\n");
        int sample_count = (entry_count < 5) ? entry_count : 5;
        for (int i = 0; i < sample_count; i++) {
            printf("%s [%s] %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
        }
    }
    
    return 0;
}