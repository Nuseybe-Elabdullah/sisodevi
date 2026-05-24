#include <stdio.h>
#include <stdlib.h>

// Sadece pointer aritmetigi ile ana ve yan kosegen toplamini bulan fonksiyon
int special_sum(int *mat, int rows, int cols) {
    int sum = 0;
    int i;
    
    // Matrisin satir sayisi kadar donuyoruz
    for (i = 0; i < rows; i++) {
        // Ana kosegen elemani (i, i)
        sum += *(mat + (i * cols) + i);
        
        // Yan kosegen elemani (i, cols - 1 - i)
        sum += *(mat + (i * cols) + (cols - 1 - i));
    }
    
    // Matris boyutu tek sayiysa, ortadaki elemani iki kere toplamis oluyoruz.
    // Bu yuzden ortadaki elemani toplamdan bir kez cikariyoruz.
    if (rows % 2 != 0 && rows == cols) {
        int center = rows / 2;
        sum -= *(mat + (center * cols) + center);
    }
    
    return sum;
}

int main() {
    int n, i, j;
    int *mat = NULL;
    
    printf("Matris boyutu (N) giriniz (NxN icin): ");
    scanf("%d", &n);
    
    if (n <= 0) {
        printf("Hata: Gecersiz boyut girdiniz.\n");
        return 1;
    }
    
    // N*N boyutunda tek boyutlu dizi icin malloc ile yer ayiriyoruz
    mat = (int *)malloc(n * n * sizeof(int));
    if (mat == NULL) { 
        printf("Bellek ayrilamadi.\n");
        return 1;
    }
    
    printf("\nLutfen %dx%d matrisin elemanlarini giriniz:\n", n, n);
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            printf("Eleman [%d][%d]: ", i, j);
            // Sadece pointer aritmetigi kullaniyoruz, [][] kullanmak yasak
            scanf("%d", (mat + (i * n) + j)); 
        }
    }
    
    printf("\nGirilen Matris:\n");
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            printf("%d\t", *(mat + (i * n) + j));
        }
        printf("\n");
    }
    
    // Fonksiyonu cagirip sonucu yazdiriyoruz
    int sonuc = special_sum(mat, n, n);
    printf("\nAna ve Yan Kosegenlerin Toplami: %d\n", sonuc);
    
    // Bellek sizintisi olmamasi icin isimiz bitince free yapiyoruz
    free(mat);
    
    return 0;
}
