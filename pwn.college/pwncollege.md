**Write-up for** [Assembly Crash Course](https://pwn.college/cse466-f2022/assembly-crash-course)
---

**Challenge 1: Set a register**
---
* Vì đây là level đầu nên yêu cầu đơn giản: gán giá trị cho trước vào thanh ghi được chỉ định
```text
In this level you will work with registers_use! Please set the following:
* rdi = 0x1337
```
* Đơn giản là dùng mov để gán 0x1337 cho $rdi
```nvim
_start:
    mov rdi, 0x1337
```

**Challenge 2: Addition**
---
* Ở level này yêu cầu tăng thanh ghi chỉ định thêm 1 giá trị cho trước
```text
Do the following:
* add 0x331337 to rdi

We will now set the following in preparation for your code:
rdi = 0xc99
```
* Vì thanh ghi $rdi đã set sẵn = 0xc99, nên nếu dùng mov $rdi, 0x331337 như level trước sẽ chỉ gán $rdi = 0x331337 chứ không phải $rdi = $rdi + 0x331337. 
* Có thể dùng add để tăng $rdi thêm 0x331337 hoặc dùng mov để gán $rdi = 0x331337+0xc99 = 331fd0
```nvim
_start:
    add rdi, 0x331337
```
```nvim
_start:
    mov rdi, 0x331fd0
```

**Challenge 3: Multiplication**
---
* Ở level trước đã làm quen với toán tử cộng, level này sẽ làm quen với toán tử nhân: imul
```text
Using your new knowledge, please compute the following:
f(x) = mx + b, where:
m = rdi
x = rsi
b = rdx
Place the value into rax given the above.
We will now set the following in preparation for your code:
rdi = 0x8ce
rsi = 0x1bd1
rdx = 0x1feb
```
* Đề yêu cầu ta tính phương trình ax + b với a là m được set tại $rdi, x được set tại $rsi và b được set tại $rdx
* Hướng giải quyết: dùng imul để nhân $rdi với $rsi, gán vào $rax rồi cộng $rax thêm $rdx
    `imul $a, $b: gán $a = $a * $b`
```nvim
_start:
    imul rdi, rsi
    mov rax, rdi
    add rax, rdx
```

**Challenge 4: Division**
--- 
* Có nhân thì sẽ có chia, level này sẽ giúp tiếp cận phép chia trong assembly
```text
Please compute the following:
speed = distance / time, where:
distance = rdi
time = rsi
Place the value of speed into rax given the above.
We will now set the following in preparation for your code:
rdi = 0x64f
rsi = 0x5f
```
* Như đề yêu cầu, nhiệm vụ của ta là gán $rax = $rdi / $rsi, tuy nhiên có chút khác biệt giữa phép chia và các phép toán trước đó
      `idiv a: chỉ có 1 toán tử a, mặc định sẽ lấy $rax/a, trả phần nguyên về $rax, trả phần dư về $rdx. Yêu cầu $rdx = 0 để kết quả được chính xác`
* Tiếp cận: vì idiv chỉ lấy giá trị của $rax làm số bị chia, nên để thực hiện phép chia $rdx / $rsi, ta phải gán $rax = $rdx trước khi chia. Kết quả cũng tự động gán vào rax nên không cần thao tác thêm
```nvim
_start:
    mov rax, rdx
    mov rdx, 0
    idiv rsi
```

**Challenge 5: Modulus**
---
* Level trước là chia lấy phần nguyên, level này sẽ tính toán chia lấy phần dư
```text
Please compute the following:
rdi % rsi
Place the value in rax.

We will now set the following in preparation for your code:
rdi = 0x39a66f69
rsi = 0x3f
```
* Như thông tin đã có ở level trước, phần dư của phép div sẽ lưu vào $rdx. Vậy nên sau khi thực hiện phép chia thì nhiệm vụ của ta chỉ còn là gán $rdx vào $rax
```nvim
_start:
    mov rax, rdi
    mov rdi, 0
    idiv rsi
    mov rax, rdi
```

**Challenge 6: Register sizes**
---
* Level này sẽ tìm hiểu về cách thanh ghi lưu trữ dữ liệu
```text
Using only the following instruction(s)
mov
Please compute the following:
rax = rdi modulo 256
rbx = rsi modulo 65536

We will now set the following in preparation for your code:
rdi = 0x2177
rsi = 0xdf5418a7
```
* Tuy yêu cầu chia lấy dư có thể thực hiện bằng idiv như level trước, chỉ có thể sử dụng mov ở level này
* 256 và 65536 lần lượt là 2^8^ và 2^16^ tương ứng với 100000000~2~(8 bit 0) và 10000000000000000~2~(16 bit 0)
    => Phần dư khi chia 256 và 65536 sẽ lần lượt là 8 và 16 bit cuối cùng của thanh ghi lưu số bị chia
* Các thanh ghi có thể truy cập những đoạn nhỏ hơn của chính nó bằng tên như sơ đồ dưới
```
+----------------------------------------+
|               rax (64 bits)            |
+--------------------+-------------------+
                     |    eax (32 bits)  |
                     +---------+---------+
                               |   ax    |
                               +----+----+
                               | ah | al |
                               +----+----+
Đối với 4 thanh ghi rax, rbx, rcx, rdx sẽ có dạng như sau:
r_x:    64bit
e_x:    32bit
_x:     16bit
_h, _l: 8bit
Các thanh ghi khác sẽ có tên gọi khác cho các thanh ghi con của nó
```

* Kích thước của thanh ghi ax là 16 bit, al, ah lần lượt là 8 bit. Trong đó ta cần những bit cuối cùng nên giữa al và ah ta sẽ lấy al
    + Đáp án cho phép mod $rdi với 256 sẽ là $dil 
    + Đáp án cho phép mod $rsi với 65536 sẽ là $si
```nvim
_start:
    mov rax, dil
    mov rbx, si
```

**Challenge 7: Bitwise shift**
---
* Sau các phép toán bình thường sẽ là các thao tác bit
```text
Using only the following instructions:
mov, shr, shl
Please perform the following:
Set rax to the 5th least significant byte of rdi
i.e.
rdi = | B7 | B6 | B5 | B4 | B3 | B2 | B1 | B0 |
Set rax to the value of B4

We will now set the following in preparation for your code:
rdi = 0x91affdcbc43713f0
```
* Ở level này sẽ dùng phép dịch bit để lấy giá trị tại byte được chỉ định
* Vì chỉ có thể sử dụng mov, shl, shr nên ta sẽ dịch sao cho B4 về cuối thanh ghi, sau đó lấy $dil gán vào $al, vậy là đã gán được byte số 4 vào $rax
```nvim
_start:
    shr rdi, 32
    mov al, dil
```

**Challenge 8: Bitwise and**
---
```text
Without using the following instructions:
mov, xchg
Please perform the following:
rax = rdi AND rsi
i.e. Set rax to the value of (rdi AND rsi)

We will now set the following in preparation for your code:
rdi = 0x72a2e58ce98b4813
rsi = 0x67bba0a6868b145c
```
* Ở bài này đã bị cấm lệnh mov, vậy nên không thể gán $rax như cách bình thường được
* Tuy nhiên ta có tính chất:
     + a xor a = 0
     + a xor b = b(a = 0)
    => Có thể dùng phép xor để gán $rax = rdi, sau đó and $rax với $rsi
```nvim
_start:
    xor rax, rax
    xor rax, rdi
    and rax, rsi
```

**Challenge 9: Bitwise logic**
---
```text
Using only the following instructions:
and, or, xor
Implement the following logic:

if x is even then
  y = 1
else
  y = 0
where:
x = rdi
y = rax

We will now set the following in preparation for your code:
rdi = 0x3292dda5
```
* Như tên level, chúng ta phải hiểu được logic của bit và áp dụng.
* Trong đó, tính chẵn lẻ của 1 số sẽ được quyết định bởi bit 2^0^ của số. Nếu bit bật thì số là số lẻ, nếu không số là số chẵn
* Nếu $rdi chẵn thì $rax = 1, $rdi lẻ thì $rax = 0. Mà chỉ được dùng and, or, xor nên sẽ dùng and 1 để kiểm tra bit 2^0^ là 1 hay 0, sau đó xor với 1 để đảo bit 1 -> 0, 0 -> 1
```nvim
_start:
    and rdi, 1
    xor rax, rax
    xor rax, rdi
    xor rax, 1
```

**Challenge 10: Memory reads and writes**
---
```text
Please perform the following:
1. Place the value stored at 0x404000 into rax
2. Increment the value stored at the address 0x404000 by 0x1337
Make sure the value in rax is the original value stored at 0x404000 and make sure
that [0x404000] now has the incremented value.

We will now set the following in preparation for your code:
[0x404000] = 0x19be30
```
* Level này chỉ đơn giản là để ta làm quen thao tác với dữ liệu tại địa chỉ, trong đó, kiến thức về địa chỉ cũng đã được cung cấp trước khi đọc đề nên có thể dễ dàng cài đặt
```nvim
_start:
    mov rax, [0x404000]
    mov rbx, 0x1337
    add [0x404000], rbx    
```

**Challenge 11: Data sizes**
---
```text
Please perform the following:
1) Set rax to the byte at 0x404000
2) Set rbx to the word at 0x404000
3) Set rcx to the double word at 0x404000
4) Set rdx to the quad word at 0x404000

We will now set the following in preparation for your code:
[0x404000] = 0x1aea24
```
* Đề yêu cầu ta lấy các dữ liệu có kích thước khác nhau tại địa chỉ cho trước, lưu lần lượt vào các thanh ghi
* Có 1 điều cần lưu ý là khi mov dữ liệu từ địa chỉ vào thanh ghi, kích thước dữ liệu được lấy sẽ trùng với kích thước thanh ghi 
`Quad word: thanh ghi 64bit`
`Double word: thanh ghi 32bit`
`Word: thanh ghi 16bit`
`Byte: thanh ghi 8bit`
    => Từ đó việc cài đặt đơn giản như sau
```nvim
_start:
    mov al, [0x404000]
    mov bx, [0x404000]
    mov ecx, [0x404000]
    mov rdx, [0x404000]
```

**Challenge 12: Dynamic address memory writes**
---
```text
For this challenge we will give you two addresses created dynamically each run. The first address
will be placed in rdi. The second will be placed in rsi.
Using the earlier mentioned info, perform the following:
1. set [rdi] = 0xdeadbeef00001337
2. set [rsi] = 0xc0ffee0000
Hint: it may require some tricks to assign a big constant to a dereferenced register. Try setting
a register to the constant then assigning that register to the derefed register.

We will now set the following in preparation for your code:
[0x404260] = 0xffffffffffffffff
[0x4049f8] = 0xffffffffffffffff
rdi = 0x404260
rsi = 0x4049f8
```
* Tuy đề có vẻ giống ở level trước, nhưng việc set dữ liệu tại các địa chỉ sẽ cần đến thanh ghi, vì vậy dựa vào kích thước dữ liệu gán vào địa chỉ, lựa chọn thanh ghi phù hợp để lưu dữ liệu rồi gán vào địa chỉ
`0xdeadbeef00001337 có 64bit ở dạng nhị phân -> thanh ghi 64bit`
`0xc0ffee0000 có 40bit ở dạng nhị phân -> thanh ghi 64bit`
```nvim
_start:
    mov rax, qword 0xdeadbeef00001337
    mov rbx, qword 0xc0ffee0000
    mov [rdi], rax
    mov [rsi], rbx
```

**Challenge 13: Consecutive memory reads**
---
```text
Perform the following:
1. load two consecutive quad words from the address stored in rdi
2. calculate the sum of the previous steps quad words.
3. store the sum at the address in rsi

We will now set the following in preparation for your code:
[0x4041e8] = 0x9bd96
[0x4041f0] = 0x20417
rdi = 0x4041e8
rsi = 0x404670
```
* Như đã được học ở những level trước, quadword sẽ lưu vào thanh ghi 64bit, do đó chỉ đơn giản là dùng 2 thanh ghi 64bit lấy dữ liệu tại đại chỉ chỉ định, sau đó gán vào địa chỉ lưu tại $rsi
```nvim
_start:
    mov rax, [rdi]
    add rax, [rdi + 8]
    mov [rsi], rax
```

**Challenge 14: The stack**
---
```text
Replace the top value of the stack with (top value of the stack - rdi).
We will now set the following in preparation for your code:
rdi = 0x1765
(stack) [0x7fffff1ffff8] = 0x3259ce5
```
* Stack là kiểu dữ liệu first in last out có nhiều tài liệu trên internet để đọc hiểu
* Trong assembly thì stack cũng có cơ chế hoạt động tương tự. Ở level này ta cần lấy dữ liệu ở top của stack, sau đó đưa dữ liệu sau khi tính toán vào lại stack
`pop $a sẽ gán dữ liệu tại top của stack cho $a và xóa top khỏi stack`
`push $a sẽ đưa giá trị tại $a lên top của stack`
```nvim
_start:
    pop rax
    sub rax, rdi
    push rax
```
**Challenge 15: Swap register values with the stack**
---
* Level này tiếp tục dùng để hiểu thêm về stack
```
Using only following instructions:
push, pop
Swap values in rdi and rsi.
i.e.
If to start rdi = 2 and rsi = 5
Then to end rdi = 5 and rsi = 2

We will now set the following in preparation for your code:
rdi = 0x3ab9d1ed
rsi = 0xad836d7
```
* Như đã nói, stack có cơ chế first in last out, hay là vào trước thì ra sau, vậy nên nếu đưa a vào trước b thì b sẽ ra trước a
* Vì chỉ có thể dùng push, pop tại level này nên ta có thể sử dụng cơ chế này của stack để đổi 2 giá trị của 2 thanh ghi được chỉ định
```nvim
_start:
    push rdi
    push rsi
    pop rdi
    pop rsi
```
**Challenge 16: Memory reads and writes with the stack**
---
```
Without using pop please calculate the average of 4 consecutive quad words stored on the stack.
Push the average on the stack. Hint:
RSP+0x?? Quad Word A
RSP+0x?? Quad Word B
RSP+0x?? Quad Word C
RSP      Quad Word D

We will now set the following in preparation for your code:
(stack) [0x7fffff200000:0x7fffff1fffe0]
= ['0x1216cdb5', '0x5e5b770', '0x29e46e6b', '0x1ce25588'] (list of things)
```
* Để hiểu được lấy giá trị từ stack mà không push, pop thì ta cần phải hiểu $rsp, $rbp trước
`Để đơn giản thì $rsp là thanh ghi lưu địa chỉ của phần tử ở top trong stack, $rbp là thanh ghi lưu địa chỉ của phần tử ở bot trong stack`
* Thực ra ở level này chưa cần hiểu vì đề yêu cầu lấy dữ liệu từ các địa chỉ được chỉ định sẵn như ở level 11 sau đó tính toán
```nvim
_start:
    mov rax, [rsp]
    add rax, [rsp + 8]
    add rax, [rsp + 16]
    add rax, [rsp + 24]
    mov rbx, 4
    div rbx
    push rax
```
**Challenge 17: Control flow**
---
```
Useful instructions for this level is:
jmp (reg1 | addr | offset) ; nop
Hint: for the relative jump, lookup how to use `labels` in x86.

Using the above knowledge, perform the following:
Create a two jump trampoline:
1. Make the first instruction in your code a jmp
2. Make that jmp a relative jump to 0x51 bytes from its current position
3. At 0x51 write the following code:
4. Place the top value on the stack into register rdi
5. jmp to the absolute address 0x403000

We will now set the following in preparation for your code:
- Loading your given code at: 0x4000ce
- (stack) [0x7fffff1ffff8] = 0xeb
```
* Để hiểu về jump thì ở level hiện tại, jump là để ta chuyển chương trình hiện tại đến địa chỉ được chọn và tiếp tục thực thi. Nôm na thì nó sẽ bỏ qua 1 đoạn code giữa địa chỉ hiện tại và địa chỉ đích và tiếp tục chương trình bắt đầu từ địa chỉ đích
`Mình stuck khá lâu ở bài này vì khi jump đến địa chỉ $+0x51 thì bị lỗi, các câu lệnh cần được viết đủ nhiều để jump có thể skip nhưng nop rồi jump vẫn bị lỗi. Chỉ có jump đến hàm được chỉ định, sau đó thay đổi dần dần số lần nop đến khi nó jump đến đúng địa chỉ được yêu cầu rồi mới thực hiện các lệnh bình thường được`
```nvim
_start:
    jmp solve
    times 0x51 nop
    solve:
        pop rdi
        mov rax, 0x403000
        jmp rax
```
**Challenge_18: Conditional branches**
---
```
Using the above knowledge, implement the following:
if [x] is 0x7f454c46:
   y = [x+4] + [x+8] + [x+12]
else if [x] is 0x00005A4D:
   y = [x+4] - [x+8] - [x+12]
else:
   y = [x+4] * [x+8] * [x+12]
where:
x = rdi, y = rax. Assume each dereferenced value is a signed dword. This means the values can start as
a negative value at each memory position.
A valid solution will use the following at least once:
jmp (any variant), cmp

We will now run multiple tests on your code, here is an example run:
- (data) [0x404000] = {4 random dwords]}
- rdi = 0x404000
```
* Level này sé bắt đầu sử dụng đến việc jmp có điều kiện, bằng cmp hoặc test đã được giới thiệu đầy đủ trước khi vào đề, chỉ việc cài đặt điều kiện nữa thôi
`Mình không hiểu tại sao kết quả 32bit lưu trong thanh ghi 64bit bị lỗi vì 32bit sau của thanh ghi không = 0, đổi qua lưu bằng thanh ghi 32bit thì sai kết quả nên mình phải shl rax 32; shr rax, 32 để xóa 32bit sau của thanh ghi thì kết quả mới đúng`
```nvim
section .text 
        global _start

_start:
        mov ebx, [rdi]
        mov eax, [rdi + 4]
        mov ecx, 0x7f454c46
        mov edx, 0x00005A4D
        cmp ebx, ecx
        je Case_1
        cmp ebx, edx
        je Case_2
        
        mov ebx, [rdi + 8]
        imul rax, rbx
        mov ebx, [rdi + 12]
        imul rax, rbx
        jmp END

        Case_1:
                mov ebx, [rdi + 8]
                add rax, rbx
                mov ebx, [rdi + 12]
                add rax, rbx
                jmp END

        Case_2:
                mov ebx, [rdi + 8]
                sub rax, rbx
                mov ebx, [rdi + 12]
                sub rax, rbx
                jmp END
        
        END:
                shl rax, 32
                shr rax, 32
```
**Challenge 19: Jump tables**
---
```
Using the above knowledge, implement the following logic:
if rdi is 0:
    jmp 0x403031
else if rdi is 1:
    jmp 0x4030d2
else if rdi is 2:
    jmp 0x40319c
else if rdi is 3:
    jmp 0x40329f
else:
    jmp 0x40334c
Please do the above with the following constraints:
- assume rdi will NOT be negative
- use no more than 1 cmp instruction
- use no more than 3 jumps (of any variant)
- we will provide you with the number to 'switch' on in rdi.
- we will provide you with a jump table base address in rsi.

Here is an example table:
    [0x404344] = 0x403031 (addrs will change)
    [0x40434c] = 0x4030d2
    [0x404354] = 0x40319c
    [0x40435c] = 0x40329f
    [0x404364] = 0x40334c
```
* Lần này vẫn là cài đặt điều kiện nhưng khó hơn trong việc nhảy đến các địa chỉ được chỉ định lưu ở địa chỉ ban đầu tại $rsi
`Mình hoàn toàn bất lực vì jmp đến [rsi + rdi * 8] luôn bị lỗi read unmapped ;-;. Mình thử nhiều cách và rồi nop trước khi cmp và jmp là cách đưa mình đến cái flag mà không dính thêm cái lỗi ngáo ngơ nào`

```nvim
_start:
        mov rcx, rdi
        times 0x301 nop
        mov rax, 4
        cmp rcx, rax
        jge Case_0
        mov rbx, [rsi + rcx * 8]
        jmp rbx

        Case_0:
                mov rbx, [rsi + 4 * 8]
                jmp rbx
```
`P/S: Tham khảo writeup của các bạn khác thì các bạn lại có thể jmp và lấy flag mà không có 1 lỗi nào, ảo lòi 4 tiếng debug cái lỗi read unmapped`

**Challenge 20: Computing averages**
---
```
Please compute the average of n consecutive quad words, where:
rdi = memory address of the 1st quad word
rsi = n (amount to loop for)
rax = average computed

We will now set the following in preparation for your code:
- [0x404250:0x404570] = {n qwords]}
- rdi = 0x404250
- rsi = 100
```
* Dù đề chỉ đơn giản là yêu cầu tính trung bình cộng của n phần tử, có thể cài đặt bằng cách dùng điều kiện lặp bth, nhưng phải để ý đến chuyện tràn số.
* Nếu cộng liên tục n số 64bit thì sẽ bị tràn, vậy nên thay vì tính $\frac{a_{1} + a_{2} +... a_{n}}{n}$ để phần tổng ở trên bị tràn thì ta chuyển tách ra thành $\frac{a_{1}}{n}$ + $\frac{a_{2}}{n}$ + ... $\frac{a_{n}}{n}$ để chống tràn
`Vì các số đều nằm trong giới hạn 64bit nên kết quả cuối luôn không bị tràn khỏi 64bit`
`Tuy nhiên vì lúc đọc đề mình không rõ liệu chỉ n chỉ ở 100 số hay nhiều hơn và cũng chưa xét đến tính chất nêu trên nên cài đặt theo hướng khác khá phức tạp.`
* Code ở dưới đây là mình cài đặt theo hướng giới hạn số không vượt quá 2^63^ để cộng lại không bị tràn.
`Ratkhodam, nên tham khảo code từ nguồn wu khác`
```nvim
section .text 
        global _start

_start:
        mov rcx, 0xffffffffffffffff/2
        mov rbx, 0
        mov r10, 0
        mov rdx, 0
        mov rax, 0
        jmp Cin
        Cin:
                cmp rdx, rsi
                je END
            
                mov r10, [rdi]
                add rdi, 0x8
                jmp solve 
                
        Cal_r:
                sub r10, rcx
                inc rbx
                jmp solve  
                
        Cal_a:
                sub rax, rcx    
                inc rbx
                jmp solve
                
        solve:
                cmp r10, rcx
                jge Cal_r   
                cmp rax, rcx
                jge Cal_a
                
                add rax, r10
                xor r10, r10
                inc rdx
                
                cmp rax, rcx
                jge Cal_a
                jmp Cin
        
        END:
                xor rdx, rdx
                xor r8, r8
                xor r9, r9
                xor r11, r11
                xor r12, r12
                
                idiv rsi                ; rax = a/n, rdx = remain
                mov r8, rax
                mov r9, rdx
                xor rdx, rdx
                mov rax, rcx    
                
                idiv rsi                ; rax = c/n, rdx = remain
                mov r11, rax
                mov r12, rdx
                xor rdx, rdx
                xor rax, rax
                
                mov r13, rbx
                imul r13, r11
                mov r14, rbx
                imul r14, r12
                
                add r8, r13
                add r9, r14
                mov rax, r9
                idiv rsi
                add r8, rax
                mov rax, r8
```

**Challenge 21: Implementing strlen**
---
```text
Using the above knowledge, please perform the following:
Count the consecutive non-zero bytes in a contiguous region of memory, where:
rdi = memory address of the 1st byte
rax = number of consecutive non-zero bytes
Additionally, if rdi = 0, then set rax = 0 (we will check)!
An example test-case, let:
rdi = 0x1000
[0x1000] = 0x41
[0x1001] = 0x42
[0x1002] = 0x43
[0x1003] = 0x00
then: rax = 3 should be set

We will now run multiple tests on your code, here is an example run:
- (data) [0x404000] = {10 random bytes},
- rdi = 0x404000
```

* Mọi thứ theo đề là chúng ta sẽ đọc các byte liên tiếp từ byte được cho tại địa chỉ lưu trong $rdi, đến khi byte = 0 thì dừng và in ra số lượng byte đã đếm
* Kết hợp conditional jump và read address từ các level trước, level này hoàn toàn áp dụng việc cài đặt một cách đơn giản:

```nvim
_start:
        xor rax, rax
        xor rbx, rbx
        cmp rdi, 0
        je END
        mov rbx, [rdi]
        inc rdi
        jmp solve
        
        solve:
                cmp rbx, 0
                je END
                inc rax
                mov rbx, [rdi]
                inc rdi
                jmp solve
        
        END: 
```

**Challenge 22: Using libary functions**
---

```text
Please implement the following logic:
str_lower(src_addr):
    i = 0
    if src_addr != 0:
        while [src_addr] != 0x00:
            if [src_addr] <= 0x5a:
                [src_addr] = foo([src_addr])
                i += 1
            src_addr += 1
    return i

foo is provided at 0x403000. foo takes a single argument as a value and returns a value.

All functions (foo and str_lower) must follow the Linux amd64 calling convention (also known as System V AMD64 ABI):
https://en.wikipedia.org/wiki/X86_calling_conventions#System_V_AMD64_ABI

Therefore, your function str_lower should look for src_addr in rdi and place the function return in rax.

An important note is that src_addr is an address in memory (where the string is located) and [src_addr] refers to the byte that exists at src_addr.

Therefore, the function foo accepts a byte as its first argument and returns a byte.

We will now run multiple tests on your code, here is an example run:
- (data) [0x404000] = {10 random bytes},
- rdi = 0x404000
```

* Bình thường ta sẽ viết 1 chương trình như ở hàm main, chương trình chính, tuy nhiên level này lại yêu cầu ta cài đặt 1 chương trình như 1 hàm được gọi theo quy tắc gọi hàm (calling convention). Trong đó tài liệu về calling convention cũng như stack, những kiến thức liên quan được cung cấp trong đề khá ít, đọc thêm từ [Assembly Calling Convention](https://cs61.seas.harvard.edu/site/2018/Asm2/) để hiểu thêm về cách truyền tham số, cơ chế hoạt động của stack khi gọi hàm
* Level này không level trước bao nhiêu, nhưng đặc biệt lưu ý ret ở cuối chương trình vì đây là 1 hàm, nếu không ret thì ctrinh sẽ không thể quay về lúc trước khi gọi hàm `(90% của 1 tuần mình debug bài này cũng là do không biết hàm phải có ret ở cuối)`
```nvim
_start:
        mov rax, 0
        mov r8, 0x403000
        cmp rdi, 0
        je last

        call str_lower

        str_lower:
                jmp solve
                solve:
                        mov rax, 0
                        mov al, byte [rdi]
                        cmp al, 0x0
                        je last 
                        cmp al, 0x5a
                        jle con
                        inc rdi
                        jmp solve

                con:
                        inc rbx
                        push rdi
                        mov rax, 0
                        mov al, byte [rdi]
                        mov rdi, 0
                        mov dil, al
                        call r8
                        pop rdi
                        mov byte [rdi], al 
                        jmp solve 
                ret

        last:
        mov rax, rbx
        ret
```

**Challenge 23: Compute the most common byte**
---

```text
Notice how rbp is always used to restore the stack to where it originally was. If we don't restore
the stack after use, we will eventually run out TM. In addition, notice how we subtracted from rsp
since the stack grows down. To make it have more space, we subtract the space we need. The ret
and call still works the same. It is assumed that you will never pass a stack address across functions,
since, as you can see from the above use, the stack can be overwritten by anyone at any time.
Once, again, please make function(s) that implements the following:
most_common_byte(src_addr, size):
    i = 0
    for i <= size-1:
        curr_byte = [src_addr + i]
        [stack_base - curr_byte] += 1

    b = 0
    max_freq = 0
    max_freq_byte = 0
    for b <= 0xff:
        if [stack_base - b] > max_freq:
            max_freq = [stack_base - b]
            max_freq_byte = b
        b += 1

    return max_freq_byte

Assumptions:
- There will never be more than 0xffff of any byte
- The size will never be longer than 0xffff
- The list will have at least one element
Constraints:
- You must put the "counting list" on the stack
- You must restore the stack like in a normal function
- You cannot modify the data at src_addr
```
* Sau tất cả thì đây là level mà mình thấy chill nhất (vì khi đã học đủ từ 22 level trước thì level này không có gì quá khó khăn)
* Điều duy nhất cần quan tâm ở đây là khai báo bộ nhớ (khoảng byte trống cho hàm mình đang viết)
* Để ý thì for đầu tiên, địa chỉ tối đa ta truy cập sẽ là src_addrr + size, tuy nhiên đây là những byte lưu biến nhập vào hàm nên không cần khai báo
* Ở for thứ 2, stack base sẽ là stack_base - b, b <= 0xff => khai báo thêm > 0xff byte cho hàm để truy cập (Tuy nhiên khai báo thêm 0xff byte để chắc chắn không bị lỗi read_unmapped như nhiều lần thử ở trước)

```nvim
_start:
        push rbp
        mov rbp, rsp
        sub rsp, 0x1fffe

        mov rbx, 0
        cmp rbx, rsi
        jl build

        build:
                mov r10b, byte [rdi + rbx]
                mov r11, rbp
                sub r11, r10
                mov r12, [r11]
                inc r12
                mov [r11], r12
                inc rbx
                cmp rbx, rsi
                jle build
                jmp solve
                sub r11, r10
                mov r12, [r11]
                inc r12
                mov [r11], r12
                inc rbx
                cmp rbx, rsi
                jle build
                jmp solve
        
        solve:
        mov rbx, 0
        mov rcx, 0
        mov rax, 0
        jmp count

        count:
                cmp rbx, 0xff
                jg end
                mov r10, rbp
                sub r10, rbx
                mov r11b, byte [r10]
                cmp r11, rcx
                jle tmp
                mov rcx, r11
                mov rax, rbx
                jmp tmp
        
        tmp:
                inc rbx
                jmp count
        
        end:
        mov rsp, rbp
        pop rbp
        ret
```
