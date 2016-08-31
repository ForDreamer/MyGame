#include "Sprite.h"
#include "../../Main/Global.h"
#include <assert.h>

Sprite::Sprite(void)
:	HudObject()
{
	Construct();
}

Sprite::Sprite(const char* name)
:	HudObject(name)
{
	Construct();
}

Sprite::~Sprite(void)
{
	if(mpVB)
	{
		mpVB->Release();
		mpVB = NULL;
	}
	mpVertices = NULL;
}

void Sprite::Construct(void)
{
	mpVB = NULL;
	mpTexture = NULL;
	mpVertices = NULL;

	bool result = CreateVertexBuffer();
	assert(result);
}

bool Sprite::CreateVertexBuffer(void)
{
	if(FAILED(Global::GetDevice()->CreateVertexBuffer(4*sizeof(SpriteVertex), D3DUSAGE_POINTS|D3DUSAGE_WRITEONLY, 
		SPRITE_FVF, D3DPOOL_DEFAULT, &mpVB, NULL)))
	{
		MessageBox(NULL, "Sprite CreateVertexBuffer Failed", "Error", MB_ICONERROR);
		return false;
	}
	if(FAILED(mpVB->Lock(0, 0, (void**)&mpVertices, 0)))
	{
		MessageBox(NULL, "Sprite pVB->Lock Failed", "Error", MB_ICONERROR);
		return false;
	}

	mpVertices[0] = SpriteVertex(-1.0f, 1.0f, 0.0f, 0xffffffff, 0.0f, 0.0f);
	mpVertices[1] = SpriteVertex(1.0f, 1.0f, 0.0f, 0xffffffff, 1.0f, 0.0f);
	mpVertices[2] = SpriteVertex(-1.0f, -1.0f, 0.0f, 0xffffffff, 0.0f, 1.0f);
	mpVertices[3] = SpriteVertex(1.0f, -1.0f, 0.0f, 0xffffffff, 1.0f, 1.0f);

	mpVB->Unlock();

	return true;
}

float Sprite::Clamp(float value)
{
	if(value > 1.0f)
	{
		value = 1.0f;
	}
	else if(value < 0.0f)
	{
		value = 0.0f;
	}

	return value;
}

void Sprite::UpdateColor(void)
{
	mState.color.r = Clamp(mState.color.r);
	mState.color.g = Clamp(mState.color.g);
	mState.color.b = Clamp(mState.color.b);
	mState.color.a = Clamp(mState.color.a);

	BYTE br = BYTE(mState.color.r * 255);
	BYTE bg = BYTE(mState.color.g * 255);
	BYTE bb = BYTE(mState.color.b * 255);
	BYTE ba = BYTE(mState.color.a * 255);

	DWORD color;
	color = (DWORD)(((BYTE)(bb))|((WORD)(bg)<<8)|((DWORD)(br)<<16)|((DWORD)(ba)<<24));

	for(int i=0; i<4; i++)
	{
		mpVertices[i].color = color;
	}
}

void Sprite::UpdateSize(void)
{
	mpVertices[0].x = -mState.size.x/2.0f + 0.5f;
	mpVertices[0].y = mState.size.y/2.0f - 0.5f;
	mpVertices[1].x = mState.size.x/2.0f - 0.5f;
	mpVertices[1].y = mState.size.y/2.0f - 0.5f;
	mpVertices[2].x = -mState.size.x/2.0f + 0.5f;
	mpVertices[2].y = -mState.size.y/2.0f + 0.5f;
	mpVertices[3].x = mState.size.x/2.0f - 0.5f;
	mpVertices[3].y = -mState.size.y/2.0f + 0.5f;
}

void Sprite::UpdateAnimator(void)
{
	assert(mState.animator.frameID >=1 && mState.animator.frameID <= mState.animator.frameMax);

	int topbase = (mState.animator.frameID - 1) / mState.animator.uPartitionNum;
	float left = (float)(mState.animator.frameID - 1) / (float)(mState.animator.uPartitionNum);
	float right = (float)(mState.animator.frameID) / (float)(mState.animator.uPartitionNum);
	float top = (float)(topbase) / (float)(mState.animator.vPartitionNum);
	float bottom = (float)(topbase + 1) / (float)(mState.animator.vPartitionNum);

	mpVertices[0].u = left;
	mpVertices[0].v = top;
	mpVertices[1].u = right;
	mpVertices[1].v = top;
	mpVertices[2].u = left;
	mpVertices[2].v = bottom;
	mpVertices[3].u = right;
	mpVertices[3].v = bottom;
}

void Sprite::UpdateWorldMatrix(void)
{
	D3DXMATRIX matTrans, matRot, matScale;
	D3DXMatrixScaling(&matScale, mState.scale.x, mState.scale.y, 1.0f);
	D3DXMatrixRotationZ(&matRot, mState.rotation);
	if(mState.alignmode == ALIGNMODE_LEFTTOP)
	{
		float halfwidth = GetWidth()*0.5f;
		float halfheight = GetHeight()*0.5f;
		D3DXMatrixTranslation(&matTrans, mState.position.x + halfwidth, mState.position.y + halfheight, mState.position.z);
	}
	else if(mState.alignmode == ALIGNMODE_CENTER)
	{
		D3DXMatrixTranslation(&matTrans, mState.position.x ,mState.position.y, mState.position.z);
	}

	mMatWorld = matScale * matRot * matTrans;
}

void Sprite::Update(void)
{
	if(mState.animator.enable)
	{
		mState.animator.frameAdvance += mState.animator.frameAdvanceSpeed * Global::GetTimeDelta();
		if(mState.animator.frameAdvance >= 1.0f)
		{
			mState.animator.frameAdvance = 0.0f;
			mState.animator.frameID++;
			if(mState.animator.frameID > mState.animator.frameMax)
			{
				if(mState.animator.mode == ANIM_LOOP)
					mState.animator.frameID = 1;
				else if(mState.animator.mode == ANIM_ONCE)
				{
					mState.animator.frameID = 1;
					mState.animator.enable = false;
					SetVisible(false);
				}
			}
			UpdateAnimator();
		}
	}
}

void Sprite::Render(void)
{
	if(mState.visible && mState.color.a > 0.0f)
	{
		PreRender();
			Global::GetDevice()->SetTransform(D3DTS_WORLD, &mMatWorld);
			Global::GetDevice()->SetTexture(0, mpTexture);
			Global::GetDevice()->SetStreamSource(0, mpVB, 0, sizeof(SpriteVertex));
			Global::GetDevice()->SetFVF(SPRITE_FVF);
			Global::GetDevice()->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
		PostRender();
	}
}

// To Do: sort sprites by render caps
void Sprite::PreRender(void)
{
	if(mState.blendmode == BLENDMODE_NONE)
	{
		Global::GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	}
	else if(mState.blendmode == BLENDMODE_ONE_MINUS_SRCALPHA)
	{
		Global::GetDevice()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		Global::GetDevice()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		Global::GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
		Global::GetDevice()->SetRenderState(D3DRS_ZWRITEENABLE, false);
	}
	else if(mState.blendmode == BLENDMODE_DEST_ONE)
	{
		Global::GetDevice()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		Global::GetDevice()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		Global::GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
		Global::GetDevice()->SetRenderState(D3DRS_ZWRITEENABLE, false);
	}
}

void Sprite::PostRender(void)
{
	Global::GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	Global::GetDevice()->SetRenderState(D3DRS_ZWRITEENABLE, true);
}