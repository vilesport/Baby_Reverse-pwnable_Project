//Declaration:
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
	Tuy nhiên compiler sẽ kiểm tra được stack có bịthay đổi hay không => Tuỳ lúc sẽ có thể tận dụng để pwn
*/
