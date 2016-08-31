#ifndef _Sprite_H_
#define _Sprite_H_

#include "HudObject.h"
#include <d3dx9.h>
#include <assert.h>

const int SPRITE_FVF = D3DFVF_XYZ|D3DFVF_DIFFUSE|D3DFVF_TEX1;
struct SpriteVertex
{
	SpriteVertex(float _x, float _y, float _z, DWORD _color, float _u, float _v)
	{
		x = _x; y = _y; z = _z;
		color = _color;
		u = _u; v = _v;
	}

    float x, y, z;
	DWORD color;
    float u, v;
};

class Sprite: public HudObject
{
public:
	enum BlendMode
	{
		BLENDMODE_NONE = 0,
		BLENDMODE_ONE_MINUS_SRCALPHA = 1,
		BLENDMODE_DEST_ONE = 2,
	};

	enum AlignMode
	{
		ALIGNMODE_CENTER = 0,
		ALIGNMODE_LEFTTOP = 1,
	};

	enum AnimMode
	{
		ANIM_LOOP = 0,
		ANIM_ONCE = 1
	};

	struct SpriteState
	{
		SpriteState(void)
		{
			position.x = position.y = position.z = 0.0f;
			size.x = size.y = 1.0f;
			scale.x = scale.y = 1.0f;
			color.r = color.g = color.b = color.a = 1.0f;
			animator.uPartitionNum = animator.vPartitionNum = 1;
			animator.frameMax = animator.frameID = 1;
			animator.frameAdvance = 0.0f;
			animator.frameAdvanceSpeed = 1.0f;
			animator.enable = false;
			animator.mode = ANIM_LOOP;
			rotation = 0.0f;
			blendmode = BLENDMODE_NONE;
			alignmode = ALIGNMODE_CENTER;
			visible = true;
		}

		struct
		{
			float x;
			float y;
			float z;
		} position;

		struct
		{
			float x;
			float y;
		} size;

		struct
		{
			float x;
			float y;
		} scale;

		struct
		{
			float r;
			float g;
			float b;
			float a;
		} color;

		struct
		{	
			int uPartitionNum;
			int vPartitionNum;
			int frameMax;
			int frameID;
			float frameAdvance;
			float frameAdvanceSpeed;
			bool enable;
			AnimMode mode;
		} animator;

		float rotation;
		BlendMode blendmode;
		AlignMode alignmode;
		bool visible;
	};

public:
	Sprite(void);
	Sprite(const char* name);
	virtual ~Sprite(void);

public:
	float GetPositionX(void) { return mState.position.x; }
	float GetPositionY(void) { return mState.position.y; }
	float GetWidth(void) { return mState.size.x; }
	float GetHeight(void) { return mState.size.y; }
	float GetScaleX(void) { return mState.scale.x; }
	float GetScaleY(void) { return mState.scale.y; }
	float GetRotation(void) { return mState.rotation; }
	float GetAlpha(void) { return mState.color.a; }
	bool GetVisible(void) { return mState.visible; }

	void SetPositionX(float x) { mState.position.x = x; UpdateWorldMatrix(); }
	void SetPositionY(float y) { mState.position.y = y; UpdateWorldMatrix(); }
	void SetPositionZ(float z) { mState.position.z = z; UpdateWorldMatrix(); }
	void SetWidth(float x) { mState.size.x = x; UpdateSize(); }
	void SetHeight(float y) { mState.size.y = y; UpdateSize(); }
	void SetScaleX(float x) { mState.scale.x = x; }
	void SetScaleY(float y) { mState.scale.y = y; }
	void SetRotation(float rotation) { mState.rotation = rotation; }
	void SetAlpha(float alpha) { mState.color.a = alpha; UpdateColor(); }
	void SetVisible(bool visible) { mState.visible = visible; }

	void SetBlendMode(BlendMode blend) { mState.blendmode = blend; }
	void SetAlignMode(AlignMode align) { mState.alignmode = align; }
	void SetPosition(float x, float y) { SetPositionX(x), SetPositionY(y); UpdateWorldMatrix(); }
	void SetSize(float x, float y) { SetWidth(x), SetHeight(y); }
	void SetScale(float x, float y) { SetScaleX(x), SetScaleY(y); }
	void SetColor(float r, float g, float b, float a) 
	{
		mState.color.r = r;
		mState.color.g = g;
		mState.color.b = b;
		mState.color.a = a;
		UpdateColor();
	}
	void SetAnimator(int uNum, int vNum, int frameMax, int frameCur, float speed, AnimMode mode=ANIM_LOOP)
	{
		assert(uNum>=1 && vNum>=1);
		mState.animator.uPartitionNum = uNum;
		mState.animator.vPartitionNum = vNum;
		mState.animator.frameMax = frameMax;
		mState.animator.frameID = frameCur;
		mState.animator.frameAdvanceSpeed = speed;
		mState.animator.frameAdvance = 0.0f;
		mState.animator.mode = mode;
		UpdateAnimator();
	}

	void SetTexture(IDirect3DTexture9* pTexture) { assert(pTexture); mpTexture = pTexture; }
	void PlayAnim(void) { mState.animator.enable = true; }
	void StopAnim(void) { mState.animator.enable = false; }
	void ResetAnim(void) { mState.animator.frameID = 1; mState.animator.frameAdvance = 0.0f; }

private:
	void Construct(void);
	bool CreateVertexBuffer(void);

	float Clamp(float value);
	void UpdateSize(void);
	void UpdateColor(void);
	void UpdateAnimator(void);
	void UpdateWorldMatrix(void);
	
	void PreRender(void);
	void PostRender(void);

protected:
	SpriteState mState;
	D3DXMATRIX mMatWorld;

private:
	IDirect3DVertexBuffer9* mpVB;
	IDirect3DTexture9* mpTexture;
	SpriteVertex* mpVertices;

public:
	void Update(void);
	void Render(void);
};

#endif