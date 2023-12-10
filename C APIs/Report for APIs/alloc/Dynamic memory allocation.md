**Dynamic memory allocation functions**
*<stdlib.h>*
---

**Notice**
    * Dynamic memory alloctation sẽ allocate vùng heap(big-endian), ngược với stack(little-endian)
---

**Malloc()**
* Declaration:
	* ptr = (cast_type*)malloc(byte_size)
* Description:
    * Malloc là 1 hàm sẽ cấp phát byte_size byte liên tiếp, sau đó trả về còn trỏ kiểu void trỏ vào byte đầu tiên
    * Malloc sẽ trả về null nếu không thể cấp phát đủ byte_size byte
---

**Realloc()**
* Declaration:
    * ptr = realloc(ptr, new size);
* Description:
    * Realloc sẽ thay đổi bộ nhớ cấp phát cho con pointer hiện tại
        * Nếu kích thước mới nhỏ hơn kích thước ban đầu thì chỉ đơn giản là deallocate phần chênh lệch
        * Nếu kích thước mới lơn hơn kích thước ban đầu thì:
            * Nếu phần bộ nhớ liền sau có thể sử dụng thì sẽ mở rộng thêm
            * Nếu phần bộ nhớ liền sau không đủ thì sẽ malloc byte_size byte mới, copy data từ ptr cũ sang vùng được allocate, sau đó deallocate vùng nhớ cũ
---

**Calloc()**
* Declaration:

* Description:

---

**Free()**
* Declaration:

* Description:


