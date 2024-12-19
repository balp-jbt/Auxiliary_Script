package main

import (
	"applied-cryptography-2024-zxb-ssl/sm3"
	"fmt"
)

func main() {
	message := []byte("abc")
	hash := sm3.SM3Hash(message)
	s := fmt.Sprintf("%x", hash)

	if s != "66c7f0f462eeedd9d1f2d46bdc10e4e24167c4875cf2f7a2297da02b8f4ba8e0" {
		panic("Err in Test_1")
	} else {
		fmt.Printf("PASS Test_1\n")
	}

	message = []byte("abcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcd")
	hash = sm3.SM3Hash(message)
	s = fmt.Sprintf("%x", hash)
	if s != "debe9ff92275b8a138604889c18e5a4d6fdb70e5387e5765293dcba39c0c5732" {
		panic("PASS Test_2")
	} else {
		fmt.Printf("PASS Test_2\n")
	}

	message = []byte("abcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabc")
	hash = sm3.SM3Hash(message)
	s = fmt.Sprintf("%x", hash)
	if s != "952eb84cacee9c10bde4d6882d29d63140ba72af6fe485085095dccd5b872453" {
		panic("PASS Test_3")
	} else {
		fmt.Printf("PASS Test_3\n")
	}

	message = []byte("abcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcd")
	hash = sm3.SM3Hash(message)
	s = fmt.Sprintf("%x", hash)
	if s != "90d52a2e85631a8d6035262626941fa11b85ce570cec1e3e991e2dd7ed258148" {
		panic("PASS Test_4")
	} else {
		fmt.Printf("PASS Test_4\n")
	}

	message = []byte("abcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabc")
	hash = sm3.SM3Hash(message)
	s = fmt.Sprintf("%x", hash)
	if s != "e1c53f367a9c5d19ab6ddd30248a7dafcc607e74e6bcfa52b00e0ba35e470421" {
		panic("PASS Test_5")
	} else {
		fmt.Printf("PASS Test_5\n")
	}

	message = []byte("abcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabc")
	hash = sm3.SM3Hash(message)
	s = fmt.Sprintf("%x", hash)
	if s != "520472cafdaf21d994c5849492ba802459472b5206503389fc81ff73adbec1b4" {
		panic("PASS Test_6")
	} else {
		fmt.Printf("PASS Test_6\n")
	}
}
