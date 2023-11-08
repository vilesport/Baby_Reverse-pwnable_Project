Declaration:
	strcpy (char *dest, const char *src)
	{
  		return memcpy (dest, src, strlen (src) + 1);
	}

+ dest: xâu đích 
+ scr: xâu nguồn

Description:
	strcpy() sẽ dùng hàm memcpy() để sao chép n = strlen(src) + 1 kí tự bắt đầu từ src, thay thế vào n kí tự bắt đầu từ dest
	=> Dùng strcpy() để gán xâu a thành xâu b nhưng sẽ có lỗi nếu strlen(dest) != strlen(src)