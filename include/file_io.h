#ifndef FILE_IO_H
#define FILE_IO_H

#include "muontra.h"
#include <string>

// ================================================================
//  KHAI BÁO CÁC HÀM ĐỌC FILE
// ================================================================
void docFileSach(ListSach &ls, const std::string &filename = "data/sach.txt");
void docFileBanDoc(ListBanDoc &lb, const std::string &filename = "data/sinhvien.txt");
void docFilePhieuMuon(ListPhieu &lp, const std::string &filename = "data/muontra.txt");

// ================================================================
//  KHAI BÁO CÁC HÀM GHI FILE
// ================================================================
void ghiFileSach(ListSach &ls, const std::string &filename = "data/sach.txt");
void ghiFileBanDoc(ListBanDoc &lb, const std::string &filename = "data/sinhvien.txt");
void ghiFilePhieuMuon(ListPhieu &lp, const std::string &filename = "data/muontra.txt");

// ================================================================
//  KHAI BÁO CÁC HÀM ĐỒNG BỘ TOÀN BỘ HỆ THỐNG
// ================================================================
void docToanBoFile(ListSach &ls, ListBanDoc &lb, ListPhieu &lp);
void ghiToanBoFile(ListSach &ls, ListBanDoc &lb, ListPhieu &lp);

#endif // FILE_IO_H
