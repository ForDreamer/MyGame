#ifndef _DialogData_H_
#define _DialogData_H_

#define MAX_RECORDS 48

static DialogRecord sDialogRecords[MAX_RECORDS] =
{
	{1,0,0,"『 欢迎来到多拉镇！<@『 那不是修理店<    的孩子吗？<@",0,0},
	{2,0,0,"『 传闻说镇子南边的<    山洞里埋着辆战车。<@『 想要那辆战车的勇士<    都集中到镇子里了。<@",0,0},
	{2,1,2,"『 山洞里的战车是<    一个叫红狼的<    勇士发现的。<@",0,0},
	{3,0,0,"『 见过住在那边的<    明齐博士吗？<@",1,2},
	{3,1,-1,"『 是吗！<    感觉如何？<@",0,0},

	{3,2,-1,"『 他好象专门研究使<    死人复生的方法。<@",0,0},
	{4,0,0,"『 那位了不起的勇士<    好象来到镇子上了。<@",0,0},
	{4,1,2,"『 出色的勇士都到<    北方去了。<@",0,0},
	{5,0,0,"『 战车不能开进镇子！<    只要这规定还在。<@『 即使是勇士<    也不例外！<@",0,0},
	{6,0,0,"『 这里是探险家们<    战斗记录的存储中心。<@『 存储游戏请选择<    模式 - 存储游戏<@",0,0},

	{6,1,-1,"『 吱.吱..<@『 存储完毕！<@",0,0},
	{6,2,-1,"『 游戏结束的时候<    在这里做一下记录。<@『 要不然就<    白辛苦了！<@",0,0},
	{7,0,0,"『 如果发现自动售货机<    就上前检查一下。<@",0,0},
	{8,0,0,"『 啊.是流浪的勇士。<    去找战车吧。<@",0,0},
	{8,1,2,"『 战车上只能坐一人。<@『 如果有了伙伴的话<    要考虑好<    谁坐战车谁步行。<@",0,0},

	{9,0,0,"『 勇士把干掉怪物<    作为赚钱的途径！<@",0,0},
	{11,0,0,"『 装备和工具<    可以分别带着<@『 如果步行去外面<    应该买一点参丸！<@",0,0},
	{12,0,0,"『 武器和防具没有装备<    等于一堆废品。<@",0,0},
	{15,0,0,"『 你听说过驾驶<    着红色战车的<    勇士的传闻吗？<@",0,0},
	{16,0,0,"『 那辆被鲜血染红的战车<@『 连勇士们都感到畏惧..<@『 他是位赏金杀手<    名叫红狼。<@",0,0},

	{17,0,0,"『 人活着的目的？<    你自己去找答案吧！<@",0,0},
	{18,0,0,"『 红狼到这里来了？<@『 水怪兄弟<    快要完蛋啦！<@",0,0},
	{19,0,0,"『 你看那被战火<    毁坏的世界！<@",0,0},
	{21,0,0,"『 想要成为勇士.还<    得靠战车帮助啊！<@『 连战车也没有<    谈什么做勇士！<@",0,0},
	{23,0,0,"『 治疗麻木的气功按摩<    要试一下吗？<@",0,0},

	{24,0,0,"『 损坏的战车能<    被牵引走。<@",0,0},
	{25,0,0,"『 这个村子遭受过<    怪物的掠夺。<@『 村子南边的山洞<    是他们的老巢！<@",0,0},
	{26,0,0,"『 这里以前叫<    时空隧道.<@『 可以把人带到<    很远的地方。<@",0,0},
	{27,0,0,"『 我是天才科学家<    明齐博士.<@『 看到完好的尸体就<    拿到我这儿来。<@",0,0},
	{29,0,0,"『 没法救.没法救。<@",0,0},

	{30,0,0,"『 这里是勇士财<    产保管处！<@",0,0},
	{32,0,0,"『 你说什么？<    想当勇士！<@『 算了吧.<    根本不可能。<@",0,0},
	{33,0,0,"爸爸『 怎么？是你呀！<    想回家了？<@",1,2},
	{33,1,-1,"爸爸『 你呀！真让我感动.<    明白吗？<@",0,0},
	{33,2,-1,"爸爸『 那就自己去<    搞一辆战车。<@",0,0},

	{34,0,0,"『 这里是废弃<    铁屑处理场！<@",0,0},
	{36,0,0,"『 你也是来寻找<    战车的吗？<@",1,2},
	{36,1,-1,"『 那个有名的红狼<    似乎潜到这里来了。<@『 快去找战车吧。<@",0,0},
	{36,2,-1,"『 嘿嘿..<    忘掉刚才我说过的话！<@",0,0},
	{36,3,2,"『 战车好象已经<    被人发现了。<@",0,0},

	{37,0,0,"『 能给我讲讲以<    前的故事吗？<@",0,0},
	{38,0,0,"『 如果要找同伴的话.<    就一直往东到波布<    镇去看看。<@",0,0},
	{39,0,0,"『 打算从这儿往<    前去吗？<@",1,2},
	{39,1,-1,"『 没带战车<    不能前进！<@",0,0},
	{39,2,-1,"『 那么回镇子好吗？<@",0,0},

	{39,3,2,"『 打算从这儿往<    前去吗？<@",4,2},
	{39,4,-1,"『 那么过去吧！<@    小心点.前面的怪物<    很凶。<@",0,0},
	{40,0,0,"『 这里是关卡<    我是看守。<@",0,0}
};

#endif