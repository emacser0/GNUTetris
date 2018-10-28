        extern scanf
        extern printf
        section .data
a_32:    dd 0
b_32:    dd 0
a:      dq 0
b:      dq 0
s:      dq 0
r1:     dq 0
r2:     dq 0
o:      dq 1
scanf_format: db "%d%d",0
printf_format: db "%lld",10,0
        section .text
        global main
main:
        push ebp
        mov ebp,esp
main_start:
        push b_32
        push a_32
        push scanf_format
        call scanf
        add esp,12
        mov eax,dword[a_32]
        cdq
        mov dword[a],eax
        mov dword[a+4],edx
        mov eax,dword[b_32]
        cdq
        shld edx,eax,1
        add eax,eax
        mov dword[b],eax
        mov dword[b+4],edx
loop_init:
        mov ecx,0
loop:
        cmp ecx,32
        jae loop_end
        push ecx
        mov ecx, dword[b]
        mov ebx, dword[b+4]
        mov eax, dword[o]
        mov edx, dword[o+4]
        and eax, ecx
        and edx, ebx
        mov dword[r2], eax
        mov dword[r2+4], edx
        mov eax, dword[o]
        mov edx, dword[o+4]
        shld edx, eax, 1
        add eax, eax
        mov dword[o], eax
        mov dword[o+4], edx
        mov ecx, dword[b]
        mov ebx, dword[b+4]
        and eax, ecx
        and edx, ebx
        mov dword[r1], eax
        mov dword[r1+4], edx
        pop ecx
if1:
        mov eax,dword[r1]
        mov edx,dword[r1+4]
        or eax,edx
        test eax,eax
        sete al
        mov ebx,eax
        mov eax,dword[r2]
        mov edx,dword[r2+4]
        or eax,edx
        test eax, eax
        sete al
        xor eax,ebx
        test al,al
        je if1_end

        mov eax,dword[r1]
        mov edx,dword[r1+4]
        or eax,edx
        test eax,eax
        je else2
if2:
        mov eax,dword[s]
        mov ebx,dword[a]
        sub eax,ebx
        mov edx,dword[s+4]
        mov ebx,dword[a+4]
        sbb edx,ebx
        mov dword[s],eax
        mov dword[s+4],edx
        jmp if2_end
else2:
        mov eax,dword[s]
        mov ebx,dword[a]
        add eax,ebx
        mov edx,dword[s+4]
        mov ebx,dword[a+4]
        adc edx,ebx
        mov dword[s],eax
        mov dword[s+4],edx
if2_end:
if1_end:
        inc ecx
        mov eax,dword[a]
        mov edx,dword[a+4]
        shld edx,eax,1
        add eax,eax
        mov dword[a],eax
        mov dword[a+4],edx
        jmp loop
loop_end:
        push dword[s+4]
        push dword[s]
        push printf_format
        call printf
        add esp,12
end_of_main:
        mov esp,ebp
        pop ebp
        xor eax,eax
        ret
