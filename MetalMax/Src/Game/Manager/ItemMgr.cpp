#include "ItemMgr.h"
#include <strsafe.h>

Item::Item(ItemID id)
:	mItemId(id)
{
	switch(id)
	{
		case ITEM_TOWEL: strcpy_s(mName, "Ã«½í"); break;
		case ITEM_COIN: strcpy_s(mName, "¹Å±Ò"); break;
		case ITEM_HP: strcpy_s(mName, "²ÎÍè");  break;
		case ITEM_BOMB: strcpy_s(mName, "ÊÖÀ×"); break;
		case ITEM_FIRE: strcpy_s(mName, "»ðÆ¿"); break;
	}
	mCost = sItemCostList[id];

	for(int i=4; i<ITEM_NAME_MAX; i++)
	{
		mName[i] = '\0';
	}
}

Item::~Item(void)
{
}


//////////////////////////////////////////////////////////////////////////////////////////////////
DECLARE_SINGLETON(ItemMgr);
static ItemMgr sItemMgr;

ItemMgr::ItemMgr(void)
:	mItemNum(0)
{
	for(int i=0; i<MAX_ITEM_NUM; i++)
	{
		mItems[i] = NULL;
	}
}

ItemMgr::~ItemMgr(void)
{
	Reset();
}

void ItemMgr::Reset(void)
{
	for(int i=0; i<MAX_ITEM_NUM; i++)
	{
		if(mItems[i])
		{
			delete mItems[i];
			mItems[i] = NULL;
		}
	}
	mItemNum = 0;
}

Item* ItemMgr::AddItem(ItemID itemId)
{
	if(mItemNum >= MAX_ITEM_NUM)
		return NULL;

	assert(mItems[mItemNum] == NULL);
	Item* pItem = new Item(itemId);
	mItems[mItemNum] = pItem;
	mItemNum++;

	return pItem;
}

void ItemMgr::RemoveItem(int pos)
{
	assert(pos < mItemNum);
	assert(mItems[pos]);

	delete mItems[pos];
	mItems[pos] = NULL;
	mItemNum--;

	for(int i=pos; i<MAX_ITEM_NUM-1; i++)
	{
		mItems[i] = mItems[i+1];
	}
	mItems[MAX_ITEM_NUM-1] = NULL; 
}