#include <bits/stdc++.h>
using namespace std;

class QuanlyTaikhoan {
private:
    string Filevi;
    string currentUser;

    vector<tuple<string, string, string, int>> DanhsachVi() {
        vector<tuple<string, string, string, int>> Vi;
        ifstream inputFile(Filevi);
        string line;
        while (getline(inputFile, line)) {
            stringstream ss(line);
            string ma, tk, loai;
            int sodu;
            getline(ss, ma, ',');
            getline(ss, tk, ',');
            getline(ss, loai, ',');
            ss >> sodu;
            Vi.emplace_back(ma, tk, loai, sodu);
        }
        inputFile.close();
        return Vi;
    }

    void luuDuLieuVi(const vector<tuple<string, string, string, int>>& Vi) {
        ofstream outputFile(Filevi);
        if (outputFile.is_open()) {
            for (const auto& vi : Vi) {
                outputFile << get<0>(vi) << "," << get<1>(vi) << "," 
                         << get<2>(vi) << "," << get<3>(vi) << endl;
            }
            outputFile.close();
        } else {
            cerr << "Không thể mở tệp để ghi ví." << endl;
        }
    }

public:
    QuanlyTaikhoan(const string& vidiem) : Filevi(vidiem), currentUser("") {
        auto danhsachvi = DanhsachVi();
        bool hasTongVi = false;
        for (const auto& vi : danhsachvi) {
            if (get<2>(vi) == "tong") {
                hasTongVi = true;
                break;
            }
        }
        if (!hasTongVi) {
            danhsachvi.emplace_back("TONG0001", "system", "tong", 100000000);
            luuDuLieuVi(danhsachvi);
        }
    }

    string TaomaVi() {
        const string charset = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
        string maVi;
        for (int i = 0; i < 8; ++i) {
            maVi += charset[rand() % charset.length()];
        }
        return maVi;
    }

    void taoViMoi() {
        if (currentUser.empty()) {
            cout << "Vui lòng đăng nhập trước." << endl;
            return;
        }
        cout << "\nTạo ví mới:" << endl;
        cout << "1. Ví chính (được tặng 100 điểm)" << endl;
        cout << "2. Ví phụ (không có điểm)" << endl;
        cout << "Chọn loại ví: ";
        
        int choice;
        cin >> choice;
        if (choice != 1 && choice != 2) {
            cout << "Lựa chọn không hợp lệ." << endl;
            return;
        }
        string loai = (choice == 1) ? "chinh" : "phu";
        int soDu = (choice == 1) ? 100 : 0;
        auto danhsachvi = DanhsachVi();
        string maVi = TaomaVi();
        danhsachvi.emplace_back(maVi, currentUser, loai, soDu);
        luuDuLieuVi(danhsachvi);
        cout << "Đã tạo ví " << loai << " mới với mã: " << maVi << endl;
        cout << "Số dư ban đầu: " << soDu << " điểm" << endl;
    }

    void XemsoduTk() {
        if (currentUser.empty()) {
            cout << "Vui lòng đăng nhập trước." << endl;
            return;
        }
        string TenTk = currentUser;
        auto danhsachvi = DanhsachVi();
        bool found = false;
        cout << "\nDanh sách ví của " << TenTk << ":" << endl;
        for (const auto& vi : danhsachvi) {
            if (get<1>(vi) == TenTk) {
                cout << "- Mã ví: " << get<0>(vi) 
                     << " | Loại: " << get<2>(vi) 
                     << " | Số dư: " << get<3>(vi) << " điểm" << endl;
                found = true;
            }
        }
        if (!found) {
            cout << "Không tìm thấy ví nào." << endl;
        }
    }
};