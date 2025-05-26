# Hệ Thống Quản Lý Ví Điểm

## Mô tả
Đây là một hệ thống quản lý ví điểm được viết bằng C++, cho phép người dùng thực hiện các chức năng như:
- Đăng ký và đăng nhập tài khoản
- Quản lý ví điểm (tạo ví mới, xem số dư, chuyển điểm)
- Xem lịch sử giao dịch
- Đổi mật khẩu
- Quản lý tài khoản (dành cho admin)

## Tính năng chính
1. **Quản lý tài khoản**
   - Đăng ký tài khoản mới với mật khẩu tự động hoặc tự nhập
   - Đăng nhập với xác thực OTP qua email (mô phỏng)
   - Đổi mật khẩu với xác thực OTP
   - Phân quyền admin/user

2. **Quản lý ví điểm**
   - Tạo ví mới (ví chính hoặc ví phụ)
   - Xem số dư các ví
   - Chuyển điểm giữa các ví với xác thực OTP

3. **Lịch sử giao dịch**
   - Xem toàn bộ lịch sử giao dịch của tài khoản

4. **Chức năng admin**
   - Xem danh sách tất cả tài khoản
   - Tạo tài khoản mới
   - Điều chỉnh thông tin tài khoản

## Công nghệ sử dụng
- Ngôn ngữ: C++
- Thư viện chuẩn: iostream, string, vector, fstream, sstream, ctime, algorithm, iomanip, cctype, functional, cstdlib

## Cài đặt và chạy chương trình
1. Clone repository này
2. Biên dịch chương trình bằng trình biên dịch C++ (g++, clang++, v.v.)
   ```
   g++ main.cpp -o point_wallet_system
   ```
3. Chạy chương trình
   ```
   ./point_wallet_system
   ```

## Cấu trúc tệp
- `accounts.txt`: Lưu trữ thông tin tài khoản
- `filevidiem.txt`: Lưu trữ thông tin các ví
- `filegiaodich.txt`: Lưu trữ lịch sử giao dịch
- `fileotp.txt`: Lưu trữ mã OTP tạm thời (mô phỏng)

## Tài khoản mặc định
- Tài khoản admin: `admin` (được tạo tự động khi chạy chương trình lần đầu)
- Ví tổng: `TONG0001` (tự động tạo với 100,000,000 điểm)

## Hướng dẫn sử dụng
1. Đăng ký tài khoản mới hoặc đăng nhập bằng tài khoản admin
2. Tùy theo quyền (admin/user) mà bạn có thể sử dụng các chức năng tương ứng
3. Các thao tác quan trọng đều yêu cầu xác thực OTP (mã sẽ hiển thị trên màn hình)

## Thành viên nhóm và công việc được giao
- **Trần Thanh Nhuận**: Phát triển chức năng tạo tài khoản, đăng nhập, OTP.
- **Mạc Thanh Toàn**: Phát triển chức năng đổi mật khẩu, xem số dư, thêm điểm.
- **Nguyễn Ngọc Toàn**: Phát triển chức năng giao dịch giữa ví, lịch sử giao dịch, kiểm tra hệ thống.

## Lưu ý
- Đảm bảo file `accounts.txt`, `filevidiem.txt`, `filegiaodich.txt` và `fileotp.txt` có quyền đọc/ghi.
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



