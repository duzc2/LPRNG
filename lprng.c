#define LUA_LIB
#include <stdint.h>
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
#include <math.h>
#if defined(__i386__)
#define LPRNG_FASTCALL	__attribute__((fastcall))
#elif defined(_M_IX86)
#define LPRNG_FASTCALL	__fastcall
#else
#define LPRNG_FASTCALL
#endif
/* ------------------------------------------------------------------------ */

/* This implements a Tausworthe PRNG with period 2^223. Based on:
 **   Tables of maximally-equidistributed combined LFSR generators,
 **   Pierre L'Ecuyer, 1991, table 3, 1st entry.
 ** Full-period ME-CF generator with L=64, J=4, k=223, N1=49.
 */
#define U64x(hi, lo)	(((uint64_t)0x##hi << 32) + (uint64_t)0x##lo)
/* PRNG state. */
struct RandomState {
	uint64_t gen[4]; /* State of the 4 LFSR generators. */
	int valid; /* State is valid. */
};

/* Union needed for bit-pattern conversion between uint64_t and double. */
typedef union {
	uint64_t u64;
	double d;
} U64double;

/* Update generator i and compute a running xor of all states. */
/*
 */
 #define TW223_GEN(i, k, q, s,rs) \
  z = rs->gen[i]; \
  z = (((z<<q)^z) >> (k-s)) ^ ((z&((uint64_t)(int64_t)-1 << (64-k)))<<s); \
  r ^= z; rs->gen[i] = z;
/* PRNG step function. Returns a double in the range 1.0 <= d < 2.0. */
uint64_t LPRNG_FASTCALL lj_math_random_step(struct RandomState *rs) {
	uint64_t z, r = 0;
	TW223_GEN(0, 63, 31, 18, rs);
	TW223_GEN(1, 58, 19, 28, rs);
	TW223_GEN(2, 55, 24, 7, rs);
	TW223_GEN(3, 47, 21, 8, rs);
	return (r & U64x(000fffff, ffffffff)) | U64x(3ff00000, 00000000);
}

/* PRNG initialization function. */
static void random_init(struct RandomState *rs, double d) {
	uint32_t r = 0x11090601; /* 64-k[i] as four 8 bit constants. */
	int i;
	for (i = 0; i < 4; i++) {
		U64double u;
		uint32_t m = 1u << (r & 255);
		r >>= 8;
		u.d = d = d * 3.14159265358979323846 + 2.7182818284590452354;
		if (u.u64 < m) u.u64 += m; /* Ensure k[i] MSB of gen[i] are non-zero. */
		rs->gen[i] = u.u64;
	}
	rs->valid = 1;
	for (i = 0; i < 10; i++)
		lj_math_random_step(rs);
}

static int random(lua_State *L) {
	int n = lua_gettop(L);
	int upVindex = lua_upvalueindex(1);
	struct RandomState *rs = lua_touserdata(L, upVindex);
	U64double u;
	double d;
	if ((!rs->valid))
		random_init(rs, 0.0);
	u.u64 = lj_math_random_step(rs);
	d = u.d - 1.0;
	if (n > 0) {
		double r1 = lua_tonumber(L, 1);
		if (n == 1) {
			d = l_mathop(floor)((d * r1) + 1.0); /* d is an int in range [1, r1] */
		} else {
			double r2 = lua_tonumber(L, 2);
			d = l_mathop(floor)((d * (r2 - r1 + 1.0)) + r1); /* d is an int in range [r1, r2] */
		}
	} /* else: d is a double in range [0, 1] */
	lua_pushnumber(L, d);
	return 1;
}
static int randomseed(lua_State *L) {
	int upVindex = lua_upvalueindex(1);
	struct RandomState *rs = lua_touserdata(L, upVindex);
	random_init(rs, lua_tonumber(L, 1));
	return 0;
}
static const struct luaL_Reg prng_funcs[] = { { "randomseed", randomseed },
		{ "random", random }, { NULL, NULL } };

LUALIB_API int luaopen_LPRNG(lua_State *L) {
    luaL_newlibtable(L,prng_funcs);
	struct RandomState *rs;
	rs = (struct RandomState *) lua_newuserdata(L, sizeof(struct RandomState));
	rs->valid = 0; /* Use lazy initialization to save some time on startup. */
	luaL_setfuncs(L, prng_funcs, 1);
	return 1;
}

