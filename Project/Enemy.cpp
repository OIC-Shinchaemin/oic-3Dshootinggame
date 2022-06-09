#include "Enemy.h"

// 敵の移動
ANIM_DATA g_EnemyAnimPosY[4] = {
{ 1.0f,-10.0f,EASE_LINEAR },
{ 2.0f,0.0f,EASE_LINEAR },
{ 3.0f,-5.0f,EASE_INOUT_SINE },
{ 4.0f,0.0f,EASE_IN_SINE },
};

ANIM_DATA g_EnemyAnimPosZ[6] = {
{ 0.0f,-FIELD_HALF_Z,EASE_LINEAR },
{ 1.0f,FIELD_HALF_Z - 10.0f,EASE_LINEAR },
{ 2.0f,FIELD_HALF_Z,EASE_OUT_SINE },
{ 3.0f,0.0f,EASE_IN_SINE },
{ 4.0f,FIELD_HALF_Z-10,EASE_OUT_SINE },
{ 5.0f,-FIELD_HALF_Z,EASE_LINEAR },
};

/**
 * コンストラクタ
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
 * デストラクタ
 *
 */
CEnemy::~CEnemy(){
}

/**
 * 初期化
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
 * 開始
 *
 */
void CEnemy::Start(const Vector3& p){
	m_Pos = p;
	m_Rot = Vector3(0, 0, 0);
	m_bShow = true;
	m_AnimTime = 0;
	m_ShotWait = 0;
	m_ShotWaitSet = 40;
	m_TargetPos = Vector3(0, 0, 0);
	m_HP = ENEMY_MAX_HP;
}

/**
 * 更新
 *
 */
void CEnemy::Update(CEnemyShot* shot, int smax) {
	// 非表示
	if (!GetShow())
	{
		return;
	}
	// 時間を進める
	m_AnimTime += CUtilities::GetFrameSecond();
	// アニメーション
	m_Pos.y = InterpolationAnim(m_AnimTime, g_EnemyAnimPosY, 4);
	m_Pos.z = InterpolationAnim(m_AnimTime, g_EnemyAnimPosZ, 6);
	
	// プレイヤーと同じ高さまで移動したら
	if (g_EnemyAnimPosY[1].Time < m_AnimTime)
	{
		// 弾の発射
		if (m_ShotWait <= 0)
		{
			CEnemyShot* newShot = CEnemyShot::FindAvailableShot(shot, smax);
			if (newShot)
			{
				m_ShotWait = m_ShotWaitSet;
				// 目標地点に向かうための方向
				Vector3 direction = m_TargetPos - m_Pos;
				// 目標地点までの距離を求める
				float distance = CVector3Utilities::Length(direction);
				// 距離が 0 以下=完全に同じ位置の場合は発射をしない
				if (distance > 0)
				{
					// 方向を正規化
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
	
	// アニメーション終了で消去
	if (g_EnemyAnimPosZ[5].Time < m_AnimTime)
	{
		m_bShow = false;
	}

}

/**
 * 描画
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
	// 非表示
	if (!GetShow())
	{
		return;
	}
	// 当たり判定の表示
	CGraphicsUtilities::RenderSphere(GetSphere(), Vector4(1, 0, 0, 0.3f));
}

/**
 * デバッグ文字描画
 *
 */
void CEnemy::RenderDebugText(int i){
	//位置の描画
	CGraphicsUtilities::RenderString(10,70 + i * 24,MOF_XRGB(0,0,0),
		"敵[%d] %s , 位置 X : %.1f , Y : %.1f , Z : %.1f",i + 1,
		(GetShow() ? "表示" : "非表示"),m_Pos.x,m_Pos.y,m_Pos.z);
}

void CEnemy::Damage(int dmg) {
	m_HP -= dmg;
	if (m_HP <= 0)
	{
		m_bShow = false;
	}
}