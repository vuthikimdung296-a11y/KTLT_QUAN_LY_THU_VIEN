#include "../include/menu.h"
#include "../include/file_io.h"

int main() {
    ListSach   ls;
    ListBanDoc lb;
    ListPhieu  lp;

    khoiTaoListSach  (ls);
    khoiTaoListBanDoc(lb);
    khoiTaoListPhieu (lp);

    docToanBoFile(ls, lb, lp);

    menuChinh(ls, lb, lp);

    ghiToanBoFile(ls, lb, lp);

    // Giải phóng toàn bộ bộ nhớ trước khi thoát
    giaiPhongListSach  (ls);
    giaiPhongListBanDoc(lb);
    giaiPhongListPhieu (lp);

    return 0;
}
