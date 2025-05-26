#include <bits/stdc++.h>
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

string TaomaVi() {
    const string charset = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    string maVi;
    for (int i = 0; i < 8; ++i) {
        maVi += charset[rand() % charset.length()];
    }
    return maVi;
}

class QuanlyTaikhoan {
private:
    string FileQuanlyTk;
    string Filevi;
    string Filegiaodich;
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

    vector<tuple<string, string, string, int, string, string>> LichsuGiaodich() {
        vector<tuple<string, string, string, int, string, string>> Giaodich;
        ifstream inputFile(Filegiaodich);
        string line;
        while (getline(inputFile, line)) {
            stringstream ss(line);
            string ma, nguon, dich, thoigian, trangthai;
            int diem;
            getline(ss, ma, ',');
            getline(ss, nguon, ',');
            getline(ss, dich, ',');
            ss >> diem;
            cin.ignore();
            getline(ss, thoigian, ',');
            getline(ss, trangthai);
            Giaodich.emplace_back(ma, nguon, dich, diem, thoigian, trangthai);
        }
        inputFile.close();
        return Giaodich;
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

    void luuDuLieuGiaodich(const vector<tuple<string, string, string, int, string, string>>& Lichsu) {
        ofstream outputFile(Filegiaodich);
        if (outputFile.is_open()) {
            for (const auto& gd : Lichsu) {
                outputFile << get<0>(gd) << "," << get<1>(gd) << "," 
                         << get<2>(gd) << "," << get<3>(gd) << "," 
                         << get<4>(gd) << "," << get<5>(gd) << endl;
            }
            outputFile.close();
        } else {
            cerr << "Không thể mở tệp để ghi giao dịch." << endl;
        }
    }

public:
    QuanlyTaikhoan(const string& fileTk, const string& vidiem, const string& giaodich, const string& otp) {
        FileQuanlyTk = fileTk;
        Filevi = vidiem;
        Filegiaodich = giaodich;
        Fileotp = otp;
        currentUser = "";
        isAdmin = false;
        // Kiểm tra và tạo ví tổng nếu chưa có
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
            // Tạo ví chính mặc định cho tài khoản mới
            if (!admin) {
                auto danhsachvi = DanhsachVi();
                string maVi = TaomaVi();
                danhsachvi.emplace_back(maVi, TenTk, "chinh", 100);
                luuDuLieuVi(danhsachvi);
                cout << "Đã tạo ví chính mặc định (" << maVi << ") với 100 điểm." << endl;
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

    void XemDanhSachNhom() {
        if (!isAdmin) {
            cout << "Bạn không có quyền truy cập chức năng này!" << endl;
            return;
        }

        auto danhsach = TK();
        cout << "\nDANH SÁCH TÀI KHOẢN HỆ THỐNG" << endl;
        cout << "--------------------------------------------------" << endl;
        cout << setw(20) << left << "Tên tài khoản" 
             << setw(25) << "Email" 
             << setw(15) << "Loại tài khoản" << endl;
        
        for (const auto& tk : danhsach) {
            cout << setw(20) << left << get<0>(tk)
                 << setw(25) << get<2>(tk)
                 << setw(15) << (get<4>(tk) ? "Admin" : "User") << endl;
        }
    }

    void AdminTaoTaiKhoan() {
        if (!isAdmin) {
            cout << "Bạn không có quyền truy cập chức năng này!" << endl;
            return;
        }
        TaoTaikhoan();
    }

    void DieuChinhThongTin() {
        if (!isAdmin) {
            cout << "Bạn không có quyền truy cập chức năng này!" << endl;
            return;
        }
        string tenTaiKhoan;
        cout << "Nhập tên tài khoản cần điều chỉnh: ";
        cin >> tenTaiKhoan;
        auto danhsach = TK();
        bool found = false;
        tuple<string, string, string, string, bool>* taiKhoanCanSua = nullptr;
        for (auto& tk : danhsach) {
            if (get<0>(tk) == tenTaiKhoan) {
                found = true;
                taiKhoanCanSua = &tk;
                break;
            }
        }
        if (!found) {
            cout << "Không tìm thấy tài khoản!" << endl;
            return;
        }

        cout << "\nTHÔNG TIN HIỆN TẠI" << endl;
        cout << "Email: " << get<2>(*taiKhoanCanSua) << endl;
        cout << "Số CCCD: " << get<3>(*taiKhoanCanSua) << endl;
        string emailMoi, cccdMoi;
        cout << "\nNhập email mới (bỏ trống nếu không đổi): ";
        cin.ignore();
        getline(cin, emailMoi);
        cout << "Nhập số CCCD mới (bỏ trống nếu không đổi): ";
        getline(cin, cccdMoi);
        if (!emailMoi.empty() || !cccdMoi.empty()) {
            string otp = TaoOTP();
            ofstream otpFile(Fileotp);
            otpFile << tenTaiKhoan << "," << otp << endl;
            otpFile.close();
            cout << "\nTHAY ĐỔI SẼ ĐƯỢC ÁP DỤNG:" << endl;
            if (!emailMoi.empty()) 
                cout << "Email: " << get<2>(*taiKhoanCanSua) << " -> " << emailMoi << endl;
            if (!cccdMoi.empty()) 
                cout << "CCCD: " << get<3>(*taiKhoanCanSua) << " -> " << cccdMoi << endl;

            string otpNhap;
            cout << "\nNhập OTP từ chủ tài khoản để xác nhận: ";
            cin >> otpNhap;

            if (otpNhap == otp) {
                if (!emailMoi.empty()) get<2>(*taiKhoanCanSua) = emailMoi;
                if (!cccdMoi.empty()) get<3>(*taiKhoanCanSua) = cccdMoi;
                
                LuulaiTaikhoan(danhsach);
                cout << "Cập nhật thông tin thành công!" << endl;
            } else {
                cout << "OTP không hợp lệ. Thao tác bị hủy bỏ." << endl;
            }
        } else {
            cout << "Không có thay đổi nào được thực hiện." << endl;
        }
    }

    void chuyenDiem() {
        if (currentUser.empty()) {
            cout << "Vui lòng đăng nhập trước." << endl;
            return;
        }
        auto danhsachvi = DanhsachVi();
        cout << "\nDanh sách ví của bạn:" << endl;
        for (const auto& vi : danhsachvi) {
            if (get<1>(vi) == currentUser) {
                cout << "- Mã ví: " << get<0>(vi) << " (" << get<2>(vi) 
                     << "), Số dư: " << get<3>(vi) << " điểm" << endl;
            }
        }
        string maViNguon, maViDich;
        int soDiem;
        cout << "\nNhập mã ví nguồn: ";
        cin >> maViNguon;
        cout << "Nhập mã ví đích: ";
        cin >> maViDich;
        cout << "Nhập số điểm cần chuyển: ";
        cin >> soDiem;
        if (soDiem <= 0) {
            cout << "Số điểm phải lớn hơn 0" << endl;
            return;
        }
        bool validNguon = false;
        bool validDich = false;
        int viNguonIndex = -1;
        int viDichIndex = -1;
        for (size_t i = 0; i < danhsachvi.size(); ++i) {
            if (get<0>(danhsachvi[i]) == maViNguon && get<1>(danhsachvi[i]) == currentUser) {
                validNguon = true;
                viNguonIndex = i;
            }
            if (get<0>(danhsachvi[i]) == maViDich) {
                validDich = true;
                viDichIndex = i;
            }
        }
        if (!validNguon) {
            cout << "Ví nguồn không hợp lệ hoặc không thuộc quyền sở hữu của bạn." << endl;
            return;
        }
        if (!validDich) {
            cout << "Ví đích không tồn tại." << endl;
            return;
        }
        if (get<3>(danhsachvi[viNguonIndex]) < soDiem) {
            cout << "Số dư không đủ để thực hiện giao dịch." << endl;
            return;
        }
        string otp = TaoOTP();
        ofstream otpFile(Fileotp);
        otpFile << currentUser << "," << otp << endl;
        otpFile.close();
        auto tk = TK();
        for (const auto& t : tk) {
            if (get<0>(t) == currentUser) {
                cout << "Đã gửi mã OTP "  << " đến email " << get<2>(t) << endl;
                break;
            }
        }
        string inputOTP;
        cout << "Nhập OTP: ";
        cin >> inputOTP;
        if (inputOTP != otp) {
            cout << "OTP không đúng. Giao dịch bị hủy." << endl;
            return;
        }
        get<3>(danhsachvi[viNguonIndex]) -= soDiem;
        get<3>(danhsachvi[viDichIndex]) += soDiem;
        luuDuLieuVi(danhsachvi);
        time_t now = time(0);
        string tg = ctime(&now);
        tg.pop_back();
        string maGD = "GD" + to_string(LichsuGiaodich().size() + 1);
        auto lsgd = LichsuGiaodich();
        lsgd.emplace_back(maGD, maViNguon, maViDich, soDiem, tg, "thanh_cong");
        luuDuLieuGiaodich(lsgd);
        cout << "Chuyển điểm thành công!" << endl;
        cout << "Mã giao dịch: " << maGD << endl;
    }

    void xemLichSuGiaoDich() {
        if (currentUser.empty()) {
            cout << "Vui lòng đăng nhập trước." << endl;
            return;
        }
        cout << "\nLịch sử giao dịch của bạn:" << endl;
        auto lsgd = LichsuGiaodich();
        auto danhsachvi = DanhsachVi();
        vector<string> userVi;
        for (const auto& vi : danhsachvi) {
            if (get<1>(vi) == currentUser) {
                userVi.push_back(get<0>(vi));
            }
        }
        bool found = false;
        for (const auto& gd : lsgd) {
            if (find(userVi.begin(), userVi.end(), get<1>(gd)) != userVi.end() || 
                find(userVi.begin(), userVi.end(), get<2>(gd)) != userVi.end()) {
                cout << "Mã GD: " << get<0>(gd) 
                     << " | Từ: " << get<1>(gd) 
                     << " | Đến: " << get<2>(gd)
                     << " | Số điểm: " << get<3>(gd)
                     << " | Thời gian: " << get<4>(gd)
                     << " | Trạng thái: " << get<5>(gd) << endl;
                found = true;
            }
        }
        if (!found) {
            cout << "Không có giao dịch nào." << endl;
        }
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
        if (isAdmin) {
            cout << "Nhập tên tài khoản muốn xem (bỏ trống để xem của bạn): ";
            cin.ignore();
            getline(cin, TenTk);
            if (TenTk.empty()) {
                TenTk = currentUser;
            }
        }
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

int main() {
    srand(time(0));
    QuanlyTaikhoan ql("accounts.txt", "filevidiem.txt", "filegiaodich.txt", "fileotp.txt");
    int choice;
    bool loggedIn = false;
    do {
        cout << "\n--- HỆ THỐNG QUẢN LÝ VÍ ĐIỂM ---" << endl;
        if (!loggedIn) {
            cout << "1. Đăng nhập" << endl;
            cout << "2. Đăng ký tài khoản" << endl;
            cout << "0. Thoát" << endl;
        } else {
            if (ql.getIsAdmin()) {
                cout << "1. Quản lý tài khoản" << endl;
                cout << "2. Tạo ví mới" << endl;
                cout << "3. Chuyển điểm" << endl;
                cout << "4. Xem lịch sử giao dịch" << endl;
                cout << "5. Xem số dư" << endl;
                cout << "6. Đổi mật khẩu" << endl;
                cout << "0. Đăng xuất" << endl;
            } else {
                cout << "1. Tạo ví mới" << endl;
                cout << "2. Chuyển điểm" << endl;
                cout << "3. Xem lịch sử giao dịch" << endl;
                cout << "4. Xem số dư" << endl;
                cout << "5. Đổi mật khẩu" << endl;
                cout << "0. Đăng xuất" << endl;
            }
        }
        cout << "Chọn chức năng: ";
        cin >> choice;
        if (!loggedIn) {
            switch (choice) {
                case 1: loggedIn = ql.login();
                break;
                case 2: ql.TaoTaikhoan();
                break;
                case 0: cout << "Đang thoát..." << endl;
                return 0;
                default: cout << "Lựa chọn không hợp lệ." << endl;
            }
        } else {
            if (ql.getIsAdmin()) {
                switch (choice) {
                    case 1: {
                        int adminChoice;
                        cout << "\nQUẢN LÝ TÀI KHOẢN" << endl;
                        cout << "1. Xem danh sách tài khoản" << endl;
                        cout << "2. Tạo tài khoản mới" << endl;
                        cout << "3. Điều chỉnh thông tin tài khoản" << endl;
                        cout << "Chọn: ";
                        cin >> adminChoice;
                        
                        switch (adminChoice) {
                            case 1: ql.XemDanhSachNhom(); break;
                            case 2: ql.AdminTaoTaiKhoan(); break;
                            case 3: ql.DieuChinhThongTin(); break;
                            default: cout << "Lựa chọn không hợp lệ!" << endl;
                        }
                        break;
                    }
                    case 2: ql.taoViMoi(); break;
                    case 3: ql.chuyenDiem(); break;
                    case 4: ql.xemLichSuGiaoDich(); break;
                    case 5: ql.XemsoduTk(); break;
                    case 6: ql.ThaydoiMatkhau(); break;
                    case 0: 
                        loggedIn = false;
                        cout << "Đã đăng xuất." << endl;
                        break;
                    default: cout << "Lựa chọn không hợp lệ." << endl;
                }
            } else {
                switch (choice) {
                    case 1: ql.taoViMoi(); break;
                    case 2: ql.chuyenDiem(); break;
                    case 3: ql.xemLichSuGiaoDich(); break;
                    case 4: ql.XemsoduTk(); break;
                    case 5: ql.ThaydoiMatkhau(); break;
                    case 0: 
                        loggedIn = false;
                        cout << "Đã đăng xuất." << endl;
                        break;
                    default: cout << "Lựa chọn không hợp lệ." << endl;
                }
            }
        }
    } while (true);
    return 0;
}