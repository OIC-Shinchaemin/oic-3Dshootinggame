#pragma once

#include "Mof.h"

// 移動可能範囲の半分
#define FIELD_HALF_X (9.0f)
#define FIELD_HALF_Z (14.0f)

/********************** Player **********************/
// 移動速度
#define PLAYER_SPEED		(0.1f)

enum PlayerMove {
	IDLE,
	LEFT,
	RIGHT,	
};

/********************** Stage **********************/
// スクロール速度
#define		SCROLL_SPEED	(1.0f)

