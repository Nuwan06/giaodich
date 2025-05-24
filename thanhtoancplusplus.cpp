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