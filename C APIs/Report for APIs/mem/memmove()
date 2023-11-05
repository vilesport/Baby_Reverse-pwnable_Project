Declaration:
	void *memmove(void *dest, const void *src, size_t n)

+ dest: con trỏ đến ô nhớ bắt đầu (đích)
+ scr: con trỏ đến ô nhớ bắt đầu (nguồn)
+ n: số block cần sao chép

Description:
	memmove() sẽ sao chép n block từ dest vào n block src, khác với memcpy() chỉ đơn giản là lặp thì memmove() sẽ kiểm có kiểm tra để tránh hiện tượng lặp block khi dest và src là 1, đổi lại là tốc độ sẽ chậm hơn so với memcpy()
	=> Dùng memmove() để đổi 1 đoạn nhớ n block bắt đầu từ dest thành 1 đoạn nhớ n block bắt đầu từ sr khi src == dest để tránh bị lỗi

