//Synopsis:
	strcpy (char *dest, const char *src)
	{
  		return memcpy (dest, src, strlen (src) + 1);
	}
/*
+ dest: xâu đích 
+ scr: xâu nguồn

Description:
	Rõ ràng có thể thấy hàm strcpy sẽ copy từ src vào dest bằng memcpy nhưng không kiểm tra độ dài của src và dest có giống nhau hay không => có lỗi khi len(src) > len(dest)
	Cụ thể khi len(src) > len(dest) thì sẽ ghi đè những block nhớ nằm kế *dest trong stack => buffer overflow 
	Tuy nhiên compiler sẽ kiểm tra được stack có bịthay đổi hay không => Nếu có ctrinh sử dụng không kiểm tra src, dest và có thể buffer overflow mà không thay đổi canary thì hoàn toàn tận dụng được
Notice: Có thể buffer overflow (qua được canary, chương trình không kiểm soát src, dest)
*/
