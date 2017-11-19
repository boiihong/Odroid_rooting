# Odroid_rooting
Rooting the linux kernel 
This project follow already revealed linux rooting method.
The vulnerability is revealed by Anthony Lineberry(anthony.lineberry@gmail.com).
The key idea of this attack is to use the /dev/mem file to maliciously modify the kernel code.
By simply opening the /dev/mem and changing some binaries, we could get the root previledge.

1. hw3_setresuid.c: change kernel function setresuid to be root from user.
2. hw3_setuid.c: change ketnel function setuid to be root from normal user.
3. report: project report.
