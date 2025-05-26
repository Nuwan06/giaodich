#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <ctime>
#include <algorithm>
#include <iomanip>
#include <cctype>
#include <functional>
#include <cstdlib>

using namespace std;

// Hàm băm mật khẩu
size_t hashFunction(const string& input) {
    hash<string> hashObj;
    return hashObj(input);
}

// Hàm sinh OTP ngẫu nhiên
string TaoOTP() {
    const string charset = "0123456789";
    string otp;
    for (int i = 0; i < 6; ++i) {
        otp += charset[rand() % charset.length()];
    }
    return otp;
}

// Hàm sinh mật khẩu tự động
string TaoMatkhauTudong() {
    const string charset = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    string matkhau;
    for (int i = 0; i < 8; ++i) {
        matkhau += charset[rand() % charset.length()];
    }
    return matkhau;
}

class QuanlyTaikhoan {
private:
    string FileQuanlyTk;
    string Fileotp;
    string currentUser;
    bool isAdmin;

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
            getline(ss, MatkhauinFile, ',');
            if (TenTkinFile == TenTk && MatkhauinFile == to_string(hashFunction(Matkhau))) {
                inputFile.close();
                return true;
            }
        }
        inputFile.close();
        return false;
    }

    void LuulaiTaikhoan(const vector<tuple<string, string, string, string, bool>>& Taikhoan) {
        ofstream outputFile(FileQuanlyTk);
        if (outputFile.is_open()) {
            for (const auto& TK : Taikhoan) {
                outputFile << get<0>(TK) << "," << get<1>(TK) << "," << get<2>(TK) << "," 
                         << get<3>(TK) << "," << (get<4>(TK) ? "1" : "0") << endl;
            }
            outputFile.close();
        } else {
            cerr << "Không thể mở tệp để ghi tài khoản." << endl;
        }
    }

    vector<tuple<string, string, string, string, bool>> TK() {
        vector<tuple<string, string, string, string, bool>> tk;
        ifstream inputFile(FileQuanlyTk);
        string line;
        while (getline(inputFile, line)) {
            stringstream ss(line);
            string ten, mk, email, cccd;
            string adminStr;
            getline(ss, ten, ',');
            getline(ss, mk, ',');
            getline(ss, email, ',');
            getline(ss, cccd, ',');
            getline(ss, adminStr);
            bool admin = (adminStr == "1");
            tk.emplace_back(ten, mk, email, cccd, admin);
        }
        inputFile.close();
        return tk;
    }

public:
    QuanlyTaikhoan(const string& fileTk,  const string& otp) {
        FileQuanlyTk = fileTk;
        Fileotp = otp;
        currentUser = "";
        isAdmin = false;
    }
    
    // Getter methods
    string getCurrentUser() const { return currentUser; }
    bool getIsAdmin() const { return isAdmin; }


    void TaoTaikhoan() {
        string TenTk, Matkhau, email, socccd;
        char choice;
        cout << "Nhập tên tài khoản mới: ";
        cin >> TenTk;
        if (KiemtraTenlogin(TenTk)) {
            cout << "Tên tài khoản '" << TenTk << "' đã tồn tại." << endl;
            return;
        }
        cout << "Bạn có muốn nhập mật khẩu? (y/n): ";
        cin >> choice;
        if (tolower(choice) == 'y') {
            cout << "Nhập mật khẩu: ";
            cin >> Matkhau;
        } else {
            Matkhau = TaoMatkhauTudong();
            cout << "Mật khẩu tự động được tạo: " << Matkhau << endl;
        }
        cout << "Nhập email: ";
        cin >> email;
        cout << "Nhập số căn cước công dân: ";
        cin >> socccd;
        bool admin = (TenTk == "admin");
        ofstream outputFile(FileQuanlyTk, ios::app);
        if (outputFile.is_open()) {
            outputFile << TenTk << "," << hashFunction(Matkhau) << "," << email << "," 
                     << socccd << "," << (admin ? "1" : "0") << endl;
            outputFile.close();
            cout << "Tài khoản '" << TenTk << "' đã được tạo thành công!" << endl;
            if (tolower(choice) != 'y') {
                cout << "Lưu ý: Mật khẩu tự động này sẽ không hiển thị lại. Hãy ghi nhớ hoặc đổi mật khẩu sau khi đăng nhập." << endl;
            }

            
        } else {
            cerr << "Không thể mở tệp để ghi." << endl;
        }
    }

    bool login() {
        string TenTk, Matkhau, inputOTP;
        cout << "Nhập tên tài khoản: ";
        cin >> TenTk;
        if (!KiemtraTenlogin(TenTk)) {
            cout << "Tên tài khoản không đúng" << endl;
            return false;
        }
        cout << "Nhập mật khẩu: ";
        cin >> Matkhau;
        if (KiemtranThongtinTk(TenTk, Matkhau)) {
            string otp = TaoOTP();
            ofstream otpFile(Fileotp);
            otpFile << TenTk << "," << otp << endl;
            otpFile.close();
            auto tk = TK();
            for (const auto& t : tk) {
                if (get<0>(t) == TenTk) {
                    cout << "Đã gửi mã OTP "  << " đến email " << get<2>(t) << endl;
                    break;
                }
            }
            cout << "Nhập OTP để xác thực: ";
            cin >> inputOTP;
            if (inputOTP == otp) {
                currentUser = TenTk;
                isAdmin = (TenTk == "admin");
                cout << "Đăng nhập thành công với tài khoản '" << TenTk << "'!" << endl;
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

    void ThaydoiMatkhau() {
        if (currentUser.empty()) {
            cout << "Vui lòng đăng nhập trước." << endl;
            return;
        }
        string MatkhauCu, MatkhauMoi, XacnhanMatkhauMoi, inputOTP;
        cout << "Nhập mật khẩu cũ: ";
        cin >> MatkhauCu;
        if (!KiemtranThongtinTk(currentUser, MatkhauCu)) {
            cout << "Mật khẩu cũ không đúng." << endl;
            return;
        }
        string otp = TaoOTP();
        ofstream otpFile(Fileotp);
        otpFile << currentUser << "," << otp << endl;
        otpFile.close();
        auto tk = TK();
        for (const auto& t : tk) {
            if (get<0>(t) == currentUser) {
                cout << "Đã gửi mã OTP " << " đến email " << get<2>(t) << endl;
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
        auto tkList = TK();
        for (auto& t : tkList) {
            if (get<0>(t) == currentUser) {
                get<1>(t) = to_string(hashFunction(MatkhauMoi));
                break;
            }
        }
        LuulaiTaikhoan(tkList);
        cout << "Đổi mật khẩu thành công!" << endl;
    }

    
};

int main() {
    srand(time(0));
    QuanlyTaikhoan x("accounts.txt","fileotp.txt");
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