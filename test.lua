package.cpath=package.cpath..';E:\\tmp\\LPRNG\\LPRNG\\bin\\Debug\\?.dll;';
print(package.cpath)
LPRNG=jit and math or require("LPRNG");
print(LPRNG)
for k,v in pairs(LPRNG) do
	print(k,v)
end
local seed = 1
LPRNG.randomseed(seed);
print("seed = ",seed)
for i = 1,10,1 do
	print(i,LPRNG.random())
end
print("random")