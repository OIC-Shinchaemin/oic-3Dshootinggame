#include "Enemy.h"

/**
 * �R���X�g���N�^
 *
 */
CEnemy::CEnemy():
m_pMesh(NULL),
m_Pos(0,0,0),
m_Rot(0,0,0),
m_bShow(false),
m_ShotWait(0),
m_ShotWaitSet(0),
m_TargetPos(0, 0, 0),
m_AnimTime(0),
m_HP(ENEMY_MAX_HP){
}

/**
 * �f�X�g���N�^
 *
 */
CEnemy::~CEnemy(){
}

/**
 * ������
 *
 */
void CEnemy::Initialize(){
	m_Pos = Vector3(0, 0, 0);
	m_Rot = Vector3(0, 0, 0);
	m_bShow = false;
	m_HP = ENEMY_MAX_HP;
	m_ShotWait = 0;
	m_ShotWaitSet = 40;
	m_TargetPos = Vector3(0, 0, 0);
	m_AnimTime = 0;
}

/**
 * �J�n
 *
 */
void CEnemy::Start(ENEMYSTART* pSt, int index){
	m_pEnemyStart = pSt;
	m_Pos = Vector3(m_pEnemyStart->PosX[index],0.0f,0.0f);
	m_Rot = Vector3(0, 0, 0);
	m_bShow = true;
	m_AnimTime = 0;
	m_ShotWait = 0;
	m_ShotWaitSet = 40;
	m_TargetPos = Vector3(0, 0, 0);
	m_HP = ENEMY_MAX_HP;
}

/**
 * �X�V
 *
 */
void CEnemy::Update(CEnemyShot* shot, int smax) {
	// ��\��
	if (!GetShow())
	{
		return;
	}
	// ���Ԃ�i�߂�
	m_AnimTime += CUtilities::GetFrameSecond();
	// �A�j���[�V����
	m_Pos.y = InterpolationAnim(m_AnimTime, m_pEnemyStart->AnimY, 4);
	m_Pos.z = InterpolationAnim(m_AnimTime, m_pEnemyStart->AnimZ, 6);
	
	// �v���C���[�Ɠ��������܂ňړ�������
	if (m_pEnemyStart->AnimY[1].Time < m_AnimTime)
	{
		// �e�̔���
		if (m_ShotWait <= 0)
		{
			CEnemyShot* newShot = CEnemyShot::FindAvailableShot(shot, smax);
			if (newShot)
			{
				m_ShotWait = m_ShotWaitSet;
				// �ڕW�n�_�Ɍ��������߂̕���
				Vector3 direction = m_TargetPos - m_Pos;
				// �ڕW�n�_�܂ł̋��������߂�
				float distance = CVector3Utilities::Length(direction);
				// ������ 0 �ȉ�=���S�ɓ����ʒu�̏ꍇ�͔��˂����Ȃ�
				if (distance > 0)
				{
					// �����𐳋K��
					direction /= distance;
					newShot->Fire(m_Pos, direction * 0.075f);
				}
			}
		}
		else
		{
			m_ShotWait--;
		}
	}
	
	// �A�j���[�V�����I���ŏ���
	if (m_pEnemyStart->AnimZ[5].Time < m_AnimTime)
	{
		m_bShow = false;
	}

}

/**
 * �`��
 *
 */
void CEnemy::Render(){
	if (!GetShow())
	{
		return;
	}
	CMatrix44 matWorld;
	matWorld.RotationZXY(m_Rot);
	matWorld.SetTranslation(m_Pos);
	m_pMesh->Render(matWorld);
}

void CEnemy::RenderDebug() {
	// ��\��
	if (!GetShow())
	{
		return;
	}
	// �����蔻��̕\��
	CGraphicsUtilities::RenderSphere(GetSphere(), Vector4(1, 0, 0, 0.3f));
}

/**
 * �f�o�b�O�����`��
 *
 */
void CEnemy::RenderDebugText(int i){
	//�ʒu�̕`��
	CGraphicsUtilities::RenderString(10,70 + i * 24,MOF_XRGB(0,0,0),
		"�G[%d] %s , �ʒu X : %.1f , Y : %.1f , Z : %.1f",i + 1,
		(GetShow() ? "�\��" : "��\��"),m_Pos.x,m_Pos.y,m_Pos.z);
}

void CEnemy::Damage(int dmg) {
	m_HP -= dmg;
	if (m_HP <= 0)
	{
		m_bShow = false;
	}
}