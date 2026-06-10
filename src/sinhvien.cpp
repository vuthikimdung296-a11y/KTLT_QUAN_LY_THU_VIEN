#include "../include/sinhvien.h"
#include "../include/muontra.h"  // Can ListPhieu (dinh nghia day du)
#include <iostream>
using namespace std;

// ================================================================
//  KHỞI TẠO / GIẢI PHÓNG
// ================================================================

void khoiTaoListBanDoc(ListBanDoc &l) {
    l.head = l.tail = NULL;
}

void giaiPhongListBanDoc(ListBanDoc &l) {
    NodeBanDoc* p = l.head;
    while (p != NULL) {
        NodeBanDoc* temp = p;
        p = p->next;
        delete temp;
    }
    l.head = l.tail = NULL;
}

// ================================================================
//  HÀM PHỤ TRỢ NỘI BỘ
// ================================================================

static NodeBanDoc* taoNodeBanDoc(BanDoc bd) {
    NodeBanDoc* p = new NodeBanDoc;
    if (p == NULL) return NULL;
    p->data = bd;
    p->next = NULL;
    return p;
}

// ================================================================
//  KIỂM TRA TRẠNG THÁI MƯỢN
// ================================================================

// Kiểm tra bạn đọc có phiếu CHƯA TRẢ (đang mượn hoặc quá hạn)
bool banDocDangMuonSach(ListPhieu &lp, string maSV) {
    NodePhieu* p = lp.head;
    while (p != NULL) {
        if (p->data.maSV == maSV &&
           (p->data.trangThai == 0 || p->data.trangThai == 2))
            return true;
        p = p->next;
    }
    return false;
}

// Kiểm tra bạn đọc có phiếu QUÁ HẠN chưa xử lý
bool banDocCoNoQuaHan(ListPhieu &lp, string maSV) {
    NodePhieu* p = lp.head;
    while (p != NULL) {
        if (p->data.maSV == maSV && p->data.trangThai == 2)
            return true;
        p = p->next;
    }
    return false;
}

// ================================================================
//  CRUD BẠN ĐỌC
// ================================================================

bool themBanDoc(ListBanDoc &l, BanDoc bd) {
    if (timBanDocTheoMa(l, bd.maSV) != NULL) return false; // Trùng mã
    NodeBanDoc* p = taoNodeBanDoc(bd);
    if (p == NULL) return false;
    if (l.head == NULL) { l.head = l.tail = p; }
    else { l.tail->next = p; l.tail = p; }
    return true;
}

bool xoaBanDoc(ListBanDoc &l, ListPhieu &lp, string maSVCanXoa) {
    if (l.head == NULL) return false;

    // Chặn xóa nếu đang mượn hoặc nợ quá hạn
    if (banDocDangMuonSach(lp, maSVCanXoa)) {
        cout << "Loi: Ban doc dang muon sach hoac co sach qua han!\n";
        return false;
    }

    // Xóa node đầu
    if (l.head->data.maSV == maSVCanXoa) {
        NodeBanDoc* p = l.head;
        l.head = l.head->next;
        if (l.head == NULL) l.tail = NULL;
        delete p;
        return true;
    }

    // Xóa node giữa hoặc cuối
    NodeBanDoc* p = l.head;
    while (p->next != NULL && p->next->data.maSV != maSVCanXoa)
        p = p->next;
    if (p->next != NULL) {
        NodeBanDoc* q = p->next;
        p->next = q->next;
        if (q == l.tail) l.tail = p;
        delete q;
        return true;
    }
    return false;
}

bool capNhatBanDoc(ListBanDoc &l, BanDoc bdMoi) {
    NodeBanDoc* p = timBanDocTheoMa(l, bdMoi.maSV);
    if (p == NULL) return false;
    p->data.tenBanDoc  = bdMoi.tenBanDoc;
    p->data.loaiBanDoc = bdMoi.loaiBanDoc;
    return true;
}

// ================================================================
//  THUẬT TOÁN BẠN ĐỌC
// ================================================================

NodeBanDoc* timBanDocTheoMa(ListBanDoc &l, string maCanTim) {
    NodeBanDoc* p = l.head;
    while (p != NULL) {
        if (p->data.maSV == maCanTim) return p;
        p = p->next;
    }
    return NULL;
}

void timBanDocTheoTen(ListBanDoc &l, string tuKhoa) {
    NodeBanDoc* p = l.head;
    bool timThay = false;
    cout << "--- Ket qua tim kiem theo tu khoa \""
         << tuKhoa << "\": ---\n";
    while (p != NULL) {
        if (kiemTraChuoiCon(p->data.tenBanDoc, tuKhoa)) {
            cout << "  MSSV: "      << p->data.maSV
                 << " | Ten: "      << p->data.tenBanDoc
                 << " | Loai: "
                 << (p->data.loaiBanDoc == 1 ? "Sinh vien" : "Giang vien")
                 << "\n";
            timThay = true;
        }
        p = p->next;
    }
    if (!timThay) cout << "  Khong tim thay ban doc nao khop.\n";
}
