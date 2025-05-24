#include <bits/stdc++.h> 
using namespace std;
size_t hashFunction(const string& input) {
    hash<string> hashObj;
    return hashObj(input);
}

class QuanlyTaikhoan {
private:
    string FileQuanlyTk;

    bool KiemtraTenlogin(const string& Tenlogin) {
        ifstream inputFile(FileQuanlyTk);
        string line;
        while (getline(inputFile, line)) {
            stringstream ss(line);
            string TenTkinFile;
            getline(ss, TenTkinFile, ',');
            if (TenTkinFile == Tenlogin) {
                inputFile.close();
                return true;
            }
        }
        inputFile.close();
        return false;
    }

    bool KiemtranThongtinTk(const string& TenTk, const string& Matkhau) {
        ifstream inputFile(FileQuanlyTk);
        string line;
        while (getline(inputFile, line)) {
            stringstream ss(line);
            string TenTkinFile, MatkhauinFile;
            getline(ss, TenTkinFile, ',');
            getline(ss,MatkhauinFile, ',');
            if (TenTkinFile == TenTk && MatkhauinFile == to_string(hashFunction(Matkhau))) {
                inputFile.close();
                return true;
            }
        }
        inputFile.close();
        return false;
    }

    void LuulaiTaikhoan(const vector<tuple<string, string, string, string, int>>& Taikhoan) {
        ofstream outputFile(FileQuanlyTk);
        if (outputFile.is_open()) {
            for (const auto& TK : Taikhoan) {
                outputFile << get<0>(TK) << "," << get<1>(TK) << "," << get<2>(TK) << "," << get<3>(TK) << "," << get<4>(TK) << endl;
            }
            outputFile.close();
        } else {
            cerr << "Không thể mở tệp để ghi." << endl;
        }
    }

    vector<tuple<string ,string, string, string, int>>  TK() {
        vector<tuple<string ,string, string, string, int>> tk;
        ifstream inputFile(FileQuanlyTk);
        string line;
    while(getline(inputFile, line)) {
        stringstream ss(line);
        string ten, mk, ngaysinh, cccd;
        int tien;
        getline(ss, ten , ',');
        getline(ss, mk, ',');
        getline(ss, ngaysinh, ',');
        getline(ss, cccd, ',');
       ss >> tien;
        tk.emplace_back(ten, mk, ngaysinh, cccd, tien);
    }
inputFile.close();
return tk;
    }
    
public:
    QuanlyTaikhoan(const string& file) : FileQuanlyTk(file) {}
    void TaoTaikhoan() {
        string TenTk, Matkhau, ngaysinh, socccd;
        int Tien = 0;
        cout << "Nhập tên tài khoản mới: ";
        cin >> TenTk;
        cout << "Nhập mật khẩu cho tài khoản '" << TenTk << "': ";
        cin >> Matkhau;
        cout << "Nhập ngày sinh(theo định dạng dd/mm/yyy): ";
        cin >> ngaysinh;
        if (ngaysinh[1] == '/') {
        ngaysinh = "0" + ngaysinh;
        }
        if(ngaysinh[4] == '/') {
            ngaysinh.insert(3, "0");
        }
        cout << "Nhập số căn cước công dân: ";
        cin >> socccd;
        if (KiemtraTenlogin(TenTk)) {
            cout << "Tên tài khoản '" << TenTk << "' đã tồn tại. Vui lòng chọn tên khác." << endl;
            return;
        }
        ofstream outputFile(FileQuanlyTk, ios::app);
        if (outputFile.is_open()) {
outputFile << TenTk << "," << hashFunction(Matkhau) << "," << ngaysinh << "," << socccd << "," << Tien << endl;
            outputFile.close();
            cout << "Tài khoản '" << TenTk << "' đã được tạo thành công!" << endl;
        } else {
            cerr << "Không thể mở tệp để ghi." << endl;
        }
    }

    bool login() {
        string TenTk, Matkhau;
        cout << "Nhập tên tài khoản: ";
        cin >> TenTk;
        cout << "Nhập mật khẩu: ";
        cin >> Matkhau;
        if (KiemtranThongtinTk(TenTk, Matkhau)) {
            cout << "Đăng nhập thành công với tài khoản '" << TenTk << "'!" << endl;
       return true;
        } else {
            cout << "Đăng nhập thất bại. Tên tài khoản hoặc mật khẩu không đúng." << endl;
            return false;
        } 
}

void XemsoduTk(){
    string TenTk;
    cout << "Nhập tên tài khoản muốn xem số dư: ";
    cin >> TenTk;
    vector<tuple<string, string, string, string, int>> tk = TK();
      for (const auto& tks : tk) {
        if (get<0>(tks) == TenTk) {
            cout <<"Số dư tài khoản '" << TenTk << "':"<< get<4>(tks) << " VNĐ"<< endl;
            return;
        }
      }
      cout << "Tên tài khoản không đúng."<< endl;
}

void NapthemTienvaoTk() {
    string TenTk;
    int Tiennap;
    cout << "Nhập tên tài khoản muốn nạp thêm tiền: ";
    cin >> TenTk;
     if (! KiemtraTenlogin(TenTk)) {
        cout << "Không tìm thấy tài khoản '" << TenTk <<"'." << endl;
        return;
    }
    cout << "Nhập số tiền cần nạp: ";
    cin >> Tiennap;
    if (Tiennap <= 0) {
        cout << "Số tiền nạp phải lớn hơn không." << endl;
    return;
    }
    vector <tuple<string, string, string, string, int>> tK = TK();
    for (auto& tks : tK) {
        if(get<0>(tks)== TenTk) {
            get<4>(tks) += Tiennap;
            break;
        }
    }
    LuulaiTaikhoan(tK);
    cout << "Nạp " << Tiennap << " Vào tài khoản '" << TenTk << "' thành công." << endl;
}

    void ThaydoiMatkhau() {
        string TenTk, MatkhauCu, MatkhauMoi, XacnhanMatkhauMoi;
        cout << "Nhập tên tài khoản muốn đổi mật khẩu: ";
        cin >> TenTk;
        if (!KiemtraTenlogin(TenTk)) {
            cout << "Tài khoản '" << TenTk << "' không tồn tại." << endl;
            return;
        }
        cout << "Nhập mật khẩu cũ: ";
        cin >> MatkhauCu;

        if (! KiemtranThongtinTk(TenTk, MatkhauCu)) {
            cout << "Mật khẩu cũ không đúng." << endl;
            return;
        }
        cout << "Nhập mật khẩu mới: ";
        cin >> MatkhauMoi;
        cout << "Xác nhận mật khẩu mới: ";
        cin >> XacnhanMatkhauMoi;
        if (MatkhauMoi != XacnhanMatkhauMoi) {
            cout << "Mật khẩu mới và mật khẩu xác nhận không khớp." << endl;
            return;
        }
        vector <tuple<string, string, string, string, int>> tk = TK();
        for (auto& tks : tk) {
            if (get<0>(tks) == TenTk) {
                get<1>(tks) = to_string(hashFunction(MatkhauMoi));
                break;
            }
        }
       LuulaiTaikhoan(tk);
        cout << "Đổi mật khẩu cho tài khoản '" << TenTk << "' thành công!" << endl;
    }
    
};

int main() {
    QuanlyTaikhoan x("accounts.txt");
    int choice;
    do {
        cout << "\n--- Chức năng ---" << endl;
        cout << "1. Tạo tài khoản" << endl;
        cout << "2. Đăng nhập" << endl;
        cout << "3. Đổi mật khẩu" << endl;
        cout << "4. xem số dư" << endl;
        cout << "5. Nạp thêm tiền vào tài khoản" << endl;
        cout << "0. Thoát" << endl;
        cout << "Chọn một chức năng: ";
        cin >> choice;
        switch (choice) {
            case 1:
                x.TaoTaikhoan();
                break;
            case 2:
               x.login();
                break;
            case 3:
                x.ThaydoiMatkhau();
                break;
                case 4:
                x.XemsoduTk();
                break;
                case 5:
                x.NapthemTienvaoTk();
                break;
            case 0:
                cout << "Đang thoát..." << endl;
                break;
            default:
                cout << "Lựa chọn không hợp lệ. Vui lòng thử lại." << endl;
        }
    } while (choice != 0);
    return 0;
}