# Hệ Thống Quản Lý Thư Viện (C++)

Chương trình quản lý thư viện viết bằng C++ thuần, sử dụng danh sách liên kết đơn, không phụ thuộc thư viện ngoài.

---

## Cấu Trúc Dự Án

```
KTLT_QUAN_LY_THU_VIEN
├── include/
│   ├── sach.h            # Cấu trúc Sach, ListSach, khai báo hàm
│   ├── sinhvien.h        # Cấu trúc BanDoc, ListBanDoc, khai báo hàm
│   ├── muontra.h         # Cấu trúc PhieuMuon, ListPhieu, khai báo hàm
|   ├── menu.h            # Khai báo các hàm menu
│   └── file_io.h         # Khai báo các hàm Đọc/Ghi file (.txt)   
|    
├── src/
│   ├── main.cpp          # Điểm vào chương trình
│   ├── sach.cpp          # Triển khai quản lý sách
│   ├── sinhvien.cpp      # Triển khai quản lý bạn đọc
│   ├── muontra.cpp       # Triển khai mượn/trả & thống kê
|   ├── menu.cpp          # Giao diện menu console
│   └── file_io.cpp       # Cài đặt logic
├── data/            
│   ├── sach.txt          # Lưu trữ vĩnh viễn danh sách các đầu sách
│   ├── sinhvien.txt      # Lưu trữ vĩnh viễn danh sách thông tin bạn đọc
│   └── muontra.txt       # Lưu trữ vĩnh viễn lịch sử và trạng thái phiếu mượn/trả
└── README.md
```

---

## Tính Năng

### Quản Lý Sách
- Thêm, xóa, sửa thông tin sách
- Tìm kiếm theo mã sách (chính xác) hoặc tên sách (từ khóa, không phân biệt hoa/thường)
- Sắp xếp danh sách theo tên (Selection Sort)
- Tự động cập nhật số lượng tồn kho

### Quản Lý Bạn Đọc
- Thêm, xóa, sửa thông tin bạn đọc
- Hỗ trợ hai loại: Sinh viên và Giảng viên
- Tìm kiếm theo mã hoặc tên
- Chặn xóa bạn đọc đang có sách mượn chưa trả

### Quản Lý Mượn / Trả
- Lập phiếu mượn với kiểm tra đầy đủ điều kiện
- Xử lý trả sách, tính tiền phạt nếu quá hạn
- Báo cáo sách đang được mượn
- Báo cáo sách quá hạn kèm số tiền phạt
- Thống kê Top 5 sách được mượn nhiều nhất

---

## Quy Tắc Nghiệp Vụ

| Quy tắc | Chi tiết |
|---|---|
| Hạn mức mượn - Sinh viên | Tối đa **3 cuốn** cùng lúc |
| Hạn mức mượn - Giảng viên | Tối đa **5 cuốn** cùng lúc |
| Chặn mượn mới | Bạn đọc có sách **quá hạn** chưa trả không được mượn thêm |
| Tiền phạt | **5.000 VND / ngày** quá hạn |
| Xóa bạn đọc | Không thể xóa nếu đang có phiếu mượn chưa đóng |
| Tồn kho | Được đồng bộ tự động sau mỗi thao tác mượn/trả |

> Các hằng số `HAN_MUC_SV`, `HAN_MUC_GV`, `TIEN_PHAT_NGAY` được định nghĩa trong `muontra.h` và có thể chỉnh sửa dễ dàng.

---

## Cài Đặt & Biên Dịch

**Yêu cầu hệ thống:** Trình biên dịch GCC/G++ hỗ trợ chuẩn C++11 trở lên.

Mở Terminal trong thư mục gốc của dự án (`KTLT_QUAN_LY_THU_VIEN`) và thực hiện tuần tự 2 bước sau:

**Bước 1: Biên dịch chương trình (Compile)**

```bash
g++ -std=c++11 -Wall src/main.cpp src/sach.cpp src/sinhvien.cpp src/muontra.cpp src/menu.cpp src/file_io.cpp -I include -o QuanLyThuVien
```

**Bước 2: Khởi chạy chương trình (Run)**

* Đối với Windows (PowerShell/CMD):

```bash
.\QuanLyThuVien.exe
```

* Đối với Linux / macOS:

```bash
./QuanLyThuVien
```

---

## Hướng Dẫn Sử Dụng

Chương trình chạy hoàn toàn trên giao diện dòng lệnh (console). Sau khi khởi động, menu chính hiện ra:

```
==================================================
     HE THONG QUAN LY THU VIEN
==================================================
  1. Quan ly Sach
  2. Quan ly Ban doc
  3. Quan ly Muon / Tra
  0. Thoat
```

Chọn mục tương ứng bằng cách nhập số và nhấn **Enter**.

**Định dạng ngày:** `YYYY-MM-DD` (ví dụ: `2025-06-15`)

---

## Chi Tiết Kỹ Thuật

- **Cấu trúc dữ liệu:** Danh sách liên kết đơn (`singly linked list`) cho cả ba thực thể Sách, Bạn Đọc, Phiếu Mượn.
- **Quản lý bộ nhớ:** Cấp phát động bằng `new`/`delete`; toàn bộ được giải phóng khi thoát chương trình.
- **Tìm kiếm chuỗi:** Tự cài đặt thuật toán tìm chuỗi con (không dùng `string::find`), hỗ trợ không phân biệt hoa/thường qua `tolower()`.
- **Tính ngày:** Chuyển đổi ngày `YYYY-MM-DD` thành số nguyên để so sánh và tính khoảng cách.
- **Tồn kho:** Sau mỗi thao tác mượn/trả, hàm `capNhatSoLuongTon()` duyệt lại toàn bộ phiếu để đảm bảo dữ liệu nhất quán.

---

