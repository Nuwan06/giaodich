#include <bits/stdc++.h>
using namespace std;
class Tk {
    private:
    string fileTk;
    bool kiemtraTk(const string& username) {
        ifstream inputFile(fileTk);
        string line;
        while (getline(inputFile, line)) {
            stringstream ss(line);
            string temp;
            while(getline(ss, temp, ','));
            if(temp == username) {
                inputFile.close();
                return true;
            }

        }
     
    inputFile.close();
    return false;
}
public:
Tk(const string& file) : fileTk(file) {}
void TaoTk() {
    string Taikhoan, Matkhau;
    cout << "Nhập tên tài khoản mới: ";
    cin >> Taikhoan;
    cout <<"Nhập mật khẩu mới: ";
    cin >> Matkhau;
    if (kiemtraTk(Taikhoan)) {
        cout << "Tên tài khoản đã tồn tại"<<endl;
        return;
    }
    ofstream outputFile(fileTk, ios::app);
    if (outputFile.is_open()) {
        outputFile << Taikhoan<<","<<Matkhau<<endl;
        outputFile.close();
        cout << "Thanh cong"<<endl;
    } else {
        cerr << "error"<<endl;
    }
}
bool login() {
    string Taikhoan, Matkhau;
    cout << "Nhập tên tài khoản: ";
    cin >> Taikhoan;
    cout <<"Nhập mật khẩu: ";
    cin >> Matkhau;
    ifstream inputFile(fileTk);
    string line;
    while(getline(inputFile, line)){
    stringstream ss(line);
    string fTk, fileMk;
    getline(ss, fTk, ',');
    getline(ss,fileMk, ',' );
if (fTk == Taikhoan && fileMk == Matkhau) {
     inputFile.close();
     cout<<"thanhcong"<<endl;
     return true;
}
}
inputFile.close();
cout<<"thatbai"<<endl;
return false;
}
};
int main() {
    Tk Tk("accounts.txt");
    int choice;
    do {
        cout << "\n--- Chức năng ---" << endl;
        cout << "1. Tạo tài khoản" << endl;
        cout << "2. Đăng nhập" << endl;
        cout << "0. Thoát" << endl;
        cout << "Chọn một chức năng: ";
        cin >> choice;

        switch (choice) {
            case 1:
                Tk.TaoTk();
break;
            case 2:
               Tk.login();
                break;
            case 0:
                cout << "Đang thoát..." << endl;
                break;
            default:
                cout << "Lựa chọn không hợp lệ. Vui lòng thử lại." << endl;
        }
    } while (choice != 0);

    return 0;
}