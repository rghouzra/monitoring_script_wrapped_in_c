#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *exeCommand(const char *command) {
    char buffer[128];
    char *result = NULL;
    FILE *fp = popen(command, "r");
    
    if (fp == NULL) {
        perror("popen");
        exit(EXIT_FAILURE);
    }

    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        if (result == NULL) {
          size_t bufflen = strlen(buffer);
          result = calloc(bufflen, 1);
          strncpy(result, buffer, bufflen - 1);
          
        } else {
            char *temp = result;
            size_t tmplen = strlen(temp);
            size_t bufflen = strlen(buffer);
            result = calloc(tmplen + bufflen, 1);
            strncpy(result, temp, tmplen);
            strncat(result, buffer, bufflen - 1);
            free(temp);
        }
    }

    pclose(fp);
    return result;
}

int main(int ac, char **av) {
    char *os;
    os = exeCommand("uname");
  if(strcmp("Linux", os))
    {
        fprintf(stderr, "os not supported yet\n");
        free(os);
        return 1;
    }
    char *unameOutput = exeCommand("uname -a");
    char *cpuInfoOutput = exeCommand("cat /proc/cpuinfo | grep 'processor' | wc -l");
    char *ramInfoOutput = exeCommand("free -m | awk 'NR==2 {print $3}'");
    char *diskInfoOutput = exeCommand("df -Bm / | awk 'NR==2 {print $3}'");
    char *lvmInfoOutput = exeCommand("lsblk | grep lvm | wc -l");
    char *cpuLoadOutput = exeCommand("top -n1 | grep 'Cpu' | awk '{print $2 + $4}'");
    char *lastBootOutput = exeCommand("who -b | awk '{print $3 \" \" $4}'");
    char *tcpConnectionsOutput = exeCommand("cat /proc/net/sockstat | awk '{print $3}' | head -2 | tail -1");
    char *userLogOutput = exeCommand("users | wc -w");
    char *networkOutput = exeCommand("hostname -I");
    char *sudoCommandsOutput = exeCommand("journalctl _COMM=sudo -q | grep COMMAND | wc -l");

    printf("#Architecture: %s", unameOutput);
    printf("#CPU physical: %s", cpuInfoOutput);
    printf("#Memory Usage: %sMB\n", ramInfoOutput);
    printf("#Disk Usage: %sMB\n", diskInfoOutput);
    printf("#CPU load: %.1f%%\n", atof(cpuLoadOutput));
    printf("#Last boot: %s\n", lastBootOutput);
    printf("#LVM use: %s\n", atoi(lvmInfoOutput) == 0 ? "no" : "yes");
    printf("#Connections TCP: %s ESTABLISHED\n", tcpConnectionsOutput);
    printf("#User log: %s\n", userLogOutput);
    printf("#Network: IP %s\n", networkOutput);
    printf("#Sudo: %s cmd\n", sudoCommandsOutput);

    free(os);
    free(unameOutput);
    free(cpuInfoOutput);
    free(ramInfoOutput);
    free(diskInfoOutput);
    free(lvmInfoOutput);
    free(cpuLoadOutput);
    free(lastBootOutput);
    free(tcpConnectionsOutput);
    free(userLogOutput);
    free(networkOutput);
    free(sudoCommandsOutput);
    return 0;
}

