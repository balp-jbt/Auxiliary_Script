a1 = 0xefecdac3ffffffff
a2 = 0x65acee1affffffff
a3 = 0xffec998affffffff

a = 0xefecdac3ffffffff65acee1affffffffffec998affffffff
b = 0x0000000000000000ffffffffffffffffffffffffffffffff
print(hex(a+b-0xefecdac40000000065acee1affffffffffec998afffffffe))
print(hex(a*b-0xefecdac3ffffffff65acee1affffffff0fffbec6ffffffff9a5311e5000000000013667500000001))



c = 0xffffffffffffffff
d = 0xffffffffffffffff
print(hex(c+d-0x0000000000000001fffffffffffffffe))
print(hex(c*d-0xfffffffffffffffe0000000000000001))

#c  0xfffffffffffffffe
#py 0x1fffffffffffffffe
#0x100000000000000020000000000000001,,

a1 = 0xffffffffffffffff
a2 = 0xffffffffffffffff
a3 = 0xffffffffffffffff
a = 0xffffffffffffffffffffffffffffffff
b = 0xffffffffffffffffffffffffffffffff
#print(hex(a*b))
print(hex(a*b-0xfffffffffffffffffffffffffffffffe00000000000000000000000000000001))


# 0xffff/ffff/ffff/ffff_ffff/ffff/ffff/fffe_0000/0000/0000/0000_0000/0000/0000/0001
# 0xffff/ffff/ffff/fffe_ffff/ffff/ffff/fffd_0000/0000/0000/0000_0000/0000/0000/0001

#print(hex(0xffffffffffffffff * 0xffffffffffffffff))
#print(hex(0xffffffffffffffffffffffffffffffff * 0xffffffffffffffff))

a1 = 0xffffffffffffffffffffffffffffffffffffffffffffffff
print(hex(a1 << 79))
print(hex(0x0000000000007fffffffffffffffffffffffffffffffffffffffffffffff80000000000000000000 - (a1<<79)))

0x7fffffffffffffffffffffffffffffffffffffffffffffff80000000000000000000
# 0x7fffffffffffffffffff / 0000_0000_0000_0000 / ffff_ffff_ffff_8000 / 0000_0000_0000_0000


a1 = 0xefecdac3ffffffff65acee1affffffffffec998affffffff
print(hex(0x00000000000077f66d61ffffffffb2d6770d7ffffffffff64cc57fffffff80000000000000000000-(a1<<79)))

a1 = 0xffffffffffffffffffffffffffffffffffffffffffffffff
print(hex(0x1ffffffffffffffffffffffffffff - (a1>>79)))
print(hex(0x1ffffffffffffffffffffffffffff - (a1>>79)))

a1 = 0xefecdac3ffffffff65acee1affffffffffec998affffffff
print(hex(0x1dfd9b587fffffffecb59dc35ffff - (a1>>79)))
print(hex(0x1dfd9b587fffffffecb59dc35ffff - (a1>>79)))

a1 = 0xefecdac3ffffffff65acee1affffffffffec998affffffff
a2 = 0xffffffffffffffffffffffffffffffff
print(hex(0xefecdac3fffffffe65acee1affffffffffec998b00000000 - (a1-a2)))