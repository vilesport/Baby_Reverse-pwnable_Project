//Declaration:
	char *strncpy (char *s1, const char *s2, size_t n)
{
  size_t size = __strnlen (s2, n);
  if (size != n)
    memset (s1 + size, '\0', n - size);
  return memcpy (s1, s2, size);
}
/*
+ s1: xâu đích 
+ s2: xâu nguồn
+ n: số kí tự cần copy

Description:
	Cách hoạt động của strncpy() tuy đã có thêm số kí tự n cần được copy, dùng memset() để set các block nhớ chênh lệch về null nhưng vẫn gọi hàm memcpy() để sao chép => buffer overflow tương tự như strcpy()
*/
