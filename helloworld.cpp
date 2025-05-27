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
