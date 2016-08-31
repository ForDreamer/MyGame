#ifndef _ItemMgr_H_
#define _ItemMgr_H_

#include "../../Engine/Misc/Singleton.h"

enum ItemID
{
	ITEM_INVALID = -1,
	ITEM_TOWEL = 0,
	ITEM_COIN = 1,
	ITEM_HP = 2,
	ITEM_BOMB = 3,
	ITEM_FIRE = 4,
	ITEM_MAX
};

static int sItemCostList[ITEM_MAX] = {0,0,10,5,20};

#define ITEM_NAME_MAX 8
class Item
{
public:
	Item(ItemID id);
	~Item(void);

	ItemID GetID(void) { return mItemId; }
	char* GetName(void) { return mName; }
	int GetCost(void) { return mCost; }

private:
	ItemID mItemId;
	char mName[ITEM_NAME_MAX];
	int mCost;
};

//////////////////////////////////////////////////////////////////////////////////////////////////
#define MAX_ITEM_NUM 8
class ItemMgr: public Singleton<ItemMgr>
{
public:
	ItemMgr(void);
	~ItemMgr(void);

	void Reset(void);

	bool IsItemFull(void) { return mItemNum == MAX_ITEM_NUM; }
	Item* GetItem(int pos) { return mItems[pos]; }
	int GetItemNum(void) { return mItemNum; }
	
	Item* AddItem(ItemID itemId);
	void RemoveItem(int pos);
	
private:
	Item* mItems[MAX_ITEM_NUM];
	int mItemNum;
};
#define gItemMgr ItemMgr::GetInstance()

#endif