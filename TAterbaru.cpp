#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

// Konstanta untuk harga kopi
const int PRICES[8][3] = {
    {25000, 27000, 29000}, // Kopi Arabika
    {24000, 26000, 28000}, // Kopi Robusta
    {23000, 25000, 27000}, // Kopi Liberica
    {22000, 24000, 26000}, // Kopi Excelsa
    {30000, 32000, 34000}, // Earl Grey Milk Tea
    {28000, 30000, 32000}, // Iced Black Coffee
    {35000, 37000, 39000}, // Matcha Espresso
    {33000, 35000, 37000}  // Dark Choco
};

const string COFFEE_NAMES[8] = {
    "Kopi Arabika",
    "Kopi Robusta",
    "Kopi Liberica",
    "Kopi Excelsa",
    "Earl Grey Milk Tea",
    "Iced Black Coffee",
    "Matcha Espresso",
    "Dark Choco"
};

const int DISCOUNT_THRESHOLD_1 = 50000; // Ambang diskon 5%
const int DISCOUNT_THRESHOLD_2 = 100000; // Ambang diskon 10%

// Prototipe fungsi
void displayMenu();
int getCoffeePrice(int kodeIndex, const string& ukuran);
void generateReceipt(const string& kasir, const string& pelanggan,
                     const string& tanggal, const string& bulan,
                     const string& tahun, const vector<string>& jenisKopi,
                     const vector<string>& ukuranKopi,
                     const vector<int>& totalHarga,
                     const vector<int>& quantities,
                     int totalBayar, int kembalian);
void saveTransactionToFile(const string& kasir, const string& pelanggan,
                            const string& tanggal, const string& bulan,
                            const string& tahun,
                            const vector<string>& jenisKopi,
                            const vector<string>& ukuranKopi,
                            const vector<int>& totalHarga,
                            const vector<int>& quantities,
                            int totalBayar, int kembalian);

int main() {
    int kode;
    string ulang = "Iya", tanggal, bulan, tahun, kasir, pelanggan;

    cout << "SELAMAT DATANG DI TOKO KOPI VHEIN!" << endl; // Pesan sambutan
    
    // Vektor untuk menyimpan pesanan
    vector<string> jenisKopi;
    vector<string> ukuranKopi;
    vector<int> totalHarga;
    vector<int> quantities; // Menyimpan jumlah dari setiap jenis kopi

    do {
        system("cls");
        displayMenu();

        do {
            cout << "Masukkan kode jenis kopi yang akan dibeli: ";
            cin >> kode;

            if (kode >= 1 && kode <= 8) {
                int quantity;
                cout << "Masukkan jumlah yang ingin dibeli: ";
                cin >> quantity;

                string ukuran;
                cout << "Pilih ukuran (Small/Medium/Large): ";
                cin >> ukuran;

                int harga = getCoffeePrice(kode - 1, ukuran); // Menghitung harga berdasarkan ukuran
                if (harga == -1) {
                    cout << "Ukuran tidak valid. Mohon masukkan Small, Medium atau Large." << endl;
                    continue; // Mengulangi jika input tidak valid
                }

                // Menyimpan kopi yang dipilih dan ukurannya
                jenisKopi.push_back(COFFEE_NAMES[kode - 1]);
                ukuranKopi.push_back(ukuran);
                totalHarga.push_back(harga * quantity); // Menghitung total harga untuk item ini
                quantities.push_back(quantity); // Menyimpan jumlah

                cout << "Apakah Anda ingin menambahkan menu lain? (Iya/Tidak): ";
                cin >> ulang;

            } else {
                cout << "Mohon maaf pilihan kode tersebut tidak tersedia." << endl;
            }
        } while (ulang == "Iya" || ulang == "iya");

        // Input detail untuk struk setelah semua pemilihan selesai
        cout << "Masukkan Nama Kasir: "; cin >> kasir; 
        cout << "Masukkan Nama Pelanggan: "; cin >> pelanggan; 
        cout << "Masukkan Tanggal Pembelian: "; cin >> tanggal; 
        cout << "Masukkan Bulan Pembelian: "; cin >> bulan; 
        cout << "Masukkan Tahun Pembelian: "; cin >> tahun; 

        // Menghitung total pembayaran dan diskon
        int totalPembayaran = 0;
        for (int price : totalHarga) {
            totalPembayaran += price; // Menjumlahkan semua harga
        }

        double diskon = 0;
        if (totalPembayaran > DISCOUNT_THRESHOLD_2) {
            diskon = totalPembayaran * 0.10; // Diskon 10%
        } else if (totalPembayaran > DISCOUNT_THRESHOLD_1) {
            diskon = totalPembayaran * 0.05; // Diskon 5%
        }

        int totalSetelahDiskon = totalPembayaran - diskon;

        // Proses pembayaran
        cout << "\nTotal pembayaran sebesar: Rp." << totalSetelahDiskon << endl; 
        int bayar;
        do {
            cout << "Masukkan nominal pembayaran: Rp."; cin >> bayar; 
            int kembalian = bayar - totalSetelahDiskon;

            if (kembalian < 0) {
                cout << endl; 
                cout << "Mohon maaf nominal pembayarannya masih kurang Rp." << -kembalian << "!" << endl; 
            } else {
                // Membuat struk dan menyimpan transaksi ke file
                generateReceipt(kasir, pelanggan, tanggal, bulan, tahun,
                                jenisKopi, ukuranKopi, totalHarga,
                                quantities, bayar, kembalian);
                
                saveTransactionToFile(kasir, pelanggan, tanggal, bulan,
                                       tahun, jenisKopi, ukuranKopi,
                                       totalHarga, quantities, bayar, kembalian);
            }
        } while (bayar < totalSetelahDiskon);

        // Menghapus vektor untuk transaksi berikutnya
        jenisKopi.clear();
        ukuranKopi.clear();
        totalHarga.clear();
        quantities.clear(); // Menghapus jumlah

        cout << "\nApakah ingin melakukan transaksi lagi? (Iya/Tidak): "; 
        cin >> ulang;

    } while (ulang == "Iya" || ulang == "iya");

    return 0;
}

void displayMenu() {
    cout << "\nSELAMAT DATANG DI TOKO KOPI VHEIN!" << endl; // Pesan sambutan di tampilan menu
    cout << "|--------------------------------------------|" << endl;
    cout << "| Kode | Jenis                 | Harga (Small) | Harga (Medium) | Harga (Large) |" << endl;
    cout << "|--------------------------------------------|" << endl;
    
    for (int i = 0; i < 8; ++i) {
        cout << "| " << setw(3) << i + 1 
             << "   | " << left<< setw(20) << COFFEE_NAMES[i]
             << " | Rp." << setw(10) << PRICES[i][0]
             << " | Rp." << setw(10) << PRICES[i][1]
             << " | Rp." << setw(10) << PRICES[i][2] 
             << " |" << endl;
    }
    
    cout << "|--------------------------------------------|" << endl;
}

int getCoffeePrice(int kodeIndex, const string& ukuran) {
    int sizeIndex = (ukuran == "Small") ? 0 : (ukuran == "Medium") ? 1 : (ukuran == "Large") ? 2 : -1;
    return (sizeIndex != -1) ? PRICES[kodeIndex][sizeIndex] : -1;
}

void generateReceipt(const string& kasir, const string& pelanggan,
                     const string& tanggal,const string& bulan,
                     const string& tahun,const vector<string>& jenisKopi,
                     const vector<string>& ukuranKopi,
                     const vector<int>& totalHarga,
                     const vector<int>& quantities,
                     int bayar,int kembalian) {

    ofstream struk;
    system("cls");
    struk.open("struk.txt");

    struk<< "\n**********************************************"<<endl; 
    struk<< "\t\t   Struk Pembelian Toko Kopi Vhein"<<endl; 
    struk<< "**********************************************"<<endl; 
    struk<< left<<setw(15)<<"Tanggal : "<<tanggal<<"-"<<bulan<<"-"<<tahun<<endl; 
    struk<< left<<setw(15)<<"Kasir   : "<<kasir<<endl; 
    struk<< left<<setw(15)<<"Pelanggan: "<<pelanggan<<endl;

    double totalPembayaran = 0;

    struk<<"----------------------------------------------"<<endl;
    
    for(size_t i = 0; i < jenisKopi.size(); ++i) {
        struk<< left<< setw(20) << jenisKopi[i] + "(" + ukuranKopi[i] + ")"
             <<" x "<< quantities[i] <<" "
             <<"= Rp."<< setw(10)<<(totalHarga[i])<<endl;
             
        totalPembayaran += totalHarga[i];
    }
    
    double diskon = (totalPembayaran > DISCOUNT_THRESHOLD_2) ? totalPembayaran * 0.10 :
                    (totalPembayaran > DISCOUNT_THRESHOLD_1) ? totalPembayaran * 0.05 : 0;

    
    struk<<"----------------------------------------------"<<endl;
    
    struk<<"Total Pembayaran     : Rp."<<fixed<<setprecision(2)<<totalPembayaran<<endl;

    
    if(diskon > 0)
       struk<<"Potongan Diskon      : Rp."<<diskon<<endl;

    
    double totalSetelahDiskon = totalPembayaran - diskon;

    struk<<"Total Setelah Diskon : Rp."<<totalSetelahDiskon<<endl;
    
    struk<<"Tunai               : Rp."<<bayar<<endl;
    
    struk<<"Kembali             : Rp."<<kembalian<<endl;

    struk<<"\nBarang yang sudah dibeli tidak dapat dikembalikan kecuali ada perjanjian."<<endl;
    
    struk<<"**********************************************"<<endl;
    
    struk<<" Terima Kasih Telah Berbelanja! "<<endl;
    
    struk<<"**********************************************"<<endl;

    struk.close();
    system("notepad.exe struk.txt");
    system("cls");
}


void saveTransactionToFile(const string& kasir,const string& pelanggan,const string& tanggal,
                            const string& bulan,const string& tahun,
                            const vector<string>& jenisKopi,
                            const vector<string>& ukuranKopi,
                            const vector<int>& totalHarga,
                            const vector<int>& quantities,
                            int bayar,int kembalian) {

   ofstream outFile("transaksi.txt", ios::app); // Append mode
   if (!outFile) {
       cerr << "Error opening file!" << endl;
       return;
   }

   outFile<< fixed<< setprecision(2);
   
   outFile<< tanggal <<" "<< bulan <<" "<< tahun <<" | "
          <<"Kasir: "<< kasir <<" | "
          <<"Pelanggan: "<< pelanggan <<" | ";

   for(size_t i = 0; i < jenisKopi.size(); ++i) {
       outFile<< jenisKopi[i] <<" ("<< ukuranKopi[i] <<" x "<< quantities[i] <<") ";
       if(i < jenisKopi.size() - 1)
           outFile<< ", ";
   }

   double totalPembayaran = 0;

   for(int price : totalHarga) {
       totalPembayaran += price;
   }

   double diskon = (totalPembayaran > DISCOUNT_THRESHOLD_2) ? totalPembayaran * 0.10 :
                   (totalPembayaran > DISCOUNT_THRESHOLD_1) ? totalPembayaran * 0.05 : 0;

   outFile<<"| Total: Rp."<<totalPembayaran<<" | "
          <<"Diskon: Rp."<<diskon<<" | "
          <<"Total Setelah Diskon: Rp."<<(totalPembayaran - diskon)<<" | "
          <<"Bayar: Rp."<< bayar <<" | "
          <<"Kembali: Rp."<< kembalian <<" \n";

   outFile.close();
}