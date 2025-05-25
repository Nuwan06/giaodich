#include <bits/stdc++.h>
using namespace std;

// Hàm hashFunctionhashFunction mật khẩu: Chuyển chuỗi mật khẩu thành giá trị băm để bảo mật
// Input: Chuỗi mật khẩu (string)
// Output: Giá trị băm (size_t)
// Xử lý: Sử dụng std::hash để tạo giá trị băm duy nhất cho mật khẩu
size_t hashFunction(const string& input) {
    hash<string> hashObj;
    return hashObj(input);
}

// Hàm sinh OTP ngẫu nhiên: Tạo mã OTP 4 chữ số để xác thực
// Input: Không có
// Output: Chuỗi OTP 4 chữ số (string)
// Xử lý: Dùng rand() để tạo số ngẫu nhiên từ 1000–9999
string generateOTP() {
    srand(time(0)); // Khởi tạo seed ngẫu nhiên dựa trên thời gian
    int otp = rand() % 9000 + 1000; // Sinh số 4 chữ số
    return to_string(otp); // Chuyển số thành chuỗi
}

class QuanlyTaikhoan {
private:
    string FileQuanlyTk; // Tên file lưu thông tin tài khoản (accounts.txt)
    string FileGiaoDich; // Tên file lưu lịch sử giao dịch (transactions.txt)
    string currentUser;  // Tên người dùng hiện tại (rỗng nếu chưa đăng nhập)
    bool isAdmin;        // Quyền admin: true nếu người dùng là admin

    // Kiểm tra tên tài khoản có tồn tại trong file accounts.txt
    // Input: Tên tài khoản (string)
    // Output: true nếu tồn tại, false nếu không
    // Xử lý: Đọc file, so sánh tên tài khoản với từng dòng
    bool KiemtraTenlogin(const string& Tenlogin) {
        ifstream inputFile(FileQuanlyTk); // Mở file tài khoản
        string line;
        while (getline(inputFile, line)) { // Đọc từng dòng
            stringstream ss(line);
            string TenTkinFile;
            getline(ss, TenTkinFile, ','); // Lấy tên tài khoản từ dòng
            if (TenTkinFile == Tenlogin) { // So sánh
                inputFile.close();
                return true;
            }
        }
        inputFile.close();
        return false;
    }

    // Kiểm tra thông tin đăng nhập (tên tài khoản và mật khẩu)
    // Input: Tên tài khoản, mật khẩu (string)
    // Output: true nếu thông tin đúng, false nếu sai
    // Xử lý: Đọc file, so sánh tên và mật khẩu băm
    bool KiemtranThongtinTk(const string& TenTk, const string& Matkhau) {
        ifstream inputFile(FileQuanlyTk); // Mở file tài khoản
        string line;
        while (getline(inputFile, line)) { // Đọc từng dòng
            stringstream ss(line);
            string TenTkinFile, MatkhauinFile;
            getline(ss, TenTkinFile, ','); // Lấy tên tài khoản
            getline(ss, MatkhauinFile, ','); // Lấy mật khẩu băm
            if (TenTkinFile == TenTk && MatkhauinFile == to_string(hashFunction(Matkhau))) { // So sánh
                inputFile.close();
                return true;
            }
        }
        inputFile.close();
        return false;
    }

    // Lưu danh sách tài khoản vào file accounts.txt
    // Input: Danh sách tài khoản (vector<tuple>)
    // Output: Không có (ghi file)
    // Xử lý: Ghi từng tài khoản vào file với định dạng CSV
    void LuulaiTaikhoan(const vector<tuple<string, string, string, string, int, int, bool>>& Taikhoan) {
        ofstream outputFile(FileQuanlyTk); // Mở file để ghi
        if (outputFile.is_open()) {
            for (const auto& TK : Taikhoan) { // Duyệt từng tài khoản
                outputFile << get<0>(TK) << "," << get<1>(TK) << "," << get<2>(TK) << "," 
                           << get<3>(TK) << "," << get<4>(TK) << "," << get<5>(TK) << "," 
                           << (get<6>(TK) ? "1" : "0") << endl; // Ghi thông tin: tên, mật khẩu, ngày sinh, CCCD, ví chính, ví phụ, admin
            }
            outputFile.close();
        } else {
            cerr << "Không thể mở tệp để ghi tài khoản." << endl; // Thông báo lỗi
        }
    }

    // Đọc danh sách tài khoản từ file accounts.txt
    // Input: Không có
    // Output: Danh sách tài khoản (vector<tuple>)
    // Xử lý: Đọc file, phân tích từng dòng thành tuple
    vector<tuple<string, string, string, string, int, int, bool>> TK() {
        vector<tuple<string, string, string, string, int, int, bool>> tk; // Danh sách tài khoản
        ifstream inputFile(FileQuanlyTk); // Mở file
        string line;
        while (getline(inputFile, line)) { // Đọc từng dòng
            stringstream ss(line);
            string ten, mk, ngaysinh, cccd;
            int viChinh, viPhu;
            bool admin;
            getline(ss, ten, ','); // Lấy tên
            getline(ss, mk, ','); // Lấy mật khẩu băm
            getline(ss, ngaysinh, ','); // Lấy ngày sinh
            getline(ss, cccd, ','); // Lấy CCCD
            ss >> viChinh; // Lấy số dư ví chính
            ss.ignore(1); // Bỏ qua dấu phẩy
            ss >> viPhu; // Lấy số dư ví phụ
            ss.ignore(1); // Bỏ qua dấu phẩy
            ss >> admin; // Lấy quyền admin (0/1)
            tk.emplace_back(ten, mk, ngaysinh, cccd, viChinh, viPhu, admin); // Thêm vào danh sách
        }
        inputFile.close();
        return tk;
    }

    // Ghi lịch sử giao dịch vào file transactions.txt
    // Input: Tên tài khoản, loại giao dịch, số điểm, ví nguồn, ví đích (string, int)
    // Output: Không có (ghi file)
    // Xử lý: Ghi một dòng giao dịch với thời gian hiện tại
    void GhiGiaoDich(const string& TenTk, const string& loaiGD, int soDiem, const string& viNguon, const string& viDich) {
        ofstream outputFile(FileGiaoDich, ios::app); // Mở file ở chế độ append
        if (outputFile.is_open()) {
            time_t now = time(0); // Lấy thời gian hiện tại
            string dt = ctime(&now);
            dt.pop_back(); // Xóa ký tự xuống dòng
            outputFile << TenTk << "," << loaiGD << "," << soDiem << "," << viNguon << "," << viDich << "," << dt << endl; // Ghi giao dịch
            outputFile.close();
        } else {
            cerr << "Không thể ghi lịch sử giao dịch." << endl; // Thông báo lỗi
        }
    }

public:
    // Constructor: Khởi tạo đối tượng với tên file tài khoản và giao dịch
    // Input: Tên file tài khoản, giao dịch (string)
    // Output: Không có
    QuanlyTaikhoan(const string& fileTk, const string& fileGD) 
        : FileQuanlyTk(fileTk), FileGiaoDich(fileGD), isAdmin(false) {}

    // Tạo tài khoản mới
    // Input: Tên tài khoản, mật khẩu, ngày sinh, CCCD (nhập từ người dùng)
    // Output: Thông báo thành công/thất bại
    // Xử lý: Kiểm tra tên tồn tại, chuẩn hóa ngày sinh, lưu tài khoản
    void TaoTaikhoan() {
        string TenTk, Matkhau, ngaysinh, socccd;
        int viChinh = 0, viPhu = 0; // Khởi tạo ví với số dư 0
        bool admin = false;
        cout << "Nhập tên tài khoản mới: ";
        cin >> TenTk;
        cout << "Nhập mật khẩu: ";
        cin >> Matkhau;
        cout << "Nhập ngày sinh (dd/mm/yyyy): ";
        cin >> ngaysinh;
        if (ngaysinh[1] == '/') ngaysinh = "0" + ngaysinh; // Chuẩn hóa: 1/2/2000 → 01/2/2000
        if (ngaysinh[4] == '/') ngaysinh.insert(3, "0"); // Chuẩn hóa: 01/2/2000 → 01/02/2000
        cout << "Nhập số căn cước công dân: ";
        cin >> socccd;
        if (KiemtraTenlogin(TenTk)) { // Kiểm tra tên đã tồn tại
            cout << "Tên tài khoản '" << TenTk << "' đã tồn tại." << endl;
            return;
        }
        if (TenTk == "admin") admin = true; // Gán quyền admin cho tài khoản "admin"
        ofstream outputFile(FileQuanlyTk, ios::app); // Mở file để thêm tài khoản
        if (outputFile.is_open()) {
            outputFile << TenTk << "," << hashFunction(Matkhau) << "," << ngaysinh << "," 
                       << socccd << "," << viChinh << "," << viPhu << "," << (admin ? "1" : "0") << endl; // Ghi thông tin
            outputFile.close();
            cout << "Tài khoản '" << TenTk << "' đã được tạo thành công!" << endl;
        } else {
            cerr << "Không thể mở tệp để ghi." << endl;
        }
    }

    // Đăng nhập với xác thực OTP
    // Input: Tên tài khoản, mật khẩu, OTP (nhập từ người dùng)
    // Output: true nếu thành công, false nếu thất bại
    // Xử lý: Kiểm tra thông tin, sinh OTP, xác thực
    bool login() {
        string TenTk, Matkhau, inputOTP;
        cout << "Nhập tên tài khoản: ";
        cin >> TenTk;
        cout << "Nhập mật khẩu: ";
        cin >> Matkhau;
        if (KiemtranThongtinTk(TenTk, Matkhau)) { // Kiểm tra thông tin đăng nhập
            string otp = generateOTP(); // Sinh OTP
            cout << "OTP của bạn là: " << otp << endl;
            cout << "Nhập OTP để xác thực: ";
            cin >> inputOTP;
            if (inputOTP == otp) { // Xác thực OTP
                cout << "Đăng nhập thành công với tài khoản '" << TenTk << "'!" << endl;
                currentUser = TenTk; // Cập nhật người dùng hiện tại
                isAdmin = (TenTk == "admin"); // Gán quyền admin
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

    // Xem số dư ví chính/phụ
    // Input: Tên tài khoản (nếu là admin)
    // Output: Số dư ví chính và ví phụ
    // Xử lý: Đọc file, tìm tài khoản, hiển thị số dư
    void XemsoduTk() {
        if (currentUser.empty()) { // Kiểm tra đăng nhập
            cout << "Vui lòng đăng nhập trước." << endl;
            return;
        }
        string TenTk = isAdmin ? "" : currentUser; // Nếu không phải admin, dùng currentUser
        if (isAdmin) {
            cout << "Nhập tên tài khoản muốn xem số dư: ";
            cin >> TenTk; // Admin nhập tên tài khoản cần xem
        }
        if (!KiemtraTenlogin(TenTk)) { // Kiểm tra tài khoản tồn tại
            cout << "Tên tài khoản không đúng." << endl;
            return;
        }
        vector<tuple<string, string, string, string, int, int, bool>> tk = TK(); // Đọc danh sách tài khoản
        for (const auto& tks : tk) {
            if (get<0>(tks) == TenTk) { // Tìm tài khoản
                cout << "Số dư tài khoản '" << TenTk << "':\n";
                cout << "Ví chính: " << get<4>(tks) << " điểm\n";
                cout << "Ví phụ: " << get<5>(tks) << " điểm\n";
                return;
            }
        }
    }

    // Thêm điểm thưởng (admin only)
    // Input: Tên tài khoản, loại ví (1: chính, 2: phụ), số điểm
    // Output: Thông báo thành công/thất bại
    // Xử lý: Cập nhật số dư, ghi giao dịch, lưu file
    void ThemDiemThuong() {
        if (currentUser.empty()) { // Kiểm tra đăng nhập
            cout << "Vui lòng đăng nhập trước." << endl;
            return;
        }
        if (!isAdmin) { // Kiểm tra quyền admin
            cout << "Chỉ admin mới có quyền thêm điểm thưởng." << endl;
            return;
        }
        string TenTk;
        cout << "Nhập tên tài khoản muốn thêm điểm thưởng: ";
        cin >> TenTk;
        if (!KiemtraTenlogin(TenTk)) { // Kiểm tra tài khoản tồn tại
            cout << "Không tìm thấy tài khoản '" << TenTk << "'." << endl;
            return;
        }
        int diemThem;
        string vi;
        cout << "Thêm vào ví chính (1) hay ví phụ (2)? Nhập 1 hoặc 2: ";
        cin >> vi;
        cout << "Nhập số điểm cần thêm: ";
        cin >> diemThem;
        if (diemThem <= 0) { // Kiểm tra số điểm hợp lệ
            cout << "Số điểm phải lớn hơn 0." << endl;
            return;
        }
        vector<tuple<string, string, string, string, int, int, bool>> tK = TK(); // Đọc danh sách tài khoản
        for (auto& tks : tK) {
            if (get<0>(tks) == TenTk) { // Tìm tài khoản
                if (vi == "1") { // Thêm vào ví chính
                    get<4>(tks) += diemThem;
                    GhiGiaoDich(TenTk, "Thêm điểm", diemThem, "Hệ thống", "Ví chính"); // Ghi giao dịch
                } else if (vi == "2") { // Thêm vào ví phụ
                    get<5>(tks) += diemThem;
                    GhiGiaoDich(TenTk, "Thêm điểm", diemThem, "Hệ thống", "Ví phụ"); // Ghi giao dịch
                } else {
                    cout << "Lựa chọn ví không hợp lệ." << endl;
                    return;
                }
                break;
            }
        }
        LuulaiTaikhoan(tK); // Lưu lại danh sách tài khoản
        cout << "Thêm " << diemThem << " điểm vào tài khoản '" << TenTk << "' thành công." << endl;
    }

    // Giao dịch giữa ví chính và ví phụ
    // Input: Loại giao dịch (1: chính → phụ, 2: phụ → chính), số điểm, OTP
    // Output: Thông báo thành công/thất bại
    // Xử lý: Kiểm tra số dư, cập nhật ví, xác thực OTP, lưu file
    void GiaoDich() {
        if (currentUser.empty()) { // Kiểm tra đăng nhập
            cout << "Vui lòng đăng nhập trước." << endl;
            return;
        }
        string TenTk = currentUser; // Tài khoản hiện tại
        if (!KiemtraTenlogin(TenTk)) { // Kiểm tra tài khoản tồn tại
            cout << "Tài khoản không tồn tại." << endl;
            return;
        }
        int soDiem;
        string loaiGD;
        cout << "Chuyển từ ví chính sang ví phụ (1) hay ví phụ sang ví chính (2)? Nhập 1 hoặc 2: ";
        cin >> loaiGD;
        cout << "Nhập số điểm cần chuyển: ";
        cin >> soDiem;
        if (soDiem <= 0) { // Kiểm tra số điểm hợp lệ
            cout << "Số điểm phải lớn hơn 0." << endl;
            return;
        }
        vector<tuple<string, string, string, string, int, int, bool>> tK = TK(); // Đọc danh sách tài khoản
        for (auto& tks : tK) {
            if (get<0>(tks) == TenTk) { // Tìm tài khoản
                if (loaiGD == "1") { // Ví chính → ví phụ
                    if (get<4>(tks) < soDiem) { // Kiểm tra số dư ví chính
                        cout << "Số điểm ví chính không đủ." << endl;
                        return;
                    }
                    get<4>(tks) -= soDiem; // Giảm ví chính
                    get<5>(tks) += soDiem; // Tăng ví phụ
                    GhiGiaoDich(TenTk, "Chuyển điểm", soDiem, "Ví chính", "Ví phụ"); // Ghi giao dịch
                } else if (loaiGD == "2") { // Ví phụ → ví chính
                    if (get<5>(tks) < soDiem) { // Kiểm tra số dư ví phụ
                        cout << "Số điểm ví phụ không đủ." << endl;
                        return;
                    }
                    get<5>(tks) -= soDiem; // Giảm ví phụ
                    get<4>(tks) += soDiem; // Tăng ví chính
                    GhiGiaoDich(TenTk, "Chuyển điểm", soDiem, "Ví phụ", "Ví chính"); // Ghi giao dịch
                } else {
                    cout << "Lựa chọn không hợp lệ." << endl;
                    return;
                }
                break;
            }
        }
        string otp = generateOTP(); // Sinh OTP
        cout << "OTP xác thực giao dịch: " << otp << endl;
        string inputOTP;
        cout << "Nhập OTP: ";
        cin >> inputOTP;
        if (inputOTP != otp) { // Xác thực OTP
            cout << "OTP không đúng. Giao dịch thất bại." << endl;
            return;
        }
        LuulaiTaikhoan(tK); // Lưu lại danh sách tài khoản
        cout << "Giao dịch thành công!" << endl;
    }

    // Xem lịch sử giao dịch (admin only)
    // Input: Không có
    // Output: Danh sách giao dịch (tài khoản, loại, số điểm, ví nguồn, ví đích, thời gian)
    // Xử lý: Đọc file transactions.txt, hiển thị từng dòng
    void XemLichSuGiaoDich() {
        if (!isAdmin) { // Kiểm tra quyền admin
            cout << "Chỉ admin mới có quyền xem lịch sử giao dịch." << endl;
            return;
        }
        ifstream inputFile(FileGiaoDich); // Mở file giao dịch
        string line;
        cout << "\n--- Lịch sử giao dịch ---\n";
        while (getline(inputFile, line)) { // Đọc từng dòng
            stringstream ss(line);
            string tenTk, loaiGD, viNguon, viDich, thoiGian;
            int soDiem;
            getline(ss, tenTk, ','); // Lấy tên tài khoản
            getline(ss, loaiGD, ','); // Lấy loại giao dịch
            ss >> soDiem; // Lấy số điểm
            ss.ignore(1); // Bỏ qua dấu phẩy
            getline(ss, viNguon, ','); // Lấy ví nguồn
            getline(ss, viDich, ','); // Lấy ví đích
            getline(ss, thoiGian, ','); // Lấy thời gian
            cout << "Tài khoản: " << tenTk << " | Loại: " << loaiGD 
                 << " | Số điểm: " << soDiem << " | Từ: " << viNguon 
                 << " | Đến: " << viDich << " | Thời gian: " << thoiGian << endl; // Hiển thị
        }
        inputFile.close();
    }

    // Đổi mật khẩu với xác thực OTP
    // Input: Mật khẩu cũ, mật khẩu mới, xác nhận mật khẩu mới, OTP
    // Output: Thông báo thành công/thất bại
    // Xử lý: Kiểm tra mật khẩu cũ, xác thực OTP, cập nhật mật khẩu mới
    void ThaydoiMatkhau() {
        if (currentUser.empty()) { // Kiểm tra đăng nhập
            cout << "Vui lòng đăng nhập trước." << endl;
            return;
        }
        string TenTk = currentUser; // Tài khoản hiện tại
        string MatkhauCu, MatkhauMoi, XacnhanMatkhauMoi, inputOTP;
        cout << "Nhập mật khẩu cũ: ";
        cin >> MatkhauCu;
        if (!KiemtranThongtinTk(TenTk, MatkhauCu)) { // Kiểm tra mật khẩu cũ
            cout << "Mật khẩu cũ không đúng." << endl;
            return;
        }
        string otp = generateOTP(); // Sinh OTP
        cout << "OTP xác thực: " << otp << endl;
        cout << "Nhập OTP: ";
        cin >> inputOTP;
        if (inputOTP != otp) { // Xác thực OTP
            cout << "OTP không đúng. Đổi mật khẩu thất bại." << endl;
            return;
        }
        cout << "Nhập mật khẩu mới: ";
        cin >> MatkhauMoi;
        cout << "Xác nhận mật khẩu mới: ";
        cin >> XacnhanMatkhauMoi;
        if (MatkhauMoi != XacnhanMatkhauMoi) { // Kiểm tra mật khẩu mới khớp
            cout << "Mật khẩu mới và xác nhận không khớp." << endl;
            return;
        }
        vector<tuple<string, string, string, string, int, int, bool>> tk = TK(); // Đọc danh sách tài khoản
        for (auto& tks : tk) {
            if (get<0>(tks) == TenTk) { // Tìm tài khoản
                get<1>(tks) = to_string(hashFunction(MatkhauMoi)); // Cập nhật mật khẩu băm
                break;
            }
        }
        LuulaiTaikhoan(tk); // Lưu lại danh sách tài khoản
        cout << "Đổi mật khẩu thành công!" << endl;
    }
};

// Hàm main: Giao diện dòng lệnh để chọn chức năng
// Input: Lựa chọn chức năng (0–7)
// Output: Kết quả của chức năng được chọn
// Xử lý: Hiển thị menu, gọi phương thức tương ứng
int main() {
    QuanlyTaikhoan x("accounts.txt", "transactions.txt"); // Khởi tạo đối tượng
    int choice;
    bool loggedIn = false; // Trạng thái đăng nhập
    do {
        cout << "\n--- Chức năng ---" << endl;
        cout << "1. Tạo tài khoản" << endl;
        cout << "2. Đăng nhập" << endl;
        if (loggedIn) { // Hiển thị thêm chức năng nếu đã đăng nhập
            cout << "3. Đổi mật khẩu" << endl;
            cout << "4. Xem số dư" << endl;
            cout << "5. Thêm điểm thưởng (admin)" << endl;
            cout << "6. Giao dịch giữa các ví" << endl;
            cout << "7. Xem lịch sử giao dịch (admin)" << endl;
            cout << "0. Thoát" << endl;
        } else {
            cout << "0. Thoát" << endl;
        }
        cout << "Chọn một chức năng: ";
        cin >> choice;
        switch (choice) {
            case 1:
                x.TaoTaikhoan(); // Gọi hàm tạo tài khoản
                break;
            case 2:
                loggedIn = x.login(); // Gọi hàm đăng nhập
                break;
            case 3:
                if (loggedIn) x.ThaydoiMatkhau(); // Gọi hàm đổi mật khẩu
                else cout << "Vui lòng đăng nhập trước." << endl;
                break;
            case 4:
                if (loggedIn) x.XemsoduTk(); // Gọi hàm xem số dư
                else cout << "Vui lòng đăng nhập trước." << endl;
                break;
            case 5:
                if (loggedIn) x.ThemDiemThuong(); // Gọi hàm thêm điểm thưởng
                else cout << "Vui lòng đăng nhập trước." << endl;
                break;
            case 6:
                if (loggedIn) x.GiaoDich(); // Gọi hàm giao dịch
                else cout << "Vui lòng đăng nhập trước." << endl;
                break;
            case 7:
                if (loggedIn) x.XemLichSuGiaoDich(); // Gọi hàm xem lịch sử giao dịch
                else cout << "Vui lòng đăng nhập trước." << endl;
                break;
            case 0:
                cout << "Đang thoát..." << endl; // Thoát chương trình
                break;
            default:
                cout << "Lựa chọn không hợp lệ." << endl; // Thông báo lựa chọn sai
        }
    } while (choice != 0); // Lặp lại đến khi chọn 0
    return 0;
}
