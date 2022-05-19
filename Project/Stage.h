#pragma once

#include "GameDefine.h"


class CStage{
private:
	CSprite3D			m_BG;
	float				m_Scroll;
public:
	CStage();
	~CStage();
	bool Load();
	void Initialize();
	void Update();
	void Render();
	void RenderDebugText();
	void Release();
};