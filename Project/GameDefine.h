#pragma once

#include "Mof.h"

// �ړ��\�͈͂̔���
#define FIELD_HALF_X (9.0f)
#define FIELD_HALF_Z (14.0f)

/********************** Player **********************/
// �ړ����x
#define PLAYER_SPEED		(0.1f)

// �ړ����x
#define PLAYERSHOT_SPEED	(0.4f)

// �e�̔��ˌ��E��
#define PLAYERSHOT_COUNT (40)

// �e�̔��ˊԊu
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
// �X�N���[�����x
#define		SCROLL_SPEED	(1.0f)

