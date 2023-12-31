#include<stdio>
#include<string.h>

//Synopsis:
int memcmp(const void *src1, const void *src2, size_t n)

/*
+ src1: con trỏ đến block bắt đầu của đoạn 1
+ src2: con trỏ đến block bắt đầu của đoạn 2
+ n: số lượng block so sánh
*/

/*Description:
	memcmp() sẽ so sánh n block bắt đầu từ src1 với n block bắt đầu từ src2
	Tại mỗi block i, nếu block tại src1 + i khác block tại src2 + i thì return (src1 + i) - (src2 + i)
	Nếu sau n block không return thì return 0
	Do đó:
		+ Nếu kết quả trả về là số âm(-1) thì đoạn src1 < đoạn src2
		+ Nếu kết quả trả về là số dương(1) thì đoạn src1 > đoạn src2
		+ Nếu kết quả trả về là 0 thì đoạn src1 = đoạn src2
	=> Dùng memcmp() để kiểm tra 1 đoạn n block của 2 mảng bộ nhớ chứ không nhất thiết là cả mảng
	Vậy rõ ràng memcmp() nguy hiểm hơn strcmp() vì strcmp chỉcmp block chứa char, còn memcmp() sẽ cmp mọi giá trị, vậy nên giới hạn của src1 - scr2 cũng sẽ lớn hơn, dễ gây lỗi hơn
Notice: buffer overflow nặng hơn strcmp do ngay khi chênh lệch sẽ trả về chênh lệch mà không quan tâm đó có phải kí tự hay không
*/
