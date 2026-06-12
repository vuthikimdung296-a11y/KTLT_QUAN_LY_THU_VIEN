#include "../include/file_io.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <windows.h>

// Helper: Cắt chuỗi theo ký tự phân tách
static std::vector<std::string> splitString(const std::string &s, char delim) {
    std::vector<std::string> tokens;
    std::stringstream ss(s);
    std::string token;
    while (std::getline(ss, token, delim)) {
        tokens.push_back(token);
    }
    return tokens;
}

// Helper: Trim whitespace ở đầu và cuối chuỗi
static std::string trimString(const std::string &str) {
    size_t first = str.find_first_not_of(" \t\r\n");
    if (first == std::string::npos) return "";
    size_t last = str.find_last_not_of(" \t\r\n");
    return str.substr(first, (last - first + 1));
}

// ================================================================
//  TRIỂN KHAI CÁC HÀM ĐỌC FILE
// ================================================================

void docFileSach(ListSach &ls, const std::string &filename) {
    std::ifstream f(filename);
    if (!f.is_open()) return;
    
    std::string line;
    while (std::getline(f, line)) {
        line = trimString(line);
        if (line.empty() || line[0] == '#') continue;
        
        std::vector<std::string> tokens = splitString(line, '|');
        if (tokens.size() < 6) continue;
        
        Sach s;
        s.maSach = trimString(tokens[0]);
        s.tenSach = trimString(tokens[1]);
        s.tacGia = trimString(tokens[2]);
        s.theLoai = trimString(tokens[3]);
        s.soLuongTong = std::stoi(trimString(tokens[4]));
        s.soLuongThucTe = std::stoi(trimString(tokens[5]));
        
        // Thêm trực tiếp vào danh sách liên kết (push back)
        NodeSach* p = new NodeSach;
        p->data = s;
        p->next = NULL;
        
        if (ls.head == NULL) {
            ls.head = ls.tail = p;
        } else {
            ls.tail->next = p;
            ls.tail = p;
        }
    }
    f.close();
}

void docFileBanDoc(ListBanDoc &lb, const std::string &filename) {
    std::ifstream f(filename);
    if (!f.is_open()) return;
    
    std::string line;
    while (std::getline(f, line)) {
        line = trimString(line);
        if (line.empty() || line[0] == '#') continue;
        
        std::vector<std::string> tokens = splitString(line, '|');
        if (tokens.size() < 3) continue;
        
        BanDoc bd;
        bd.maSV = trimString(tokens[0]);
        bd.tenBanDoc = trimString(tokens[1]);
        bd.loaiBanDoc = std::stoi(trimString(tokens[2]));
        
        // Thêm trực tiếp vào danh sách liên kết (push back)
        NodeBanDoc* p = new NodeBanDoc;
        p->data = bd;
        p->next = NULL;
        
        if (lb.head == NULL) {
            lb.head = lb.tail = p;
        } else {
            lb.tail->next = p;
            lb.tail = p;
        }
    }
    f.close();
}

void docFilePhieuMuon(ListPhieu &lp, const std::string &filename) {
    std::ifstream f(filename);
    if (!f.is_open()) return;
    
    std::string line;
    while (std::getline(f, line)) {
        line = trimString(line);
        if (line.empty() || line[0] == '#') continue;
        
        std::vector<std::string> tokens = splitString(line, '|');
        if (tokens.size() < 7) continue;
        
        PhieuMuon pm;
        pm.maPhieu = trimString(tokens[0]);
        pm.maSV = trimString(tokens[1]);
        pm.maSach = trimString(tokens[2]);
        pm.ngayMuon = trimString(tokens[3]);
        pm.ngayHanTra = trimString(tokens[4]);
        pm.ngayTra = trimString(tokens[5]);
        if (pm.ngayTra == "NULL" || pm.ngayTra == "null") {
            pm.ngayTra = "";
        }
        pm.trangThai = std::stoi(trimString(tokens[6]));
        
        // Thêm trực tiếp vào danh sách liên kết (push back)
        NodePhieu* p = new NodePhieu;
        p->data = pm;
        p->next = NULL;
        
        if (lp.head == NULL) {
            lp.head = lp.tail = p;
        } else {
            lp.tail->next = p;
            lp.tail = p;
        }
    }
    f.close();
}

// ================================================================
//  TRIỂN KHAI CÁC HÀM GHI FILE
// ================================================================

void ghiFileSach(ListSach &ls, const std::string &filename) {
    std::ofstream f(filename);
    if (!f.is_open()) return;
    
    NodeSach* cur = ls.head;
    while (cur != NULL) {
        f << cur->data.maSach << "|"
          << cur->data.tenSach << "|"
          << cur->data.tacGia << "|"
          << cur->data.theLoai << "|"
          << cur->data.soLuongTong << "|"
          << cur->data.soLuongThucTe << "\n";
        cur = cur->next;
    }
    f.close();
}

void ghiFileBanDoc(ListBanDoc &lb, const std::string &filename) {
    std::ofstream f(filename);
    if (!f.is_open()) return;
    
    NodeBanDoc* cur = lb.head;
    while (cur != NULL) {
        f << cur->data.maSV << "|"
          << cur->data.tenBanDoc << "|"
          << cur->data.loaiBanDoc << "\n";
        cur = cur->next;
    }
    f.close();
}

void ghiFilePhieuMuon(ListPhieu &lp, const std::string &filename) {
    std::ofstream f(filename);
    if (!f.is_open()) return;
    
    f << "# maPhieu|maSV|maSach|ngayMuon|ngayHanTra|ngayTra|trangThai\n";
    NodePhieu* cur = lp.head;
    while (cur != NULL) {
        std::string ngayTraVal = cur->data.ngayTra;
        if (ngayTraVal.empty()) ngayTraVal = "";
        
        f << cur->data.maPhieu << "|"
          << cur->data.maSV << "|"
          << cur->data.maSach << "|"
          << cur->data.ngayMuon << "|"
          << cur->data.ngayHanTra << "|"
          << ngayTraVal << "|"
          << cur->data.trangThai << "\n";
        cur = cur->next;
    }
    f.close();
}

// ================================================================
//  TRIỂN KHAI CÁC HÀM ĐỒNG BỘ TOÀN BỘ HỆ THỐNG
// ================================================================

void docToanBoFile(ListSach &ls, ListBanDoc &lb, ListPhieu &lp) {
    CreateDirectoryA("data", NULL);
    docFileSach(ls, "data/sach.txt");
    docFileBanDoc(lb, "data/sinhvien.txt");
    docFilePhieuMuon(lp, "data/muontra.txt");
    
    // Đồng bộ tồn kho ngay sau khi load
    capNhatSoLuongTon(ls, lp);
}

void ghiToanBoFile(ListSach &ls, ListBanDoc &lb, ListPhieu &lp) {
    CreateDirectoryA("data", NULL);
    ghiFileSach(ls, "data/sach.txt");
    ghiFileBanDoc(lb, "data/sinhvien.txt");
    ghiFilePhieuMuon(lp, "data/muontra.txt");
}
