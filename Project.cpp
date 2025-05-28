#include <iostream>   // Thư viện cho các thao tác nhập/xuất
#include <string>     // Thư viện sử dụng dự liệu string
#include <vector>     // Thư viện sử dụng vector (mảng động)
#include <fstream>    // Thư viện thao tác với tệp đọc và ghi
#include <sstream>    // Thư viện thao tác với luồng chuỗi
#include <ctime>      // Thư viện cho các hàm liên quan đến thời gian
#include <algorithm>  // Thư viện cho find để xem lịch sử giao dịch
#include <iomanip>    // Thư viện cho định dạng đầu ra
#include <cctype>     // Thư viện cho các hàm xử lý ký tự 
#include <functional> // Thư viện cho hàm hash có trong cpp
#include <cstdlib>    // Thư viện tạo chuỗi kí tự ngẫu nhiên là rand() và srand()
using namespace std;

// Hàm băm mật khẩu
// Quy trình: Sử dụng std::hash để tạo ra giá trị băm từ chuỗi đầu vào.
size_t hashFunction(const string& input) {   // Đầu vào: chuỗi mật khẩu cần băm
    hash<string> hashObj;                    // Tạo một đối tượng băm cho chuỗi
    return hashObj(input);                   // Trả về giá trị băm
}

// Hàm sinh OTP ngẫu nhiên
string TaoOTP() {
    const string charset = "0123456789";
    string otp;
    for (int i = 0; i < 6; ++i) {                   // Lặp 6 lần để tạo OTP 6 chữ số
        otp += charset[rand() % charset.length()];  // Nối thêm một ký tự ngẫu nhiên từ bộ ký tự
    }
    return otp;   // Trả về OTP đã tạo
}

// Hàm sinh mật khẩu tự động
string TaoMatkhauTudong() {
    const string charset = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    string matkhau;
    for (int i = 0; i < 8; ++i) {     // Lặp 8 lần để tạo mật khẩu 8 ký tự
        matkhau += charset[rand() % charset.length()];  // Thêm ký tự ngẫu nhiên từ charset vào mật khẩu
    }
    return matkhau;   // Trả về mật khẩu đã tạo
}

// Hàm sinh mã ví ngẫu nhiên
string TaomaVi() {
    const string charset = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    string maVi;
    for (int i = 0; i < 8; ++i) {    // Lặp 8 lần để tạo mã ví 8 ký tự
        maVi += charset[rand() % charset.length()];  // Thêm ký tự ngẫu nhiên từ charset vào mã ví
    }
    return maVi;   // Trả về mã ví đã tạo
}

// Lớp để quản lý tài khoản, ví và giao dịch
class QuanlyTaikhoan {
private:
    string FileQuanlyTk;  // Đường dẫn tệp chứa dữ liệu tài khoản
    string Filevi;        // Đường dẫn tệp chứa dữ liệu ví
    string Filegiaodich;  // Đường dẫn tệp chứa dữ liệu giao dịch
    string Fileotp;       // Đường dẫn tệp chứa OTP tạm thời
    string currentUser;   // Lưu trữ tên người đung đang đăng nhập
    bool isAdmin;         // Xem người đang đăng nhập có phải admin

 // Kiểm tra xem tên người dùng đã tồn tại trong tệp tài khoản hay chưa
    bool KiemtraTenlogin(const string& Tenlogin) {  // Đầu vào: Tenlogin
        ifstream inputFile(FileQuanlyTk);   // Mở tệp tài khoản để đọc
        string line;
        while (getline(inputFile, line)) {  // Đọc từng dòng
            stringstream ss(line);          // Tạo một stringstream để phân tích dòng
            string TenTkinFile;             
            getline(ss, TenTkinFile, ',');  // Trích xuất tên người dùng cách nhau dấu phẩy
            if (TenTkinFile == Tenlogin) {  // So sánh với tên người dùng đầu vào
                inputFile.close();
                return true;                // Tên người dùng đã được tìm thấy
            }
        }
        inputFile.close();
        return false;                       // Tên người dùng không được tìm thấy
    }

// Xác thực tên người dùng và mật khẩu đã băm
    bool KiemtranThongtinTk(const string& TenTk, const string& Matkhau) {  // Đầu vào: tên người dùng, mật khẩu
        ifstream inputFile(FileQuanlyTk);    // Mở tệp tài khoản để đọc
        string line;
        while (getline(inputFile, line)) {   // Đọc từng dòng
            stringstream ss(line);           // Tạo một stringstream để phân tích dòng
            string TenTkinFile, MatkhauinFile;  
            getline(ss, TenTkinFile, ',');   // Trích xuất tên người dùng
            getline(ss, MatkhauinFile, ','); // Trích xuất mật khẩu dẫ băm
            // So sánh tên người dùng và mật khẩu đã băm
            if (TenTkinFile == TenTk && MatkhauinFile == to_string(hashFunction(Matkhau))) {
                inputFile.close();
                return true;  // Thông tin đăng nhập khớp
            }
        }
        inputFile.close();   // Thông tin đăng nhập không khớp
        return false;
    }

// Lưu một vector các tuple tài khoản vào tệp tài khoản
// Đầu vào: Taikhoan (vector các tuple chứa dữ liệu tài khoản)
    void LuulaiTaikhoan(const vector<tuple<string, string, string, string, bool>>& Taikhoan) {
        ofstream outputFile(FileQuanlyTk);      // Mở tệp tài khoản để ghi
        if (outputFile.is_open()) {            // Kiểm tra xem tệp đã được mở thành công chưa
            for (const auto& TK : Taikhoan) {  // Lặp qua từng tuple tài khoản
                // Ghi dữ liệu tài khoản vào tệp, phân tách bằng dấu phẩy
                outputFile << get<0>(TK) << "," << get<1>(TK) << "," << get<2>(TK) << "," 
                         << get<3>(TK) << "," << (get<4>(TK) ? "1" : "0") << endl;
            }
            outputFile.close();
        } else {
            cerr << "Không thể mở tệp để ghi tài khoản." << endl;  // Thông báo lỗi nếu không thể mở tệp
        }
    }

 // Đọc tất cả dữ liệu tài khoản từ tệp tài khoản
    vector<tuple<string, string, string, string, bool>> TK() {
        vector<tuple<string, string, string, string, bool>> tk;  // Vector để lưu trữ dữ liệu tài khoản
        ifstream inputFile(FileQuanlyTk);     // Mở tệp tài khoản để đọc
        string line;
        while (getline(inputFile, line)) {    // Đọc từng dòng
            stringstream ss(line);            // Tạo một stringstream để phân tích dòng
            string ten, mk, email, cccd;      // Các biến để lưu trữ các trường tài khoản
            string adminStr;                  // Chuỗi để lưu trữ trạng thái admin đúng hoặc không ("1" hoặc "0")
            // Trích xuất các trường được phân tách bằng dấu phẩy
            getline(ss, ten, ',');
            getline(ss, mk, ',');
            getline(ss, email, ',');
            getline(ss, cccd, ',');
            getline(ss, adminStr);
            bool admin = (adminStr == "1");    // Chuyển đổi "1" thành true, "0" thành false
            tk.emplace_back(ten, mk, email, cccd, admin);  // Thêm tuple tài khoản vào vector
        }
        inputFile.close();
        return tk;   // Trả về vector dữ liệu tài khoản
    }

// Đọc tất cả dữ liệu ví từ tệp ví
    vector<tuple<string, string, string, int>> DanhsachVi() {
        vector<tuple<string, string, string, int>> Vi;   // Vector để lưu trữ dữ liệu ví
        ifstream inputFile(Filevi);           // Mở tệp ví để đọc
        string line;
        while (getline(inputFile, line)) {
            stringstream ss(line);            // Tạo luồng chuỗi để phân tích dòng
            string ma, tk, loai;              // Các biến để lưu trữ các trường ví
            int sodu;            
            getline(ss, ma, ',');          // Đọc thông tin ví
            getline(ss, tk, ',');
            getline(ss, loai, ',');
            ss >> sodu;
            Vi.emplace_back(ma, tk, loai, sodu);  // Thêm tuple ví vào vector
        }
        inputFile.close();
        return Vi;  // Trả về vector dữ liệu ví
    }

// Đọc tất cả dữ liệu giao dịch từ tệp lịch sử giao dịch
    vector<tuple<string, string, string, string, string, int>> LichsuGiaodich() {
        vector<tuple<string, string, string, string, string, int>> Giaodich; // Vector để lưu trữ dữ liệu giao dịch
        ifstream inputFile(Filegiaodich);   // Mở tệp lịch sử giao dịch để đọc
        string line;
        while (getline(inputFile, line)) {
            stringstream ss(line);
            string ma, nguon, dich, thoigian, trangthai;  // Các biến để lưu trữ các trường giao dịch
            int diem;
            getline(ss, ma, ',');        // Đọc thông tin lịch sử giao dịch
            getline(ss, nguon, ',');
            getline(ss, dich, ',');
            getline(ss, thoigian, ',');
            getline(ss, trangthai, ',');
            ss >> diem;
            Giaodich.emplace_back(ma, nguon, dich, thoigian, trangthai, diem);  // Thêm tuple giao dịch vào vector
        }
        inputFile.close();
        return Giaodich;   // Trả về vector dữ liệu giao dịch
    }

// Lưu một vector các tuple ví vào tệp ví
// Đầu vào: Vi (vector các tuple chứa dữ liệu ví)
    void luuDuLieuVi(const vector<tuple<string, string, string, int>>& Vi) {
        ofstream outputFile(Filevi);      // Mở tệp ví để ghi (ghi đè nội dung hiện có)
        if (outputFile.is_open()) {
            for (const auto& vi : Vi) {   // Lặp qua từng tuple ví
                outputFile << get<0>(vi) << "," << get<1>(vi) << ","     // Ghi dữ liệu ví vào tệp, phân tách bằng dấu phẩy
                         << get<2>(vi) << "," << get<3>(vi) << endl;
            }
            outputFile.close();
        } else {
            cerr << "Không thể mở tệp để ghi ví." << endl;   // Thông báo lỗi
        }
    }

 // Lưu một vector các tuple giao dịch vào tệp lịch sử giao dịch 
 // Đầu vào: Lichsu (vector các tuple chứa dữ liệu giao dịch)   
    void luuDuLieuGiaodich(const vector<tuple<string, string, string, string, string, int>>& Lichsu) {
        ofstream outputFile(Filegiaodich);    // Mở tệp lịch sử giao dịch để ghi  
        if (outputFile.is_open()) {
            for (const auto& gd : Lichsu) {   // Lặp qua từng tuple giao dịch
                outputFile << get<0>(gd) << "," << get<1>(gd) << ","     // Ghi dữ liệu giao dịch vào tệp, phân tách bằng dấu phẩy
                         << get<2>(gd) << "," << get<3>(gd) << "," 
                         << get<4>(gd) << "," << get<5>(gd) << endl;
            }
            outputFile.close();
        } else {
            cerr << "Không thể mở tệp để ghi giao dịch." << endl;  // Thông báo lỗi
        }
    }

public:
// Hàm constructor khởi tạo
// Đầu vào: fileTk, vidiem, giaodich, otp (đường dẫn đến các tệp dữ liệu)
// Quy trình: Thiết lập các đường dẫn tệp, khởi tạo trạng thái người dùng hiện tại và kiểm tra/tạo ví tổng hệ thống.
    QuanlyTaikhoan(const string& fileTk, const string& vidiem, const string& giaodich, const string& otp) {
        FileQuanlyTk = fileTk;
        Filevi = vidiem;
        Filegiaodich = giaodich;
        Fileotp = otp;
        currentUser = "";   // Không có người dùng nào đăng nhập ban đầu
        isAdmin = false;    // Không phải admin ban đầu
        // Kiểm tra và tạo ví tổng nếu chưa có
        auto danhsachvi = DanhsachVi();       // Lấy danh sách ví hiện có
        bool hasTongVi = false;
        for (const auto& vi : danhsachvi) {   // Duyệt qua các ví
            if (get<2>(vi) == "tong") {       // Nếu tìm thấy ví loại "tong trả về true
                hasTongVi = true;
                break;
            }
        }
        if (!hasTongVi) {    // Nếu chưa có ví tổng
            // Thêm ví tổng vào danh sách với mã "TONG0001", thuộc sở hữu "system", loại "tong" và 100,000,000 điểm
            danhsachvi.emplace_back("TONG0001", "admin", "tong", 100000000);
            luuDuLieuVi(danhsachvi);  // Lưu lại danh sách ví đã cập nhật vào file
        }
         auto tk = TK();                   // Lấy danh sách tài khoản hiện có
        bool hasAdmin = false; 
        for (const auto& Tk : tk) {        // Duyệt qua danh sách tài khoản
            if (get<0>(Tk) == "admin") {   // Nếu tìm thấy tài khoản admin trả về true
                hasAdmin = true;
                break;                      // thoát khỏi hàm
            }
        }
        if (!hasAdmin) {                    // Mấu chưa có tài khoản admin
            // Thêm tàu khoản admin với tài khoản "admin" mật khẩu abcd1234 
            string mkadmin = "abcd1234";
            tk.emplace_back("admin", to_string(hashFunction(mkadmin)), "nhuantranthanh@gmail.com", "01234567" , "1");
            LuulaiTaikhoan(tk);            // Lưu lại tài khoản admin
        }
    }

// Các Phương Thức Public (Chức năng Chính)

// Lấy tên người dùng hiện tại
    string getCurrentUser() const { return currentUser; }   // Đầu ra: string (tên người dùng hiện tại)
// Kiểm tra xem người dùng hiện tại có phải là admin không    
    bool getIsAdmin() const { return isAdmin; }            // Đầu ra: bool (true nếu là admin, false nếu ngược lại)

 // Tạo tài khoản mới (dùng cho cả đăng ký thông thường và admin tạo)
 // Quy trình: Yêu cầu tên người dùng, kiểm tra trùng lặp, cho phép nhập hoặc tự động tạo mật khẩu,
    //            lấy email, số CCCD, và tạo ví chính mặc định cho tài khoản mới.
    void TaoTaikhoan() {
        string TenTk, Matkhau, email, socccd;
        char choice;
        cout << "Nhập tên tài khoản mới: ";
        cin >> TenTk;
        if (KiemtraTenlogin(TenTk)) {   // Kiểm tra xem tên tài khoản đã tồn tại chưa
            cout << "Tên tài khoản '" << TenTk << "' đã tồn tại." << endl;
            return;    // Thoát nếu tên tài khoản đã tồn tại
        }
        cout << "Bạn có muốn nhập mật khẩu? (y/n): ";
        cin >> choice;
        if (tolower(choice) == 'y') {
            cout << "Nhập mật khẩu: ";
            cin >> Matkhau;
        } else {     // Nếu không, tự động tạo mật khẩu
            Matkhau = TaoMatkhauTudong();
            cout << "Mật khẩu tự động được tạo: " << Matkhau << endl;
        ofstream outputFile("matkhautudong.txt", ios::app);
        outputFile << TenTk << "," << Matkhau << endl;
        outputFile.close();
        }
        cout << "Nhập email: ";
        cin >> email;
        cout << "Nhập số căn cước công dân: ";
        cin >> socccd;
        bool admin = (TenTk == "admin");   // Xác định tài khoản có phải là admin hay không (nếu tên tài khoản là "admin")
        ofstream outputFile(FileQuanlyTk, ios::app);   // Mở tệp tài khoản để ghi (chế độ nối thêm vào cuối)
        if (outputFile.is_open()) {
            // Ghi thông tin tài khoản vào tệp: TênTk, mật khẩu băm, email, CCCD, trạng thái admin (1/0)
            outputFile << TenTk << "," << hashFunction(Matkhau) << "," << email << "," 
                     << socccd << "," << (admin ? "1" : "0") << endl;
            outputFile.close();
            cout << "Tài khoản '" << TenTk << "' đã được tạo thành công!" << endl;
            if (tolower(choice) != 'y') {
                cout << "Lưu ý: Mật khẩu tự động này sẽ được gửi về email của bạn và không hiển thị lại. Hãy ghi nhớ hoặc đổi mật khẩu sau khi đăng nhập." << endl;
            }
            // Tạo ví chính mặc định cho tài khoản mới (trừ tài khoản admin)
            if (!admin) {   // Nếu không là admin
                auto danhsachvi = DanhsachVi();  // Lấy danh sách ví hiện có
                string maVi = TaomaVi();         // Tạo mã ví mới
                danhsachvi.emplace_back(maVi, TenTk, "chinh", 100);  // Thêm ví chính với 100 điểm
                luuDuLieuVi(danhsachvi);         // Lưu lại danh sách ví
                cout << "Đã tạo ví chính mặc định (" << maVi << ") với 100 điểm." << endl;
            }
        } else {
            cerr << "Không thể mở tệp để ghi." << endl;   // Thông báo lỗi
        }
    }

// Đăng nhập vào hệ thống
// Quy trình: Xác thực tên người dùng và mật khẩu, gửi OTP, yêu cầu nhập OTP để xác thực cuối cùng.
    bool login() {
        string TenTk, Matkhau, inputOTP;
        cout << "Nhập tên tài khoản: ";
        cin >> TenTk;
        if (!KiemtraTenlogin(TenTk)) {   // Kiểm tra tên người dùng có tồn tại không
            cout << "Tên tài khoản không đúng" << endl;
            return false;
        }
        cout << "Nhập mật khẩu: ";
        cin >> Matkhau;
        if (KiemtranThongtinTk(TenTk, Matkhau)) {  // Kiểm tra mật khẩu
            string otp = TaoOTP();      // Tạo OTP
            ofstream otpFile(Fileotp);  // Mở tệp OTP để ghi
            otpFile << TenTk << "," << otp << endl;  // Lưu OTP cùng tên người dùng vào tệp
            otpFile.close();
            auto tk = TK();             // Lấy danh sách tài khoản
            for (const auto& t : tk) {  // Tìm email của người dùng để thông báo OTP đã gửi
                if (get<0>(t) == TenTk) {
                    cout << "Đã gửi mã OTP "  << " đến email " << get<2>(t) << endl;  // Thông báo (giả lập)
                    break;
                }
            }
            cout << "Nhập OTP để xác thực: ";
            cin >> inputOTP;
            if (inputOTP == otp) {    // So sánh OTP nhập với OTP đã tạo
                currentUser = TenTk;  // Đặt người dùng hiện tại
                isAdmin = (TenTk == "admin");  // Đặt trạng thái admin
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

// Thay đổi mật khẩu cho tài khoản hiện tại
// Quy trình: Yêu cầu mật khẩu cũ, xác thực, gửi OTP, yêu cầu nhập OTP,
    //            xác nhận mật khẩu mới và cập nhật mật khẩu đã băm.
    void ThaydoiMatkhau() {
        if (currentUser.empty()) {   // Yêu cầu đăng nhập trước
            cout << "Vui lòng đăng nhập trước." << endl;
            return;
        }
        string MatkhauCu, MatkhauMoi, XacnhanMatkhauMoi, inputOTP;
        cout << "Nhập mật khẩu cũ của tài khoản " << currentUser <<" :";
        cin >> MatkhauCu;
        if (!KiemtranThongtinTk(currentUser, MatkhauCu)) {   // Kiểm tra mật khẩu cũ
            cout << "Mật khẩu cũ không đúng." << endl;
            return;
        }
        string otp = TaoOTP();  //Tạo otp
        ofstream otpFile(Fileotp);
        otpFile << currentUser << "," << otp << endl;
        otpFile.close();
        auto tk = TK();
        for (const auto& t : tk) {
            if (get<0>(t) == currentUser) {
                cout << "Đã gửi mã OTP " << " đến email " << get<2>(t) << endl;  // Thông báo (giả lập)
                break;
            }
        }
        cout << "Nhập OTP: ";
        cin >> inputOTP;  
        if (inputOTP != otp) {  // kiểm tra otp
            cout << "OTP không đúng. Đổi mật khẩu thất bại." << endl;
            return;
        }
        cout << "Nhập mật khẩu mới: ";
        cin >> MatkhauMoi;
        cout << "Xác nhận mật khẩu mới: ";
        cin >> XacnhanMatkhauMoi;
        if (MatkhauMoi != XacnhanMatkhauMoi) {   // Kiểm tra mật khẩu mới và xác nhận
            cout << "Mật khẩu mới và xác nhận không khớp." << endl;
            return;
        }
        auto tkList = TK();       // Lấy danh sách tài khoản
        for (auto& t : tkList) {  // Tìm tài khoản hiện tại để cập nhật mật khẩu
            if (get<0>(t) == currentUser) {   
                get<1>(t) = to_string(hashFunction(MatkhauMoi));  // Cập nhật mật khẩu đã băm
                break;
            }
        }
        LuulaiTaikhoan(tkList);  // Lưu lại danh sách tài khoản đã cập nhật
        cout << "Đổi mật khẩu thành công!" << endl;
    }

// Xem danh sách tài khoản (chỉ dành cho admin)
// Quy trình: Kiểm tra quyền admin, sau đó hiển thị tất cả tài khoản với tên, email và loại tài khoản.
    void XemDanhSachNhom() {
        if (!isAdmin) {  // Kiểm tra quyền admin
            cout << "Bạn không có quyền truy cập chức năng này!" << endl;
            return;
        }

        auto danhsach = TK();  // Lấy danh sách tài khoản
        cout << "\nDANH SÁCH TÀI KHOẢN HỆ THỐNG" << endl;
        cout << "--------------------------------------------------" << endl;
        cout << setw(20) << left << "Tên tài khoản"   // Định dạng đầu ra
             << setw(25) << "Email" 
             << setw(15) << "Loại tài khoản" << endl;
        
        for (const auto& tk : danhsach) {  // Duyệt và in thông tin từng tài khoản
            cout << setw(20) << left << get<0>(tk)
                 << setw(25) << get<2>(tk)
                 << setw(15) << (get<4>(tk) ? "Admin" : "User") << endl;  // Hiển thị "Admin" hoặc "User"
        }
    }

// Admin tạo tài khoản mới (chỉ dành cho admin)
// Quy trình: Kiểm tra quyền admin, sau đó gọi hàm TaoTaikhoan().
    void AdminTaoTaiKhoan() {
        if (!isAdmin) {
            cout << "Bạn không có quyền truy cập chức năng này!" << endl;
            return;
        }
        TaoTaikhoan();
    }

// Điều chỉnh thông tin tài khoản (chỉ dành cho admin)
// Quy trình: Yêu cầu tên tài khoản, tìm kiếm, hiển thị thông tin hiện tại, cho phép nhập email/CCCD mới,
 //            gửi OTP đến chủ tài khoản và yêu cầu xác nhận bằng OTP đó.
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
         // Con trỏ tới tuple tài khoản cần sửa
        tuple<string, string, string, string, bool>* taiKhoanCanSua = nullptr;
        for (auto& tk : danhsach) {    // Tìm tài khoản theo tên
            if (get<0>(tk) == tenTaiKhoan) {
                found = true;
                taiKhoanCanSua = &tk;   // Gán con trỏ tới tài khoản tìm thấy
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
        if (!emailMoi.empty() || !cccdMoi.empty()) {   // Nếu có bất kỳ thay đổi nào
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
                if (!emailMoi.empty()) get<2>(*taiKhoanCanSua) = emailMoi;  // Cập nhật email
                if (!cccdMoi.empty()) get<3>(*taiKhoanCanSua) = cccdMoi;    // Cập nhật CCCD
                
                LuulaiTaikhoan(danhsach);    // Lưu lại danh sách tài khoản đã cập nhật
                cout << "Cập nhật thông tin thành công!" << endl;
            } else {
                cout << "OTP không hợp lệ. Thao tác bị hủy bỏ." << endl;
            }
        } else {
            cout << "Không có thay đổi nào được thực hiện." << endl;
        }
    }

// Chuyển điểm giữa các ví
// Quy trình: Hiển thị ví của người dùng, yêu cầu thông tin giao dịch, kiểm tra tính hợp lệ của ví và số dư,
    //            gửi OTP để xác nhận giao dịch, cập nhật số dư ví và lưu lại lịch sử giao dịch.
    void chuyenDiem() {
        if (currentUser.empty()) {   // Yêu cầu đăng nhập trước
            cout << "Vui lòng đăng nhập trước." << endl;
            return;
        }
        auto danhsachvi = DanhsachVi();      // Lấy danh sách ví
        cout << "\nDanh sách ví của bạn:" << endl;
        for (const auto& vi : danhsachvi) {  // Hiển thị các ví thuộc về người dùng hiện tại
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
        if (cin.fail()) {
            cerr << "Số điểm quá lớn";
            cin.clear();
            return;
        }
        if (soDiem <= 0 ) {  // Kiểm tra số điểm hợp lệ
            cout << "Số điểm không hợp lệ" << endl;
            return;
        }
        bool validNguon = false;   // kiểm tra ví nguồn hợp lệ
        bool validDich = false;    // kiểm tra ví đích hợp lệ
        int viNguonIndex = 0;      // Chỉ số của ví nguồn trong vector
        int viDichIndex = 0;       // Chỉ số của ví đích trong vector
        for (size_t i = 0; i < danhsachvi.size(); ++i) {  // Duyệt qua danh sách ví để tìm ví nguồn và đích
            // Kiểm tra ví nguồn phải thuộc về người dùng hiện tại
            if (get<0>(danhsachvi[i]) == maViNguon && get<1>(danhsachvi[i]) == currentUser) {
                validNguon = true;
                viNguonIndex = i;
            }
            if (get<0>(danhsachvi[i]) == maViDich) {   // Kiểm tra ví đích tồn tại
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
        if (get<3>(danhsachvi[viNguonIndex]) < soDiem) {   // Kiểm tra số dư ví nguồn
            cout << "Số dư không đủ để thực hiện giao dịch." << endl;
            return;
        }
        string otp = TaoOTP();  // Tạo OTP cho giao dịch (giả lập)
        ofstream otpFile(Fileotp);
        otpFile << currentUser << "," << otp << endl;
        otpFile.close();
        auto tk = TK();
        for (const auto& t : tk) {
            if (get<0>(t) == currentUser) {   // Tìm email của người dùng để thông báo gửi OTP
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
        // Thực hiện chuyển điểm
        get<3>(danhsachvi[viNguonIndex]) -= soDiem;  // Trừ điểm từ ví nguồn
        get<3>(danhsachvi[viDichIndex]) += soDiem;   // Cộng điểm vào ví đích
        luuDuLieuVi(danhsachvi);    // Lưu lại dữ liệu ví đã cập nhật
        time_t now = time(0);       // Lấy thời gian hiện tại
        string tg = ctime(&now);    // Chuyển đổi thời gian sang chuỗi
        tg.pop_back();              // Xóa ký tự newline cuối chuỗi thời gian
        string maGD = "GD" + to_string(LichsuGiaodich().size() + 1);   // Tạo mã giao dịch mới
        auto lsgd = LichsuGiaodich();   // Lấy lịch sử giao dịch hiện có
        lsgd.emplace_back(maGD, maViNguon, maViDich, tg, "thanh_cong", soDiem);   // Thêm giao dịch mới vào lịch sử
        luuDuLieuGiaodich(lsgd);       // Lưu lại lịch sử giao dịch
        cout << "Chuyển điểm thành công!" << endl;
        cout << "Mã giao dịch: " << maGD << endl;
    }

// Xem lịch sử giao dịch của người dùng hiện tại
// Quy trình: Hiển thị tất cả các giao dịch liên quan đến ví của người dùng hiện tại.
    void xemLichSuGiaoDich() {
        if (currentUser.empty()) {   // Yêu cầu đăng nhập trước
            cout << "Vui lòng đăng nhập trước." << endl;
            return;
        }
        cout << "\nLịch sử giao dịch của bạn:" << endl;
        auto lsgd = LichsuGiaodich();    // Lấy tất cả lịch sử giao dịch
        auto danhsachvi = DanhsachVi();  // Lấy danh sách ví
        vector<string> userVi;           // Vector để lưu trữ mã ví của người dùng hiện tại
        for (const auto& vi : danhsachvi) {   // Thu thập tất cả mã ví của người dùng hiện tại
            if (get<1>(vi) == currentUser) {
                userVi.push_back(get<0>(vi));
            }
        } 
        bool found = false;   // kiểm tra xem có giao dịch nào được tìm thấy không
        for (const auto& gd : lsgd) {   // Duyệt qua lịch sử giao dịch
            // Nếu ví nguồn hoặc ví đích của giao dịch thuộc về người dùng hiện tại
            if (find(userVi.begin(), userVi.end(), get<1>(gd)) != userVi.end() || //kiểm tra ví nguồn(get<1>(gd)) có nằm trong userVi không.
                find(userVi.begin(), userVi.end(), get<2>(gd)) != userVi.end()) {
                cout << "Mã GD: " << get<0>(gd) 
                     << " | Từ: " << get<1>(gd) 
                     << " | Đến: " << get<2>(gd)
                     << " | Thời gian " << get<3>(gd)
                     << " | Trạng thái: " << get<4>(gd) << " | Số điểm: " << get<5>(gd) << endl;
                found = true;
            }
        }
        if (!found) {
            cout << "Không có giao dịch nào." << endl;
        }
    }

// Tạo ví mới cho người dùng hiện tại
// Quy trình: Cho phép người dùng chọn tạo ví chính (100 điểm) hoặc ví phụ (0 điểm),
    //            tạo mã ví và lưu ví mới.
    void taoViMoi() {
        if (currentUser.empty()) {
            cout << "Vui lòng đăng nhập trước." << endl;  // Yêu cầu đăng nhập trước
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
        string loai = (choice == 1) ? "chinh" : "phu";   // Đặt loại ví
        int soDu = (choice == 1) ? 100 : 0;              // Đặt số dư ban đầu
        auto danhsachvi = DanhsachVi();   // Lấy danh sách ví hiện có
        string maVi = TaomaVi();          // Tạo mã ví mới
        danhsachvi.emplace_back(maVi, currentUser, loai, soDu);   // Thêm ví mới vào danh sách
        luuDuLieuVi(danhsachvi);     // Lưu lại danh sách ví
        cout << "Đã tạo ví " << loai << " mới với mã: " << maVi << endl;
        cout << "Số dư ban đầu: " << soDu << " điểm" << endl;
    }

// Xem số dư của tài khoản hoặc một tài khoản cụ thể (nếu là admin)
// Quy trình: Hiển thị tất cả các ví và số dư của người dùng hiện tại.
    //            Nếu là admin, có thể xem ví của người dùng khác.
    void XemsoduTk() {
        if (currentUser.empty()) {    // Yêu cầu đăng nhập trước
            cout << "Vui lòng đăng nhập trước." << endl;
            return;
        }
        string TenTk = currentUser;   // Mặc định là người dùng hiện tại
        if (isAdmin) {   // Nếu là admin, có thể chọn xem của tài khoản khác
            cout << "Nhập tên tài khoản muốn xem (bỏ trống để xem của bạn): ";
            cin.ignore();
            getline(cin, TenTk);
            if (TenTk.empty()) {   // Nếu bỏ trống, vẫn là người dùng hiện tại
                TenTk = currentUser;
            }
        }
        auto danhsachvi = DanhsachVi();   // Lấy danh sách ví
        bool found = false;               // kiểm tra xem có ví nào được tìm thấy không
        cout << "\nDanh sách ví của " << TenTk << ":" << endl;
        for (const auto& vi : danhsachvi) {   // Duyệt qua danh sách ví
            if (get<1>(vi) == TenTk) {        // Nếu ví thuộc về tài khoản được chỉ định
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

// Hàm quên mật khẩu
// Quy trình: đọc tên tài khoản, nhập đúng otp để đổi mật khẩu
void quenmatkhau() {
    string TenTk, Matkhaumoi;           
        cout << "Nhập tên tài khoản: ";
        cin >> TenTk;
        if (!KiemtraTenlogin(TenTk)) {   // Kiểm tra tên người dùng có tồn tại không
            cout << "Tên tài khoản không đúng" << endl;
            return;
        }
        string otp = TaoOTP();  // Tạo OTP cho giao dịch (giả lập)
        ofstream otpFile(Fileotp);
        otpFile << TenTk << "," << otp << endl;
        otpFile.close();
        auto tk = TK();
        for (const auto& t : tk) {
            if (get<0>(t) == TenTk) {   // Tìm email của người dùng để thông báo gửi OTP
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
            cout << "Nhập mật khẩu mới: ";
            cin >> Matkhaumoi;
            auto tkList = TK();       // Lấy danh sách tài khoản
        for (auto& t : tkList) {  // Tìm tài khoản hiện tại để cập nhật mật khẩu
            if (get<0>(t) == TenTk) {   
                get<1>(t) = to_string(hashFunction(Matkhaumoi));  // Cập nhật mật khẩu đã băm
                break;
            }
        }
        LuulaiTaikhoan(tkList);  // Lưu lại danh sách tài khoản đã cập nhật
        cout << "Đổi mật khẩu thành công!" << endl;
        }
};

int main() {
    srand(time(0));  // Khởi tạo bộ sinh số ngẫu nhiên với thời gian hiện tại
    // Tạo đối tượng QuanlyTaikhoan với các tệp dữ liệu
    QuanlyTaikhoan ql("accounts.txt", "filevidiem.txt", "filegiaodich.txt", "fileotp.txt");
    int choice;    // Biến lưu lựa chọn của người dùng
    bool loggedIn = false;  // Kiểm tra trạng thái đăng nhập
    do {
        cout << "\n--- HỆ THỐNG QUẢN LÝ VÍ ĐIỂM ---" << endl;
        if (!loggedIn) {    // Menu khi chưa đăng nhập
            cout << "1. Đăng nhập" << endl;
            cout << "2. Đăng ký tài khoản" << endl;
            cout << "3. Quên mật khẩu" << endl;
            cout << "0. Thoát" << endl;
        } else {    // Menu khi đã đăng nhập
            if (ql.getIsAdmin()) {   // Menu cho admin
                cout << "1. Quản lý tài khoản(Chỉ dành cho admin)" << endl;
                cout << "2. Tạo ví mới" << endl;
                cout << "3. Chuyển điểm" << endl;
                cout << "4. Xem lịch sử giao dịch" << endl;
                cout << "5. Xem số dư" << endl;
                cout << "6. Đổi mật khẩu" << endl;
                cout << "0. Đăng xuất" << endl;
            } else {  // Menu cho người dùng thông thường
                cout << "1. Tạo ví mới" << endl;
                cout << "2. Chuyển điểm" << endl;
                cout << "3. Xem lịch sử giao dịch" << endl;
                cout << "4. Xem số dư" << endl;
                cout << "5. Đổi mật khẩu" << endl;
                cout << "0. Đăng xuất" << endl;
            }
        }
        cout << "Chọn chức năng: ";
        cin >> choice;    // Đọc lựa chọn của người dùng
        if (!loggedIn) {  // Xử lý lựa chọn khi chưa đăng nhập
            switch (choice) {
                case 1: loggedIn = ql.login();
                break;
                case 2: ql.TaoTaikhoan();
                break;
                case 3: ql.quenmatkhau();
                break;
                case 0: cout << "Đang thoát..." << endl;
                return 0;
                default: cout << "Lựa chọn không hợp lệ." << endl;
            }
        } else {   // Xử lý lựa chọn khi đã đăng nhập
            if (ql.getIsAdmin()) {  // Xử lý lựa chọn cho admin
                switch (choice) {    
                    case 1: {      // Quản lý tài khoản (menu con)
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
            } else {   // Xử lý lựa chọn cho người dùng thông thường
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
    } while (true);   // Lặp lại menu cho đến khi người dùng chọn thoát
    return 0;
}
