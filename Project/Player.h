#pragma once

#include "GameDefine.h"
#include "PlayerShot.h"

class CPlayer{
private:
	CMeshContainer	m_Mesh;
	CVector3		m_Pos;
	float			m_RotZ;
	PlayerMove		m_Move;

	CMeshContainer	m_SMesh;
	CPlayerShot		m_SArray[PLAYERSHOT_COUNT];
	int				m_SWait;

	PlayerShotMode	m_SMode;

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
	PlayerMove GetMove() { return m_Move; }
	
	void UpdateMode();
	void UpdateSingleMode();
	void UpdateDoubleMode();
	void UpdateTrippleMode();
};