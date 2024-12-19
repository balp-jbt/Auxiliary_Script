package sm3

import (
	"fmt"
	"testing"
)

func Test_1(t *testing.T) {
	ori_str := []byte("abc")
	hash := SM3Hash(ori_str)
	s := fmt.Sprintf("%x", hash)
	fmt.Println(s)
	if s != "66c7f0f462eeedd9d1f2d46bdc10e4e24167c4875cf2f7a2297da02b8f4ba8e0" {
		t.Errorf("Err in Test_1")
	}
}
