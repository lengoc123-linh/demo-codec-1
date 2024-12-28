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
        return nut ? nut->chieucao : 0;}
    int hesolech(node* nut) {
        return chieucao(nut->trai) - chieucao(nut->phai);}
    void capnhatchieucao(node* nut) {
        nut->chieucao = max(chieucao(nut->trai), chieucao(nut->phai)) + 1;}

    node* quayphai(node* y) {
        node* x = y->trai; node* t2 = x->phai;
        x->phai = y; y->trai = t2;
        capnhatchieucao(y); capnhatchieucao(x);
        return x;}

    node* quaytrai(node* x) {
        node* y = x->phai;
        node* t2 = y->trai;
        y->trai = x; x->phai = t2;
        capnhatchieucao(x); capnhatchieucao(y);
        return y;}

    node* canbang(node* nut) {
        int hl = hesolech(nut);
        if (hl > 1) { if (hesolech(nut->trai) < 0) {
                nut->trai = quaytrai(nut->trai);}
        return quayphai(nut);}
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
            nut->phai = them(nut->phai, sk);}
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
    void timsukientheongay(priority_queue<sukien>& pq) {
        duyet(goc, pq);
    }
};

class hangdoi {
private:
    queue<sukien> q;

public:
    void them(const sukien& sk) {
        q.push(sk);
    }

    bool rong() {
        return q.empty();
    }

    sukien lay() {
        if (rong()) {
            throw runtime_error("Hang doi rong");
        }
        sukien sk = q.front();
        q.pop();
        return sk;
    }

    void hien() {
        queue<sukien> tam = q;
        while (!tam.empty()) {
            cout << tam.front().mota << " - " << tam.front().ngay << " ("
                 << tam.front().giobatdau << " -> " << tam.front().gioketthuc << ")\n";
            tam.pop();
        }
    }
};

class thoikhoabieu {
public:
    vector<vector<sukien>> tkb;

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

            tkb[i].emplace_back(sukien(0, mota, ngay, giobatdau, gioketthuc));
        }
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

class quanlysukien {
private:
    cay_avl cay;
    hangdoi sukientrunglap;
    thoikhoabieu tkb;

public:
    void nhapthoikhoabieu() {
        tkb.nhap();
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
    for (auto& sk : tkb.tkb[thu - 1]) {
        int bd1 = stoi(sk.giobatdau.substr(0, 2)) * 60 + stoi(sk.giobatdau.substr(3, 2));
        int bd2 = stoi(sukienmoi.giobatdau.substr(0, 2)) * 60 + stoi(sukienmoi.giobatdau.substr(3, 2));
        int kt1 = stoi(sk.gioketthuc.substr(0, 2)) * 60 + stoi(sk.gioketthuc.substr(3, 2));
        int kt2 = stoi(sukienmoi.gioketthuc.substr(0, 2)) * 60 + stoi(sukienmoi.gioketthuc.substr(3, 2));

        if (!(kt1 <= bd2 || bd1 >= kt2)) {
            cout << "Su kien moi (" << sukienmoi.mota << ") trung voi su kien: " << sk.mota << "\n";
            cout << "Thoi gian su kien moi: " << sukienmoi.giobatdau << " -> " << sukienmoi.gioketthuc << "\n";
            cout << "Thoi gian su kien trung lap: " << sk.giobatdau << " -> " << sk.gioketthuc << "\n";

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

    if (!trungLap) {
        // Thêm sự kiện mới vào thời khóa biểu nếu không trùng
        tkb.tkb[thu - 1].push_back(sukienmoi);
        cay.them(sukienmoi);
    } else {
        // So sánh và thêm sự kiện mới hoặc giữ nguyên sự kiện cũ theo độ ưu tiên
        cout << "Xet do uu tien...\n";
        if (sukienmoi.douutien > tkb.tkb[thu - 1].back().douutien) {
            cout << "Them su kien moi vao thoi khoa bieu.\n";
            tkb.tkb[thu - 1].push_back(sukienmoi);
            cay.them(sukienmoi);
        } else {
            cout << "Su kien moi co do uu tien thap hon, khong duoc them vao thoi khoa bieu.\n";
        }
    }
}

 void hienthisukien() {
        int luachon;
        cout << "\n=== Hien Thi Su Kien ===\n";
        cout << "Nhap thu (1: Thu 2, 2: Thu 3, ..., 6: Thu 7): ";
        cin >> luachon;
        cin.ignore();

        if (luachon < 1 || luachon > 6) {
            cout << "Thu khong hop le.\n";
            return;
        }

        priority_queue<sukien> pq;

        for (auto& sk : tkb.tkb[luachon - 1]) {
            pq.push(sk);
        }

        cout << "Danh sach su kien trong thoi khoa bieu thu " << luachon + 1 << ":\n";
        while (!pq.empty()) {
            sukien sk = pq.top();
            cout << sk.mota << " - " << sk.giobatdau << " -> " << sk.gioketthuc;
            if (sk.douutien != 0) {
                cout << " - Do uu tien: " << sk.douutien;
            }
            cout << "\n";
            pq.pop();
        }
    }

};
int main() {
    quanlysukien qlsukien;
    qlsukien.nhapthoikhoabieu();

    while (true) {
        cout << "\n=== Quan Ly Su Kien ===\n";
        cout << "1. Them su kien\n";
        cout << "2. Hien thi su kien\n";
        cout << "3. Thoat\n";
        cout << "Nhap lua chon: ";
        int chon;
        cin >> chon;
        cin.ignore();

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
