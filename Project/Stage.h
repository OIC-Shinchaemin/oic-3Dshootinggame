#pragma once

#include "GameDefine.h"
#include "Enemy.h"

// ìGÇÃèoåªèÓïÒç\ë¢ëÃ
typedef struct tag_ENEMYSTART {
	int Count;
	float* PosX;
	float* Scroll;
}ENEMYSTART;

class CStage{
private:
	CSprite3D			m_BG;
	float				m_Scroll;
	ENEMYSTART*			m_pEnemyStart;
	CMeshContainer		m_EnemyMesh;
	int					m_EnemyNo;
public:
	CStage();
	~CStage();
	bool Load();
	void Initialize(ENEMYSTART* pSt);
	void Update(CEnemy* ene, int ecnt);
	void Render();
	void RenderDebugText();
	void Release();
};