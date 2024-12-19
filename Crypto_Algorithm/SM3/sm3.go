package sm3

import (
	"encoding/binary"
)

var IV = [8]uint32{
	0x7380166F, 0x4914B2B9, 0x172442D7, 0xDA8A0600,
	0xA96F30BC, 0x163138AA, 0xE38DEE4D, 0xB0FB0E4E,
}

var T_j_le_15 uint32 = 0x79cc4519
var T_j_gt_15 uint32 = 0x7a879d8a

func T(j int) uint32 {
	if j >= 0 && j <= 15 {
		return 0x79cc4519
	}
	if j >= 16 && j <= 63 {
		return 0x7a879d8a
	}
	panic("Panic in func T\n")
}

func FF(x uint32, y uint32, z uint32, j int) uint32 {
	if j >= 0 && j <= 15 {
		return x ^ y ^ z
	}

	return (x & y) | (x & z) | (y & z)
}

func GG(x uint32, y uint32, z uint32, j int) uint32 {
	if j < 16 {
		return x ^ y ^ z
	}
	return (x & y) | ((^x) & z)
}

func Rotate_Left_Shift(x uint32, count int) uint32 {
	count = count % 32
	return (x << count) | (x >> (32 - count))
}

func P0(x uint32) uint32 {
	return x ^ Rotate_Left_Shift(x, 9) ^ Rotate_Left_Shift(x, 17)
}

func P1(x uint32) uint32 {
	return x ^ Rotate_Left_Shift(x, 15) ^ Rotate_Left_Shift(x, 23)
}

func pad_message(message []byte) []byte {
	message_len := len(message) * 8
	remain_len := message_len % 512
	var k int

	if remain_len+1 <= 448 {
		k = 447 - remain_len
	} else {
		k = 959 - remain_len
	}

	k++
	padding_byte_len := k / 8

	// fmt.Printf("%d\n%d\n", k, padding_byte_len)

	if k%8 != 0 || padding_byte_len == 0 {
		panic("Panic in func padding\n")
	}

	message = append(message, 0x80)
	for i := 1; i < padding_byte_len; i++ {
		message = append(message, 0x00)
	}

	lengthBytes := make([]byte, 8)
	binary.BigEndian.PutUint64(lengthBytes, uint64(message_len))
	message = append(message, lengthBytes...)

	return message
}

// 68 is W, 64 is W'
func extend_message(block []byte) ([68]uint32, [64]uint32) {
	var W_1st [68]uint32
	var W_2nd [64]uint32

	for i := 0; i <= 15; i++ {
		W_1st[i] = binary.BigEndian.Uint32(block[i*4 : (i+1)*4])
	}

	for j := 16; j <= 67; j++ {
		W_1st[j] = P1(W_1st[j-16]^W_1st[j-9]^Rotate_Left_Shift(W_1st[j-3], 15)) ^
			Rotate_Left_Shift(W_1st[j-13], 7) ^ W_1st[j-6]
	}

	for j := 0; j <= 63; j++ {
		W_2nd[j] = W_1st[j] ^ W_1st[j+4]
	}

	return W_1st, W_2nd
}

func CF(V [8]uint32, block []byte) [8]uint32 {
	W_1st, W_2nd := extend_message(block)

	A, B, C, D, E, F, G, H := V[0], V[1], V[2], V[3], V[4], V[5], V[6], V[7]

	for j := 0; j < 64; j++ {
		// fmt.Printf("j=%d\n", j)
		SS1 := Rotate_Left_Shift((Rotate_Left_Shift(A, 12) + E + Rotate_Left_Shift(T(j), j)), 7)
		SS2 := SS1 ^ Rotate_Left_Shift(A, 12)
		TT1 := FF(A, B, C, j) + D + SS2 + W_2nd[j]
		TT2 := GG(E, F, G, j) + H + SS1 + W_1st[j]
		D = C
		C = Rotate_Left_Shift(B, 9)
		B = A
		A = TT1
		H = G
		G = Rotate_Left_Shift(F, 19)
		F = E
		E = P0(TT2)
	}

	var V_new [8]uint32
	for i := 0; i < 8; i++ {
		V_new[i] = V[i] ^ [8]uint32{A, B, C, D, E, F, G, H}[i]
	}
	return V_new
}

func SM3Hash(message []byte) [32]byte {
	message = pad_message(message)
	var V = IV

	for i := 0; i < len(message); i += 64 {
		block := message[i : i+64]
		V = CF(V, block)
	}

	var result [32]byte
	for i, v := range V {
		binary.BigEndian.PutUint32(result[i*4:(i+1)*4], v)
	}
	return result
}
