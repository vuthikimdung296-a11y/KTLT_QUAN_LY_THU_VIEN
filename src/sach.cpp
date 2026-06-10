#include "../include/sach.h"
#include <cctype>   // tolower()
using namespace std;

// ================================================================
//  KHỞI TẠO / GIẢI PHÓNG
// ================================================================

void khoiTaoListSach(ListSach &l) {
    l.head = l.tail = NULL;
}

void giaiPhongListSach(ListSach &l) {
    NodeSach* p = l.head;
    while (p != NULL) {
        NodeSach* temp = p;
        p = p->next;
        delete temp;
    }
    l.head = l.tail = NULL;
}

// ================================================================
//  HÀM PHỤ TRỢ NỘI BỘ
// ================================================================

static NodeSach* taoNodeSach(Sach s) {
    NodeSach* p = new NodeSach;
    if (p == NULL) return NULL;
    p->data = s;
    p->data.soLuongThucTe = s.soLuongTong; // Ban đầu tồn = tổng nhập
    p->next = NULL;
    return p;
}

// ================================================================
//  TIỆN ÍCH CHUỖI - dùng chung toàn hệ thống
//  Tự cài đặt - không dùng string::find()
//  Hỗ trợ không phân biệt hoa/thường nhờ tolower()
// ================================================================
bool kiemTraChuoiCon(string chuoiLon, string chuoiNho) {
    int n = (int)chuoiLon.length();
    int m = (int)chuoiNho.length();
    if (m == 0) return true;
    if (m > n)  return false;
    for (int i = 0; i <= n - m; i++) {
        int j;
        for (j = 0; j < m; j++) {
            if (tolower(chuoiLon[i+j]) != tolower(chuoiNho[j])) break;
        }
        if (j == m) return true;
    }
    return false;
}

// ================================================================
//  CRUD SÁCH
// ================================================================

bool themSach(ListSach &l, Sach s) {
    if (timSachTheoMa(l, s.maSach) != NULL) return false; // Trùng mã
    NodeSach* p = taoNodeSach(s);
    if (p == NULL) return false;
    if (l.head == NULL) { l.head = l.tail = p; }
    else { l.tail->next = p; l.tail = p; }
    return true;
}

bool xoaSach(ListSach &l, string maCanXoa) {
    if (l.head == NULL) return false;

    // Xóa node đầu
    if (l.head->data.maSach == maCanXoa) {
        NodeSach* p = l.head;
        l.head = l.head->next;
        if (l.head == NULL) l.tail = NULL;
        delete p;
        return true;
    }

    // Xóa node giữa hoặc cuối
    NodeSach* p = l.head;
    while (p->next != NULL && p->next->data.maSach != maCanXoa)
        p = p->next;
    if (p->next != NULL) {
        NodeSach* q = p->next;
        p->next = q->next;
        if (q == l.tail) l.tail = p;
        delete q;
        return true;
    }
    return false;
}

bool capNhatSach(ListSach &l, Sach sMoi) {
    NodeSach* p = timSachTheoMa(l, sMoi.maSach);
    if (p == NULL) return false;

    // Tính chênh lệch để không làm sai số đang mượn
    int chenhLech = sMoi.soLuongTong - p->data.soLuongTong;
    if (p->data.soLuongThucTe + chenhLech < 0) return false;

    p->data.tenSach       = sMoi.tenSach;
    p->data.tacGia        = sMoi.tacGia;
    p->data.theLoai       = sMoi.theLoai;
    p->data.soLuongTong   = sMoi.soLuongTong;
    p->data.soLuongThucTe += chenhLech;
    return true;
}

// ================================================================
//  THUẬT TOÁN SÁCH
// ================================================================

// Tìm kiếm chính xác theo mã - O(n)
NodeSach* timSachTheoMa(ListSach &l, string maCanTim) {
    NodeSach* p = l.head;
    while (p != NULL) {
        if (p->data.maSach == maCanTim) return p;
        p = p->next;
    }
    return NULL;
}

// Tìm kiếm theo từ khóa tên - trả về kết quả đầu tiên khớp
NodeSach* timSachTheoTen(ListSach &l, string tuKhoa) {
    NodeSach* p = l.head;
    while (p != NULL) {
        if (kiemTraChuoiCon(p->data.tenSach, tuKhoa)) return p;
        p = p->next;
    }
    return NULL;
}

// Selection Sort theo tên - O(n²)
// Hoán đổi DATA, không hoán đổi con trỏ
void sapXepSachTheoTen(ListSach &l) {
    if (l.head == NULL || l.head->next == NULL) return;
    for (NodeSach* p = l.head; p != NULL; p = p->next) {
        NodeSach* minNode = p;
        for (NodeSach* q = p->next; q != NULL; q = q->next)
            if (q->data.tenSach < minNode->data.tenSach) minNode = q;
        if (minNode != p) {
            Sach temp     = p->data;
            p->data       = minNode->data;
            minNode->data = temp;
        }
    }
}
