//Synopsis:
void *memcpy (void *dest, const void *src, size_t n)
{
  unsigned long int dstp = (long int) dest;
  unsigned long int srcp = (long int) src;
  /* Copy from the beginning to the end.  */
  /* If there not too few bytes to copy, use word copy.  */
  if (n >= OP_T_THRES)
    {
      /* Copy just a few bytes to make DSTP aligned.  */
      n -= (-dstp) % OPSIZ;
      BYTE_COPY_FWD (dstp, srcp, (-dstp) % OPSIZ);
      /* Copy whole pages from SRCP to DSTP by virtual address manipulation,
	 as much as possible.  */
      PAGE_COPY_FWD_MAYBE (dstp, srcp, n, n);
      /* Copy from SRCP to DSTP taking advantage of the known alignment of
	 DSTP.  Number of bytes remaining is put in the third argument,
	 i.e. in n.  This number may vary from machine to machine.  */
      WORD_COPY_FWD (dstp, srcp, n, n);
      /* Fall out and copy the tail.  */
    }
  /* There are just a few bytes to copy.  Use byte memory operations.  */
  BYTE_COPY_FWD (dstp, srcp, n);
  return dest;
}
/*
+ dest: con trỏ đến ô nhớ bắt đầu (đích)
+ scr: con trỏ đến ô nhớ bắt đầu (nguồn)
+ n: số block cần sao chép

Description:
	memcpy() sẽ sao chép n block từ dest vào n block src, tuy nhiên hàm không kiểm tra liệu block đang chép vào có thuộc quản lí của dest hay không, nên có thể xảy ra buffer overflow -> thay đổi dữ liệu của n block liên tiếp, có thể ghi đè lên block nhớ không thuộc quản lí của dest
Notice: luôn copy n block từ src vào dest mà không kiểm tra -> buffer overflow
*/
