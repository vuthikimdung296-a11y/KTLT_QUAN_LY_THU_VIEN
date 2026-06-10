#include "../include/muontra.h"
#include <iostream>
#include <sstream>
using namespace std;

// ================================================================
//  KHỞI TẠO / GIẢI PHÓNG
// ================================================================

void khoiTaoListPhieu(ListPhieu &l) {
    l.head = l.tail = NULL;
}

void giaiPhongListPhieu(ListPhieu &l) {
    NodePhieu* p = l.head;
    while (p != NULL) {
        NodePhieu* temp = p;
        p = p->next;
        delete temp;
    }
    l.head = l.tail = NULL;
}

// ================================================================
//  TIỆN ÍCH THỜI GIAN  (format: YYYY-MM-DD)
// ================================================================

static int chuyenNgayThanhSo(string ngay) {
    if (ngay.length() < 10) return 0;
    int y, m, d;
    char c1, c2;
    stringstream ss(ngay);
    ss >> y >> c1 >> m >> c2 >> d;
    if (m < 3) { y--; m += 12; }
    return 365*y + y/4 - y/100 + y/400 + (153*m + 2)/5 + d;
}

int tinhKhoangCachNgay(string ngayTruoc, string ngaySau) {
    return chuyenNgayThanhSo(ngaySau) - chuyenNgayThanhSo(ngayTruoc);
}

// ================================================================
//  ĐỒNG BỘ TỒN KHO
// ================================================================

void capNhatSoLuongTon(ListSach &ls, ListPhieu &lp) {
    // Bước 1: Reset toàn bộ về tổng nhập kho
    NodeSach* ps = ls.head;
    while (ps != NULL) {
        ps->data.soLuongThucTe = ps->data.soLuongTong;
        ps = ps->next;
    }
    // Bước 2: Trừ mỗi cuốn đang bị mượn hoặc quá hạn
    NodePhieu* pp = lp.head;
    while (pp != NULL) {
        if (pp->data.trangThai == 0 || pp->data.trangThai == 2) {
            NodeSach* s = timSachTheoMa(ls, pp->data.maSach);
            if (s != NULL) {
                if (s->data.soLuongThucTe > 0) s->data.soLuongThucTe--;
                else s->data.soLuongThucTe = 0; // Chốt tại 0, không cho âm
            }
        }
        pp = pp->next;
    }
}

// ================================================================
//  HÀM PHỤ TRỢ NỘI BỘ
// ================================================================

static NodePhieu* taoNodePhieu(PhieuMuon pm) {
    NodePhieu* node = new NodePhieu;
    if (node == NULL) return NULL;
    node->data = pm;
    node->next = NULL;
    return node;
}

static int demSoSachDangMuon(ListPhieu &lp, string maSV) {
    int count = 0;
    NodePhieu* p = lp.head;
    while (p != NULL) {
        if (p->data.maSV == maSV &&
           (p->data.trangThai == 0 || p->data.trangThai == 2))
            count++;
        p = p->next;
    }
    return count;
}

// ================================================================
//  MƯỢN / TRẢ
// ================================================================

NodePhieu* timPhieuTheoMa(ListPhieu &l, string maCanTim) {
    NodePhieu* p = l.head;
    while (p != NULL) {
        if (p->data.maPhieu == maCanTim) return p;
        p = p->next;
    }
    return NULL;
}

bool lapPhieuMuon(ListPhieu &lp, ListBanDoc &lb,
                  ListSach  &ls, PhieuMuon p) {

    // Bước 1: Kiểm tra trùng mã phiếu
    if (timPhieuTheoMa(lp, p.maPhieu) != NULL) {
        cout << "Loi: Ma phieu [" << p.maPhieu << "] da ton tai!\n";
        return false;
    }
    // Bước 2: Kiểm tra bạn đọc tồn tại
    NodeBanDoc* bd = timBanDocTheoMa(lb, p.maSV);
    if (bd == NULL) {
        cout << "Loi: Khong tim thay ban doc [" << p.maSV << "]!\n";
        return false;
    }
    // Bước 3: Chặn mượn nếu đang nợ quá hạn
    if (banDocCoNoQuaHan(lp, p.maSV)) {
        cout << "Loi: Ban doc [" << p.maSV
             << "] dang co sach QUA HAN chua tra!\n";
        return false;
    }
    // Bước 4: Kiểm tra hạn mức số lượng
    int soHienTai = demSoSachDangMuon(lp, p.maSV);
    if (bd->data.loaiBanDoc == 1 && soHienTai >= HAN_MUC_SV) {
        cout << "Loi: Sinh vien da muon toi da " << HAN_MUC_SV << " cuon!\n";
        return false;
    }
    if (bd->data.loaiBanDoc == 2 && soHienTai >= HAN_MUC_GV) {
        cout << "Loi: Giang vien da muon toi da " << HAN_MUC_GV << " cuon!\n";
        return false;
    }
    // Bước 5: Kiểm tra sách còn trên kệ
    NodeSach* s = timSachTheoMa(ls, p.maSach);
    if (s == NULL || s->data.soLuongThucTe <= 0) {
        cout << "Loi: Sach [" << p.maSach << "] khong co san tren ke!\n";
        return false;
    }
    // Bước 6: Tạo phiếu và thêm vào danh sách
    p.trangThai = 0;
    p.ngayTra   = "";

    NodePhieu* nodeMoi = taoNodePhieu(p);
    if (nodeMoi == NULL) return false;

    if (lp.head == NULL) { lp.head = lp.tail = nodeMoi; }
    else { lp.tail->next = nodeMoi; lp.tail = nodeMoi; }

    capNhatSoLuongTon(ls, lp);
    cout << "Lap phieu [" << p.maPhieu << "] thanh cong!"
         << " Han tra: " << p.ngayHanTra << "\n";
    return true;
}

bool traSach(ListPhieu &lp, ListSach &ls,
             string maPhieu, string ngayTraThucTe) {

    NodePhieu* p = timPhieuTheoMa(lp, maPhieu);
    if (p == NULL) {
        cout << "Loi: Khong tim thay phieu [" << maPhieu << "]!\n";
        return false;
    }
    if (p->data.trangThai == 1) {
        cout << "Loi: Phieu [" << maPhieu << "] da duoc tra roi!\n";
        return false;
    }

    p->data.ngayTra = ngayTraThucTe;

    int lechNgay = tinhKhoangCachNgay(p->data.ngayHanTra, ngayTraThucTe);
    if (lechNgay > 0) {
        long long tienPhat = (long long)lechNgay * TIEN_PHAT_NGAY;
        cout << "--- Phieu [" << maPhieu << "] QUA HAN "
             << lechNgay << " ngay! ---\n";
        cout << "--- Tien phat: " << tienPhat << " VND ---\n";
    } else {
        cout << "--- Tra sach dung han thanh cong! ---\n";
    }
    // Luôn cập nhật = 1 (Đã trả) dù quá hạn hay đúng hạn
    // để capNhatSoLuongTon cộng lại sách lên kệ đúng
    p->data.trangThai = 1;

    capNhatSoLuongTon(ls, lp);
    return true;
}

// ================================================================
//  THỐNG KÊ & BÁO CÁO
// ================================================================

int thongKeSoSachDangMuon(ListPhieu &lp) {
    int count = 0;
    NodePhieu* p = lp.head;
    while (p != NULL) {
        if (p->data.trangThai == 0 || p->data.trangThai == 2) count++;
        p = p->next;
    }
    return count;
}

int thongKeSoSachQuaHan(ListPhieu &lp) {
    int count = 0;
    NodePhieu* p = lp.head;
    while (p != NULL) {
        if (p->data.trangThai == 2) count++;
        p = p->next;
    }
    return count;
}

void inDanhSachSachDangMuon(ListPhieu &lp, ListSach &ls, ListBanDoc &lb) {
    cout << "==================================================\n";
    cout << "        DANH SACH SACH DANG DUOC MUON             \n";
    cout << "==================================================\n";
    bool coPhieu = false;
    NodePhieu* p = lp.head;
    while (p != NULL) {
        if (p->data.trangThai == 0 || p->data.trangThai == 2) {
            NodeSach*   s  = timSachTheoMa(ls, p->data.maSach);
            NodeBanDoc* bd = timBanDocTheoMa(lb, p->data.maSV);
            cout << "  Phieu  : " << p->data.maPhieu << "\n";
            cout << "  Sach   : "
                 << (s  ? s->data.tenSach    : p->data.maSach) << "\n";
            cout << "  Ban doc: "
                 << (bd ? bd->data.tenBanDoc : p->data.maSV)
                 << " (" << p->data.maSV << ")\n";
            cout << "  Muon   : " << p->data.ngayMuon
                 << " | Han tra: " << p->data.ngayHanTra;
            if (p->data.trangThai == 2) cout << "  [!] QUA HAN";
            cout << "\n  ------------------------------------------\n";
            coPhieu = true;
        }
        p = p->next;
    }
    if (!coPhieu) cout << "  Hien tai khong co sach nao dang duoc muon.\n";
    cout << "==================================================\n";
}

void inDanhSachSachQuaHan(ListPhieu &lp, ListSach &ls,
                          ListBanDoc &lb, string ngayHomNay) {
    cout << "==================================================\n";
    cout << "            DANH SACH SACH QUA HAN                \n";
    cout << "==================================================\n";
    bool coPhieu = false;
    NodePhieu* p = lp.head;
    while (p != NULL) {
        if (p->data.trangThai == 2) {
            NodeSach*   s  = timSachTheoMa(ls, p->data.maSach);
            NodeBanDoc* bd = timBanDocTheoMa(lb, p->data.maSV);
            int ngayQH = tinhKhoangCachNgay(p->data.ngayHanTra, ngayHomNay);
            long long phat = (ngayQH > 0)
                           ? (long long)ngayQH * TIEN_PHAT_NGAY : 0;
            cout << "  Phieu   : " << p->data.maPhieu << "\n";
            cout << "  Sach    : "
                 << (s  ? s->data.tenSach    : p->data.maSach) << "\n";
            cout << "  Ban doc : "
                 << (bd ? bd->data.tenBanDoc : p->data.maSV)
                 << " (" << p->data.maSV << ")\n";
            cout << "  Han tra : " << p->data.ngayHanTra
                 << " | Qua han: " << ngayQH << " ngay\n";
            cout << "  Tien phat: " << phat << " VND\n";
            cout << "  ------------------------------------------\n";
            coPhieu = true;
        }
        p = p->next;
    }
    if (!coPhieu) cout << "  Khong co sach nao qua han.\n";
    cout << "==================================================\n";
}

struct KeSachThongKe {
    string maSach;
    string tenSach;
    int    soLuotMuon;
};

void timTop5SachMuonNhieuNhat(ListPhieu &lp, ListSach &ls) {
    int n = 0;
    NodeSach* ps = ls.head;
    while (ps != NULL) { n++; ps = ps->next; }

    if (n == 0) { cout << "Thu vien chua co sach nao.\n"; return; }

    // Mảng động - tự cài, không dùng vector
    KeSachThongKe* mangTk = new KeSachThongKe[n];
    ps = ls.head;
    for (int i = 0; i < n; i++) {
        mangTk[i].maSach     = ps->data.maSach;
        mangTk[i].tenSach    = ps->data.tenSach;
        mangTk[i].soLuotMuon = 0;
        ps = ps->next;
    }

    NodePhieu* pp = lp.head;
    while (pp != NULL) {
        for (int i = 0; i < n; i++) {
            if (mangTk[i].maSach == pp->data.maSach) {
                mangTk[i].soLuotMuon++; break;
            }
        }
        pp = pp->next;
    }

    // Selection Sort giảm dần
    for (int i = 0; i < n - 1; i++) {
        int maxIdx = i;
        for (int j = i + 1; j < n; j++)
            if (mangTk[j].soLuotMuon > mangTk[maxIdx].soLuotMuon) maxIdx = j;
        if (maxIdx != i) {
            KeSachThongKe tmp = mangTk[i];
            mangTk[i] = mangTk[maxIdx];
            mangTk[maxIdx] = tmp;
        }
    }

    cout << "==================================================\n";
    cout << "      TOP 5 CUON SACH DUOC MUON NHIEU NHAT        \n";
    cout << "==================================================\n";
    int top = (n < 5) ? n : 5;
    int viTri = 1;
    for (int i = 0; i < top; i++) {
        if (mangTk[i].soLuotMuon > 0) {
            cout << "  " << viTri++ << ". " << mangTk[i].tenSach
                 << " [" << mangTk[i].maSach << "]"
                 << " - So luot muon: " << mangTk[i].soLuotMuon << "\n";
        }
    }
    if (viTri == 1) cout << "  Chua co luot muon nao.\n";
    cout << "==================================================\n";

    delete[] mangTk;
}
