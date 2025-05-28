# README - Hệ Thống Quản Lý Ví Điểm

## Giới thiệu dự án
Hệ thống Quản lý Ví Điểm là một ứng dụng console được phát triển bằng C++ giúp quản lý tài khoản người dùng, ví điểm và các giao dịch chuyển điểm giữa các ví. Hệ thống bao gồm các chức năng chính :
- Đăng ký/đăng nhập tài khoản với xác thực OTP
- Quản lý ví (tạo mới, xem số dư)
- Thực hiện giao dịch chuyển điểm
- Xem lịch sử giao dịch
- Phân quyền admin/user
- Nên sử dụng một tệp tin cho tất cả người dùng vì:
  + Dễ xác thực đăng nhập
  + Quản lý tập trung các tài khoản
  + Phù hợp cho hệ thống có < 10.000 user
  + Dễ tri xuất và kiểm tra thông tin 

## Thành viên tham gia và phân công công việc

### **Trần Thanh Nhuận**: Quản lý Tài khoản & Xác thực
- Phụ trách:
  - Hệ thống đăng nhập/đăng ký (`login()`, `TaoTaikhoan()`)
  - Sinh mật khẩu tự động ( `TaoMatkhauTudong()`)
  - Quản lý mật khẩu (`ThaydoiMatkhau()`, `hashFunction()`)
  - Hệ thông quên mật khẩu (`quenmatkhau()`)
  - Hệ thống OTP (`TaoOTP()`, xác thực OTP)
  - File `accounts.txt`
  - File `matkhautudong.txt`

### **Mạc Thanh Toàn**: Quản lý Ví & Giao dịch
- Phụ trách:
  - Hệ thống ví (`DanhsachVi()`, `taoViMoi()`, `TaomaVi()`)
  - Chức năng chuyển điểm (`chuyenDiem()`)
  - Quản lý số dư (`XemsoduTk()`)
  - File `filevidiem.txt`

### **Nguyễn Ngọc Toàn**: Quản lý Giao dịch & Admin
- Phụ trách:
  - Lịch sử giao dịch (`LichsuGiaodich()`, `xemLichSuGiaoDich()`)
  - Chức năng admin (`XemDanhSachNhom()`, `AdminTaoTaiKhoan()`, `DieuChinhThongTin()`)
  - File `filegiaodich.txt`
  - File `fileotp.txt`

## Đặc tả chức năng

### 1. Quản lý tài khoản
- Đăng ký tài khoản mới với xác thực email và CCCD
- Đăng nhập với xác thực 2 bước (mật khẩu + OTP)
- Đổi mật khẩu với xác thực OTP
- Quên mật khẩu và đổi lại với xác thực otp
- Phân quyền admin/user

### 2. Quản lý ví
- Tạo ví mới (ví chính/phụ)
- Xem số dư các ví
- Chuyển điểm giữa các ví với xác thực OTP

### 3. Quản lý giao dịch
- Xem lịch sử giao dịch

### 4. Chức năng admin
- Xem danh sách tất cả tài khoản
- Tạo tài khoản mới
- Điều chỉnh thông tin tài khoản

## Cài đặt và chạy chương trình

### Yêu cầu hệ thống
- Trình biên dịch C++ (g++, clang++, Visual Studio)
- Hệ điều hành: Windows/Linux/macOS
- Thư viện chuẩn: iostream, string, vector, fstream, sstream, ctime, algorithm, iomanip, cctype, functional, cstdlib

### Các bước cài đặt
1. Clone repository:
   ```
   git clone [repository-url]
   ```
2. Biên dịch chương trình:
   ```
   g++ main.cpp -o point_wallet_system
   ```
3. Chạy chương trình:
   ```
   ./point_wallet_system
   ```

### Các file dữ liệu
- `accounts.txt`: Lưu thông tin tài khoản
- `filevidiem.txt`: Lưu thông tin các ví
- `filegiaodich.txt`: Lưu lịch sử giao dịch
- `fileotp.txt`: Lưu OTP tạm thời

## Hướng dẫn sử dụng

1. **Đăng ký tài khoản**:
   - Chọn chức năng 2 từ menu chính
   - Nhập thông tin theo hướng dẫn

2. **Đăng nhập**:
   - Chọn chức năng 1 từ menu chính
   - Nhập tên đăng nhập và mật khẩu
   - Nhập OTP được gửi đến email (hiển thị trên `fileotp.txt`)

3. **Tạo ví mới**:
   - Đăng nhập và chọn chức năng "Tạo ví mới"
   - Chọn loại ví (chính/phụ)

4. **Chuyển điểm**:
   - Chọn chức năng "Chuyển điểm"
   - Nhập thông tin ví nguồn, ví đích và số điểm
   - Xác nhận bằng OTP

5. **Xem lịch sử giao dịch**:
   - Chọn chức năng "Xem lịch sử giao dịch"
6. **Xem danh sách  tài khoản và điều chỉnh thông tin tài khoản (chỉ dùng cho admin)**:
   - Chọn chúc năng "Quản lý tài khoản(Chỉ dành cho admin)"
## Tài khoản mặc định
- Tài khoản admin: `admin` , mật khẩu: `abcd1234` (tự động tạo khi chạy lần đầu)
- Ví tổng hệ thống: `TONG0001` (100,000,000 điểm)

## Lưu ý
- Chương trình sử dụng mô phỏng gửi OTP (OTP sẽ hiển thị trên `fileotp.txt` thay vì gửi email thực)
- Mật khẩu được lưu dưới dạng băm để đảm bảo an toàn
- Dữ liệu được lưu trong các file text đơn giản

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
[11] Hướng dẫn sử dụng Git: https://git-scm.com/doc
[12] Mẫu thiết kế hệ thống quản lý ví điện tử


