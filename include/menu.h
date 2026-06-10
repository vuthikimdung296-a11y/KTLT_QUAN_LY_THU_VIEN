#ifndef MENU_H
#define MENU_H

#include "muontra.h"   // include day du tat ca: sach + sinhvien + muontra

// ================================================================
//  KHAI BÁO HÀM MENU
// ================================================================

void menuSach    (ListSach   &ls, ListPhieu &lp);
void menuSinhVien(ListBanDoc &lb, ListPhieu &lp);
void menuMuonTra (ListPhieu  &lp, ListSach  &ls, ListBanDoc &lb);
void menuChinh   (ListSach   &ls, ListBanDoc &lb, ListPhieu  &lp);

#endif // MENU_H
