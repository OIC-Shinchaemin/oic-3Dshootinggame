#pragma once

#include "Mof.h"

// 移動可能範囲の半分
#define FIELD_HALF_X (9.0f)
#define FIELD_HALF_Z (14.0f)

/********************** Player **********************/
// 移動速度
#define PLAYER_SPEED		(0.1f)

// 移動速度
#define PLAYERSHOT_SPEED	(0.4f)

// 弾の発射限界数
#define PLAYERSHOT_COUNT (40)

// 弾の発射間隔
#define PLAYERSHOT_WAIT (5)

enum PlayerMove {
	IDLE,
	LEFT,
	RIGHT,
};

enum PlayerShotMode {
	MODE_SINGLE,
	MODE_DOUBLE,
	MODE_TRIPPLE,
};

/********************** Stage **********************/
// スクロール速度
#define		SCROLL_SPEED	(1.0f)

// 敵
#define ENEMY_COUNT			(20)
#define ENEMY_MAX_HP		(5)
#define ENEMYSHOT_COUNT		(200)

// イージングの種類
enum EASING_TYPE {
	EASE_LINEAR,
	EASE_IN_SINE,
	EASE_OUT_SINE,
	EASE_INOUT_SINE,
};
// アニメーション用構造体
struct ANIM_DATA {
	float Time;
	float Value;
	EASING_TYPE EasingType;
};
float InterpolationAnim(float AnimTime, ANIM_DATA* AnimData, int cnt);