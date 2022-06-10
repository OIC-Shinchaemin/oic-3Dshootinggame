#pragma once

#include "Mof.h"

// �ړ��\�͈͂̔���
#define FIELD_HALF_X (9.0f)
#define FIELD_HALF_Z (14.0f)

/********************** Player **********************/
// �v���C���[�̃��C�t
#define PLAYER_MAX_LIFE		(5)

// �ړ����x
#define PLAYER_SPEED		(0.1f)

// �ړ����x
#define PLAYERSHOT_SPEED	(0.4f)

// �e�̔��ˌ��E��
#define PLAYERSHOT_COUNT	(40)

// �e�̔��ˊԊu
#define PLAYERSHOT_WAIT		(5)

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
// �X�N���[�����x
#define		SCROLL_SPEED	(1.0f)

/********************** Enemy **********************/
// �G
#define ENEMY_MAX_COUNT		(20)
#define ENEMY_COUNT			(20)

#define ENEMY_MAX_HP		(5)
#define ENEMYSHOT_COUNT		(200)

// �C�[�W���O�̎��
enum EASING_TYPE {
	EASE_LINEAR,
	EASE_IN_SINE,
	EASE_OUT_SINE,
	EASE_INOUT_SINE,
};
// �A�j���[�V�����p�\����
struct ANIM_DATA {
	float Time;
	float Value;
	EASING_TYPE EasingType;
};

// �G�̏o�����\����
struct ENEMYSTART {
	int Count;
	float* PosX;
	float* Scroll;
	ANIM_DATA* AnimY;
	ANIM_DATA* AnimZ;
};

float InterpolationAnim(float AnimTime, ANIM_DATA* AnimData, int cnt);