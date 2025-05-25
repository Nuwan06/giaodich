#include <bits/stdc++.h>
using namespace std;

// Hàm băm mật khẩu
size_t hashFunction(const string& input) {
    hash<string> hashObj;
    return hashObj(input);
}

// Hàm sinh OTP ngẫu nhiên
string generateOTP() {
    const string charset = "0123456789";
    string otp;
    for (int i = 0; i < 6; ++i) {
        otp += charset[rand() % charset.length()];
    }
    return otp;
}

// Hàm sinh mật khẩu tự động (8 ký tự gồm chữ hoa, chữ thường và số)
string generatePassword() {
    const string charset = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    string password;
    for (int i = 0; i < 8; ++i) {
        password += charset[rand() % charset.length()];
    }
    return password;
}
class QuanlyTaikhoan {
private:
    string FileQuanlyTk; // File lưu tài khoản
    string currentUser;  // Người dùng hiện tại
    bool isAdmin;        // Phân quyền: true nếu là admin

    // Kiểm tra tên tài khoản có tồn tại không
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

    // Kiểm tra thông tin đăng nhập
    bool KiemtranThongtinTk(const string& TenTk, const string& Matkhau) {
        ifstream inputFile(FileQuanlyTk);
        string line;
        while (getline(inputFile, line)) {
            stringstream ss(line);
            string TenTkinFile, MatkhauinFile;
            getline(ss, TenTkinFile, ',');
            getline(ss, MatkhauinFile, ',');
            if (TenTkinFile == TenTk && MatkhauinFile == to_string(hashFunction(Matkhau))) {
                inputFile.close();
                return true;
            }
        }
        inputFile.close();
        return false;
    }

    // Lưu danh sách tài khoản
    void LuulaiTaikhoan(const vector<tuple<string, string, string, string, int, int, bool>>& Taikhoan) {
        ofstream outputFile(FileQuanlyTk);
        if (outputFile.is_open()) {
            for (const auto& TK : Taikhoan) {
                outputFile << get<0>(TK) << "," << get<1>(TK) << "," << get<2>(TK) << "," 
                           << get<3>(TK) << "," << get<4>(TK) << "," << get<5>(TK) << "," 
                           << (get<6>(TK) ? "1" : "0") << endl;
            }
            outputFile.close();
        } else {
            cerr << "Không thể mở tệp để ghi tài khoản." << endl;
        }
    }

    // Đọc danh sách tài khoản
    vector<tuple<string, string, string, string, int, int, bool>> TK() {
        vector<tuple<string, string, string, string, int, int, bool>> tk;
        ifstream inputFile(FileQuanlyTk);
        string line;
        while (getline(inputFile, line)) {
            stringstream ss(line);
            string ten, mk, email, cccd;
            int viChinh, viPhu;
            bool admin;
            getline(ss, ten, ',');
            getline(ss, mk, ',');
            getline(ss, email, ',');
            getline(ss, cccd, ',');
            ss >> viChinh;
            ss.ignore(1);
            ss >> viPhu;
            ss.ignore(1);
            ss >> admin;
            tk.emplace_back(ten, mk, email, cccd, viChinh, viPhu, admin);
        }
        inputFile.close();
        return tk;
    }
    public:
    QuanlyTaikhoan(const string& fileTk) {
        FileQuanlyTk = fileTk;

    }

    // Tạo tài khoản với mật khẩu tự động nếu cần
    void TaoTaikhoan() {
        string TenTk, Matkhau, email, socccd;
        int viChinh = 0, viPhu = 0;
        bool admin = false;
        char choice;
        
        cout << "Nhập tên tài khoản mới: ";
        cin >> TenTk;
        
        cout << "Bạn có muốn nhập mật khẩu? (y/n): ";
        cin >> choice;
        
        if (tolower(choice) == 'y') {
            cout << "Nhập mật khẩu: ";
            cin >> Matkhau;
        } else {
            Matkhau = generatePassword();
            cout << "Mật khẩu tự động được tạo: " << Matkhau << endl;
        }
        
        cout << "Nhập email: ";
        cin >> email;
        cout << "Nhập số căn cước công dân: ";
        cin >> socccd;
        
        if (KiemtraTenlogin(TenTk)) {
            cout << "Tên tài khoản '" << TenTk << "' đã tồn tại." << endl;
            return;
        }
        
        if (TenTk == "admin") admin = true; // Tài khoản admin mặc định
        
        ofstream outputFile(FileQuanlyTk, ios::app);
        if (outputFile.is_open()) {
            outputFile << TenTk << "," << hashFunction(Matkhau) << "," << email << "," 
                       << socccd << "," << viChinh << "," << viPhu << "," << (admin ? "1" : "0") << endl;
            outputFile.close();
            cout << "Tài khoản '" << TenTk << "' đã được tạo thành công!" << endl;
            if (tolower(choice) != 'y') {
                cout << "Lưu ý: Mật khẩu tự động này sẽ không hiển thị lại. Hãy ghi nhớ hoặc đổi mật khẩu sau khi đăng nhập." << endl;
            }
        } else {
            cerr << "Không thể mở tệp để ghi." << endl;
        }
    }

    // Đăng nhập với OTP
    bool login() {
        string TenTk, Matkhau, inputOTP;
        cout << "Nhập tên tài khoản: ";
        cin >> TenTk;
        cout << "Nhập mật khẩu: ";
        cin >> Matkhau;
        if (KiemtranThongtinTk(TenTk, Matkhau)) {
            string otp = generateOTP();
            ofstream outputFile("otp");
            outputFile << TenTk<<"," << otp << endl;
            vector<tuple<string, string, string, string, int, int, bool>> tk = TK();
            for (auto& tks : tk) {
              if (get<0>(tks) == TenTk) {
             string email = get<2>(tks);
            
        cout << "Da gui ma OTP "  << " den email " << email << endl; // In thông báo gửi OTP ra console.
            break;
            }
        }
    
            cout << "Nhập OTP để xác thực: ";
            cin >> inputOTP;
            if (inputOTP == otp) {
                cout << "Đăng nhập thành công với tài khoản '" << TenTk << "'!" << endl;
                currentUser = TenTk;
                isAdmin = (TenTk == "admin"); // Kiểm tra quyền admin
                return true;
            } else {
                cout << "OTP không đúng. Đăng nhập thất bại." << endl;
                return false;
            }
        } else {
            cout << "Tên tài khoản hoặc mật khẩu không đúng." << endl;
            return false;
        }
    }
    // Đổi mật khẩu với OTP
    void ThaydoiMatkhau() {
        if (currentUser.empty()) {
            cout << "Vui lòng đăng nhập trước." << endl;
            return;
        }
        string TenTk = currentUser;
        string MatkhauCu, MatkhauMoi, XacnhanMatkhauMoi, inputOTP;
        cout << "Nhập mật khẩu cũ: ";
        cin >> MatkhauCu;
        if (!KiemtranThongtinTk(TenTk, MatkhauCu)) {
            cout << "Mật khẩu cũ không đúng." << endl;
            return;
        }
        string otp = generateOTP();
            ofstream outputFile("otp");
            outputFile << TenTk<<"," << otp << endl;
            vector<tuple<string, string, string, string, int, int, bool>> tkmk = TK();
            for (auto& tks : tkmk) {
              if (get<0>(tks) == TenTk) {
             string email = get<2>(tks);
            
        cout << "Da gui ma OTP "  << " den email " << email << endl; // In thông báo gửi OTP ra console.
            break;
            }
        }
        cout << "Nhập OTP: ";
        cin >> inputOTP;
        if (inputOTP != otp) {
            cout << "OTP không đúng. Đổi mật khẩu thất bại." << endl;
            return;
        }
        cout << "Nhập mật khẩu mới: ";
        cin >> MatkhauMoi;
        cout << "Xác nhận mật khẩu mới: ";
        cin >> XacnhanMatkhauMoi;
        if (MatkhauMoi != XacnhanMatkhauMoi) {
            cout << "Mật khẩu mới và xác nhận không khớp." << endl;
            return;
        }
        vector<tuple<string, string, string, string, int, int, bool>> tk = TK();
        for (auto& tks : tk) {
            if (get<0>(tks) == TenTk) {
                get<1>(tks) = to_string(hashFunction(MatkhauMoi));
                break;
            }
        }
        LuulaiTaikhoan(tk);
        cout << "Đổi mật khẩu thành công!" << endl;
    }
};
int main() {
    srand(time(0));
    QuanlyTaikhoan x("accounts.txt");
    int choice;
    bool loggedIn = false;
    do {
        cout << "\n--- Chức năng ---" << endl;
        cout << "1. Tạo tài khoản" << endl;
        cout << "2. Đăng nhập" << endl;
        if (loggedIn) {
            cout << "3. Đổi mật khẩu" << endl;
            cout << "0. Thoát" << endl;
        } else {
            cout << "0. Thoát" << endl;
        }
        cout << "Chọn một chức năng: ";
        cin >> choice;
        switch (choice) {
            case 1:
                x.TaoTaikhoan();
                break;
            case 2:
                loggedIn = x.login();
                break;
            case 3:
                if (loggedIn) x.ThaydoiMatkhau();
                else cout << "Vui lòng đăng nhập trước." << endl;
                break;
            case 0:
                cout << "Đang thoát..." << endl;
                break;
            default:
                cout << "Lựa chọn không hợp lệ." << endl;
        }
    } while (choice != 0);
    return 0;
}