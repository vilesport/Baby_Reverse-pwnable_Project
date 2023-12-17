//Synopsis:
	rettype inhibit_loop_to_libcall memmove (a1const void *dest, a2const void *src, size_t len)
{
  unsigned long int dstp = (long int) dest;
  unsigned long int srcp = (long int) src;
  /* This test makes the forward copying code be used whenever possible.
     Reduces the working set.  */
  if (dstp - srcp >= len)	/* *Unsigned* compare!  */
    {
      /* Copy from the beginning to the end.  */
#if MEMCPY_OK_FOR_FWD_MEMMOVE
      dest = memcpy (dest, src, len);
#else
      /* If there not too few bytes to copy, use word copy.  */
      if (len >= OP_T_THRES)
	{
	  /* Copy just a few bytes to make DSTP aligned.  */
	  len -= (-dstp) % OPSIZ;
	  BYTE_COPY_FWD (dstp, srcp, (-dstp) % OPSIZ);
	  /* Copy whole pages from SRCP to DSTP by virtual address
	     manipulation, as much as possible.  */
	  PAGE_COPY_FWD_MAYBE (dstp, srcp, len, len);
	  /* Copy from SRCP to DSTP taking advantage of the known
	     alignment of DSTP.  Number of bytes remaining is put
	     in the third argument, i.e. in LEN.  This number may
	     vary from machine to machine.  */
	  WORD_COPY_FWD (dstp, srcp, len, len);
	  /* Fall out and copy the tail.  */
	}
      /* There are just a few bytes to copy.  Use byte memory operations.  */
      BYTE_COPY_FWD (dstp, srcp, len);
#endif /* MEMCPY_OK_FOR_FWD_MEMMOVE */
    }
  else
    {
      /* Copy from the end to the beginning.  */
      srcp += len;
      dstp += len;
      /* If there not too few bytes to copy, use word copy.  */
      if (len >= OP_T_THRES)
	{
	  /* Copy just a few bytes to make DSTP aligned.  */
	  len -= dstp % OPSIZ;
	  BYTE_COPY_BWD (dstp, srcp, dstp % OPSIZ);
	  /* Copy from SRCP to DSTP taking advantage of the known
	     alignment of DSTP.  Number of bytes remaining is put
	     in the third argument, i.e. in LEN.  This number may
	     vary from machine to machine.  */
	  WORD_COPY_BWD (dstp, srcp, len, len);
	  /* Fall out and copy the tail.  */
	}
      /* There are just a few bytes to copy.  Use byte memory operations.  */
      BYTE_COPY_BWD (dstp, srcp, len);
    }
  RETURN (dest);
}

+ dest: con trỏ đến xâu 1
+ src: con trỏ đến xâu 2
+ len: số block cần chép

/*
Description:
	memmove() sẽ có 1 lần kiểm tra xem với độ dài len thì dest có bị lặp lên src hay không, vì memcpy() chép từ begin đến end nên nếu src nằm ở end của dest sẽ bị lỗi, nhưng memmove sẽ có kiểm tra, nếu lặp thì sẽ chép từ end về begin
	Tuy có kiểm tra dest và scr bịđè nhưng memmove() vẫn không kiểm tra vùng quản lí của dest và src, có thể nối liền dest và scr làm 1 xâu (nếu sao chép xâu) -> vẫn có buffer overflow
Notice: Nếu dest + len >= src thì sẽ chép từ end đến begin, nếu không sẽ giống memcpy(), và vẫn có thể gây buffer overflow như memcpy()
