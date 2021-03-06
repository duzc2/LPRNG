从luajit里移植出来的 LPRNG算法，可以从lua调用。
手机上运行luajit，但是luajit不支持大内存，所以当时服务器上跑的是lua，跟客户端需要用同一个种子跑出来相同的随机序列。
所以把这个算法移植出来，让服务器跟客户端统一。

# LPRNG
Pseudorandom number generator transplant from luajit source code.

# WHY
<I>
Enhanced PRNG for math.random()
LuaJIT uses a Tausworthe PRNG with period 2^223 to implement math.random() and math.randomseed(). The quality of the PRNG results is much superior compared to the standard Lua implementation which uses the platform-specific ANSI rand().<br/>
The PRNG generates the same sequences from the same seeds on all platforms and makes use of all bits in the seed argument. math.random() without arguments generates 52 pseudo-random bits for every call. The result is uniformly distributed between 0.0 and 1.0. It's correctly scaled up and rounded for math.random(n [,m]) to preserve uniformity.<br/>
</I>
*from http://luajit.org/extensions.html#math_random<br/>
# Version
Current version of LPRNG is tested on Lua 5.2

# Build

Windows:

gcc.exe -O2 -Wall -g -DBUILD_DLL -IE:\lua\lua-5.2.4\src -IE:\lua\lua-5.2.4\src -c lprng.c -o obj\Debug\lprng.o

g++.exe -shared -Wl,--output-def=bin\Debug\libLPRNG.def -Wl,--out-implib=bin\Debug\libLPRNG.a -Wl,--dll -LE:\lua\lua-5.2.4\src -LE:\lua\lua-5.2.4\src obj\Debug\lprng.o -o bin\Debug\LPRNG.dll -static-libgcc -llua52

Linux:

gcc -O2 -fPIC -Wall -g -DBUILD_DLL -I/root/lua52/src/lua-5.2.4/src/ -c lprng.c -o lprng.o

gcc -shared -fPIC -Wl,--dll -L/root/lua52/src/lua-5.2.4/src/ lprng.o -o LPRNG.so -static-libgcc -llua

VS: /fp:strict
