#include "Player.h"

/**
 * �R���X�g���N�^
 */
CPlayer::CPlayer() :
m_Mesh(),
m_Pos(0.0f,0.0f,0.0f),
m_RotZ(0.0f){
}

/**
 * �f�X�g���N�^
 */
CPlayer::~CPlayer(){
}

/**
 * �ǂݍ���
 */
bool CPlayer::Load(void){
	// ���b�V���̓ǂݍ���
	m_Mesh.Load("player.mom");
	return true;
}

/**
 * ������
 */
void CPlayer::Initialize(void){
	m_Pos = Vector3(0, 0, -FIELD_HALF_Z + 2);
	m_RotZ = 0;
	
}

/**
 * �X�V
 */
#define MAX_SKILLTIME 100
int SkillTimer = MAX_SKILLTIME;

void CPlayer::Update(void){
	float Roll = 0;
	float PlayerSpeed = PLAYER_SPEED;
	float RotSpeed = MOF_ToRadian(10);

	if (g_pInput->IsKeyHold(MOFKEY_LSHIFT) && SkillTimer > 0 ) {
		PlayerSpeed *= 4;
		RotSpeed *= 4;
		SkillTimer-= 2;
	}
	else
	{
		if (MAX_SKILLTIME > SkillTimer)
			SkillTimer++;
	}

	// ���ړ�
	if (g_pInput->IsKeyHold(MOFKEY_LEFT)) {
		m_Pos.x = max(m_Pos.x - PlayerSpeed, -FIELD_HALF_X);
		Roll -= MOF_MATH_PI;
		m_Move = PlayerMove::LEFT;
	}
	else if (g_pInput->IsKeyHold(MOFKEY_RIGHT)) {
		m_Pos.x = min(m_Pos.x + PlayerSpeed, FIELD_HALF_X);
		Roll += MOF_MATH_PI;
		m_Move = PlayerMove::RIGHT;
	}else{ m_Move = PlayerMove::IDLE; }

	if (g_pInput->IsKeyHold(MOFKEY_UP)) {
		m_Pos.z = min(m_Pos.z + PlayerSpeed, FIELD_HALF_Z);
	}
	if (g_pInput->IsKeyHold(MOFKEY_DOWN)) {
		m_Pos.z = max(m_Pos.z - PlayerSpeed, -FIELD_HALF_Z);
	}

	if (Roll == 0) {
		RotSpeed = min(abs(m_RotZ) * 0.1f, RotSpeed);
	}
		
	if (abs(m_RotZ) <= RotSpeed || signbit(m_RotZ) != signbit(Roll)) {
		m_RotZ += Roll;
	}	
	m_RotZ -= copysignf(min(RotSpeed, abs(m_RotZ)), m_RotZ);
	
}

/**
 * �`��
 */
void CPlayer::Render(void){
	CMatrix44 matWorld;
	matWorld.RotationZ(m_RotZ);
	matWorld.SetTranslation(m_Pos);
	m_Mesh.Render(matWorld);

	for (int i = 0; i < SkillTimer; i++) {
		CGraphicsUtilities::RenderString(800 + i * 2, 0, MOF_COLOR_YELLOW,"|");
	}
}

/**
 * �f�o�b�O�����`��
 */
void CPlayer::RenderDebugText(void){
	// �ʒu�̕`��
	CGraphicsUtilities::RenderString(10,40,MOF_XRGB(0,0,0),
			"�v���C���[�ʒu X : %.1f , Y : %.1f , Z : %.1f",m_Pos.x,m_Pos.y,m_Pos.z);
	
	
}

/**
 * ���
 */
void CPlayer::Release(void){
	m_Mesh.Release();
}