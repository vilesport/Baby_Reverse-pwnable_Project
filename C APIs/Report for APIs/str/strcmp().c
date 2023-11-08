Declaration:
	int STRCMP (const char *str1, const char *str2)
	{
		const unsigned char *s1 = (const unsigned char *) str1;
 		const unsigned char *s2 = (const unsigned char *) str2;
		unsigned char c1, c2;
		do
		{
			c1 = (unsigned char) *s1++;
		      	c2 = (unsigned char) *s2++;
		      	if (c1 == '\0')
				return c1 - c2;
		}
		while (c1 == c2);
		return c1 - c2;
	}

+ str1: xâu số 1
+ str2: xâu số 2

Description:
	strcmp() sẽ so sánh n kí tự (với n là kích thước xâu nhỏ hơn) của xâu 1 với xâu 2, nếu (str1 + i) != (str2 + i) thì return (str1 + i) - (str2 + i)
	Nếu sau n kí tự không return thì return 0
	Do đó:
		+ Nếu kết quả trả về là số âm(-1) thì str1 < str2
		+ Nếu kết quả trả về là số dương(1) thì str1 > str2
		+ Nếu kết quả trả về là 0 thì str1 = str2
	=> dùng strcmp() để kiểm tra thứ tự từ điển của 2 xâu 
