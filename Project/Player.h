#pragma once

#include "GameDefine.h"

// ˆÚ“®‘¬“x
#define PLAYER_SPEED		(0.1f)

class CPlayer{
private:
	CMeshContainer	m_Mesh;
	CVector3		m_Pos;
	float			m_RotZ;
	bool			m_bMove;
public:
	CPlayer();
	~CPlayer();
	bool Load();
	void Initialize();
	void Update();
	void Render();
	void RenderDebugText();
	const CVector3 GetPosition(){ return m_Pos; }
	void Release();
	bool IsMove() { return m_bMove; }
};