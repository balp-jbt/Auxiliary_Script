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