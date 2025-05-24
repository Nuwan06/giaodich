// Bao gồm các thư viện chuẩn của C++
#include <bits/stdc++.h>
using namespace std;

// Hàm hashFunction mật khẩu để mã hóa mật khẩu người dùng
// Input: Chuỗi mật khẩu (string input)
// Output: Giá trị băm kiểu size_t
// Thủ tục: Sử dụng std::hash để tạo giá trị băm từ chuỗi đầu vào
size_t hashFunction(const string& input) {
    hash<string> hashObj; // Tạo đối tượng hash
    return hashObj(input); // Trả về giá trị băm
}

// Hàm sinh OTP ngẫu nhiên để xác thực
// Input: Không có
// Output: Chuỗi OTP 4 chữ số (string)
// Thủ tục: Sinh số ngẫu nhiên từ 1000-9999 và chuyển thành chuỗi
string generateOTP() {
    srand(time(0)); // Khởi tạo seed dựa trên thời gian hiện tại
    int otp = rand() % 9000 + 1000; // Sinh số ngẫu nhiên từ 1000 đến 9999
    return to_string(otp); // Chuyển số thành chuỗi và trả về
}

class QuanlyTaikhoan {
private:
    string FileQuanlyTk; // File lưu thông tin tài khoản (accounts.txt)
    string FileGiaoDich; // File lưu lịch sử giao dịch (transactions.txt)
    string currentUser;  // Tên người dùng hiện tại (sau khi đăng nhập)
    bool isAdmin;        // Quyền admin: true nếu là admin, false nếu người dùng thường

    // Kiểm tra xem tên tài khoản đã tồn tại trong file chưa
    // Input: Tên tài khoản cần kiểm tra (string Tenlogin)
    // Output: true nếu tên đã tồn tại, false nếu chưa
    // Thủ tục: Đọc từng dòng trong file, so sánh tên tài khoản với Tenlogin
    bool KiemtraTenlogin(const string& Tenlogin) {
        ifstream inputFile(FileQuanlyTk); // Mở file accounts.txt để đọc
        string line;
        while (getline(inputFile, line)) { // Đọc từng dòng
            stringstream ss(line); // Tạo stream để phân tích dòng
            string TenTkinFile;
            getline(ss, TenTkinFile, ','); // Lấy tên tài khoản từ dòng (phân cách bởi dấu phẩy)
            if (TenTkinFile == Tenlogin) { // Nếu tên khớp
                inputFile.close(); // Đóng file
                return true; // Trả về true
            }
        }
        inputFile.close(); // Đóng file nếu không tìm thấy
        return false; // Trả về false
    }

    // Kiểm tra thông tin đăng nhập (tên và mật khẩu)
    // Input: Tên tài khoản (string TenTk), mật khẩu (string Matkhau)
    // Output: true nếu thông tin đúng, false nếu sai
    // Thủ tục: Đọc file, so sánh tên và mật khẩu băm với thông tin nhập vào
    bool KiemtranThongtinTk(const string& TenTk, const string& Matkhau) {
        ifstream inputFile(FileQuanlyTk); // Mở file accounts.txt để đọc
        string line;
        while (getline(inputFile, line)) { // Đọc từng dòng
            stringstream ss(line); // Tạo stream để phân tích
            string TenTkinFile, MatkhauinFile;
            getline(ss, TenTkinFile, ','); // Lấy tên tài khoản
            getline(ss, MatkhauinFile, ','); // Lấy mật khẩu băm từ file
            // So sánh tên và mật khẩu (băm mật khẩu nhập vào để so sánh)
            if (TenTkinFile == TenTk && MatkhauinFile == to_string(hashFunction(Matkhau))) {
                inputFile.close(); // Đóng file
                return true; // Trả về true nếu khớp
            }
        }
        inputFile.close(); // Đóng file nếu không tìm thấy
        return false; // Trả về false nếu không khớp
    }

public:
    // Constructor: Khởi tạo đối tượng QuanlyTaikhoan
    // Input: Tên file tài khoản (fileTk), tên file giao dịch (fileGD)
    // Output: Không có
    // Thủ tục: Gán tên file và khởi tạo trạng thái ban đầu
    QuanlyTaikhoan(const string& fileTk, const string& fileGD) 
        : FileQuanlyTk(fileTk), FileGiaoDich(fileGD), isAdmin(false) {}

    // Tạo tài khoản mới
    // Input: Người dùng nhập tên tài khoản, mật khẩu, ngày sinh, CCCD (qua cin)
    // Output: Tài khoản được lưu vào file accounts.txt, thông báo thành công/thất bại
    // Thủ tục: 
    // 1. Nhập thông tin từ người dùng
    // 2. Kiểm tra tên tài khoản có tồn tại chưa
    // 3. Định dạng ngày sinh (thêm số 0 nếu cần)
    // 4. Ghi thông tin vào file
    void TaoTaikhoan() {
        string TenTk, Matkhau, ngaysinh, socccd;
        int viChinh = 0, viPhu = 0; // Khởi tạo số dư ví chính/phụ bằng 0
        bool admin = false; // Quyền admin mặc định là false
        cout << "Nhập tên tài khoản mới: ";
        cin >> TenTk; // Nhập tên tài khoản
        cout << "Nhập mật khẩu: ";
        cin >> Matkhau; // Nhập mật khẩu
        cout << "Nhập ngày sinh (dd/mm/yyyy): ";
        cin >> ngaysinh; // Nhập ngày sinh
        // Định dạng ngày sinh: Thêm số 0 nếu ngày/tháng < 10
        if (ngaysinh[1] == '/') ngaysinh = "0" + ngaysinh;
        if (ngaysinh[4] == '/') ngaysinh.insert(3, "0");
        cout << "Nhập số căn cước công dân: ";
        cin >> socccd; // Nhập CCCD
        // Kiểm tra tên tài khoản đã tồn tại chưa
        if (KiemtraTenlogin(TenTk)) {
            cout << "Tên tài khoản '" << TenTk << "' đã tồn tại." << endl;
            return;
        }
        // Tài khoản tên "admin" sẽ có quyền admin
        if (TenTk == "admin") admin = true;
        // Mở file để ghi (chế độ append để thêm vào cuối file)
        ofstream outputFile(FileQuanlyTk, ios::app);
        if (outputFile.is_open()) {
            // Ghi thông tin tài khoản: tên, mật khẩu băm, ngày sinh, CCCD, ví chính, ví phụ, quyền admin
            outputFile << TenTk << "," << hashFunction(Matkhau) << "," << ngaysinh << "," 
                       << socccd << "," << viChinh << "," << viPhu << "," << (admin ? "1" : "0") << endl;
            outputFile.close(); // Đóng file
            cout << "Tài khoản '" << TenTk << "' đã được tạo thành công!" << endl;
        } else {
            cerr << "Không thể mở tệp để ghi." << endl; // Báo lỗi nếu không mở được file
        }
    }

    // Đăng nhập vào hệ thống với OTP
    // Input: Người dùng nhập tên tài khoản, mật khẩu, OTP (qua cin)
    // Output: true nếu đăng nhập thành công, false nếu thất bại
    // Thủ tục:
    // 1. Nhập tên và mật khẩu
    // 2. Kiểm tra thông tin đăng nhập
    // 3. Sinh OTP và yêu cầu người dùng nhập OTP
    // 4. Cập nhật trạng thái người dùng (currentUser, isAdmin) nếu thành công
    bool login() {
        string TenTk, Matkhau, inputOTP;
        cout << "Nhập tên tài khoản: ";
        cin >> TenTk; // Nhập tên tài khoản
        cout << "Nhập mật khẩu: ";
        cin >> Matkhau; // Nhập mật khẩu
        // Kiểm tra thông tin đăng nhập
        if (KiemtranThongtinTk(TenTk, Matkhau)) {
            string otp = generateOTP(); // Sinh OTP ngẫu nhiên
            cout << "OTP của bạn là: " << otp << endl; // Hiển thị OTP
            cout << "Nhập OTP để xác thực: ";
            cin >> inputOTP; // Nhập OTP từ người dùng
            // Kiểm tra OTP
            if (inputOTP == otp) {
                cout << "Đăng nhập thành công với tài khoản '" << TenTk << "'!" << endl;
                currentUser = TenTk; // Lưu tên người dùng hiện tại
                isAdmin = (TenTk == "admin"); // Kiểm tra quyền admin
                return true; // Trả về true
            } else {
                cout << "OTP không đúng. Đăng nhập thất bại." << endl;
                return false; // Trả về false nếu OTP sai
            }
        } else {
            cout << "Tên tài khoản hoặc mật khẩu không đúng." << endl;
            return false; // Trả về false nếu thông tin sai
        }
    }
};
