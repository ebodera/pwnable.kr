# random
The sixth challenge from pwnable.kr

## Description
Daddy, teach me how to use random value in programming!

`ssh random@pwnable.kr -p2222` (pw:guest)

## Solution
```sh
random@ubuntu:~$ ls -lh
total 20K
-r--r----- 1 random_pwn root     49 Jun 30  2014 flag
-r-sr-x--- 1 random_pwn random 8.4K Jun 30  2014 random
-rw-r--r-- 1 root       root    301 Jun 30  2014 random.c
```

### Capturing the Flag
