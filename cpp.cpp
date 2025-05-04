#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

using namespace std;

class Auth {
private:
    string accountsFile;

    // Hàm kiểm tra xem tên tài khoản đã tồn tại chưa
    bool isUsernameExists(const string& username) {
        ifstream inputFile(accountsFile);
        string line;
        while (getline(inputFile, line)) {
            stringstream ss(line);
            string storedUsername;
            getline(ss, storedUsername, ',');
            if (storedUsername == username) {
                inputFile.close();
                return true;
            }
        }
        inputFile.close();
        return false;
    }

public:
    Auth(const string& file) : accountsFile(file) {}

    // Hàm để tạo một tài khoản mới
    void createAccount() {
        string username, password;
        cout << "Nhập tên tài khoản mới: ";
        cin >> username;
        cout << "Nhập mật khẩu cho tài khoản '" << username << "': ";
        cin >> password;

        if (isUsernameExists(username)) {
            cout << "Tên tài khoản '" << username << "' đã tồn tại. Vui lòng chọn tên khác." << endl;
            return;
        }

        ofstream outputFile(accountsFile, ios::app);
        if (outputFile.is_open()) {
            outputFile << username << "," << password << endl;
            outputFile.close();
            cout << "Tài khoản '" << username << "' đã được tạo thành công!" << endl;
        } else {
            cerr << "Không thể mở tệp để ghi." << endl;
        }
    }

    // Hàm để đăng nhập
    bool login() {
        string username, password;
        cout << "Nhập tên tài khoản: ";
        cin >> username;
        cout << "Nhập mật khẩu: ";
        cin >> password;

        ifstream inputFile(accountsFile);
        string line;
        while (getline(inputFile, line)) {
            stringstream ss(line);
            string storedUsername, storedPassword;
            getline(ss, storedUsername, ',');
            getline(ss, storedPassword, ',');
            if (storedUsername == username && storedPassword == password) {
                inputFile.close();
                cout << "Đăng nhập thành công với tài khoản '" << username << "'!" << endl;
                return true;
            }
        }
        inputFile.close();
        cout << "Đăng nhập thất bại. Tên tài khoản hoặc mật khẩu không đúng." << endl;
        return false;
    }
};

int main() {
    Auth authentication("accounts.txt");
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
                authentication.createAccount();
break;
            case 2:
                authentication.login();
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
