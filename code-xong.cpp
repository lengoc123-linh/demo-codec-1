#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <algorithm>
#include <regex>
#include <limits>
using namespace std;

struct sukien {
    int douutien;
    string mota, ngay;
    string giobatdau, gioketthuc;

    sukien(int uutien, const string& mt, const string& ng,
           const string& gbd, const string& gkt)
        : douutien(uutien), mota(mt), ngay(ng),
          giobatdau(gbd), gioketthuc(gkt) {}

    sukien() : douutien(0), mota(""), ngay(""),
               giobatdau("00:00"), gioketthuc("00:00") {}

    bool operator<(const sukien& other) const {
        if (giobatdau == other.giobatdau) {
            return douutien < other.douutien;
        }
        return giobatdau > other.giobatdau;
    }

    bool operator==(const sukien& other) const {
        return mota == other.mota && giobatdau == other.giobatdau && gioketthuc == other.gioketthuc;
    }
};

bool kiemtrathoigian(const string& time) {
    regex pattern(R"(^([01][0-9]|2[0-3]):[0-5][0-9]$)");
    return regex_match(time, pattern);
}

class cay_avl {
private:
    struct node {
        sukien sk;
        node* trai;
        node* phai;
        int chieucao;
        node(sukien s) : sk(s), trai(nullptr), phai(nullptr), chieucao(1) {}
    };
    node* goc;

    int chieucao(node* nut) {
        return nut ? nut->chieucao : 0;
    }

    int hesolech(node* nut) {
        return chieucao(nut->trai) - chieucao(nut->phai);
    }

    void capnhatchieucao(node* nut) {
        nut->chieucao = max(chieucao(nut->trai), chieucao(nut->phai)) + 1;
    }

    node* quayphai(node* y) {
        node* x = y->trai;
        node* t2 = x->phai;
        x->phai = y;
        y->trai = t2;
        capnhatchieucao(y);
        capnhatchieucao(x);
        return x;
    }

    node* quaytrai(node* x) {
        node* y = x->phai;
        node* t2 = y->trai;
        y->trai = x;
        x->phai = t2;
        capnhatchieucao(x);
        capnhatchieucao(y);
        return y;
    }

    node* canbang(node* nut) {
        int hl = hesolech(nut);
        if (hl > 1) {
            if (hesolech(nut->trai) < 0) {
                nut->trai = quaytrai(nut->trai);
            }
            return quayphai(nut);
        }
        if (hl < -1) {
            if (hesolech(nut->phai) > 0) {
                nut->phai = quayphai(nut->phai);
            }
            return quaytrai(nut);
        }
        return nut;
    }

    node* them(node* nut, const sukien& sk) {
        if (!nut) return new node(sk);
        if (sk.giobatdau < nut->sk.giobatdau) {
            nut->trai = them(nut->trai, sk);
        } else {
            nut->phai = them(nut->phai, sk);
        }
        capnhatchieucao(nut);
        return canbang(nut);
    }

    void duyet(node* nut, priority_queue<sukien>& pq) {
        if (nut != nullptr) {
            duyet(nut->trai, pq);
            pq.push(nut->sk);
            duyet(nut->phai, pq);
        }
    }

public:
    cay_avl() : goc(nullptr) {}

    void them(const sukien& sk) {
        goc = them(goc, sk);
    }

    void timsukientheongay(priority_queue<sukien>& km) {
        duyet(goc, km);
    }
};

class thoikhoabieu {
public:
    vector<vector<sukien>> tkb;
    cay_avl cay;

    thoikhoabieu() {
        tkb.resize(6);
    }

    void nhap() {
        for (int i = 0; i < 6; ++i) {
            string ngay;
            cout << "Nhap thoi gian cho thu " << i + 2 << " (co the de trong neu khong co su kien):\n";
            int sosukien;
            while (true) {
                cout << "Nhap so su kien trong ngay: ";
                cin >> sosukien;

                if (cin.fail() || sosukien < 0) {
                    cout << "Vui long nhap mot so nguyen duong.\n";
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                } else {
                    cin.ignore();
                    break;
                }
            }

            for (int j = 0; j < sosukien; ++j) {
                string mota, giobatdau, gioketthuc;

                cout << "Nhap mo ta su kien " << j + 1 << ": ";
                getline(cin, mota);

                cout << "Nhap gio bat dau (dinh dang 24h, VD: 14:20): ";
                getline(cin, giobatdau);
                while (!kiemtrathoigian(giobatdau)) {
                    cout << "Nhap sai dinh dang, vui long nhap lai: ";
                    getline(cin, giobatdau);
                }

                cout << "Nhap gio ket thuc (dinh dang 24h, VD: 14:20): ";
                getline(cin, gioketthuc);
                while (!kiemtrathoigian(gioketthuc)) {
                    cout << "Nhap sai dinh dang, vui long nhap lai: ";
                    getline(cin, gioketthuc);
                }

                sukien sukienmoi(0, mota, ngay, giobatdau, gioketthuc);
                tkb[i].emplace_back(sukienmoi);
                cay.them(sukienmoi);
            }
        }
    }

    void themSukienVaoCay(const sukien& sk) {
        cay.them(sk); // Thêm sự kiện vào cây AVL
    }

    void hienSukienTuCay() {
        priority_queue<sukien> pq;
        cay.timsukientheongay(pq);

        cout << "\nDanh sach su kien trong cay AVL (theo thu tu thoi gian):\n";
        while (!pq.empty()) {
            sukien sk = pq.top();
            pq.pop();
            cout << "  Mo ta: " << sk.mota
                 << ", Gio bat dau: " << sk.giobatdau
                 << ", Gio ket thuc: " << sk.gioketthuc;
            if (sk.douutien != 0) {
                cout << ", Do uu tien: " << sk.douutien;
            }
            cout << "\n";
        }
    }

    void hien() {
        for (int i = 0; i < 6; ++i) {
            cout << "Thu " << i + 2 << ":\n";
            for (const auto& sk : tkb[i]) {
                cout << sk.mota << " - " << sk.giobatdau << " -> " << sk.gioketthuc << "\n";
            }
        }
    }
};

// Cập nhật hàm `hienthisukien` trong lớp `quanlysukien`
class quanlysukien {
private:
    thoikhoabieu tkb;

    bool kiemtrasukientrung(const sukien& sk, vector<sukien>& sukienngay, sukien& sukientrunglap) {
        for (auto& skcu : sukienngay) {
            int bd1 = stoi(skcu.giobatdau.substr(0, 2)) * 60 + stoi(skcu.giobatdau.substr(3, 2));
            int kt1 = stoi(skcu.gioketthuc.substr(0, 2)) * 60 + stoi(skcu.gioketthuc.substr(3, 2));
            int bd2 = stoi(sk.giobatdau.substr(0, 2)) * 60 + stoi(sk.giobatdau.substr(3, 2));
            int kt2 = stoi(sk.gioketthuc.substr(0, 2)) * 60 + stoi(sk.gioketthuc.substr(3, 2));

            if (!(kt1 <= bd2 || bd1 >= kt2)) {
                sukientrunglap = skcu; // Ghi nhận sự kiện cũ bị trùng
                return true;
            }
        }
        return false;
    }

    sukien xulysukientrunglap(sukien& skcu, sukien& skmoi) {
        cout << "Su kien moi trung lap voi su kien: " << skcu.mota << "\n";
        cout << "Thoi gian su kien cu: " << skcu.giobatdau << " -> " << skcu.gioketthuc << "\n";
        cout << "Thoi gian su kien moi: " << skmoi.giobatdau << " -> " << skmoi.gioketthuc << "\n";

        int douutien;
        do {
            cout << "Nhap do uu tien cua su kien cu (1->10): ";
            cin >> douutien;
            if (cin.fail() || douutien < 1 || douutien > 10) {
                cout << "Gia tri khong hop le. Vui long nhap lai.\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            } else {
                break;
            }
        } while (true);
        skcu.douutien = douutien;

        do {
            cout << "Nhap do uu tien cua su kien moi (1->10): ";
            cin >> douutien;
            if (cin.fail() || douutien < 1 || douutien > 10) {
                cout << "Gia tri khong hop le. Vui long nhap lai.\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            } else {
                break;
            }
        } while (true);
        skmoi.douutien = douutien;

        if (skmoi.douutien > skcu.douutien) {
            cout << "Su kien moi co uu tien cao hon, thay the su kien cu.\n";
            return skmoi;
        } else {
            cout << "Su kien cu co uu tien cao hon, giu lai su kien cu.\n";
            return skcu;
        }
    }

public:
	void nhapthoikhoabieu() {
        tkb.nhap();  // Gọi hàm nhap() của lớp thoikhoabieu
    }
    void themsukien() {
    int thu;
    string mota, giobatdau, gioketthuc;
    do {
        cout << "Nhap thu (1: Thu 2, 2: Thu 3, ..., 6: Thu 7): ";
        cin >> thu;
        if (cin.fail() || thu < 1 || thu > 6) {
            cout << "Thu khong hop le. Vui long nhap lai (1 den 6).\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Bỏ qua các ký tự còn lại trong dòng
        } else {
            break;
        }
    } while (true);

    cin.ignore();
    // Nhập các thông tin khác
    cout << "Nhap mo ta su kien: ";
    getline(cin, mota);

    cout << "Nhap gio bat dau (dinh dang 24h, VD: 14:20): ";
    getline(cin, giobatdau);
    while (!kiemtrathoigian(giobatdau)) {
        cout << "Nhap sai dinh dang, vui long nhap lai: ";
        getline(cin, giobatdau);
    }

    cout << "Nhap gio ket thuc (dinh dang 24h, VD: 14:20): ";
    getline(cin, gioketthuc);
    while (!kiemtrathoigian(gioketthuc)) {
        cout << "Nhap sai dinh dang, vui long nhap lai: ";
        getline(cin, gioketthuc);
    }

    sukien sukienmoi(0, mota, "Thu " + to_string(thu + 1), giobatdau, gioketthuc);

    bool trungLap = false;
    sukien sukiencu;  // Lưu sự kiện cũ nếu có trùng lặp
    for (auto& sk : tkb.tkb[thu - 1]) {
        int bd1 = stoi(sk.giobatdau.substr(0, 2)) * 60 + stoi(sk.giobatdau.substr(3, 2));
        int bd2 = stoi(sukienmoi.giobatdau.substr(0, 2)) * 60 + stoi(sukienmoi.giobatdau.substr(3, 2));
        int kt1 = stoi(sk.gioketthuc.substr(0, 2)) * 60 + stoi(sk.gioketthuc.substr(3, 2));
        int kt2 = stoi(sukienmoi.gioketthuc.substr(0, 2)) * 60 + stoi(sukienmoi.gioketthuc.substr(3, 2));

        if (!(kt1 <= bd2 || bd1 >= kt2)) {
            cout << "Su kien moi (" << sukienmoi.mota << ") trung voi su kien: " << sk.mota << "\n";
            cout << "Thoi gian su kien moi: " << sukienmoi.giobatdau << " -> " << sukienmoi.gioketthuc << "\n";
            cout << "Thoi gian su kien trung lap: " << sk.giobatdau << " -> " << sk.gioketthuc << "\n";

            // Lưu sự kiện cũ để so sánh
            sukiencu = sk;

            // Yêu cầu nhập độ ưu tiên của sự kiện cũ
            int douutien;
            cout << "Vui long nhap do uu tien cua su kien trong thoi khoa bieu (1->10): ";
            cin >> douutien;
            while (cin.fail() || douutien < 1 || douutien > 10) {
                cout << "Do uu tien khong hop le, vui long nhap lai (1->10): ";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cin >> douutien;
            }
            cin.ignore();
            sk.douutien = douutien;

            // Nhập độ ưu tiên cho sự kiện mới
            cout << "Vui long nhap do uu tien cua su kien moi (1->10): ";
            cin >> douutien;
            while (cin.fail() || douutien < 1 || douutien > 10) {
                cout << "Do uu tien khong hop le, vui long nhap lai (1->10): ";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cin >> douutien;
            }
            cin.ignore();
            sukienmoi.douutien = douutien;

            trungLap = true;
            break;
        }
    }

    if (trungLap) {
        // So sánh và chọn sự kiện có độ ưu tiên cao hơn
        if (sukienmoi.douutien > sukiencu.douutien) {
            // Loại bỏ sự kiện cũ, thêm sự kiện mới vào
            cout << "Su kien moi co do uu tien cao hon, thay the su kien cu.\n";
            // Cập nhật thời khóa biểu và cây AVL
            tkb.tkb[thu - 1].erase(remove(tkb.tkb[thu - 1].begin(), tkb.tkb[thu - 1].end(), sukiencu), tkb.tkb[thu - 1].end());
            tkb.tkb[thu - 1].push_back(sukienmoi);
            tkb.themSukienVaoCay(sukienmoi);
        } else {
            // Giữ sự kiện cũ và loại bỏ sự kiện mới
            cout << "Su kien cu co do uu tien cao hon, giu su kien cu.\n";
        }
    } else {
        // Nếu không có trùng lặp, thêm sự kiện mới vào
        tkb.tkb[thu - 1].push_back(sukienmoi);
        tkb.themSukienVaoCay(sukienmoi);
    }
}
     void hienthisukien() {
    int thu;
    do {
        cout << "Nhap thu muon xem (1: Thu 2, 2: Thu 3, ..., 6: Thu 7): ";
        cin >> thu;
        if (cin.fail() || thu < 1 || thu > 6) {
            cout << "Thu khong hop le. Vui long nhap lai (1 den 6).\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Bỏ qua các ký tự còn lại trong dòng
        } else {
            break;
        }
    } while (true);

    cout << "Danh sach su kien vao thu " << thu << ":\n";
    bool coSuKien = false;

    for (auto& sk : tkb.tkb[thu - 1]) {
        cout << "Su kien: " << sk.mota << endl;
        cout << "Gio bat dau: " << sk.giobatdau << endl;
        cout << "Gio ket thuc: " << sk.gioketthuc << endl;
        cout << "Do uu tien: " << sk.douutien << endl;
        cout << "---------------------------\n";
        coSuKien = true;
    }

    if (!coSuKien) {
        cout << "Khong co su kien nao trong thu nay.\n";
    }
}

};

int main() {
	
    quanlysukien qlsukien;
    qlsukien.nhapthoikhoabieu();  // Nhập thời khóa biểu ban đầu

    while (true) {
        cout << "\n=== Quan Ly Su Kien ===\n";
        cout << "1. Them su kien\n";
        cout << "2. Hien thi su kien\n";
        cout << "3. Thoat\n";
        cout << "Nhap lua chon: ";
        int chon;
        cin >> chon;

        if (chon == 1) {
            qlsukien.themsukien();
        } else if (chon == 2) {
            qlsukien.hienthisukien();
        } else if (chon == 3) {
            break;
        } else {
            cout << "Lua chon khong hop le.\n";
        }
    }

    return 0;
}
