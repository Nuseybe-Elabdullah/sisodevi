#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>

// global değişkenler
int sayac = 0;
pid_t pid; 

// child sürecin sinyal yakalama fonksiyonu
void child_sinyal_yakala(int sig) {
    if (sig == SIGINT) {
        printf("SIGINT alindi ancak devam ediliyor...\n");
    } 
    else if (sig == SIGCONT) {
        printf("Cocuk: Islem yeniden baslatildi\n");
    }
}

// parent sürecin alarm gelince çalışacak fonksiyonu
void parent_alarm_yakala(int sig) {
    if (sig == SIGALRM) {
        printf("Ebeveyn: Cocuk durduruluyor...\n");
        // child'a SIGSTOP gönderip durduruyoruz
        kill(pid, SIGSTOP);
        
        // 2 saniye bekliyoruz
        sleep(2);
        
        printf("Ebeveyn: Cocuk devam ediyor...\n");
        // child'a SIGCONT gönderip tekrar başlatıyoruz
        kill(pid, SIGCONT);
        
        // sonraki alarm için tekrar kuruyoruz (3 saniye)
        alarm(3);
    }
}

int main() {
    
    // fork ile çocuk süreç oluşturuyoruz
    pid = fork();
    
    if (pid < 0) {
        printf("Fork basarisiz oldu!\n");
        return 1;
    }
    
    // Child process kısmı
    if (pid == 0) {
        // child kendine gelen sinyalleri yakalayacak
        signal(SIGINT, child_sinyal_yakala);
        signal(SIGCONT, child_sinyal_yakala);
        
        // sonsuz döngüde sayacı yazdırıyoruz
        while(1) {
            printf("Cocuk sayaci: %d\n", sayac);
            sayac++;
            sleep(1); // her saniye bir artıyor
        }
    } 
    // Parent process kısmı
    else {
        // parent alarm sinyalini yakalayacak
        signal(SIGALRM, parent_alarm_yakala);
        
        // ilk alarmı kuruyoruz
        alarm(3);
        
        // yaklaşık 10 saniye bekle
        sleep(10);
        
        // programı bitirmeden önce child'a SIGINT gönderiyoruz
        printf("Ebeveyn: SIGINT gonderiliyor...\n");
        kill(pid, SIGINT);
        
        // mesajın gitmesi için azıcık bekleyelim
        sleep(1);
        
        // en son child'i tamamen sonlandırıyoruz (SIGKILL kullanarak öldürüyoruz)
        kill(pid, SIGKILL);
        printf("Ebeveyn: Cocuk sonlandirildi.\n");
        
        return 0;
    }
    
    return 0;
}

/*
Bu kodun çalışma mantığı:
Bu programda fork() ile bir ebeveyn (parent) ve bir çocuk (child) süreç oluşturduk. 
Çocuk süreç sürekli çalışan bir döngüde kendi sayacını arttırıp ekrana basıyor.
Ebeveyn süreç ise alarm() kurarak her 3 saniyede bir tetikleniyor. Alarm çalınca 
çocuk sürece SIGSTOP atıp onu donduruyor, 2 saniye bekleyip SIGCONT atarak çözüyor. 
Bu sırada çocuk süreç sinyal yakalayıcı (signal() ile bağladığımız) fonksiyonlarına 
gidip gerekli yazıları basıyor. En sonunda ebeveyn yaklaşık 10 saniye dolduğunda 
çocuğa SIGINT gönderiyor ve ardından süreci tamamen öldürüp programdan çıkıyor.
*/
