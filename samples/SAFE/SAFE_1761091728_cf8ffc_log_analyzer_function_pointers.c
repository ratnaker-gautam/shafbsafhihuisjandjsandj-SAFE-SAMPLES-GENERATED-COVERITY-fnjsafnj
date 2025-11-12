//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: function_pointers ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 1000

typedef struct {
    char timestamp[20];
    char level[10];
    char message[MAX_LINE_LENGTH];
} LogEntry;

typedef void (*LogProcessor)(LogEntry*);

void count_errors(LogEntry* entry) {
    static int error_count = 0;
    if (strcmp(entry->level, "ERROR") == 0) {
        error_count++;
        printf("Error count: %d\n", error_count);
    }
}

void print_warnings(LogEntry* entry) {
    if (strcmp(entry->level, "WARN") == 0) {
        printf("Warning: %s - %s\n", entry->timestamp, entry->message);
    }
}

void process_log_file(const char* filename, LogProcessor processor) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error: Cannot open file %s\n", filename);
        return;
    }

    char line[MAX_LINE_LENGTH];
    int line_count = 0;

    while (fgets(line, sizeof(line), file) && line_count < MAX_ENTRIES) {
        if (strlen(line) == 0) continue;

        LogEntry entry;
        memset(&entry, 0, sizeof(entry));

        int parsed = sscanf(line, "%19s %9s %1023[^\n]", 
                           entry.timestamp, entry.level, entry.message);
        
        if (parsed == 3) {
            for (int i = 0; i < strlen(entry.level); i++) {
                entry.level[i] = toupper(entry.level[i]);
            }
            processor(&entry);
            line_count++;
        }
    }

    if (fclose(file) != 0) {
        fprintf(stderr, "Error closing file\n");
    }
}

int validate_filename(const char* filename) {
    if (!filename || strlen(filename) == 0 || strlen(filename) > 255) {
        return 0;
    }
    
    for (int i = 0; i < strlen(filename); i++) {
        if (!isalnum(filename[i]) && filename[i] != '.' && filename[i] != '-' && filename[i] != '_') {
            return 0;
        }
    }
    return 1;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <logfile>\n", argv[0]);
        return 1;
    }

    if (!validate_filename(argv[1])) {
        fprintf(stderr, "Error: Invalid filename\n");
        return 1;
    }

    printf("Processing log file: %s\n", argv[1]);
    printf("\nError Statistics:\n");
    process_log_file(argv[1], count_errors);
    
    printf("\nWarning Messages:\n");
    process_log_file(argv[1], print_warnings);

    return 0;
}