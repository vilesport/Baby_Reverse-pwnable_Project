**Dynamic memory allocation functions (<stdlib.h>)**
---

**Notice**
    * Dynamic memory alloctation sẽ allocate vùng heap(big-endian), ngược với stack(little-endian)
---

**Malloc()**
* Synopsis:
	* '''ptr = (cast_type*)malloc(byte_size)'''
* Description:
    * Malloc là 1 hàm sẽ cấp phát byte_size byte liên tiếp, sau đó trả về còn trỏ kiểu void trỏ vào byte đầu tiên
    * Malloc sẽ trả về null nếu không thể cấp phát đủ byte_size byte
    * Malloc sẽ không init dữ liệu ở các byte, đồng nghĩa sau khi malloc các byte sẽ chứa dữ liệu bất kì
---

**Calloc()**
* Synopsis:
    * '''ptr = (cast_type*)calloc(num, size)'''
* Description:
    * Calloc có chức năng tương tự malloc nhưng có điểm khác biệt
        * Calloc khi alloc memory sẽ init các byte được alloc
            * Từ đó calloc sẽ chậm hơn malloc (nhưng không quá đáng kể)
            * Có thể hiểu calloc là malloc + memset
---

**Realloc()**
* Synopsis:
    * '''ptr = realloc(ptr, new size)'''
* Description:
    * Realloc sẽ thay đổi bộ nhớ cấp phát cho pointer hiện tại
        * Nếu kích thước mới nhỏ hơn kích thước ban đầu thì chỉ đơn giản là deallocate phần chênh lệch
        * Nếu kích thước mới lơn hơn kích thước ban đầu thì:
            * Nếu phần bộ nhớ liền sau có thể sử dụng thì sẽ mở rộng thêm
            * Nếu phần bộ nhớ liền sau không đủ thì sẽ malloc byte_size byte mới, copy data từ ptr cũ sang vùng được allocate, sau đó deallocate vùng nhớ cũ
---

**Free()**
* Synopsis:
    * '''free(ptr)'''
* Description:
    * Free sẽ xoá ptr được alloc bằng các hàm alloc khác, đồng thời đánh dấu các byte đã alloc thành free để sử dụng cho các lần alloc sau
    * Free chỉ xoá ptr và đánh dấu memory block là free chứ không xoá dữ liệu trong memory block, do đó vẫn có thể truy cập dữ liệu trên memory block đã được free nếu có lưu ptr (tuy nhiên, những memory block này có thể bị lấp đầy bằng junk nên không chắc dữ liệu cũ vẫn còn)

