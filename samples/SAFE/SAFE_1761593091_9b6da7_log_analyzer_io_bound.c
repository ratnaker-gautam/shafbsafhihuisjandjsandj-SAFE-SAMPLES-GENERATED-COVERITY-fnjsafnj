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
    char message[MAX_LINE_LENGTH];
};

int parse_log_line(const char* line, struct LogEntry* entry) {
    if (line == NULL || entry == NULL) return 0;
    
    char temp[MAX_LINE_LENGTH];
    if (strlen(line) >= MAX_LINE_LENGTH) return 0;
    strcpy(temp, line);
    
    char* timestamp = strtok(temp, " ");
    char* level = strtok(NULL, " ");
    char* message = strtok(NULL, "\n");
    
    if (timestamp == NULL || level == NULL || message == NULL) return 0;
    
    if (strlen(timestamp) >= 32 || strlen(level) >= 16 || strlen(message) >= MAX_LINE_LENGTH) {
        return 0;
    }
    
    strcpy(entry->timestamp, timestamp);
    strcpy(entry->level, level);
    strcpy(entry->message, message);
    
    return 1;
}

int count_log_level(const struct LogEntry* entries, int count, const char* level) {
    int total = 0;
    for (int i = 0; i < count; i++) {
        if (strcmp(entries[i].level, level) == 0) {
            total++;
        }
    }
    return total;
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
    if (len > 0 && filename[len-1] == '\n') {
        filename[len-1] = '\0';
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
        if (strlen(line) > 0 && line[strlen(line)-1] == '\n') {
            line[strlen(line)-1] = '\0';
        }
        
        if (parse_log_line(line, &entries[entry_count])) {
            entry_count++;
        }
    }
    
    fclose(file);
    
    printf("\nLog Analysis Results:\n");
    printf("Total entries processed: %d\n", entry_count);
    printf("INFO entries: %d\n", count_log_level(entries, entry_count, "INFO"));
    printf("WARNING entries: %d\n", count_log_level(entries, entry_count, "WARNING"));
    printf("ERROR entries: %d\n", count_log_level(entries, entry_count, "ERROR"));
    printf("DEBUG entries: %d\n", count_log_level(entries, entry_count, "DEBUG"));
    
    if (entry_count > 0) {
        printf("\nFirst 5 entries:\n");
        int display_count = entry_count < 5 ? entry_count : 5;
        for (int i = 0; i < display_count; i++) {
            printf("%s [%s] %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
        }
    }
    
    return 0;
}