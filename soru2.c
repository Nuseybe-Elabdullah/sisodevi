#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>

int sayac = 1;
pid_t pid; 

// sinyal yakalama kısmı
void child_sinyal_yakala(int sig) {
    if (sig == SIGINT) {
        printf("SIGINT alindi ancak devam ediliyor...\n");
    } 
    else if (sig == SIGCONT) {
        printf("Islem devam etti\n");
    }
}

// alarm gelince çalışacak yer
void parent_alarm_yakala(int sig) {
    if (sig == SIGALRM) {
        // child process'i durduruyoruz
        kill(pid, SIGSTOP);
        
        // 2 saniye bekliyoruz
        sleep(2);
        
        // tekrar devam ettiriyoruz
        kill(pid, SIGCONT);
        
        // 3 saniye sonra tekrar alarm çalması için kuruyoruz
        alarm(3);
    }
}

int main() {
    
    // fork ile child process yaratıyoruz
    pid = fork();
    
    if (pid < 0) {
        printf("Fork hatasi!\n");
        return 1;
    }
    
    // çocuk süreç
    if (pid == 0) {
        // sinyalleri yakalayacağımız fonksiyonu signal ile atıyoruz
        signal(SIGINT, child_sinyal_yakala);
        signal(SIGCONT, child_sinyal_yakala);
        
        // sonsuz döngü
        while(1) {
            printf("Sayac: %d\n", sayac);
            sayac++;
            sleep(1); // 1 saniye bekletiyoruz
        }
    } 
    // ebeveyn süreç
    else {
        // alarm sinyalini yakala
        signal(SIGALRM, parent_alarm_yakala);
        
        // ilk alarmı 3 saniyeye kuruyoruz
        alarm(3);
        
        // 10 saniye uyuyarak çalışmasını izliyoruz
        sleep(10);
        
        // süremiz doldu, SIGINT atıyoruz
        kill(pid, SIGINT);
        
        // yazının çıkması için kısa bekleme
        sleep(1);
        
        // en son tamamen sonlandırıp çıkıyoruz
        kill(pid, SIGKILL);
        
        printf("Program bitti.\n");
        return 0;
    }
    
    return 0;
}
