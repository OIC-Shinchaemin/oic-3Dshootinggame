#pragma once

#include "GameDefine.h"
#include "PlayerShot.h"
#include "Enemy.h"

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

	bool			m_bDead;
	int				m_Life;
	bool			m_Restart;

public:
	CPlayer();
	~CPlayer();
	bool Load();
	void Initialize();
	void Update();
	void Render();
	void RenderDebug();	
	void RenderDebugText();
	const CVector3 GetPosition(){ return m_Pos; }
	void Release();
	CSphere GetSphere() { return CSphere(m_Pos, 0.4f); }
	PlayerMove GetMove() { return m_Move; }
	
	void CollisionEnemy(CEnemy& ene);	
	void CollisionEnemyShot(CEnemyShot& shot);

	bool IsDead() { return m_bDead; }
	void UpdateMode();
	void UpdateSingleMode();
	void UpdateDoubleMode();
	void UpdateTrippleMode();
};