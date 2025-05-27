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
