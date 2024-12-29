package sm3

import (
	"encoding/binary"
)

type SM3Context struct {
	V         [8]uint32
	buffer    []byte
	total_len uint64
}

func NewSM3Context() *SM3Context {
	return &SM3Context{
		V:         IV,
		buffer:    make([]byte, 0, 128),
		total_len: 0,
	}
}

func (ctx *SM3Context) Update(data []byte) {
	if len(data) == 0 {
		return
	}

	ctx.total_len += uint64(len(data) * 8)

	// While the current data >= 512 bit, it can be CF preliminarily
	// First handle the left content
	if len(ctx.buffer) > 0 {
		spaceLeft := 64 - len(ctx.buffer)
		if len(data) >= spaceLeft {
			ctx.buffer = append(ctx.buffer, data[:spaceLeft]...)
			ctx.V = CF(ctx.V, ctx.buffer)
			data = data[spaceLeft:]
			ctx.buffer = ctx.buffer[:0]
		} else {
			ctx.buffer = append(ctx.buffer, data...)
			return
		}
	}

	// Second divided it by 64Byte
	for len(data) >= 64 {
		ctx.V = CF(ctx.V, data[:64])
		data = data[64:]
	}

	// Left ones
	if len(data) > 0 {
		ctx.buffer = append(ctx.buffer, data...)
	}
}

// Get Hash
func (ctx *SM3Context) Final() [32]byte {
	// [NOTE] In Go language, this is a Shallow Copy, NOT ptr assignment
	final_ctx := *ctx

	message_len := final_ctx.total_len
	remain_len := message_len % 512
	final_ctx.buffer = append(final_ctx.buffer, 0x80)

	var k int
	if remain_len+1 <= 448 {
		k = 448 - (int(remain_len) + 1)
	} else {
		k = 960 - (int(remain_len) + 1)
	}
	k++
	padding := make([]byte, k/8-1)
	final_ctx.buffer = append(final_ctx.buffer, padding...)

	length_bits := make([]byte, 8)
	binary.BigEndian.PutUint64(length_bits, message_len)
	final_ctx.buffer = append(final_ctx.buffer, length_bits...)

	for i := 0; i < len(final_ctx.buffer); i += 64 {
		final_ctx.V = CF(final_ctx.V, final_ctx.buffer[i:i+64])
	}

	var result [32]byte
	for i, v := range final_ctx.V {
		binary.BigEndian.PutUint32(result[i*4:(i+1)*4], v)
	}

	return result
}

func (ctx *SM3Context) Reset() {
	ctx.V = IV
	ctx.buffer = ctx.buffer[:0]
	ctx.total_len = 0
}

func Sum(data []byte) [32]byte {
	ctx := NewSM3Context()
	ctx.Update(data)
	return ctx.Final()
}
