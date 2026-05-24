#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

// Parent'in child PID'sine alarm handler icinden ulasabilmesi icin global yaptik
pid_t child_pid = -1;

// Child process icin SIGINT yakalayici fonksiyon
void child_sigint_handler(int sig) {
    // Sinyal parametresini kullanmadigimiz icin warning vermesin diye yazdik
    (void)sig; 
    printf("\n[Child %d] SIGINT alindi ancak devam ediliyor...\n", getpid());
}

// Child process icin SIGCONT yakalayici fonksiyon
void child_sigcont_handler(int sig) {
    (void)sig; 
    printf("\n[Child %d] Islem devam etti\n", getpid());
}

// Parent process icin alarm sinyali yakalayici fonksiyon
void parent_alarm_handler(int sig) {
    (void)sig; 
    
    if (child_pid > 0) {
        printf("\n[Parent %d] Alarm tetiklendi! Child'a SIGSTOP yollaniyor...\n", getpid());
        kill(child_pid, SIGSTOP); // Child'i durdur
        
        printf("[Parent %d] 2 saniye bekleniyor...\n", getpid());
        sleep(2); // Parent burada 2 saniye uyur
        
        printf("[Parent %d] Child'a SIGCONT yollaniyor...\n", getpid());
        kill(child_pid, SIGCONT); // Child'i kaldigi yerden devam ettir
        
        // Dongunun devam etmesi icin sonraki alarmi kuruyoruz
        alarm(3);
    }
}

int main() {
    printf("Ana surec (Parent) basladi. PID: %d\n", getpid());
    
    // Child process olusturuyoruz
    child_pid = fork();
    
    if (child_pid < 0) { 
        perror("fork basarisiz");
        return 1;
    }
    
    if (child_pid == 0) {
        // --- CHILD PROCESS ---
        
        // Child'in kendi sinyal yakalayicilarini ayarliyoruz
        signal(SIGINT, child_sigint_handler);
        signal(SIGCONT, child_sigcont_handler);
        
        int counter = 1;
        printf("[Child %d] basladi. Sonsuz donguye giriyor...\n", getpid());
        
        // Her saniye sayaci ekrana yazdiran sonsuz dongu
        while(1) {
            printf("[Child %d] Calisiyor... Sayac: %d\n", getpid(), counter++);
            sleep(1); 
        }
    } 
    else {
        // --- PARENT PROCESS ---
        
        // Parent icin alarm sinyali handler'ini atiyoruz
        signal(SIGALRM, parent_alarm_handler);
        
        // Ilk alarmi 3 saniye sonrasina kuruyoruz
        alarm(3);
        
        int time_left = 10;
        printf("[Parent %d] 10 saniyelik zamanlayici basladi.\n", getpid());
        
        // 10 saniye boyunca bekle. Eger sinyal gelip sleep'i keserse kalan sureyi dondurur.
        while (time_left > 0) {
            time_left = sleep(time_left);
        }
        
        printf("\n[Parent %d] 10 saniye doldu!\n", getpid());
        
        // Child'a SIGINT gonderiyoruz (kapanmayacak, sadece mesaj verecek)
        printf("[Parent %d] Child'a SIGINT gonderiliyor...\n", getpid());
        kill(child_pid, SIGINT);
        
        // Child'in ekrana mesaji yazmasi icin ufak bir bekleme
        sleep(1);
        
        // Child'i gercekten kapatmak icin SIGTERM kullaniyoruz
        printf("[Parent %d] Child sonlandiriliyor (SIGTERM)...\n", getpid());
        kill(child_pid, SIGTERM);
        
        // Zombi process olmamasi icin child'in tamamen kapanmasini bekliyoruz
        wait(NULL);
        printf("[Parent %d] Child kapandi. Zombi process engellendi.\n", getpid());
        
        printf("[Parent %d] Program bitti.\n", getpid());
    }
    
    return 0;
}
