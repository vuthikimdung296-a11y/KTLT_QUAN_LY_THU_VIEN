#ifndef SINHVIEN_H
#define SINHVIEN_H

#include "sach.h"   // Can ListSach de kiem tra khi xoa ban doc

// ================================================================
//  STRUCT THỰC THỂ: BẠN ĐỌC
// ================================================================
struct BanDoc {
    std::string maSV;
    std::string tenBanDoc;
    int loaiBanDoc; // 1: Sinh viên, 2: Giảng viên
};

// ================================================================
//  DANH SÁCH LIÊN KẾT BẠN ĐỌC
// ================================================================
struct NodeBanDoc {
    BanDoc      data;
    NodeBanDoc* next;
};
struct ListBanDoc {
    NodeBanDoc* head;
    NodeBanDoc* tail;
};

// Forward declare ListPhieu (dinh nghia day du trong muontra.h)
struct ListPhieu;

// ================================================================
//  KHAI BÁO HÀM
// ================================================================

// Khởi tạo / Giải phóng
void khoiTaoListBanDoc  (ListBanDoc &l);
void giaiPhongListBanDoc(ListBanDoc &l);

// CRUD
bool themBanDoc   (ListBanDoc &l, BanDoc bd);
bool xoaBanDoc    (ListBanDoc &l, ListPhieu &lp, std::string maSVCanXoa);
bool capNhatBanDoc(ListBanDoc &l, BanDoc bdMoi);

// Thuật toán
NodeBanDoc* timBanDocTheoMa (ListBanDoc &l, std::string maCanTim);
void        timBanDocTheoTen(ListBanDoc &l, std::string tuKhoa);

// Kiểm tra trạng thái mượn
bool banDocDangMuonSach(ListPhieu &lp, std::string maSV);
bool banDocCoNoQuaHan  (ListPhieu &lp, std::string maSV);

#endif // SINHVIEN_H
