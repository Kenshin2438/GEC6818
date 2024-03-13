```
[root@GEC6818 /cy]#uname -a
Linux GEC6818 3.4.39-gec #3 SMP PREEMPT Tue Jun 4 15:57:34 CST 2019 armv7l GNU/Linux
```

设备支持的 `VFP` 版本和 `NEON` 可用性

```
[root@GEC6818 /cy]#cat /proc/cpuinfo
Processor	: ARMv7 Processor rev 3 (v7l)
processor	: 0
BogoMIPS	: 1581.05

processor	: 1
BogoMIPS	: 1591.29

processor	: 2
BogoMIPS	: 1591.29

processor	: 3
BogoMIPS	: 1591.29

processor	: 4
BogoMIPS	: 1591.29

processor	: 5
BogoMIPS	: 1591.29

processor	: 6
BogoMIPS	: 1591.29

processor	: 7
BogoMIPS	: 1591.29

Features	: swp half thumb fastmult vfp edsp neon vfpv3 tls vfpv4
CPU implementer	: 0x41
CPU architecture: 7
CPU variant	: 0x0
CPU part	: 0xd03
CPU revision	: 3

Hardware	: s5p6818
Revision	: 0000
Serial		: 0000000000000000
```

---

- ["features" in ARM Linux's /proc/cpuinfo](http://publicclu2.blogspot.com/2013/05/features-in-arm-linuxs-proccpuinfo.html)
- [What do the flags in /proc/cpuinfo mean?](https://unix.stackexchange.com/questions/43539/what-do-the-flags-in-proc-cpuinfo-mean)
- [ARM architecture family - Wikipedia](https://en.wikipedia.org/wiki/ARM_architecture_family)

```
swp      : SWP instruction (atomic read-modify-write)
half     : half: Half-word loads and stores
thumb    : 16-bit instruction set
fastmult : fast multiplication (32-bit * 32-bit)
vfp      : Floating-point (vfpv3, vfpv4)
edsp     : DSP extensions
neon     : Advanced SIMD/NEON
tls      : Thread-local storage
```