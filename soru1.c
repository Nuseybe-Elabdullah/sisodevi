#include <stdio.h>
#include <stdlib.h>

// köşegen toplamlarını bulan fonksiyonumuz
int special_sum(int *mat, int rows, int cols) {
    int toplam = 0;
    int i;
    
    // satır ve sütun aynı olduğu için tek döngü yeterli
    for (i = 0; i < rows; i++) {
        // ana köşegen elemanını buluyoruz (i,i)
        // pointer ile satır*cols + sütun mantığıyla gidiyoruz
        toplam += *(mat + i * cols + i);
        
        // ikinci köşegen elemanı (i, cols-i-1)
        // ama ortadaki eleman iki kere toplanmasın diye kontrol yapıyoruz
        if (i != cols - i - 1) {
            toplam += *(mat + i * cols + (cols - i - 1));
        }
    }
    
    return toplam;
}

int main() {
    int n, i, j;
    int *matris;
    int sonuc;
    
    // kullanıcıdan boyutu alıyoruz
    printf("Matris boyutunu girin (N): ");
    scanf("%d", &n);
    
    // tek boyutlu dizi olarak bellek ayırıyoruz
    // malloc ile N*N kadar int yeri açıyoruz
    matris = (int*)malloc(n * n * sizeof(int));
    
    if (matris == NULL) {
        printf("Bellek ayrilamadi!\n");
        return 1;
    }
    
    printf("\nMatris elemanlarini sirayla giriniz:\n");
    // elemanları kullanıcıdan alıyoruz
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            printf("[%d][%d] elemani: ", i, j);
            // pointer aritmetiği kullanarak adres gösteriyoruz
            scanf("%d", (matris + i * n + j));
        }
    }
    
    // matrisi ekrana yazdıralım ki doğru mu girdik görelim
    printf("\nGirilen matris:\n");
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            printf("%d\t", *(matris + i * n + j));
        }
        printf("\n");
    }
    
    // fonksiyonu çağırıp sonucu alıyoruz
    sonuc = special_sum(matris, n, n);
    
    // sonucu ekrana basıyoruz
    printf("\nAna ve ikinci kosegen elemanlarinin toplami: %d\n", sonuc);
    
    // işimiz bitti, belleği sisteme geri veriyoruz
    free(matris);
    
    return 0;
}
/*
Bu kodun çalışma mantığı:
Burada N boyutlu bir matrisi bellekte arka arkaya (tek boyutlu olarak) tuttuk. 
Elemanlara ulaşmak için normal dizi indexleri ([i][j]) yerine işaretçi aritmetiği 
kullandık. Yani başlangıç adresinin üzerine "satır * toplam_sütun + sütun" kadar 
ekleyerek istediğimiz elemanın adresini bulduk. Köşegenleri toplarken de, tam 
ortaya denk gelen elemanı iki kere saymamak için küçük bir if kontrolü yazdım.
*/
