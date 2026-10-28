#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define JUMLAH_PERALATAN 5
#define MAKS_TRANSAKSI 10
#define PANJANG_NAMA 100

/* Data peralatan disimpan dalam array */
char namaPeralatan[JUMLAH_PERALATAN][40] = {
    "Tenda Kapasitas 4 Orang",
    "Sleeping Bag",
    "Kompor Portable",
    "Matras Camping",
    "Lampu Camping"
};

int hargaPeralatan[JUMLAH_PERALATAN] = {
    50000, 15000, 20000, 10000, 12000
};

/* Array untuk menyimpan riwayat transaksi */
char namaPenyewa[MAKS_TRANSAKSI][PANJANG_NAMA];
int pilihanPeralatan[MAKS_TRANSAKSI];
int jumlahUnit[MAKS_TRANSAKSI];
int lamaSewa[MAKS_TRANSAKSI];
int subtotalTransaksi[MAKS_TRANSAKSI];
int diskonTransaksi[MAKS_TRANSAKSI];
int totalBayar[MAKS_TRANSAKSI];
int jumlahTransaksi = 0;

/* Membaca bilangan bulat dan memvalidasi input */
int inputAngka(const char pesan[]) {
    char baris[100];
    char *akhir;
    long angka;

    while (1) {
        printf("%s", pesan);

        if (fgets(baris, sizeof(baris), stdin) == NULL) {
            printf("Input tidak terbaca. Coba lagi.\n");
            continue;
        }

        angka = strtol(baris, &akhir, 10);

        while (isspace((unsigned char)*akhir)) {
            akhir++;
        }

        if (akhir == baris || *akhir != '\0') {
            printf("Input harus berupa angka. Silakan coba lagi.\n");
        } else if (angka < 1 || angka > 1000000) {
            printf("Masukkan angka antara 1 sampai 1000000.\n");
        } else {
            return (int)angka;
        }
    }
}

/* Fungsi 1: menampilkan daftar peralatan */
void tampilkanPeralatan(void) {
    int i;

    printf("\n========== DAFTAR PERALATAN ==========\n");
    printf("%-4s %-30s Harga/Hari\n", "No.", "Nama Peralatan");
    printf("--------------------------------------\n");

    for (i = 0; i < JUMLAH_PERALATAN; i++) {
        printf("%-4d %-30s Rp%d\n", i + 1,
               namaPeralatan[i], hargaPeralatan[i]);
    }

    printf("======================================\n");
}

/* Fungsi 2: melakukan penyewaan */
void sewaPeralatan(void) {
    char nama[PANJANG_NAMA];
    char jawaban;
    char barisJawaban[20];
    int pilihan, unit, hari;
    int subtotal, diskonPersen, diskon, total;

    if (jumlahTransaksi >= MAKS_TRANSAKSI) {
        printf("\nKapasitas riwayat sudah penuh. Maksimal 10 transaksi.\n");
        return;
    }

    printf("\n========== SEWA PERALATAN ==========\n");
    printf("Nama penyewa: ");

    if (fgets(nama, sizeof(nama), stdin) == NULL) {
        printf("Nama tidak terbaca.\n");
        return;
    }
    nama[strcspn(nama, "\n")] = '\0';

    while (strlen(nama) == 0) {
        printf("Nama tidak boleh kosong. Masukkan nama: ");
        if (fgets(nama, sizeof(nama), stdin) == NULL) {
            return;
        }
        nama[strcspn(nama, "\n")] = '\0';
    }

    tampilkanPeralatan();

    while (1) {
        pilihan = inputAngka("Pilih nomor peralatan [1-5]: ");
        if (pilihan >= 1 && pilihan <= JUMLAH_PERALATAN) {
            break;
        }
        printf("Nomor peralatan tidak tersedia.\n");
    }

    unit = inputAngka("Jumlah unit yang disewa (minimal 1): ");
    hari = inputAngka("Lama penyewaan dalam hari (minimal 1): ");

    subtotal = hargaPeralatan[pilihan - 1] * unit * hari;

    if (hari >= 5) {
        diskonPersen = 15;
    } else if (hari >= 3) {
        diskonPersen = 10;
    } else {
        diskonPersen = 0;
    }

    diskon = subtotal * diskonPersen / 100;
    total = subtotal - diskon;

    /* Simpan transaksi ke array */
    strcpy(namaPenyewa[jumlahTransaksi], nama);
    pilihanPeralatan[jumlahTransaksi] = pilihan - 1;
    jumlahUnit[jumlahTransaksi] = unit;
    lamaSewa[jumlahTransaksi] = hari;
    subtotalTransaksi[jumlahTransaksi] = subtotal;
    diskonTransaksi[jumlahTransaksi] = diskon;
    totalBayar[jumlahTransaksi] = total;
    jumlahTransaksi++;

    printf("\n========== STRUK PENYEWAAN ==========\n");
    printf("Nama Penyewa   : %s\n", nama);
    printf("Peralatan      : %s\n", namaPeralatan[pilihan - 1]);
    printf("Harga per Hari : Rp%d\n", hargaPeralatan[pilihan - 1]);
    printf("Jumlah Unit    : %d\n", unit);
    printf("Lama Sewa      : %d hari\n", hari);
    printf("-------------------------------------\n");
    printf("Subtotal       : Rp%d\n", subtotal);
    printf("Diskon (%d%%)    : Rp%d\n", diskonPersen, diskon);
    printf("-------------------------------------\n");
    printf("TOTAL BAYAR    : Rp%d\n", total);
    printf("=====================================\n");
    printf("Terima kasih telah menggunakan layanan Adventure Camp!\n");

    while (1) {
        printf("Kembali ke menu utama? (Y/T): ");
        if (fgets(barisJawaban, sizeof(barisJawaban), stdin) == NULL) {
            jawaban = 'Y';
        } else {
            jawaban = barisJawaban[0];
        }
        jawaban = (char)toupper((unsigned char)jawaban);
        if (jawaban == 'Y') {
            return;
        } else if (jawaban == 'T') {
            printf("Terima kasih. Program selesai.\n");
            exit(0);
        } else {
            printf("Masukkan Y untuk kembali atau T untuk keluar.\n");
        }
    }
}

/* Fungsi 3: menampilkan riwayat transaksi dan total pendapatan */
void tampilkanRiwayat(void) {
    int i;
    int pendapatan = 0;

    printf("\n========== RIWAYAT TRANSAKSI ==========\n");

    if (jumlahTransaksi == 0) {
        printf("Belum ada transaksi penyewaan.\n");
        return;
    }

    printf("%-3s %-18s %-25s Total\n", "No", "Nama", "Peralatan");
    printf("---------------------------------------------------------------\n");

    for (i = 0; i < jumlahTransaksi; i++) {
        printf("%-3d %-18s %-25s Rp%d\n",
               i + 1,
               namaPenyewa[i],
               namaPeralatan[pilihanPeralatan[i]],
               totalBayar[i]);
        pendapatan += totalBayar[i];
    }

    printf("---------------------------------------------------------------\n");
    printf("Jumlah Transaksi : %d\n", jumlahTransaksi);
    printf("Total Pendapatan : Rp%d\n", pendapatan);
    printf("========================================\n");
}

/* Fungsi 4: menu utama */
void tampilkanMenu(void) {
    printf("\n========================================\n");
    printf("             ADVENTURE CAMP\n");
    printf("       SISTEM PENYEWAAN PERALATAN\n");
    printf("========================================\n");
    printf("1. Lihat Daftar Peralatan\n");
    printf("2. Sewa Peralatan\n");
    printf("3. Lihat Riwayat Transaksi\n");
    printf("4. Keluar\n");
    printf("========================================\n");
}

int main(void) {
    int menu;

    do {
        tampilkanMenu();
        menu = inputAngka("Pilih menu [1-4]: ");

        switch (menu) {
            case 1:
                tampilkanPeralatan();
                break;
            case 2:
                sewaPeralatan();
                break;
            case 3:
                tampilkanRiwayat();
                break;
            case 4:
                printf("\nTerima kasih telah menggunakan Adventure Camp.\n");
                printf("Program selesai.\n");
                break;
            default:
                printf("Menu tidak tersedia. Pilih menu 1 sampai 4.\n");
        }
    } while (menu != 4);

    return 0;
}
