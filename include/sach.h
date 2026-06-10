#ifndef SACH_H
#define SACH_H

#include <string>

// ================================================================
//  STRUCT THỰC THỂ: SÁCH
// ================================================================
struct Sach {
    std::string maSach;
    std::string tenSach;
    std::string tacGia;
    std::string theLoai;
    int soLuongTong;    // Tổng số sách thư viện sở hữu
    int soLuongThucTe;  // Số lượng còn lại trên kệ (chưa bị mượn)
};

// ================================================================
//  DANH SÁCH LIÊN KẾT SÁCH
// ================================================================
struct NodeSach {
    Sach      data;
    NodeSach* next;
};
struct ListSach {
    NodeSach* head;
    NodeSach* tail;
};

// ================================================================
//  KHAI BÁO HÀM
// ================================================================

// Khởi tạo / Giải phóng
void khoiTaoListSach  (ListSach &l);
void giaiPhongListSach(ListSach &l);

// CRUD
bool themSach   (ListSach &l, Sach s);
bool xoaSach    (ListSach &l, std::string maCanXoa);
bool capNhatSach(ListSach &l, Sach sMoi);

// Thuật toán
NodeSach* timSachTheoMa (ListSach &l, std::string maCanTim);
NodeSach* timSachTheoTen(ListSach &l, std::string tuKhoa);
void      sapXepSachTheoTen(ListSach &l);

// Tiện ích chuỗi (dùng chung toàn hệ thống)
bool kiemTraChuoiCon(std::string chuoiLon, std::string chuoiNho);

#endif // SACH_H
