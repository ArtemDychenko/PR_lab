# include <unistd.h>
# include <stdio.h>
int main () {
    int N;
    printf("Wprowadź liczbę poziomów");
    scanf("%d", &N);
    /* utworz proces potomny */
    int status;

    for(int i = 0; i < N; i++) {
        if (i % 2 == 0) { // Tworzenie dwóch podprocesów co drugi raz
            int pid1 = fork();
            int pid2;
          
            if (pid1 < 0) {
                printf("Błąd podczas tworzenia pierwszego procesu potomnego na poziomie %d\n", i);
                return 1;
            } 
            else if (pid1 == 0) {
                printf("Proces potomny pid: %d ppid: %d\n", getpid(), getppid());
                sleep(1);
                printf("Na poziomie %d:\n", i);
                 // Proces potomny kończy działanie po wyświetleniu informacji
            }
            else {
                pid2 = fork(); // Tworzenie drugiego procesu potomnego
                if (pid2 < 0) {
                    printf("Błąd podczas tworzenia drugiego procesu potomnego na poziomie %d\n", i);
                    return 1;
                }
                if (pid2 == 0) {
                    printf("Proces potomny pid: %d ppid: %d\n", getpid(), getppid());
                    sleep(1);
                    printf("Na poziomie %d:\n", i);
                    // Proces potomny kończy działanie po wyświetleniu informacji
                }
                else {
                    waitpid(pid1, &status, 0);
                    waitpid(pid2, &status, 0);
                    printf("Procesy potomne %d i %d zakończone\n", pid1, pid2);
                    return 0;
                }
                
                return 0;
            }
            
        }
        else { // Tworzenie tylko jednego procesu potomnego w przypadku nieparzystego poziomu
            int pid = fork();
          
            if (pid < 0) {
                printf("Błąd podczas tworzenia procesu potomnego na poziomie %d\n", i);
                return 1;
            } 
            else if (pid == 0) {
                printf("Proces potomny pid: %d ppid: %d\n", getpid(), getppid());
                sleep(1);
                printf("Na poziomie %d:\n", i);
                // Proces potomny kończy działanie po wyświetleniu informacji
            }
            else {
                waitpid(pid, &status, 0);
                printf("Proces potomny %d zakończony\n", pid);
                return 0;
            }
        }
    }

    execlp("ps","-u artem_dychenko", "--forest", NULL); // Wywołanie polecenia ps
    return 0;
}