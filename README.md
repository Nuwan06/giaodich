

# Hệ thống Quản lý Ví Điểm Thưởng

## Mô tả dự án
Hệ thống quản lý ví điểm thưởng được phát triển để hỗ trợ đăng ký, đăng nhập tài khoản, quản lý ví điểm thưởng (ví chính và ví phụ), giao dịch giữa các ví, và theo dõi lịch sử giao dịch. Hệ thống sử dụng OTP để xác thực an toàn và phân quyền giữa người dùng thường và admin.

### Chức năng chính
- **Tạo tài khoản**: Người dùng nhập thông tin (tên, mật khẩu, ngày sinh, CCCD) để tạo tài khoản.
- **Đăng nhập**: Đăng nhập với tên, mật khẩu và OTP xác thực.
- **Đổi mật khẩu**: Đổi mật khẩu với OTP.
- **Quản lý ví**:
  - Xem số dư ví chính và ví phụ (đơn vị: điểm).
  - Thêm điểm thưởng (chỉ admin).
  - Giao dịch: Chuyển điểm giữa ví chính và ví phụ (yêu cầu OTP).
- **Lịch sử giao dịch**: Ghi và xem lịch sử giao dịch (chỉ admin).

## Yêu cầu
- **Ngôn ngữ**: C++.
- **Trình biên dịch**: g++.
- **Hệ điều hành**: Windows, Linux, hoặc macOS.

## Hướng dẫn chạy chương trình
1. **Chuẩn bị**:
   - Lưu mã nguồn vào file `QuanlyTaikhoan.cpp`.
   - Đảm bảo đã cài g++ (`g++ --version` để kiểm tra).

2. **Biên dịch**:
   ```bash
   g++ QuanlyTaikhoan.cpp -o QuanlyTaikhoan
   ```

3. **Chạy chương trình**:
   ```bash
   ./QuanlyTaikhoan
   ```

4. **Sử dụng**:
   - Tạo tài khoản (chọn 1).
   - Đăng nhập (chọn 2) với OTP.
   - Sử dụng các chức năng khác (xem số dư, thêm điểm, giao dịch, v.v.).

## Cấu trúc file
- **accounts.txt**: Lưu thông tin tài khoản (tên, mật khẩu băm, ngày sinh, CCCD, ví chính, ví phụ, quyền admin).
  - Ví dụ: `user1,123456789...,01/01/2000,123456789,500,500,0`
- **transactions.txt**: Lưu lịch sử giao dịch (tài khoản, loại giao dịch, số điểm, ví nguồn, ví đích, thời gian).
  - Ví dụ: `user1,Thêm điểm,1000,Hệ thống,Ví chính,Wed May 24 14:12:00 2025`

## Thành viên nhóm và công việc được giao
- **Trần Thanh Nhuận**: Phát triển chức năng tạo tài khoản, đăng nhập, OTP.
- **Mạc Thanh Toàn**: Phát triển chức năng đổi mật khẩu, xem số dư, thêm điểm.
- **Nguyễn Ngọc Toàn**: Phát triển chức năng giao dịch giữa ví, lịch sử giao dịch, kiểm tra hệ thống.

## Lưu ý
- Đảm bảo file `accounts.txt` và `transactions.txt` có quyền đọc/ghi.
- OTP trong chương trình được sinh ngẫu nhiên và hiển thị (mô phỏng). Trong thực tế, cần tích hợp gửi OTP qua email/SMS.
## TÀI LIỆU THAM KHẢO

[1] "C++ Language Reference", cppreference.com. Truy cập tại: https://en.cppreference.com/w/cpp  
[2] "File streams in C++", cplusplus.com. Truy cập tại: https://cplusplus.com/doc/tutorial/files/  
[3] "String streams in C++", GeeksforGeeks. Truy cập tại: https://www.geeksforgeeks.org/stringstream-c-applications/  
[4] "Generating random numbers in C++", GeeksforGeeks. Truy cập tại: https://www.geeksforgeeks.org/generating-random-number-cpp/  
[5] "std::vector", cppreference.com. Truy cập tại: https://en.cppreference.com/w/cpp/container/vector  
[6] "Time utilities in C++", cppreference.com. Truy cập tại: https://en.cppreference.com/w/cpp/chrono/c/time  
[7] "Manipulators in C++", cppreference.com. Truy cập tại: https://en.cppreference.com/w/cpp/io/manip  
[8] "Structs and Classes in C++", Programiz. Truy cập tại: https://www.programiz.com/cpp-programming/structure  
[9] "system() Function in C++", cplusplus.com. Truy cập tại: https://cplusplus.com/reference/cstdlib/system/  
[10] GeeksforGeeks. (n.d.). Hashing Data Structure. Truy cập từ: https://www.geeksforgeeks.org/hashing-data-structure/



