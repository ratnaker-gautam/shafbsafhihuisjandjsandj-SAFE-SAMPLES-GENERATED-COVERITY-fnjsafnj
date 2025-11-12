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
    
    char temp[MAX_LINE_LENGTH];
    if (strlen(line) >= MAX_LINE_LENGTH) return 0;
    strcpy(temp, line);
    
    char* timestamp = strtok(temp, " ");
    char* level = strtok(NULL, " ");
    char* message = strtok(NULL, "\n");
    
    if (timestamp == NULL || level == NULL || message == NULL) return 0;
    
    if (strlen(timestamp) >= 32 || strlen(level) >= 16 || strlen(message) >= MAX_LINE_LENGTH) return 0;
    
    strcpy(entry->timestamp, timestamp);
    strcpy(entry->level, level);
    strcpy(entry->message, message);
    
    return 1;
}

int count_log_level(const struct LogEntry* entries, int count, const char* level) {
    if (entries == NULL || level == NULL || count <= 0 || count > MAX_ENTRIES) return 0;
    
    int total = 0;
    for (int i = 0; i < count; i++) {
        if (strcmp(entries[i].level, level) == 0) {
            total++;
        }
    }
    return total;
}

void print_statistics(const struct LogEntry* entries, int count) {
    if (entries == NULL || count <= 0 || count > MAX_ENTRIES) return;
    
    printf("Log Statistics:\n");
    printf("Total entries: %d\n", count);
    printf("INFO entries: %d\n", count_log_level(entries, count, "INFO"));
    printf("WARNING entries: %d\n", count_log_level(entries, count, "WARNING"));
    printf("ERROR entries: %d\n", count_log_level(entries, count, "ERROR"));
}

int read_log_file(const char* filename, struct LogEntry* entries, int* count) {
    if (filename == NULL || entries == NULL || count == NULL) return 0;
    
    FILE* file = fopen(filename, "r");
    if (file == NULL) return 0;
    
    char line[MAX_LINE_LENGTH];
    *count = 0;
    
    while (fgets(line, sizeof(line), file) != NULL && *count < MAX_ENTRIES) {
        line[strcspn(line, "\n")] = '\0';
        
        if (parse_log_line(line, &entries[*count])) {
            (*count)++;
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
    
    print_statistics(entries, entry_count);
    
    printf("\nSearch for entries with specific level (INFO/WARNING/ERROR): ");
    char search_level[16];
    
    if (fgets(search_level, sizeof(search_level), stdin) == NULL) {
        return 1;
    }
    
    search_level[strcspn(search_level, "\n")] = '\0';
    
    if (strlen(search_level) > 0) {
        printf("\nEntries with level '%s':\n", search_level);
        int found = 0;
        for (int i = 0; i < entry_count; i++) {
            if (strcmp(entries[i].level, search_level) == 0) {
                printf("%s %s %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
                found = 1;
            }
        }
        if (!found) {
            printf("No entries found with level '%s'\n", search_level);
        }
    }
    
    return 0;
}