#ifndef MUONTRA_H
#define MUONTRA_H

#include "sinhvien.h"  // Can BanDoc, ListBanDoc
                       // sinhvien.h da include sach.h

// ================================================================
//  HẰNG SỐ NGHIỆP VỤ
//  Quy ước format ngày: YYYY-MM-DD  (ví dụ: "2025-06-08")
// ================================================================
const int       HAN_MUC_SV     = 3;    // Sinh viên mượn tối đa 3 cuốn
const int       HAN_MUC_GV     = 5;    // Giảng viên mượn tối đa 5 cuốn
const long long TIEN_PHAT_NGAY = 2000; // Phạt 2000đ / ngày quá hạn

// ================================================================
//  STRUCT THỰC THỂ: PHIẾU MƯỢN
// ================================================================
struct PhieuMuon {
    std::string maPhieu;
    std::string maSV;
    std::string maSach;
    std::string ngayMuon;
    std::string ngayHanTra; // Hạn phải trả - format YYYY-MM-DD
    std::string ngayTra;    // Ngày trả thực tế (rỗng nếu chưa trả)
    int trangThai;          // 0: Đang mượn, 1: Đã trả, 2: Quá hạn
};

// ================================================================
//  DANH SÁCH LIÊN KẾT PHIẾU MƯỢN
// ================================================================
struct NodePhieu {
    PhieuMuon  data;
    NodePhieu* next;
};
struct ListPhieu {
    NodePhieu* head;
    NodePhieu* tail;
};

// ================================================================
//  KHAI BÁO HÀM
// ================================================================

// Khởi tạo / Giải phóng
void khoiTaoListPhieu  (ListPhieu &l);
void giaiPhongListPhieu(ListPhieu &l);

// Mượn / Trả
bool lapPhieuMuon(ListPhieu &lp, ListBanDoc &lb,
                  ListSach  &ls, PhieuMuon p);
bool traSach     (ListPhieu &lp, ListSach &ls,
                  std::string maPhieu, std::string ngayTraThucTe);

// Tìm kiếm
NodePhieu* timPhieuTheoMa(ListPhieu &l, std::string maCanTim);

// Đồng bộ tồn kho (gọi sau mượn/trả)
void capNhatSoLuongTon(ListSach &ls, ListPhieu &lp);

// Thống kê số lượng
int thongKeSoSachDangMuon(ListPhieu &lp);
int thongKeSoSachQuaHan  (ListPhieu &lp);

// Báo cáo chi tiết
void inDanhSachSachDangMuon  (ListPhieu &lp, ListSach &ls, ListBanDoc &lb);
void inDanhSachSachQuaHan    (ListPhieu &lp, ListSach &ls, ListBanDoc &lb,
                              std::string ngayHomNay);
void timTop5SachMuonNhieuNhat(ListPhieu &lp, ListSach &ls);

// Tiện ích thời gian
int tinhKhoangCachNgay(std::string ngayTruoc, std::string ngaySau);

#endif // MUONTRA_H
