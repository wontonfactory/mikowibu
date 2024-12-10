#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>
#include <ctime>
#include <numeric> // Untuk std::accumulate

using namespace std;

// Konstanta untuk harga kopi (Array 2D digunakan untuk menyimpan harga untuk ukuran Small, Medium, dan Large) (modul 5)

const int PRICES[28][3] = {
    {25000, 27000, 29000}, // Kopi Arabika
    {24000, 26000, 28000}, // Kopi Robusta
    {23000, 25000, 27000}, // Kopi Liberica
    {22000, 24000, 26000}, // Kopi Excelsa
    {30000, 32000, 34000}, // Earl Grey Milk Tea
    {28000, 30000, 32000}, // Iced Black Coffee
    {35000, 37000, 39000}, // Matcha Espresso
    {33000, 35000, 37000}, // Dark Choco
    {27000, 29000, 31000}, // Caramel Macchiato
    {26000, 28000, 30000}, // Hazelnut Latte
    {31000, 33000, 35000}, // Vanilla Cappuccino
    {29000, 31000, 33000}, // Almond Milk Latte
    {34000, 36000, 38000}, // Tiramisu Coffee
    {32000, 34000, 36000}, // Mocha Latte
    {28000, 30000, 32000}, // Cold Brew
    {30000, 32000, 34000}, // Nitro Coffee
    {37000, 39000, 41000}, // White Chocolate Mocha
    {35000, 37000, 39000}, // Salted Caramel Latte
    {33000, 35000, 37000}, // Cinnamon Roll Coffee
    {31000, 33000, 35000}, // Honey Lavender Latte
    {36000, 38000, 40000}, // Pumpkin Spice Latte
    {32000, 34000, 36000}, // Peppermint Mocha
    {30000, 32000, 34000}, // Chai Tea Latte
    {34000, 36000, 38000}, // Gingerbread Coffee
    {29000, 31000, 33000}, // Coconut Latte
    {28000, 30000, 32000}, // Toffee Nut Coffee
    {31000, 33000, 35000}, // Maple Pecan Latte
    {26000, 28000, 30000}  // Lemon Honey Tea
};

// Konstanta untuk nama kopi (Array 1D digunakan untuk menyimpan nama kopi)

const string COFFEE_NAMES[28] = {
    "Kopi Arabika", "Kopi Robusta", "Kopi Liberica", "Kopi Excelsa",
    "Earl Grey Milk Tea", "Iced Black Coffee", "Matcha Espresso", "Dark Choco",
    "Caramel Macchiato", "Hazelnut Latte", "Vanilla Cappuccino", "Almond Milk Latte",
    "Tiramisu Coffee", "Mocha Latte", "Cold Brew", "Nitro Coffee",
    "White Chocolate Mocha", "Salted Caramel Latte", "Cinnamon Roll Coffee",
    "Honey Lavender Latte", "Pumpkin Spice Latte", "Peppermint Mocha", "Chai Tea Latte",
    "Gingerbread Coffee", "Coconut Latte", "Toffee Nut Coffee", "Maple Pecan Latte",
    "Lemon Honey Tea"
};


const int DISCOUNT_THRESHOLD_1 = 50000; // Ambang diskon 5%
const int DISCOUNT_THRESHOLD_2 = 100000; // Ambang diskon 10%

// Prototipe fungsi (modul 6)
void displayMenu();
int getCoffeePrice(int kodeIndex, const string& ukuran);
void generateReceipt(const string& kasir, const string& pelanggan, const string& dineOrTakeAway, const string& nomorMeja, const string& tanggal, const string& bulan, const string& tahun,
                     const vector<string>& jenisKopi, const vector<string>& ukuranKopi,
                     const vector<int>& totalHarga, const vector<int>& quantities,
                     int bayar, int kembalian);
void saveTransactionToFile(const string& kasir, const string& pelanggan, const string& dineOrTakeAway, const string& nomorMeja, const string& tanggal, const string& bulan, const string& tahun,
                            const vector<string>& jenisKopi, const vector<string>& ukuranKopi,
                            const vector<int>& totalHarga, const vector<int>& quantities,
                            int bayar, int kembalian);
void cancelOrder(vector<string>* jenisKopi, vector<string>* ukuranKopi,
                 vector<int>* totalHarga, vector<int>* quantities);

void displayWelcomeMessage();
void displayThankYouMessage();
void clearTerminal();

int main() {

    // Menampilkan pesan sambutan

    displayWelcomeMessage();

    int kode;
    string ulang = "Iya", kasir, pelanggan;
    string dineOrTakeAway;
    string nomorMeja;

    // Menggunakan fungsi time untuk mendapatkan tanggal dan waktu saat ini

    time_t now = time(0);
    tm *ltm = localtime(&now);
    
    string tanggal = to_string(ltm->tm_mday); // Hari // Operasi konversi integer ke string
    string bulan = to_string(ltm->tm_mon + 1); // Bulan (0-11 jadi ditambah 1) // Perbaikan untuk bulan (dimulai dari 1)
    string tahun = to_string(ltm->tm_year + 1900); // Tahun (tahun sejak 1900) // Operasi aritmatika untuk tahun (modul 2)


    
    // Input detail kasir dan pelanggan sebelum memilih menu

    cout << "----------------------------------------------------------------------" << endl;
    cout << "Masukkan Nama Kasir: ";
    getline(cin, kasir);
    
    cout << "Masukkan Nama Pelanggan: ";
    getline(cin, pelanggan);

    // Input pilihan Dine In atau Take Away

    cout << "Apakah ini untuk Dine In atau Take Away? (Dine In/Take Away): ";
    getline(cin, dineOrTakeAway);

    if (dineOrTakeAway == "Dine In" || dineOrTakeAway == "dine in") {
        cout << "Masukkan nomor meja (1-20): ";
        getline(cin, nomorMeja);
        // Loop untuk validasi input nomor meja (Dine In)
        while (stoi(nomorMeja) < 1 || stoi(nomorMeja) > 20) { // Validasi input nomor meja (operasi kondisi) (modul 3)
            cout << "Nomor meja tidak valid. Masukkan nomor meja (1-20): ";
            getline(cin, nomorMeja);
        }
    } else {
        nomorMeja = "N/A"; // Tidak ada nomor meja untuk Take Away
    }

    cout << "Tanggal Pembelian: " << tanggal << endl;
    cout << "Bulan Pembelian: " << bulan << endl;
    cout << "Tahun Pembelian: " << tahun << endl;
    cout << "**********************************************************************" << endl;

    // Vektor untuk menyimpan pesanan

    vector<string> jenisKopi;
    vector<string> ukuranKopi;
    vector<int> totalHarga;
    vector<int> quantities;

    do {
        system("cls");
        displayMenu();
        
        do { // Loop untuk memasukkan kode jenis kopi (modul 4)
            cout << "Masukkan kode jenis kopi yang akan dibeli \t: ";
            cin >> kode;
            if (kode >= 1 && kode <= 28) { // Operasi kondisi untuk validasi kode kopi (modul 3)
                int quantity;
                cout << "Masukkan jumlah yang ingin dibeli \t\t: ";
                cin >> quantity;
                
                string ukuran;
                cout << "Pilih ukuran (Small/Medium/Large) \t\t: ";
                cin >> ukuran;

                int harga = getCoffeePrice(kode - 1, ukuran); // Pemanggilan fungsi untuk mendapatkan harga kopi
                if (harga == -1) { // Penanganan input ukuran tidak valid
                    cout << "Ukuran tidak valid. Mohon masukkan Small, Medium atau Large." << endl;
                    continue;
                }

                // Menyimpan kopi yang dipilih dan ukurannya

                jenisKopi.push_back(COFFEE_NAMES[kode - 1]); // Operasi pada array (modul 5)
                ukuranKopi.push_back(ukuran);
                totalHarga.push_back(harga * quantity); // Operasi aritmatika (modul 2)
                quantities.push_back(quantity);

                cout << "Apakah Anda ingin menambahkan menu lain? (Iya/Tidak): ";
                cin >> ulang;
                cout << "\n----------------------------------------------------------------------" << endl;
            } 
            else {
                cout << "Mohon maaf pilihan kode tersebut tidak tersedia." << endl;
            }
        } while (ulang == "Iya" || ulang == "iya");

        // Membersihkan layar

        clearTerminal();

        // Opsi untuk membatalkan pesanan sebelum menyelesaikan pembayaran

        char cancelOption;
        cout << "\nApakah Anda ingin membatalkan pesanan? (Y/N): ";
        cin >> cancelOption;

        if (cancelOption == 'Y' || cancelOption == 'y') {
            cancelOrder(&jenisKopi, &ukuranKopi, &totalHarga, &quantities); // Pemanggilan fungsi dengan pointer (modul 7)
            if (jenisKopi.empty()) {
                cout << "\n\nSemua pesanan telah dibatalkan. Apakah ingin memulai transaksi baru? (Iya/Tidak): ";
                cin >> ulang;

                if (ulang != "Iya" && ulang != "iya") {
                    cout << "Terima kasih telah bertransaksi di Toko Kopi Vhein!" << endl;
                    break;
                }
                continue;
            }

        }

        // Menghitung total pembayaran dan diskon

        int totalPembayaran = accumulate(totalHarga.begin(), totalHarga.end(), 0);  // Operasi aritmatika (modul 2)

        double diskon = 0;
        if (totalPembayaran > DISCOUNT_THRESHOLD_2) {
            diskon = totalPembayaran * 0.10; // Diskon 10% // Operasi kondisi (modul 3)
        } else if (totalPembayaran > DISCOUNT_THRESHOLD_1) {
            diskon = totalPembayaran * 0.05; // Diskon 5% // Operasi kondisi (modul 3)
        }
        
        double totalSetelahDiskon = totalPembayaran - diskon;

        // Proses pembayaran

        cout << "\nTotal pembayaran sebesar: Rp." << fixed << setprecision(2) << totalSetelahDiskon << endl;
        
        int bayar;
        do {
            cout << "Masukkan nominal pembayaran: Rp.";
            cin >> bayar;

            int kembalian = bayar - totalSetelahDiskon; // Operasi aritmatika (modul 2)
            if (kembalian < 0) {  // Operasi kondisi (modul 3)
                cout << endl;
                cout << "Mohon maaf nominal pembayarannya masih kurang Rp." << -kembalian << "!" << endl;
            } else {
                generateReceipt(kasir, pelanggan, dineOrTakeAway, nomorMeja, tanggal, bulan, tahun,
                                jenisKopi, ukuranKopi,
                                totalHarga, quantities,
                                bayar, kembalian); // Pembuatan struk
                
                saveTransactionToFile(kasir, pelanggan, dineOrTakeAway, nomorMeja, tanggal, bulan, tahun,
                                       jenisKopi, ukuranKopi,
                                       totalHarga, quantities,
                                       bayar, kembalian); // Penyimpanan ke file (modul 8)
            }
        } while (bayar < totalSetelahDiskon);

        // Menghapus vektor untuk transaksi berikutnya

        jenisKopi.clear();
        ukuranKopi.clear();
        totalHarga.clear();
        quantities.clear();

        cout << "\nApakah ingin melakukan transaksi lagi? (Iya/Tidak): ";
        cin >> ulang;


    } while (ulang == "Iya" || ulang == "iya");

    displayThankYouMessage();

    return 0;
}


// Fungsi menampilkan menu (modul 6)

void displayMenu() {
    cout << "------------------------TOKO KOPI VHEIN------------------------" << endl;
    cout << "===============================================================" << endl;
    cout << "============================| MENU |===========================" << endl;
    cout << "===============================================================" << endl;
    cout << "| Kode | Jenis Kopi            |    Small   Medium   Large    |" << endl;
    cout << "|-------------------------------------------------------------|" << endl;

    for (int i = 0; i < 28; ++i) { // Loop untuk menampilkan menu kopi (modul 4)
        cout << "| " << setw(4) << i + 1 
             << " | " << left << setw(22) << COFFEE_NAMES[i] 
             << "| Rp." << setw(6) << PRICES[i][0] 
             << " Rp." << setw(6) << PRICES[i][1] 
             << " Rp." << setw(6) << PRICES[i][2] 
             << "|" << endl;
    }

    cout << "===============================================================" << endl;
}


// Fungsi menghitung harga kopi (modul 6)

int getCoffeePrice(int kodeIndex, const string& ukuran) {
    int sizeIndex = (ukuran == "Small") ? 0 : (ukuran == "Medium") ? 1 : (ukuran == "Large") ? 2 : -1;

    return (sizeIndex != -1) ? PRICES[kodeIndex][sizeIndex] : -1;
}

// Fungsi pembatalan pesanan (modul 6)
void cancelOrder(vector<string>* jenisKopi, vector<string>* ukuranKopi,
                 vector<int>* totalHarga, vector<int>* quantities) {
    if (jenisKopi->empty()) {
        cout << "\nTidak ada pesanan untuk dibatalkan." << endl;
        return;
    }

    cout << "----------------------------------------------------------------------" << endl;
    cout << "\nPesanan saat ini:" << endl;
    for (size_t i = 0; i < jenisKopi->size(); ++i) { // Loop untuk pembatalan pesanan (modul 4)
        cout << (i + 1) << ". " << jenisKopi->at(i) << " (" << ukuranKopi->at(i) 
             << ") x" << quantities->at(i) << ": Rp." << totalHarga->at(i) << endl;
    }
    cout << "----------------------------------------------------------------------" << endl;

    int indexToCancel;
    cout << "\nMasukkan nomor pesanan yang ingin dibatalkan: ";
    cin >> indexToCancel;

    if (indexToCancel < 1 || indexToCancel > jenisKopi->size()) {
        cout << "\nNomor pesanan tidak valid!" << endl;
        return;
    }

    // Hapus item yang dipilih dari pesanan

    int removeIndex = indexToCancel - 1;
    jenisKopi->erase(jenisKopi->begin() + removeIndex);
    ukuranKopi->erase(ukuranKopi->begin() + removeIndex);
    totalHarga->erase(totalHarga->begin() + removeIndex);
    quantities->erase(quantities->begin() + removeIndex);

    cout << "\nPesanan berhasil dibatalkan!" << endl;
    cout << "**********************************************************************" << endl;

    // Tampilkan kembali pesanan yang tersisa

    if (jenisKopi->empty()) {
        cout << "**********************************************************************" << endl;
        cout << "\nSemua pesanan telah dibatalkan." << endl;
        cout << "**********************************************************************" << endl;
    } else {
        cout << "**********************************************************************" << endl;
        cout << "\nPesanan yang tersisa:" << endl;
        for (size_t i = 0; i < jenisKopi->size(); ++i) {
            cout << (i + 1) << ". " << jenisKopi->at(i) << " (" << ukuranKopi->at(i) 
                 << ") x" << quantities->at(i) << ": Rp." << totalHarga->at(i) << endl;
        }
        cout << "**********************************************************************" << endl;
    }
}

// Fungsi membuat struk (modul 6)

void generateReceipt(const string& kasir, const string& pelanggan, const string& dineOrTakeAway, const string& nomorMeja, const string& tanggal, const string& bulan, const string& tahun,
                     const vector<string>& jenisKopi, const vector<string>& ukuranKopi,
                     const vector<int>& totalHarga, const vector<int>& quantities,
                     int bayar, int kembalian) {
    ofstream struk("struk.txt"); // pengelolaan file (modul 8)

    system("cls");

    // Header Struk

    struk << "\n========================================================================" << endl;
    struk << "\n                    STRUK PEMBELIAN TOKO KOPI VHEIN                    " << endl;
    struk << "                   Jalan Nanas No 8 Ngantang, Malang                 \n" << endl;
    struk << "=========================================================================" << endl;

    // Informasi Tanggal, Kasir, dan Pelanggan

    struk << left << setw(15) << "Tanggal" << ": " << tanggal << "-" << bulan << "-" << tahun << endl;
    struk << left << setw(15) << "Kasir" << ": " << kasir << endl;
    struk << left << setw(15) << "Pelanggan" << ": " << pelanggan << " (" << dineOrTakeAway << ")" << endl;
    if (dineOrTakeAway == "Dine In" || dineOrTakeAway == "dine in") {
        struk << left << setw(15) << "Nomor Meja" << ": " << nomorMeja << endl;
    }

    // Pemisah

    struk << "-------------------------------------------------------------------------" << endl;

    // Daftar Pesanan

    struk << left << setw(45) << "Jenis Kopi (Ukuran)" << setw(12) << "Jumlah" << "Harga" << endl;
    struk << "-------------------------------------------------------------------------" << endl;

    for (size_t i = 0; i < jenisKopi.size(); ++i) {
        struk << left << setw(45) << jenisKopi[i] + " (" + ukuranKopi[i] + ")"
              << setw(12) << quantities[i]
              << "Rp." << totalHarga[i] << endl;
    }

    // Total dan Diskon

    struk << "-------------------------------------------------------------------------" << endl;
    double totalPembayaran = accumulate(totalHarga.begin(), totalHarga.end(), 0.0); // Loop untuk perhitungan total pembayaran (modul 4)
    double diskon = (totalPembayaran > DISCOUNT_THRESHOLD_2) ? totalPembayaran * 0.10 :
                    (totalPembayaran > DISCOUNT_THRESHOLD_1) ? totalPembayaran * 0.05 : 0;

    struk << left << setw(57) << "Total Pembayaran" << "Rp." << fixed << setprecision(2) << totalPembayaran << endl;
    if (diskon > 0) {
        struk << left << setw(57) << "Potongan Diskon" << "Rp." << diskon << endl;
    }
    double totalSetelahDiskon = totalPembayaran - diskon;
    struk << left << setw(57) << "Total Setelah Diskon" << "Rp." << totalSetelahDiskon << endl;

    // Pembayaran dan Kembalian

    struk << left << setw(57) << "Tunai" << "Rp." << bayar << endl;
    struk << left << setw(57) << "Kembali" << "Rp." << kembalian << endl;

    // Footer

    struk << "\nBarang yang sudah dibeli tidak dapat dikembalikan kecuali ada perjanjian." << endl;
    struk << "=========================================================================" << endl;
    struk << "                      TERIMA KASIH TELAH BERBELANJA!                      " << endl;
    struk << "=========================================================================" << endl;

    struk.close();
    system("notepad.exe struk.txt");
}


// Fungsi menyimpan transaksi secara permanen (modul 6)

void saveTransactionToFile(const string& kasir,const string& pelanggan,const string& dineOrTakeAway,const string& nomorMeja,const string& tanggal,const string& bulan,const string& tahun,
                            const vector<string>& jenisKopi,const vector<string>& ukuranKopi,
                            const vector<int>& totalHarga,const vector<int>& quantities,
                            int bayar,int kembalian) {

   ofstream outFile("transaksi.txt", ios::app);   
   
   if (!outFile) {      
       cerr <<"Error opening file!"<< endl;     
       return;    
   }  

   outFile<< fixed<< setprecision(2);   

   outFile<< tanggal <<" "<< bulan <<" "<< tahun <<" | "
           <<"Kasir: "<< kasir <<" | "
           <<"Pelanggan: "<< pelanggan <<" ("<< dineOrTakeAway << ") | ";  

   if(dineOrTakeAway == "Dine In" || dineOrTakeAway == "dine in") {
       outFile<<"Nomor Meja: "<<nomorMeja<<" | "; 
   }

   for(size_t i = 0; i < jenisKopi.size(); ++i){   // Loop untuk menyimpan transaksi
       outFile<< jenisKopi[i] <<" ("<< ukuranKopi[i] <<" x "<< quantities[i] <<") ";  
       if(i < jenisKopi.size() - 1)
           outFile<< ", ";  
   }  

   double totalPembayaran = accumulate(totalHarga.begin(), totalHarga.end(), 0);  

   double diskon = (totalPembayaran > DISCOUNT_THRESHOLD_2) ? totalPembayaran * 0.10 : 
                   (totalPembayaran > DISCOUNT_THRESHOLD_1) ? totalPembayaran * 0.05 : 0;  

   outFile<<"| Total: Rp."<<totalPembayaran<<" | "
          <<"Diskon: Rp."<<diskon<<" | "
          <<"Total Setelah Diskon: Rp."<<(totalPembayaran - diskon) <<" | "
          <<"Bayar: Rp."<< bayar <<" | "
          <<"Kembali: Rp."<< kembalian <<" \n";  

   outFile.close();  
}

// Fungsi pembatalan pesanan (modul 6)

void cancelOrder(vector<string>& jenisKopi, vector<string>& ukuranKopi,
                 vector<int>& totalHarga, vector<int>& quantities) {

   if (jenisKopi.empty()) {  
       cout<<"\nTidak ada pesanan untuk dibatalkan."<< endl;  
       return;  
   }  

   int indexToCancel;  
   
   cout<<"\nPesanan saat ini:"<<endl;  

   for(size_t i = 0; i < jenisKopi.size(); ++i){  
       cout<<(i +1)<<". "<<jenisKopi[i]<<"("<<ukuranKopi[i]<<") x"<<quantities[i]<<": Rp."<<totalHarga[i]<<endl;  
   }  

   cout<<"\nMasukkan nomor pesanan yang ingin dibatalkan: ";  
   
   cin>>indexToCancel;

   if(indexToCancel <1 || indexToCancel >jenisKopi.size()){  
       cout<<"\nNomor pesanan tidak valid!"<<endl;  
       return;  
   }  

   // Hapus item yang dipilih dari pesanan
   int removeIndex = indexToCancel -1;  
   
   jenisKopi.erase(jenisKopi.begin() + removeIndex);  
   ukuranKopi.erase(ukuranKopi.begin() + removeIndex);  
   totalHarga.erase(totalHarga.begin() + removeIndex);  
   quantities.erase(quantities.begin() + removeIndex);  

   cout<<"\nPesanan berhasil dibatalkan!"<<endl;

}

// Fungsi display pesan walcome ( modul 6)

void displayWelcomeMessage() {
    system("cls");
    cout << "======================================================================" << endl;
    cout << "||                                                                  ||" << endl;
    cout << "||               SELAMAT DATANG DI TOKO KOPI VHEIN!                 ||" << endl;
    cout << "||                Jalan Nanas No 8 Ngantang, Malang                 ||" << endl;
    cout << "||                                                                  ||" << endl;
    cout << "======================================================================" << endl;
    cout << "\n         Nikmati kopi berkualitas terbaik hanya untuk Anda!         " << endl;
    cout << "======================================================================" << endl;
    cout << endl;
}


// Fungsi display closing (modul 6)

void displayThankYouMessage() {
    system("cls");
    cout << "======================================================================" << endl;
    cout << "||                                                                  ||" << endl;
    cout << "||                    TERIMA KASIH TELAH BERKUNJUNG!                ||" << endl;
    cout << "======================================================================" << endl;
    cout << "||                    Semoga hari Anda menyenangkan!                ||" << endl;
    cout << "||                Kami tunggu kunjungan Anda berikutnya.            ||" << endl;
    cout << "||                                                                  ||" << endl;
    cout << "======================================================================" << endl;
    cout << endl;
}

void clearTerminal() {
    #ifdef _WIN32
        system("cls"); // Clear terminal on Windows
    #else
        system("clear"); // Clear terminal on Unix/Linux/Mac
    #endif
}