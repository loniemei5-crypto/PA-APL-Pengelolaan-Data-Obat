#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <conio.h>
#include <windows.h>
#include <sstream>
#include <cctype>
#include "json.hpp"

using namespace std;
using json = nlohmann::json;

struct Obat
{
    string id;
    string nama;
    string jenis;
    string expired;
    int harga;
    int stok;
};

struct User
{
    string username;
    string password;
    string role;
};

struct Transaksi
{
    string id;
    string obat;
    int jumlah;
    int total;
    string status;
};

json loadJSON(string filename)
{
    ifstream file(filename);
    if (!file.is_open())
        return json::object();
    json data;
    file >> data;
    return data;
}

void saveJSON(string filename, json data)
{
    ofstream file(filename);
    file << data.dump(4) << endl;
}

void setColor(int color)
{
    SetConsoleTextAttribute(
        GetStdHandle(STD_OUTPUT_HANDLE),
        color);
}

string formatRupiah(int angka)
{
    string hasil = to_string(angka);
    int insertPosition = hasil.length() - 3;

    while (insertPosition > 0)
    {
        hasil.insert(insertPosition, ".");
        insertPosition -= 3;
    }

    return "Rp " + hasil;
}

vector<Obat> jsonToObat(json obatJson)
{
    vector<Obat> data;

    for (auto &o : obatJson["obat"])
    {
        Obat ob;

        ob.id = o.value("id", "");
        ob.nama = o.value("nama", "");
        ob.jenis = o.value("jenis", "");
        ob.expired = o.value("expired", "-");
        ob.harga = o.value("harga", 0);
        ob.stok = o.value("stok", 0);

        data.push_back(ob);
    }

    return data;
}

json obatToJson(vector<Obat> &data)
{
    json obatJson;
    obatJson["obat"] = json::array();

    for (auto &o : data)
    {
        json obj;
        obj["id"] = o.id;
        obj["nama"] = o.nama;
        obj["jenis"] = o.jenis;
        obj["expired"] = o.expired;
        obj["harga"] = o.harga;
        obj["stok"] = o.stok;

        obatJson["obat"].push_back(obj);
    }

    return obatJson;
}

json convertToJSON(vector<Obat> &data)
{
    json j;
    j["obat"] = json::array();

    for (auto &o : data)
    {
        json item;
        item["id"] = o.id;
        item["nama"] = o.nama;
        item["jenis"] = o.jenis;
        item["expired"] = o.expired;
        item["harga"] = o.harga;
        item["stok"] = o.stok;

        j["obat"].push_back(item);
    }

    return j;
}

bool ValidasiPassword(string password)
{
    bool adaAngka = false;
    bool adaHuruf = false;

    if (password.length() < 6)
    {
        return false;
    }

    for (char c : password)
    {
        if (isdigit(c))
        {
            adaAngka = true;
        }
        else if (isalpha(c))
        {
            adaHuruf = true;
        }
    }

    return adaAngka && adaHuruf;
}

void registerUser(json &users)
{
    try
    {
        string username, password;
        system("cls");

        setColor(11);

        cout << "╔════════════════════════════════════╗\n";

        setColor(13);

        cout << "║          MENU REGISTER             ║\n";

        setColor(11);

        cout << "╠════════════════════════════════════╣\n";

        setColor(10);

        cout << "║ Username : ";

        setColor(7);
        cin >> username;

        setColor(10);

        cout << "║ Password : ";

        setColor(7);
        cin >> password;

        cout << "╚════════════════════════════════════╝\n";

        for (auto &u : users["users"])
        {
            if (u["username"] == username)
            {
                throw runtime_error("Username Telah diGunakan!");
                return;
            }
        }

        cout << "Enter password: ";
        cin >> password;
        if (!ValidasiPassword(password))
        {
            setColor(12);
            throw runtime_error("Password harus minimal 6 karakter dan mengandung huruf serta angka!");
            return;
        }

        json u;

        u["username"] = username;
        u["password"] = password;

        u["role"] = "kasir";

        users["users"].push_back(u);

        saveJSON("users.json", users);

        cout << "Registrasi berhasil!\n";
    }
    catch (exception &e)
    {
        cout << "Gagal Register! : " << e.what() << endl;
    }
}

string loginUser(json users)
{
    string username, password;

    system("cls");

    setColor(11);

    cout << "╔════════════════════════════════════╗\n";

    setColor(14);

    cout << "║            MENU LOGIN              ║\n";

    setColor(11);

    cout << "╠════════════════════════════════════╣\n";

    setColor(10);

    cout << "║ Username : ";

    setColor(7);
    cin >> username;

    setColor(10);

    cout << "║ Password : ";

    setColor(7);
    cin >> password;

    cout << "╚════════════════════════════════════╝\n";

    for (auto &u : users["users"])
    {
        if (u["username"] == username && u["password"] == password)
        {
            return u["role"];
        }
    }
    return "gagal";
}

void tampilkanObat(vector<Obat> &data)
{
    system("cls");
    setColor(11);

    cout << "\n";
    cout << "╔═════════════════════════════════════════════════════════════════════╗\n";
    cout << "║                         DATA OBAT APOTEK                            ║\n";
    cout << "╠════╦════════════════╦════════════╦══════════╦══════════╦═══════════ ╣\n";

    setColor(14);

    cout << "║ No ║ Nama Obat      ║ Jenis      ║ Harga    ║ Stok     ║ Expired    ║\n";

    setColor(11);

    cout << "╠════╬══════════════════════╬════════════╬══════════╬══════════╬════  ╣\n";

    int no = 1;

    for (auto &o : data)
    {
        setColor(7);

        cout << "║ "
             << left << setw(3) << no++
             << "║ "
             << setw(15) << o.nama
             << "║ "
             << setw(11) << o.jenis
             << "║ "
             << setw(9) << formatRupiah(o.harga)
             << "║ "
             << setw(9) << o.stok
             << "║ "
             << setw(10)
             << (o.expired.empty() ? "-" : o.expired)
             << "║\n";
    }

    setColor(11);

    cout << "╚════╩════════════════╩════════════╩══════════╩══════════╩═══════════ ╝\n";

    setColor(7);
    cin.ignore();
    system("pause");
}

bool validTanggal(string tanggal)
{
    int tahun, bulan, hari;
    char strip1, strip2;

    stringstream ss(tanggal);

    ss >> tahun >> strip1 >> bulan >> strip2 >> hari;

    if (ss.fail())
    {
        return false;
    }

    if (strip1 != '-' || strip2 != '-')
    {
        return false;
    }

    if (bulan < 1 || bulan > 12)
    {
        return false;
    }

    if (hari < 1 || hari > 31)
    {
        return false;
    }

    return true;
}

void tambahObat(vector<Obat> &data)
{
    try
    {
        system("cls");

        Obat o;

        int nomor = data.size() + 1;
        o.id = "OB" + to_string(nomor);

        setColor(11);

        cout << "╔══════════════════════════════════════════════╗\n";

        setColor(10);

        cout << "║               TAMBAH OBAT                   ║\n";

        setColor(11);

        cout << "╠══════════════════════════════════════════════╣\n";

        setColor(14);

        cout << "║ ID Obat    : " << o.id << endl;

        setColor(3);

        cout << "║ Nama Obat  : ";

        setColor(7);
        cin >> o.nama;

        if (o.nama.empty())
            throw runtime_error("Nama tidak boleh kosong!");

        setColor(3);

        cout << "║ Jenis      : ";

        setColor(7);
        cin >> o.jenis;

        if (o.jenis.empty())
            throw runtime_error("Jenis tidak boleh kosong!");

        setColor(3);

        cout << "║ Expired    : ";

        setColor(7);
        cin >> o.expired;

        if (!validTanggal(o.expired))
            throw runtime_error("Format tanggal tidak valid!");

        setColor(3);

        cout << "║ Harga      : ";

        setColor(7);

        if (!(cin >> o.harga))
            throw runtime_error("Harga harus angka!");

        if (o.harga <= 0)
            throw runtime_error("Harga harus lebih dari 0!");

        setColor(3);

        cout << "║ Stok       : ";

        setColor(7);

        if (!(cin >> o.stok))
            throw runtime_error("Stok harus angka!");

        if (o.stok < 0)
            throw runtime_error("Stok tidak boleh negatif!");

        setColor(11);

        cout << "╠══════════════════════════════════════════════╣\n";

        data.push_back(o);
        saveJSON("obat.json", convertToJSON(data));

        setColor(10);

        cout << "║      [✔] Obat berhasil ditambahkan!         ║\n";

        setColor(11);

        cout << "╚══════════════════════════════════════════════╝\n";

        setColor(7);

        system("pause");
    }
    catch (exception &e)
    {
        cin.clear();
        cin.ignore(1000, '\n');

        setColor(12);

        cout << "\n╔══════════════════════════════════════════════╗\n";
        cout << "║              TERJADI ERROR                  ║\n";
        cout << "╠══════════════════════════════════════════════╣\n";
        cout << "║ " << e.what() << endl;
        cout << "╚══════════════════════════════════════════════╝\n";

        setColor(7);

        system("pause");
    }
}
void updateObat(vector<Obat> &data)
{
    system("cls");

    tampilkanObat(data);

    int nomor;

    setColor(11);
    cout << "\n╔══════════════════════════════════════════════╗\n";
    setColor(14);
    cout << "║                UPDATE OBAT                     ║\n";
    setColor(11);
    cout << "╠════════════════════════════════════════════════╣\n";
    setColor(3);
    cout << "║ Masukkan Nomor Obat yang diPilih : ";

    setColor(7);

    if (!(cin >> nomor))
    {
        cin.clear();
        cin.ignore(1000, '\n');

        setColor(12);
        cout << "\nInput Harus Angka!\n";

        setColor(12);

        Sleep(1000);
        return;
    }

    if (nomor < 1 || nomor > data.size())
    {
        setColor(12);

        cout << "\nNomor Obat Tidak Valid!\n";
        setColor(7);

        Sleep(1000);
        return;
    }

    Obat *o = &data[nomor - 1];
    setColor(11);
    cout << "╠══════════════════════════════════════════════╣\n";
    setColor(10);
    cout << "║ Data Lama                                    ║\n";
    setColor(7);

    cout << "║ Nama     :" << o->nama << endl;
    cout << "║ Jenis    :" << o->jenis << endl;
    cout << "║ Expired  :" << o->expired << endl;
    cout << "║ Harga    :" << formatRupiah(o->harga) << endl;
    cout << "║ Stok     :" << o->stok << endl;

    setColor(11);
    cout << "╠══════════════════════════════════════════════╣\n";
    setColor(14);

    cout << "║              INPUT DATA BARU                 ║\n";
    setColor(3);

    string input;

    cin.ignore();

    cout << "║ Nama Baru [" << o->nama << "] : ";
    getline(cin, input);

    if (!input.empty())
    {
        o->nama = input;
    }

    cout << "║ Jenis Baru [" << o->jenis << "] : ";
    getline(cin, input);

    if (!input.empty())
    {
        o->jenis = input;
    }

    cout << "║ Harga Baru [" << formatRupiah(o->harga) << "] : ";
    getline(cin, input);

    if (!input.empty())
    {
        o->harga = stoi(input);
    }

    cout << "║ Stok Baru [" << o->stok << "] : ";
    getline(cin, input);

    if (!input.empty())
    {
        o->stok = stoi(input);
    }

    cout << "║ Expired Baru [" << o->expired << "] : ";
    getline(cin, input);

    if (!input.empty())
    {
        if (!validTanggal(input))
        {
            setColor(12);

            cout << "\nFormat tanggal tidak valid!\n";

            setColor(7);

            Sleep(1000);
            return;
        }

        o->expired = input;
    }

    saveJSON("obat.json", convertToJSON(data));
    setColor(11);
    cout << "╠══════════════════════════════════════════════╣\n";
    setColor(10);
    cout << "║        Data obat berhasil diupdate!          ║\n";
    setColor(11);
    cout << "╚══════════════════════════════════════════════╝\n";
    setColor(7);
    system("pause");
}

void hapusObat(vector<Obat> &data)
{
    system("cls");

    tampilkanObat(data);

    int nomor;

    setColor(11);

    cout << "\n╔══════════════════════════════════════════════╗\n";

    setColor(12);

    cout << "║                 HAPUS OBAT                  ║\n";

    setColor(11);

    cout << "╠══════════════════════════════════════════════╣\n";

    setColor(3);

    cout << "║ Masukkan Nomor Obat : ";

    setColor(7);

    if (!(cin >> nomor))
    {
        cin.clear();
        cin.ignore(1000, '\n');

        setColor(12);

        cout << "\nInput harus angka!\n";

        setColor(7);

        Sleep(1000);
        return;
    }

    if (nomor < 1 || nomor > data.size())
    {
        setColor(12);

        cout << "\nNomor obat tidak valid!\n";

        setColor(7);

        Sleep(1000);
        return;
    }

    Obat *o = &data[nomor - 1];

    setColor(11);

    cout << "╠══════════════════════════════════════════════╣\n";

    setColor(14);

    cout << "║            DATA YANG AKAN DIHAPUS           ║\n";

    setColor(7);

    cout << "║ ID       : " << o->id << endl;
    cout << "║ Nama     : " << o->nama << endl;
    cout << "║ Jenis    : " << o->jenis << endl;
    cout << "║ Harga    : " << formatRupiah(o->harga) << endl;
    cout << "║ Stok     : " << o->stok << endl;
    cout << "║ Expired  : " << o->expired << endl;

    setColor(11);

    cout << "╠══════════════════════════════════════════════╣\n";

    char konfirmasi;

    setColor(3);

    cout << "║ Yakin ingin menghapus? (y/n) : ";

    setColor(7);

    cin >> konfirmasi;

    if (konfirmasi == 'y' || konfirmasi == 'Y')
    {
        string namaObat = o->nama;

        data.erase(data.begin() + (nomor - 1));

        saveJSON("obat.json", convertToJSON(data));

        setColor(10);

        cout << "\nObat " << namaObat << " berhasil dihapus!\n";
    }
    else
    {
        setColor(14);

        cout << "\nPenghapusan dibatalkan!\n";
    }

    setColor(7);

    Sleep(1000);
}

void searchObat(vector<Obat> &data)
{
    string keyword;

    cout << "Masukkan Nama Obat :";
    cin >> keyword;

    bool ditemukan = false;

    for (auto &o : data)
    {
        string nama = o.nama;

        if (nama.find(keyword) != string::npos)
        {

            ditemukan = true;

            cout << o.id << " | "
                 << o.nama << " | "
                 << o.jenis << " | "
                 << o.expired << " | "
                 << o.harga << " | "
                 << o.stok << endl;
        }
    }
    if (!ditemukan)
    {
        cout << "Obat dengan nama " << keyword << " tidak ditemukan!\n";
    }
}

void sortingNamaObat(vector<Obat> &data)
{
    vector<Obat> sortedData;

    for (auto &o : data)
    {
        sortedData.push_back(o);
    }

    sort(sortedData.begin(), sortedData.end(), [](Obat a, Obat b)
         { return a.nama < b.nama; });

    cout << "============= DAFTAR OBAT (SORTING NAMA) =============\n";

    for (auto &o : sortedData)
    {
        cout << o.id << " | "
             << o.nama << " | "
             << o.jenis << " | "
             << o.expired << " | "
             << o.harga << " | "
             << o.stok << endl;
    }
}

void transaksiData(vector<Obat> &data, json &transaksi)
{
    string id;
    int jumlah;

    tampilkanObat(data);

    cout << "\n Masukkan ID obat : ";
    cin >> id;

    for (auto &o : data)
    {
        if (o.id == id)
        {
            cout << "Masukkan Jumlah Obat yang di Beli : ";
            cin >> jumlah;

            if (jumlah > o.stok)
            {
                cout << "Stok Tidak Mencukupi!\n";
                return;
            }

            int totalHarga = jumlah * o.harga;
            cout << "\nTotal Harga : " << totalHarga << endl;
            cout << "\nLanjutkan Pembayaran ? (y/n) : ";
            char bayar;
            cin >> bayar;

            json t;

            int nomor = transaksi["transaksi"].size() + 1;

            t["id"] = "TR" + to_string(nomor);
            t["obat"] = o.nama;
            t["jumlah"] = jumlah;
            t["total"] = totalHarga;

            if (bayar == 'y')
            {
                t["status"] = "Selesai";
                o.stok -= jumlah;

                cout << "\n=========== STRUK ===========\n";
                cout << "ID Transaksi : " << t["id"] << endl;
                cout << "Nama Obat : " << t["obat"] << endl;
                cout << "Jumlah : " << t["jumlah"] << endl;
                cout << "--------------------------------\n";
                cout << "Total Bayar  : " << formatRupiah(totalHarga) << endl;
                cout << "Status       : SELESAI\n";
                cout << "===============================\n";
            }

            else
            {
                t["status"] = "Dibatalkan";
                cout << "Transaksi Dibatalkan!\n";
            }

            transaksi["transaksi"].push_back(t);
            saveJSON("obat.json", convertToJSON(data));

            return;
        }
    }
    cout << "Data obat dengan ID " << id << " tidak ditemukan!\n";
}

void riwayatTransaksi(json transaksi)
{
    cout << "\n================ RIWAYAT TRANSAKSI ================\n";

    cout << left
         << setw(10) << "ID"
         << setw(20) << "Obat"
         << setw(10) << "Jumlah"
         << setw(15) << "Total"
         << setw(15) << "Status"
         << endl;

    cout << "===================================================\n";

    for (auto &t : transaksi["transaksi"])
    {

        cout << left
             << setw(10) << t["id"].get<string>()
             << setw(20) << t["obat"].get<string>()
             << setw(10) << t["jumlah"].get<int>()
             << setw(15) << t["total"].get<int>()
             << setw(15) << t["status"].get<string>()
             << endl;
    }
}
void viewMenu(vector<Obat> &data)
{
    int pilihan;

    do
    {
        system("cls");

        setColor(11);

        cout << "╔══════════════════════════════════════╗\n";
        setColor(14);
        cout << "║              MENU VIEW               ║\n";
        setColor(11);
        cout << "╠══════════════════════════════════════╣\n";
        setColor(10);

        cout << "║  [1] Tampilkan Semua Data Obat       ║\n";
        cout << "║  [2] Search Nama Obat                ║\n";
        cout << "║  [3] Sorting Nama Obat               ║\n";
        cout << "║  [4] Kembali                         ║\n";
        cout << "╠══════════════════════════════════════╣\n";
        setColor(3);

        cout << "║ Pilihan : ";

        setColor(7);

        if (!(cin >> pilihan))
        {
            cin.clear();
            cin.ignore(1000, '\n');

            setColor(12);

            cout << "\nInput harus angka!\n";

            setColor(7);

            Sleep(1000);

            continue;
        }

        if (pilihan < 1 || pilihan > 4)
        {
            setColor(12);

            cout << "\nPilihan hanya 1 - 4!\n";

            setColor(7);

            Sleep(1000);

            continue;
        }

        if (pilihan == 1)
        {
            tampilkanObat(data);
        }
        else if (pilihan == 2)
        {
            searchObat(data);
        }
        else if (pilihan == 3)
        {
            sortingNamaObat(data);
        }
        else if (pilihan == 4)
        {
            break;
        }
    } while (pilihan != 4);
}

void menuKasir(vector<Obat> &data, json &transaksi)
{
    int pilihan;

    do
    {

        system("cls");

        setColor(11);

        cout << "╔══════════════════════════════════════╗\n";

        setColor(14);

        cout << "║              MENU KASIR              ║\n";

        setColor(11);

        cout << "╠══════════════════════════════════════╣\n";

        setColor(10);

        cout << "║  [1] View Data Obat                 ║\n";
        cout << "║  [2] Cari Obat                      ║\n";
        cout << "║  [3] Transaksi                      ║\n";
        cout << "║  [4] Riwayat Transaksi              ║\n";
        cout << "║  [5] Kembali                        ║\n";

        setColor(11);

        cout << "╠══════════════════════════════════════╣\n";

        setColor(3);

        cout << "║ Pilihan : ";

        setColor(7);
        cin >> pilihan;

        if (pilihan == 1)
        {
            tampilkanObat(data);
        }
        else if (pilihan == 2)
        {
            searchObat(data);
        }
        else if (pilihan == 3)
        {
            transaksiData(data, transaksi);
        }
        else if (pilihan == 4)
        {
            riwayatTransaksi(transaksi);
        }

    } while (pilihan != 5);
}

void menuAdmin(vector<Obat> &data, json &transaksi)
{
    int pilihan;

    do
    {
        system("cls");

        setColor(11);

        cout << "╔══════════════════════════════════════╗\n";

        setColor(12);

        cout << "║              MENU ADMIN              ║\n";

        setColor(11);

        cout << "╠══════════════════════════════════════╣\n";

        setColor(10);

        cout << "║  [1] Tambah Data Obat               ║\n";
        cout << "║  [2] Lihat Data Obat                ║\n";
        cout << "║  [3] Update Obat                    ║\n";
        cout << "║  [4] Hapus Obat                     ║\n";
        cout << "║  [5] Riwayat Transaksi              ║\n";
        cout << "║  [6] Logout                         ║\n";

        setColor(11);

        cout << "╠══════════════════════════════════════╣\n";

        setColor(3);

        cout << "║ Pilihan : ";

        setColor(7);
        cin >> pilihan;

        if (pilihan == 1)
        {
            tambahObat(data);
        }
        else if (pilihan == 2)
        {
            viewMenu(data);
        }
        else if (pilihan == 3)
        {
            updateObat(data);
        }
        else if (pilihan == 4)
        {
            hapusObat(data);
        }
        else if (pilihan == 5)
        {
            riwayatTransaksi(transaksi);
        }

    } while (pilihan != 6);
}

int main()
{
    SetConsoleOutputCP(CP_UTF8);
    json users = loadJSON("users.json");
    json obat = loadJSON("obat.json");
    json transaksi = loadJSON("transaksi.json");

    vector<Obat> data;

    for (auto &o : obat["obat"])
    {
        Obat ob;

        ob.id = o["id"];
        ob.nama = o["nama"];
        ob.jenis = o["jenis"];
        ob.expired = o["expired"];
        ob.harga = o["harga"];
        ob.stok = o["stok"];

        data.push_back(ob);
    }

    if (!users.contains("users"))
    {
        users["users"] = json::array();
    }

    if (!obat.contains("obat"))
    {
        obat["obat"] = json::array();
    }

    if (!transaksi.contains("transaksi"))
    {
        transaksi["transaksi"] = json::array();
    }

    int pilihan;

    do
    {
        system("cls");

        setColor(14);

        cout << "Loading System";

        for (int i = 0; i < 5; i++)
        {
            cout << ".";
            Sleep(300);
        }

        Sleep(500);

        system("cls");

        setColor(11);

        cout << "╔══════════════════════════════════════════════════════════════════════╗\n";
        cout << "║                                                                      ║\n";

        setColor(10);

        cout << "║        █████╗ ██████╗  ██████╗ ████████╗███████╗██╗  ██╗             ║\n";
        cout << "║       ██╔══██╗██╔══██╗██╔═══██╗╚══██╔══╝██╔════╝██║ ██╔╝             ║\n";
        cout << "║       ███████║██████╔╝██║   ██║   ██║   █████╗  █████╔╝              ║\n";
        cout << "║       ██╔══██║██╔═══╝ ██║   ██║   ██║   ██╔══╝  ██╔═██╗              ║\n";
        cout << "║       ██║  ██║██║     ╚██████╔╝   ██║   ███████╗██║  ██╗             ║\n";
        cout << "║       ╚═╝  ╚═╝╚═╝      ╚═════╝    ╚═╝   ╚══════╝╚═╝  ╚═╝             ║\n";

        setColor(11);

        cout << "║                                                                      ║\n";
        cout << "╠══════════════════════════════════════════════════════════════════════╣\n";

        setColor(14);

        cout << "║                SISTEM PENGELOLAAN DATA OBAT APOTEK                   ║\n";

        setColor(11);

        cout << "╠══════════════════════════════════════════════════════════════════════╣\n";

        setColor(10);

        cout << "║   [1] REGISTER                                                       ║\n";
        cout << "║   [2] LOGIN                                                          ║\n";
        cout << "║   [3] KELUAR                                                         ║\n";

        setColor(11);

        cout << "╠══════════════════════════════════════════════════════════════════════╣\n";

        cout << "╠══════════════════════════════════════════════════════════════════════╣\n";

        setColor(3);

        cout << "║   Masukkan Pilihan (1-3) : ";

        setColor(7);

        if (!(cin >> pilihan))
        {
            cin.clear();
            cin.ignore(1000, '\n');

            setColor(12);

            cout << "\nInput harus angka!\n";

            setColor(7);

            Sleep(1000);

            continue;
        }

        if (pilihan < 1 || pilihan > 3)
        {
            setColor(12);

            cout << "\nPilihan hanya 1 - 3!\n";

            setColor(7);

            Sleep(1000);

            continue;
        }

        if (pilihan == 1)
        {
            registerUser(users);
        }

        else if (pilihan == 2)
        {
            int kesempatanLogin = 3;
            bool berhasil = false;

            while (kesempatanLogin > 0)
            {
                string role = loginUser(users);

                if (role == "admin")
                {
                    cout << "Anda Login Sebagai Admin!\n";
                    menuAdmin(data, transaksi);

                    berhasil = true;
                    break;
                }
                else if (role == "kasir")
                {
                    cout << "Anda Login Sebagai Kasir!\n";
                    menuKasir(data, transaksi);

                    berhasil = true;
                    break;
                }
                else
                {
                    kesempatanLogin--;
                    cout << "\nUsername atau Password Anda Salah!\n";
                }
            }
            if (!berhasil)
            {
                cout << "Kesempatan login habis! Program Berhenti....\n";
                return 0;
            }
        }

    } while (pilihan != 3);

    return 0;
}