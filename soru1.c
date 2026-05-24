#include <stdio.h>
#include <stdlib.h>

// köşegenleri toplayan fonksiyon
int special_sum(int *mat, int rows, int cols) {
    int toplam = 0;
    int i;
    
    // tek döngü ile köşegenlerde geziyoruz
    for (i = 0; i < rows; i++) {
        // ana köşegen elemanı (i,i)
        toplam += *(mat + i * cols + i);
        
        // ikinci köşegen elemanı
        // i=0 ise cols-1'e bakar vs.
        if (i != cols - i - 1) { // ortadaki eleman iki kere sayılmasın diye
            toplam += *(mat + i * cols + (cols - i - 1));
        }
    }
    
    return toplam;
}

int main() {
    int n, i, j;
    int *matris;
    int sonuc;
    
    // kullanıcıdan N değeri alıyoruz
    printf("Matris boyutu (N): ");
    scanf("%d", &n);
    
    // tek boyutlu blok olarak yer açıyoruz (malloc ile)
    matris = (int*)malloc(n * n * sizeof(int));
    
    if (matris == NULL) {
        printf("Bellek hatasi!\n");
        return 1;
    }
    
    printf("\nMatris elemanlarini girin:\n");
    // döngü ile elemanları alıyoruz
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            printf("[%d][%d]: ", i, j);
            // pointer ile adres hesaplayıp scanf'e veriyoruz
            scanf("%d", (matris + i * n + j));
        }
    }
    
    // matrisi ekranda görelim
    printf("\nMatrisimiz:\n");
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            printf("%d\t", *(matris + i * n + j));
        }
        printf("\n");
    }
    
    // fonksiyonu çağırıyoruz
    sonuc = special_sum(matris, n, n);
    
    printf("\nKosegenlerin toplami: %d\n", sonuc);
    
    // işimiz bitti temizleyelim
    free(matris);
    
    return 0;
}
