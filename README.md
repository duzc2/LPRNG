# LPRNG
Pseudorandom number generator transplant from luajit source code.

# WHY

Enhanced PRNG for math.random()
LuaJIT uses a Tausworthe PRNG with period 2^223 to implement math.random() and math.randomseed(). The quality of the PRNG results is much superior compared to the standard Lua implementation which uses the platform-specific ANSI rand().
The PRNG generates the same sequences from the same seeds on all platforms and makes use of all bits in the seed argument. math.random() without arguments generates 52 pseudo-random bits for every call. The result is uniformly distributed between 0.0 and 1.0. It's correctly scaled up and rounded for math.random(n [,m]) to preserve uniformity.

# Version
Current version of LPRNG is tested on Lua 5.2
