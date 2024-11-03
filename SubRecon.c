#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>

#define MAX_LINE_LENGTH 1024

int main() {
    char domain_file[256];
    char command[1024];
    time_t start_time;
    struct tm *start_tm;

    // Get the current time
    time(&start_time);

    // Get the broken-down time
    start_tm = localtime(&start_time);

    // Print a message indicating when the tool was started
    printf("Tool started at: %02d:%02d:%02d\n", start_tm->tm_hour, start_tm->tm_min, start_tm->tm_sec);
    printf("Running...\n");

    // Ask user to enter a file for domain
    printf("Enter a file for domain: ");
    fgets(domain_file, 256, stdin);
    domain_file[strcspn(domain_file, "\n")] = 0; // remove newline character

    // Run assetfinder and subfinder in parallel
    pid_t pid1, pid2;
    if ((pid1 = fork()) == 0) {
        // Child process 1: run assetfinder
        printf("Assetfinder started at: %02d:%02d:%02d\n", start_tm->tm_hour, start_tm->tm_min, start_tm->tm_sec);
        sprintf(command, "cat %s | assetfinder -subs-only >> temp_assetfinder.txt", domain_file);
        system(command);
        exit(0);
    } else if ((pid2 = fork()) == 0) {
        // Child process 2: run subfinder
        printf("Subfinder started at: %02d:%02d:%02d\n", start_tm->tm_hour, start_tm->tm_min, start_tm->tm_sec);
        sprintf(command, "cat %s | subfinder  -all -silent >> temp_subfinder.txt", domain_file);
        system(command);
        exit(0);
    }

    // Wait for both child processes to finish
    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);

    // Merge the output of assetfinder and subfinder
    FILE *fp1 = fopen("temp_assetfinder.txt", "r");
    FILE *fp2 = fopen("temp_subfinder.txt", "r");
    FILE *fp3 = fopen("Subdomain.txt", "w");
    char line[MAX_LINE_LENGTH];

    while (fgets(line, MAX_LINE_LENGTH, fp1)) {
        fputs(line, fp3);
    }
    while (fgets(line, MAX_LINE_LENGTH, fp2)) {
        fputs(line, fp3);
    }

    fclose(fp1);
    fclose(fp2);
    fclose(fp3);

    // Remove temporary files
    remove("temp_assetfinder.txt");
    remove("temp_subfinder.txt");

    // Run httpx on Subdomain.txt file
    printf("Httpx started at: %02d:%02d:%02d\n", start_tm->tm_hour, start_tm->tm_min, start_tm->tm_sec);
    sprintf(command, "cat Subdomain.txt | httpx -silent >> Alive-domain.txt");
    system(command);

    // Run waybackurls and naabu in parallel on Alive-domain.txt
    pid_t pid3, pid4;
    if ((pid3 = fork()) == 0) {
        // Child process 3: run waybackurls
        printf("Waybackurls started at: %02d:%02d:%02d\n", start_tm->tm_hour, start_tm->tm_min, start_tm->tm_sec);
        sprintf(command, "cat Alive-domain.txt | waybackurls >> waybackurls.txt");
        system(command);
        exit(0);
    } else if ((pid4 = fork()) == 0) {
        // Child process 4: run naabu
        printf("Naabu started at: %02d:%02d:%02d\n", start_tm->tm_hour, start_tm->tm_min, start_tm->tm_sec);
        sprintf(command, "cat Alive-domain.txt | dnsx -silent -a -ro | naabu -silent  -top-ports 1000 -exclude-ports 80,443,21,25 -o naabu.txt");
        system(command);
        exit(0);
    }

    // Wait for both child processes to finish
    waitpid(pid3, NULL, 0);
    waitpid(pid4, NULL, 0);

    return 0;
}
