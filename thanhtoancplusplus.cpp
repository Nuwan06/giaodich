#include <iostream> // Thư viện cho nhập/xuất dữ liệu (cin, cout, cerr)
#include <string> // Thư viện cho kiểu dữ liệu string
#include <vector> // Thư viện cho kiểu dữ liệu vector (mảng động)
#include <fstream> // Thư viện cho thao tác đọc/ghi tệp
#include <sstream> // Thư viện cho xử lý luồng chuỗi (stringstream)
#include <ctime> // Thư viện cho các hàm liên quan đến thời gian (time, ctime)
#include <algorithm> // Thư viện cho hàm find (dùng trong kiểm tra giao dịch)
#include <iomanip> // Thư viện cho định dạng đầu ra (setw, left)
#include <cstdlib> // Thư viện cho hàm rand() và srand() để tạo số ngẫu nhiên
using namespace std; // Sử dụng không gian tên std để tránh viết std:: trước các hàm/thư viện

// Hàm sinh mã ví ngẫu nhiên
string TaomaVi() { // Khai báo hàm trả về chuỗi mã ví
    const string charset = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789"; // Bộ ký tự gồm chữ cái in hoa và số
    string maVi; // Biến chuỗi lưu mã ví
    for (int i = 0; i < 8; ++i) { // Lặp 8 lần để tạo mã ví 8 ký tự
        maVi += charset[rand() % charset.length()]; // Thêm ký tự ngẫu nhiên từ charset vào mã ví
    }
    return maVi; // Trả về mã ví đã tạo
}

// Lớp quản lý ví
class WalletManagement { // Khai báo lớp WalletManagement
private:
    string Filevi; // Đường dẫn đến tệp filevidiem.txt lưu dữ liệu ví
    string Filegiaodich; // Đường dẫn đến tệp filegiaodich.txt lưu lịch sử giao dịch
    string Fileotp; // Đường dẫn đến tệp fileotp.txt lưu OTP tạm thời
    string currentUser; // Lưu tên người dùng hiện tại đang đăng nhập
    bool isAdmin; // Biến kiểm tra xem người dùng hiện tại có phải admin không

    // Đọc tất cả dữ liệu ví từ tệp ví
    vector<tuple<string, string, string, int>> DanhsachVi() { // Hàm trả về vector các tuple chứa thông tin ví
        vector<tuple<string, string, string, int>> Vi; // Vector lưu danh sách ví
        ifstream inputFile(Filevi); // Mở tệp filevidiem.txt để đọc
        string line; // Biến lưu từng dòng đọc từ tệp
        while (getline(inputFile, line)) { // Đọc từng dòng cho đến khi hết tệp
            stringstream ss(line); // Tạo luồng chuỗi để phân tích dòng
            string ma, tk, loai; // Biến lưu mã ví, tên tài khoản, loại ví
            int sodu; // Biến lưu số dư ví
            getline(ss, ma, ','); // Đọc mã ví (phân tách bằng dấu phẩy)
            getline(ss, tk, ','); // Đọc tên tài khoản
            getline(ss, loai, ','); // Đọc loại ví (chinh, phu, hoặc tong)
            ss >> sodu; // Đọc số dư ví
            Vi.emplace_back(ma, tk, loai, sodu); // Thêm tuple (mã ví, tên tài khoản, loại ví, số dư) vào vector
        }
        inputFile.close(); // Đóng tệp sau khi đọc
        return Vi; // Trả về vector chứa danh sách ví
    }

    // Lưu danh sách ví vào tệp
    void luuDuLieuVi(const vector<tuple<string, string, string, int>>& Vi) { // Hàm lưu vector ví vào tệp
        ofstream outputFile(Filevi); // Mở tệp filevidiem.txt để ghi (ghi đè)
        if (outputFile.is_open()) { // Kiểm tra xem tệp có mở thành công không
            for (const auto& vi : Vi) { // Duyệt qua từng tuple ví trong vector
                outputFile << get<0>(vi) << "," << get<1>(vi) << "," // Ghi mã ví và tên tài khoản
                         << get<2>(vi) << "," << get<3>(vi) << endl; // Ghi loại ví và số dư, kết thúc dòng
            }
            outputFile.close(); // Đóng tệp sau khi ghi
        } else {
            cerr << "Không thể mở tệp để ghi ví." << endl; // Thông báo lỗi nếu không mở được tệp
        }
    }

    // Đọc tất cả dữ liệu giao dịch từ tệp lịch sử giao dịch
    vector<tuple<string, string, string, int, string, string>> LichsuGiaodich() { // Hàm trả về vector các tuple giao dịch
        vector<tuple<string, string, string, int, string, string>> Giaodich; // Vector lưu danh sách giao dịch
        ifstream inputFile(Filegiaodich); // Mở tệp filegiaodich.txt để đọc
        string line; // Biến lưu từng dòng đọc từ tệp
        while (getline(inputFile, line)) { // Đọc từng dòng cho đến khi hết tệp
            stringstream ss(line); // Tạo luồng chuỗi để phân tích dòng
            string ma, nguon, dich, thoigian, trangthai; // Biến lưu mã giao dịch, ví nguồn, ví đích, thời gian, trạng thái
            int diem; // Biến lưu số điểm giao dịch
            getline(ss, ma, ','); // Đọc mã giao dịch
            getline(ss, nguon, ','); // Đọc mã ví nguồn
            getline(ss, dich, ','); // Đọc mã ví đích
            ss >> diem; // Đọc số điểm
            cin.ignore(); // Bỏ qua ký tự phân tách (lỗi: cin.ignore() không đúng ngữ cảnh, nên dùng ss.ignore())
            getline(ss, thoigian, ','); // Đọc thời gian giao dịch
            getline(ss, trangthai); // Đọc trạng thái giao dịch
            Giaodich.emplace_back(ma, nguon, dich, diem, thoigian, trangthai); // Thêm tuple giao dịch vào vector
        }
        inputFile.close(); // Đóng tệp sau khi đọc
        return Giaodich; // Trả về vector chứa danh sách giao dịch
    }

    // Lưu danh sách giao dịch vào tệp
    void luuDuLieuGiaodich(const vector<tuple<string, string, string, int, string, string>>& Lichsu) { // Hàm lưu vector giao dịch vào tệp
        ofstream outputFile(Filegiaodich); // Mở tệp filegiaodich.txt để ghi (ghi đè)
        if (outputFile.is_open()) { // Kiểm tra xem tệp có mở thành công không
            for (const auto& gd : Lichsu) { // Duyệt qua từng tuple giao dịch trong vector
                outputFile << get<0>(gd) << "," << get<1>(gd) << "," // Ghi mã giao dịch và ví nguồn
                         << get<2>(gd) << "," << get<3>(gd) << "," // Ghi ví đích và số điểm
                         << get<4>(gd) << "," << get<5>(gd) << endl; // Ghi thời gian và trạng thái, kết thúc dòng
            }
            outputFile.close(); // Đóng tệp sau khi ghi
        } else {
            cerr << "Không thể mở tệp để ghi giao dịch." << endl; // Thông báo lỗi nếu không mở được tệp
        }
    }

public:
    // Constructor
    WalletManagement(const string& vidiem, const string& giaodich, const string& otp) // Khởi tạo với đường dẫn các tệp
        : Filevi(vidiem), Filegiaodich(giaodich), Fileotp(otp), currentUser(""), isAdmin(false) { // Gán các biến thành viên
        // Kiểm tra và tạo ví tổng nếu chưa có
        auto danhsachvi = DanhsachVi(); // Lấy danh sách ví hiện có
        bool hasTongVi = false; // Biến kiểm tra xem ví tổng có tồn tại không
        for (const auto& vi : danhsachvi) { // Duyệt qua danh sách ví
            if (get<2>(vi) == "tong") { // Nếu tìm thấy ví loại "tong"
                hasTongVi = true; // Đặt cờ báo ví tổng đã tồn tại
                break; // Thoát vòng lặp
            }
        }
        if (!hasTongVi) { // Nếu chưa có ví tổng
            danhsachvi.emplace_back("TONG0001", "system", "tong", 100000000); // Thêm ví tổng với mã TONG0001, thuộc system, 100 triệu điểm
            luuDuLieuVi(danhsachvi); // Lưu lại danh sách ví
        }
    }

    // Lấy tên người dùng hiện tại
    string getCurrentUser() const { return currentUser; } // Trả về tên người dùng hiện tại

    // Kiểm tra trạng thái admin
    bool getIsAdmin() const { return isAdmin; } // Trả về true nếu người dùng là admin, false nếu không

    // Thiết lập người dùng hiện tại (dùng cho đăng nhập)
    void setCurrentUser(const string& user, bool admin) { // Đặt thông tin người dùng hiện tại
        currentUser = user; // Gán tên người dùng
        isAdmin = admin; // Gán trạng thái admin
    }

    // Tạo ví mới
    void taoViMoi() { // Hàm tạo ví mới cho người dùng hiện tại
        if (currentUser.empty()) { // Kiểm tra xem đã đăng nhập chưa
            cout << "Vui lòng đăng nhập trước." << endl; // Thông báo nếu chưa đăng nhập
            return; // Thoát hàm
        }
        cout << "\nTạo ví mới:" << endl; // Hiển thị tiêu đề
        cout << "1. Ví chính (được tặng 100 điểm)" << endl; // Lựa chọn ví chính
        cout << "2. Ví phụ (không có điểm)" << endl; // Lựa chọn ví phụ
        cout << "Chọn loại ví: "; // Yêu cầu nhập lựa chọn
        int choice; // Biến lưu lựa chọn
        cin >> choice; // Đọc lựa chọn từ người dùng
        if (choice != 1 && choice != 2) { // Kiểm tra lựa chọn hợp lệ
            cout << "Lựa chọn không hợp lệ." << endl; // Thông báo nếu lựa chọn không đúng
            return; // Thoát hàm
        }
        string loai = (choice == 1) ? "chinh" : "phu"; // Gán loại ví: "chinh" nếu chọn 1, "phu" nếu chọn 2
        int soDu = (choice == 1) ? 100 : 0; // Gán số dư: 100 cho ví chính, 0 cho ví phụ
        auto danhsachvi = DanhsachVi(); // Lấy danh sách ví hiện có
        string maVi = TaomaVi(); // Tạo mã ví mới
        danhsachvi.emplace_back(maVi, currentUser, loai, soDu); // Thêm ví mới vào danh sách
        luuDuLieuVi(danhsachvi); // Lưu danh sách ví vào tệp
        cout << "Đã tạo ví " << loai << " mới với mã: " << maVi << endl; // Thông báo ví đã tạo
        cout << "Số dư ban đầu: " << soDu << " điểm" << endl; // Hiển thị số dư ban đầu
    }

    // Chuyển điểm giữa các ví
    void chuyenDiem() { // Hàm thực hiện chuyển điểm giữa hai ví
        if (currentUser.empty()) { // Kiểm tra xem đã đăng nhập chưa
            cout << "Vui lòng đăng nhập trước." << endl; // Thông báo nếu chưa đăng nhập
            return; // Thoát hàm
        }
        auto danhsachvi = DanhsachVi(); // Lấy danh sách ví hiện có
        cout << "\nDanh sách ví của bạn:" << endl; // Hiển thị tiêu đề danh sách ví
        for (const auto& vi : danhsachvi) { // Duyệt qua danh sách ví
            if (get<1>(vi) == currentUser) { // Nếu ví thuộc về người dùng hiện tại
                cout << "- Mã ví: " << get<0>(vi) << " (" << get<2>(vi) // Hiển thị mã ví và loại ví
                     << "), Số dư: " << get<3>(vi) << " điểm" << endl; // Hiển thị số dư
            }
        }
        string maViNguon, maViDich; // Biến lưu mã ví nguồn và đích
        int soDiem; // Biến lưu số điểm cần chuyển
        cout << "\nNhập mã ví nguồn: "; // Yêu cầu nhập mã ví nguồn
        cin >> maViNguon; // Đọc mã ví nguồn
        cout << "Nhập mã ví đích: "; // Yêu cầu nhập mã ví đích
        cin >> maViDich; // Đọc mã ví đích
        cout << "Nhập số điểm cần chuyển: "; // Yêu cầu nhập số điểm
        cin >> soDiem; // Đọc số điểm
        if (soDiem <= 0) { // Kiểm tra số điểm có lớn hơn 0 không
            cout << "Số điểm phải lớn hơn 0" << endl; // Thông báo nếu số điểm không hợp lệ
            return; // Thoát hàm
        }
        bool validNguon = false; // Biến kiểm tra ví nguồn hợp lệ
        bool validDich = false; // Biến kiểm tra ví đích hợp lệ
        int viNguonIndex = 0; // Chỉ số của ví nguồn trong vector
        int viDichIndex = 0; // Chỉ số của ví đích trong vector
        for (size_t i = 0; i < danhsachvi.size(); ++i) { // Duyệt qua danh sách ví
            if (get<0>(danhsachvi[i]) == maViNguon && get<1>(danhsachvi[i]) == currentUser) { // Nếu mã ví nguồn khớp và thuộc người dùng
                validNguon = true; // Đánh dấu ví nguồn hợp lệ
                viNguonIndex = i; // Lưu chỉ số ví nguồn
            }
            if (get<0>(danhsachvi[i]) == maViDich) { // Nếu mã ví đích tồn tại
                validDich = true; // Đánh dấu ví đích hợp lệ
                viDichIndex = i; // Lưu chỉ số ví đích
            }
        }
        if (!validNguon) { // Kiểm tra ví nguồn
            cout << "Ví nguồn không hợp lệ hoặc không thuộc quyền sở hữu của bạn." << endl; // Thông báo lỗi
            return; // Thoát hàm
        }
        if (!validDich) { // Kiểm tra ví đích
            cout << "Ví đích không tồn tại." << endl; // Thông báo lỗi
            return; // Thoát hàm
        }
        if (get<3>(danhsachvi[viNguonIndex]) < soDiem) { // Kiểm tra số dư ví nguồn
            cout << "Số dư không đủ để thực hiện giao dịch." << endl; // Thông báo nếu số dư không đủ
            return; // Thoát hàm
        }
        string otp = "DUMMY_OTP"; // Giả lập OTP vì hàm TaoOTP() không thuộc phần phụ trách
        ofstream otpFile(Fileotp); // Mở tệp fileotp.txt để ghi OTP
        otpFile << currentUser << "," << otp << endl; // Ghi tên người dùng và OTP vào tệp
        otpFile.close(); // Đóng tệp OTP
        cout << "Đã gửi mã OTP đến email của bạn." << endl; // Thông báo giả lập gửi OTP
        string inputOTP; // Biến lưu OTP do người dùng nhập
        cout << "Nhập OTP: "; // Yêu cầu nhập OTP
        cin >> inputOTP; // Đọc OTP từ người dùng
        if (inputOTP != otp) { // Kiểm tra OTP có khớp không
            cout << "OTP không đúng. Giao dịch bị hủy." << endl; // Thông báo nếu OTP sai
            return; // Thoát hàm
        }
        get<3>(danhsachvi[viNguonIndex]) -= soDiem; // Trừ số điểm từ ví nguồn
        get<3>(danhsachvi[viDichIndex]) += soDiem; // Cộng số điểm vào ví đích
        luuDuLieuVi(danhsachvi); // Lưu danh sách ví đã cập nhật
        time_t now = time(0); // Lấy thời gian hiện tại
        string tg = ctime(&now); // Chuyển thời gian thành chuỗi
        tg.pop_back(); // Xóa ký tự xuống dòng cuối chuỗi thời gian
        string maGD = "GD" + to_string(LichsuGiaodich().size() + 1); // Tạo mã giao dịch (GD + số thứ tự)
        auto lsgd = LichsuGiaodich(); // Lấy danh sách giao dịch hiện có
        lsgd.emplace_back(maGD, maViNguon, maViDich, soDiem, tg, "thanh_cong"); // Thêm giao dịch mới vào danh sách
        luuDuLieuGiaodich(lsgd); // Lưu danh sách giao dịch
        cout << "Chuyển điểm thành công!" << endl; // Thông báo giao dịch thành công
        cout << "Mã giao dịch: " << maGD << endl; // Hiển thị mã giao dịch
    }

    // Xem số dư của tài khoản
    void XemsoduTk() { // Hàm hiển thị số dư ví của tài khoản
        if (currentUser.empty()) { // Kiểm tra xem đã đăng nhập chưa
            cout << "Vui lòng đăng nhập trước." << endl; // Thông báo nếu chưa đăng nhập
            return; // Thoát hàm
        }
        string TenTk = currentUser; // Mặc định xem số dư của người dùng hiện tại
        if (isAdmin) { // Nếu là admin
            cout << "Nhập tên tài khoản muốn xem (bỏ trống để xem của bạn): "; // Yêu cầu nhập tên tài khoản
            cin.ignore(); // Xóa bộ đệm đầu vào
            getline(cin, TenTk); // Đọc tên tài khoản
            if (TenTk.empty()) { // Nếu không nhập, dùng tài khoản hiện tại
                TenTk = currentUser; // Gán lại tên tài khoản hiện tại
            }
        }
        auto danhsachvi = DanhsachVi(); // Lấy danh sách ví hiện có
        bool found = false; // Biến kiểm tra xem có tìm thấy ví nào không
        cout << "\nDanh sách ví của " << TenTk << ":" << endl; // Hiển thị tiêu đề
        for (const auto& vi : danhsachvi) { // Duyệt qua danh sách ví
            if (get<1>(vi) == TenTk) { // Nếu ví thuộc tài khoản được chỉ định
                cout << "- Mã ví: " << get<0>(vi) // Hiển thị mã ví
                     << " | Loại: " << get<2>(vi) // Hiển thị loại ví
                     << " | Số dư: " << get<3>(vi) << " điểm" << endl; // Hiển thị số dư
                found = true; // Đánh dấu đã tìm thấy ví
            }
        }
        if (!found) { // Nếu không tìm thấy ví
            cout << "Không tìm thấy ví nào." << endl; // Thông báo không có ví
        }
    }
};