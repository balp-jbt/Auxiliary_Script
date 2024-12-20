package sm3

import (
	"fmt"
	"testing"
)

func Test_1(t *testing.T) {
	ori_str := []byte("abc")
	hash := Sum(ori_str)
	s := fmt.Sprintf("%x", hash)
	if s != "66c7f0f462eeedd9d1f2d46bdc10e4e24167c4875cf2f7a2297da02b8f4ba8e0" {
		t.Errorf("Err in Test_1")
	}
}

func Test_2(t *testing.T) {
	// Single calculation
	data := []byte("abc")
	hash := Sum(data)
	hash_s := fmt.Sprintf("%x", hash)
	if hash_s != "66c7f0f462eeedd9d1f2d46bdc10e4e24167c4875cf2f7a2297da02b8f4ba8e0" {
		t.Errorf("Err in Test_2_1")
	}

	// Incremental computation
	ctx := NewSM3Context()
	ctx.Update([]byte("abcdabcdabcdabcdabcdabcdabcdabcd"))
	ctx.Final()
	ctx.Update([]byte("abcdabcdabcdabcdabcdabcdabcdabcd"))
	hash = ctx.Final()
	hash_s = fmt.Sprintf("%x", hash)
	if hash_s != "debe9ff92275b8a138604889c18e5a4d6fdb70e5387e5765293dcba39c0c5732" {
		t.Errorf("Err in Test_2_2")
	}

	// Reset
	ctx.Reset()
	ctx.Update([]byte("abcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabc"))
	hash = ctx.Final()
	hash_s = fmt.Sprintf("%x", hash)
	if hash_s != "520472cafdaf21d994c5849492ba802459472b5206503389fc81ff73adbec1b4" {
		t.Errorf("Err in Test_2_3")
	}

	// Special length = 56 * 8(padding > 512)
	special_str := "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcd"
	ctx.Reset()
	ctx.Update([]byte(special_str))
	hash = ctx.Final()
	hash_s = fmt.Sprintf("%x", hash)
	if hash_s != "95d341bf7ce7330028fb844e9cdb047cd9a7bc46e19f63cc12b54f1c557ed668" {
		t.Errorf("Err in Test_2_3")
	}
}

func Test_3(t *testing.T) {
	// longstr

	joke_1 := `
	从前有一个年轻人叫小明，他是一个普通的上班族，每天都为生活奔波，过着平凡而枯燥的日子。他每天早上都挤地铁，晚上加班到很晚，但他有一个爱好——听笑话。不过，他总觉得那些笑话都不够好笑，于是他下定决心，有朝一日，他一定要创造一个“笑破天际”的笑话。

	一天，小明因为太累在地铁上睡着了，等他醒来时，发现已经坐过站了。站台上空无一人，地铁广播在回荡：“请不要把你的人生搞得这么累。下班了就早点回家吧！”小明哭笑不得，下定决心这一天要给自己找点乐子。他走到站外发现一家老旧的酒吧，门上挂着一个牌子：“笑话酒吧——讲一个笑话，赢一杯酒。”

	小明一听，眼睛都亮了。自己不是正想找机会讲笑话吗？他推开门走了进去，发现里面坐着五花八门的人，有西装革履的上班族，有穿着奇装异服的艺术家，还有几个神秘兮兮的老者。吧台后面站着一个大胡子的老板，正在用一副“你敢不敢”的眼神看着进来的小明。

	大胡子老板说：“小伙子，想喝酒吗？很简单，只要你讲的笑话能逗乐我们这儿最不爱笑的人，就能赢得一杯免费的酒。不过，要是没讲好，那就得给大家唱首歌！”

	小明犹豫了一下，看了看旁边的“最不爱笑的人”。这人戴着墨镜，双臂抱胸，面无表情，看起来像个退休的黑社会老大。小明咽了咽口水，心想：“我可是从小看笑话长大的，这有什么难的！”

	于是，他走上前清了清嗓子，说：“大家好，我是第一次来，我来讲一个我自己编的笑话——”

	他深吸一口气，开始讲：“有一天，一只乌龟走进了一家银行，对柜员说：‘我要存钱！’柜员看了它一眼，说：‘您要存多少呢？’乌龟想了想，说：‘五毛！’柜员愣了一下，说：‘就五毛？’乌龟点点头：‘对，五毛！’柜员忍不住问：‘那您为什么还跑这么远来银行呢？’乌龟说：‘我妈说了，钱要放在安全的地方！’”

	小明讲完后，全场静默。
	
	`

	joke_2 := `
	那位最不爱笑的人依旧面无表情，整个酒吧安静得只听得见冰块在酒杯里晃动的声音。小明心里有点慌了，但还是硬着头皮对自己说：“没事，笑话这东西，是需要点后劲的。”

	这时，大胡子老板冷冷地说：“这……也太冷了吧，来吧，按照规则，唱一首歌。”

	小明想拒绝，但规则就是规则，他只好站起来唱了一首《小星星》。他唱得五音不全，全场哄堂大笑，甚至连那个最不爱笑的人嘴角都忍不住抽动了一下。

	大胡子老板看着小明说：“小伙子，你这个笑话不行啊，要不要再试一次？”

	小明不甘心，决定再拼一次。他掏出手机查了一下网上的经典笑话，选了一个他觉得最搞笑的。于是，他又站了起来，说：“刚才那个可能不太适合大家的口味，我再来一个！

	“有一个人去买饼，对老板说：‘老板，来个饼，少放点芝麻。’老板说：‘行！’结果拿到手一看，饼上就放了一颗芝麻。那人不乐意了，说：‘我说少放芝麻，可没说让我找芝麻啊！’”

	讲完后，全场再次陷入沉默，最不爱笑的人甚至摘下了墨镜，用一种“不屑”的眼神看着他。小明彻底崩溃了，叹了口气说：“算了，我放弃吧，酒我也不喝了，我还是回家吧。”

	就在小明转身要走的时候，那个最不爱笑的人突然开口了：“小伙子，你刚才说饼的那个笑话，其实有点意思。”小明一听，惊喜地回头：“真的？！”那人点了点头，但接着又补了一句：“不过我不是笑，是饿了。你要是再讲个和吃的有关的笑话，我就请你喝酒。”

	小明瞬间有了信心。他想了想，又讲了一个关于吃的笑话：“有一天，一块披萨走在街上，它突然觉得很热，就跑进了一家理发店。理发师问它：‘你怎么了？’披萨说：‘没事，就是想剪点儿发热量！’”

	这次，全场哄堂大笑，连最不爱笑的人也终于憋不住了，哈哈大笑起来。大胡子老板拍拍小明的肩膀，说：“可以啊，小伙子，这杯酒你配得上了！”

	小明拿着酒杯，心里美滋滋的。他离开酒吧的时候，发现月亮挂在天上，夜风凉爽。他想：原来，让别人笑，也是一种快乐啊。

	自那天起，小明决定每天讲一个笑话，记录下生活中的小欢乐。后来，他居然成了一个知名的脱口秀演员。而每次采访有人问他为什么要讲笑话时，他都会微笑着说：“因为人生已经够苦了，多点笑声，总归是好事。”
	`

	ctx := NewSM3Context()
	ctx.Update([]byte(joke_1))

	hash := ctx.Final()
	hash_s := fmt.Sprintf("%x", hash)
	if hash_s != "95405487021d0b3ca08d177beb941a6b68079fb74522382dddf77ef216e9eca3" {
		fmt.Println(hash_s)
		t.Errorf("Err in Test_2_4")
	}

	ctx.Update([]byte(joke_2))
	hash = ctx.Final()
	hash_s = fmt.Sprintf("%x", hash)
	if hash_s != "5200d24f93e6065a23692cfe05cadc207c3be628271f805a8fade39263ea97df" {
		fmt.Println(hash_s)
		t.Errorf("Err in Test_2_4")
	}
}
