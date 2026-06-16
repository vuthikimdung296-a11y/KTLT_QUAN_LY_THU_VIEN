#include "../include/menu.h"
#include <iostream>
using namespace std;

// ================================================================
//  KIỂM TRA ĐỊNH DẠNG NGÀY (YYYY-MM-DD)
// ================================================================
bool kiemTraFormatNgay(const string &ngay) {
    if (ngay.length() != 10) return false;
    if (ngay[4] != '-' || ngay[7] != '-') return false;
    for (int i = 0; i < 10; i++)
        if (i != 4 && i != 7 && !isdigit(ngay[i])) return false;
    return true;
}

// ================================================================
//  TIỆN ÍCH NHẬP LIỆU
// ================================================================

static int docSoNguyen(const char* prompt, int min, int max) {
    int val;
    while (true) {
        cout << prompt;
        if (cin >> val && val >= min && val <= max) {
            cin.ignore();
            return val;
        }
        cout << "  [!] Vui long nhap so tu " << min << " den " << max << "!\n";
        cin.clear();
        while (cin.get() != '\n');
    }
}

static void docChuoi(const char* prompt, string &s) {
    cout << prompt;
    getline(cin, s);
}

static void inDongKe(char c = '-', int n = 50) {
    for (int i = 0; i < n; i++) putchar(c);
    putchar('\n');
}

// ================================================================
//  MENU QUẢN LÝ SÁCH
// ================================================================

void menuSach(ListSach &ls, ListPhieu &lp) {
    (void)lp;
    int chon;
    do {
        cout << "\n"; inDongKe('=');
        cout << "         QUAN LY SACH\n"; inDongKe('=');
        cout << "  1. Them sach moi\n";
        cout << "  2. Xoa sach\n";
        cout << "  3. Sua thong tin sach\n";
        cout << "  4. Tim kiem theo Ma sach\n";
        cout << "  5. Tim kiem theo Ten sach\n";
        cout << "  6. Sap xep & Hien thi danh sach\n";
        cout << "  0. Quay lai\n";
        inDongKe();
        chon = docSoNguyen("  Chon: ", 0, 6);

        if (chon == 1) {
            Sach s;
            cout << "\n-- THEM SACH MOI --\n";
            docChuoi("  Ma sach  : ", s.maSach);
            if (timSachTheoMa(ls, s.maSach)) {
                cout << "  [!] Ma sach da ton tai!\n"; continue;
            }
            docChuoi("  Ten sach : ", s.tenSach);
            docChuoi("  Tac gia  : ", s.tacGia);
            docChuoi("  The loai : ", s.theLoai);
            s.soLuongTong = docSoNguyen("  So luong : ", 1, 9999);
            themSach(ls, s)
                ? cout << "  [OK] Them sach thanh cong!\n"
                : cout << "  [!] Them sach that bai!\n";

        }  else if (chon == 2) {
            string ma;
            cout << "\n-- XOA SACH --\n";
            docChuoi("  Ma sach can xoa: ", ma);
            
            // Kiem tra rang buoc: Sach co dang duoc muon khong?
            bool dangMuon = false;
            NodePhieu* p = lp.head;
            while (p != NULL) {
                if (p->data.maSach == ma && (p->data.trangThai == 0 || p->data.trangThai == 2)) {
                    dangMuon = true; break;
                }
                p = p->next;
            }
            
            if (dangMuon) {
                cout << "  [!] Loi: Sach nay dang duoc muon, khong the xoa!\n";
            } else {
                xoaSach(ls, ma)
                    ? cout << "  [OK] Da xoa!\n"
                    : cout << "  [!] Khong tim thay ma sach!\n";
            }

        } else if (chon == 3) {
            cout << "\n-- SUA SACH (de trong + Enter = giu nguyen) --\n";
            string ma; docChuoi("  Ma sach can sua: ", ma);
            NodeSach* node = timSachTheoMa(ls, ma);
            if (!node) { cout << "  [!] Khong tim thay!\n"; continue; }
            Sach s = node->data;
            string tmp;
            cout << "  Ten moi  [" << s.tenSach << "]: "; getline(cin, tmp);
            if (!tmp.empty()) s.tenSach = tmp;
            cout << "  Tac gia  [" << s.tacGia  << "]: "; getline(cin, tmp);
            if (!tmp.empty()) s.tacGia = tmp;
            cout << "  The loai [" << s.theLoai << "]: "; getline(cin, tmp);
            if (!tmp.empty()) s.theLoai = tmp;
            s.soLuongTong = docSoNguyen("  So luong: ", 1, 9999);
            capNhatSach(ls, s)
                ? cout << "  [OK] Da cap nhat!\n"
                : cout << "  [!] Cap nhat that bai (so luong moi qua nho)!\n";

        } else if (chon == 4) {
            string ma; docChuoi("\n  Ma sach: ", ma);
            NodeSach* k = timSachTheoMa(ls, ma);
            if (k) cout << "  Tim thay: " << k->data.tenSach
                        << " | Ton: " << k->data.soLuongThucTe << "\n";
            else   cout << "  Khong tim thay!\n";

        } else if (chon == 5) {
            string kw; docChuoi("\n  Tu khoa ten: ", kw);
            bool found = false;
            NodeSach* cur = ls.head;
            while (cur) {
                if (kiemTraChuoiCon(cur->data.tenSach, kw)) {
                    cout << "  " << cur->data.maSach
                         << " | " << cur->data.tenSach
                         << " | Ton: " << cur->data.soLuongThucTe << "\n";
                    found = true;
                }
                cur = cur->next;
            }
            if (!found) cout << "  Khong tim thay!\n";

        } else if (chon == 6) {
            sapXepSachTheoTen(ls);
            cout << "\n-- DANH SACH SACH (da sap xep theo ten) --\n";
            inDongKe();
            NodeSach* cur = ls.head;
            if (!cur) { cout << "  (Danh sach trong)\n"; }
            while (cur) {
                cout << "  " << cur->data.maSach
                     << " | " << cur->data.tenSach
                     << " | " << cur->data.tacGia
                     << " | Tong: " << cur->data.soLuongTong
                     << " | Ton: "  << cur->data.soLuongThucTe << "\n";
                cur = cur->next;
            }
            inDongKe();
        }
    } while (chon != 0);
}

// ================================================================
//  MENU QUẢN LÝ SINH VIÊN / BẠN ĐỌC
// ================================================================

void menuSinhVien(ListBanDoc &lb, ListPhieu &lp) {
    int chon;
    do {
        cout << "\n"; inDongKe('=');
        cout << "         QUAN LY BAN DOC\n"; inDongKe('=');
        cout << "  1. Them ban doc moi\n";
        cout << "  2. Xoa ban doc\n";
        cout << "  3. Sua thong tin ban doc\n";
        cout << "  4. Tim kiem theo Ma\n";
        cout << "  5. Tim kiem theo Ten\n";
        cout << "  6. Hien thi tat ca ban doc\n";
        cout << "  0. Quay lai\n";
        inDongKe();
        chon = docSoNguyen("  Chon: ", 0, 6);

        if (chon == 1) {
            BanDoc bd;
            cout << "\n-- THEM BAN DOC MOI --\n";
            docChuoi("  Ma ban doc : ", bd.maSV);
            if (timBanDocTheoMa(lb, bd.maSV)) {
                cout << "  [!] Ma da ton tai!\n"; continue;
            }
            docChuoi("  Ho ten     : ", bd.tenBanDoc);
            bd.loaiBanDoc = docSoNguyen("  Loai (1=SV/2=GV): ", 1, 2);
            themBanDoc(lb, bd)
                ? cout << "  [OK] Them thanh cong!\n"
                : cout << "  [!] Them that bai!\n";

        } else if (chon == 2) {
            string ma; docChuoi("\n  Ma can xoa: ", ma);
            xoaBanDoc(lb, lp, ma)
                ? cout << "  [OK] Da xoa!\n"
                : cout << "  [!] Khong xoa duoc!\n";

        } else if (chon == 3) {
            string ma; docChuoi("\n  Ma can sua: ", ma);
            NodeBanDoc* node = timBanDocTheoMa(lb, ma);
            if (!node) { cout << "  [!] Khong tim thay!\n"; continue; }
            BanDoc bd = node->data;
            string tmp;
            cout << "  Ten moi [" << bd.tenBanDoc << "]: "; getline(cin, tmp);
            if (!tmp.empty()) bd.tenBanDoc = tmp;
            bd.loaiBanDoc = docSoNguyen("  Loai (1=SV/2=GV): ", 1, 2);
            capNhatBanDoc(lb, bd)
                ? cout << "  [OK] Da cap nhat!\n"
                : cout << "  [!] Cap nhat that bai!\n";

        } else if (chon == 4) {
            string ma; docChuoi("\n  Ma ban doc: ", ma);
            NodeBanDoc* k = timBanDocTheoMa(lb, ma);
            if (k) cout << "  " << k->data.maSV << " | " << k->data.tenBanDoc
                        << " | " << (k->data.loaiBanDoc == 1 ? "Sinh vien" : "Giang vien") << "\n";
            else   cout << "  Khong tim thay!\n";

        } else if (chon == 5) {
            string kw; docChuoi("\n  Tu khoa ten: ", kw);
            timBanDocTheoTen(lb, kw);

        } else if (chon == 6) {
            cout << "\n-- DANH SACH BAN DOC --\n"; inDongKe();
            NodeBanDoc* cur = lb.head;
            if (!cur) { cout << "  (Danh sach trong)\n"; }
            while (cur) {
                cout << "  " << cur->data.maSV
                     << " | " << cur->data.tenBanDoc
                     << " | " << (cur->data.loaiBanDoc == 1 ? "Sinh vien" : "Giang vien") << "\n";
                cur = cur->next;
            }
            inDongKe();
        }
    } while (chon != 0);
}

// ================================================================
//  MENU MƯỢN / TRẢ & BÁO CÁO
// ================================================================

void menuMuonTra(ListPhieu &lp, ListSach &ls, ListBanDoc &lb) {
    // Bat buoc nhap ngay hom nay de dong bo qua han
    string ngayHienTai;
    while (true) {
        cout << "\n[He thong] Nhap ngay hom nay (YYYY-MM-DD) de cap nhat du lieu: ";
        getline(cin, ngayHienTai);
        if (kiemTraFormatNgay(ngayHienTai)) break;
        cout << "  -> Loi: Sai dinh dang ngay!\n";
    }
    capNhatTrangThaiQuaHan(lp, ngayHienTai);
    capNhatSoLuongTon(ls, lp);
    int chon;
    do {
        cout << "\n"; inDongKe('=');
        cout << "         QUAN LY MUON / TRA SACH\n"; inDongKe('=');
        cout << "  1. Lap phieu muon\n";
        cout << "  2. Tra sach\n";
        cout << "  3. Xem tat ca phieu muon\n";
        cout << "  4. Bao cao: Sach dang muon\n";
        cout << "  5. Bao cao: Sach qua han\n";
        cout << "  6. Bao cao: Top 5 sach muon nhieu nhat\n";
        cout << "  0. Quay lai\n";
        inDongKe();
        chon = docSoNguyen("  Chon: ", 0, 6);

        if (chon == 1) {
            PhieuMuon p;
            cout << "\n-- LAP PHIEU MUON --\n";
            docChuoi("  Ma phieu   : ", p.maPhieu);
            docChuoi("  Ma ban doc : ", p.maSV);
            docChuoi("  Ma sach    : ", p.maSach);
            docChuoi("  Ngay muon  : ", p.ngayMuon);
            docChuoi("  Han tra    : ", p.ngayHanTra);
            lapPhieuMuon(lp, lb, ls, p);

        } else if (chon == 2) {
            string maPhieu, ngayTra;
            cout << "\n-- TRA SACH --\n";
            docChuoi("  Ma phieu  : ", maPhieu);
            docChuoi("  Ngay tra  : ", ngayTra);
            traSach(lp, ls, maPhieu, ngayTra);

        } else if (chon == 3) {
            cout << "\n-- TAT CA PHIEU MUON --\n"; inDongKe();
            NodePhieu* cur = lp.head;
            if (!cur) { cout << "  (Chua co phieu nao)\n"; }
            while (cur) {
                cout << "  " << cur->data.maPhieu
                     << " | " << cur->data.maSV
                     << " | " << cur->data.maSach
                     << " | " << cur->data.ngayMuon
                     << " -> " << cur->data.ngayHanTra
                     << " | ";
                switch (cur->data.trangThai) {
                    case 0: cout << "Dang muon"; break;
                    case 1: cout << "Da tra";    break;
                    case 2: cout << "Qua han";   break;
                }
                cout << "\n";
                cur = cur->next;
            }
            inDongKe();

        } else if (chon == 4) {
            inDanhSachSachDangMuon(lp, ls, lb);

        } else if (chon == 5) {
            string ngayHomNay;
            docChuoi("  Nhap ngay hom nay (YYYY-MM-DD): ", ngayHomNay);
            inDanhSachSachQuaHan(lp, ls, lb, ngayHomNay);

        } else if (chon == 6) {
            timTop5SachMuonNhieuNhat(lp, ls);
        }
    } while (chon != 0);
}

// ================================================================
//  MENU CHÍNH
// ================================================================

void menuChinh(ListSach &ls, ListBanDoc &lb, ListPhieu &lp) {
    int chon;
    do {
        cout << "\n"; inDongKe('=', 50);
        cout << "     HE THONG QUAN LY THU VIEN\n";
        inDongKe('=', 50);
        cout << "  1. Quan ly Sach\n";
        cout << "  2. Quan ly Ban doc\n";
        cout << "  3. Quan ly Muon / Tra\n";
        cout << "  0. Thoat\n";
        inDongKe('-', 50);
        chon = docSoNguyen("  Chon: ", 0, 3);

        if      (chon == 1) menuSach    (ls, lp);
        else if (chon == 2) menuSinhVien(lb, lp);
        else if (chon == 3) menuMuonTra (lp, ls, lb);

    } while (chon != 0);
    cout << "\n  Tam biet!\n";
}
