#include "Player.h"

/**
 * コンストラクタ
 */
CPlayer::CPlayer() :
m_Mesh(),
m_Pos(0.0f,0.0f,0.0f),
m_RotZ(0.0f),
m_SMesh(),
m_bDead(false),
m_SArray(),
m_SWait(),
m_SMode()
{
}

/**
 * デストラクタ
 */
CPlayer::~CPlayer(){
}

/**
 * 読み込み
 */
bool CPlayer::Load(void){
	// メッシュの読み込み
	m_Mesh.Load("player.mom");
	m_SMesh.Load("pshot.mom");

	for (int i = 0; i < PLAYERSHOT_COUNT; i++) {
		m_SArray[i].SetMesh(&m_SMesh);
	}
	return true;
}

/**
 * 初期化
 */
void CPlayer::Initialize(void){
	m_Pos = Vector3(0, 0, -FIELD_HALF_Z + 2);
	m_RotZ = 0;
	m_SMode = PlayerShotMode::MODE_DOUBLE;
	m_bDead = false;
	for (int i = 0; i < PLAYERSHOT_COUNT; i++) {
		m_SArray[i].Initialize();
	}
}

/**
 * 更新
 */
#define MAX_SKILLTIME 100
int SkillTimer = MAX_SKILLTIME;

void CPlayer::Update(void){
	if (m_bDead)
	{
		return;
	}

	float Roll = 0;
	float PlayerSpeed = PLAYER_SPEED;
	float RotSpeed = MOF_ToRadian(10);

	if (g_pInput->IsKeyHold(MOFKEY_LSHIFT) && SkillTimer > 4 ) {
		PlayerSpeed *= 4;
		RotSpeed *= 4;
		SkillTimer-= 4;
	}
	else
	{
		if (MAX_SKILLTIME > SkillTimer)
			SkillTimer++;
	}

	// 横移動
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
	
	// キー１：Single、キー2：Double、キー3：Trippleにしてね。
	UpdateMode();
	
	if (m_SWait <= 0) {
		if (g_pInput->IsKeyHold(MOFKEY_Z)) {

			switch (m_SMode) {
			case MODE_SINGLE:
				UpdateSingleMode();
				break;
			case MODE_DOUBLE:
				UpdateDoubleMode();
				break;
			case MODE_TRIPPLE:
				UpdateTrippleMode();
				break;
			}		
		}
	}
	else {
		m_SWait--;
	}

	for (int i = 0; i < PLAYERSHOT_COUNT; i++) {
		m_SArray[i].Update();
	}
}

void CPlayer::UpdateMode() {
	if (g_pInput->IsKeyPush(MOFKEY_1)) {
		m_SMode = MODE_SINGLE;
	}
	else if (g_pInput->IsKeyPush(MOFKEY_2)) {
		m_SMode = MODE_DOUBLE;
	}
	else if (g_pInput->IsKeyPush(MOFKEY_3)) {
		m_SMode = MODE_TRIPPLE;
	}
}

void CPlayer::UpdateSingleMode() {
	for (int i = 0; i < PLAYERSHOT_COUNT; i++) {
		if (m_SArray[i].GetShow()) continue;

		CVector3 spos(0, 0, 0);
		spos += m_Pos;
		CVector3 spd(0, 0, PLAYERSHOT_SPEED);
		m_SWait = PLAYERSHOT_WAIT;
		m_SArray[i].Fire(spos, spd, m_SMode);
		break;
	}
}

void CPlayer::UpdateDoubleMode() {
	for (int cnt = 0; cnt < 2; cnt++) {
		for (int i = 0; i < PLAYERSHOT_COUNT; i++) {
			if (m_SArray[i].GetShow()) continue;

			CVector3 spos(0.4f * (cnt * 2 - 1), 0, 0);
			spos.RotationZ(m_RotZ);
			spos += m_Pos;
			CVector3 spd(0, 0, PLAYERSHOT_SPEED);
			m_SWait = PLAYERSHOT_WAIT;
			m_SArray[i].Fire(spos, spd, m_SMode);
			break;
		}
	}
}

#define TRIPPLE_RAD		0.05f
void CPlayer::UpdateTrippleMode() {
	for (int cnt = 0; cnt < 3; cnt++) {
		for (int i = 0; i < PLAYERSHOT_COUNT; i++) {
			if (m_SArray[i].GetShow()) continue;

			CVector3 spos(0.4f * (cnt * 1 - 1), 0, 0);
			spos.RotationZ(m_RotZ);
			spos += m_Pos;
			CVector3 spd(cnt * TRIPPLE_RAD - TRIPPLE_RAD, 0, PLAYERSHOT_SPEED);
			m_SWait = PLAYERSHOT_WAIT;
			m_SArray[i].Fire(spos,spd, m_SMode);
			break;
		}
	}
}

void CPlayer::CollisionEnemy(CEnemy& ene) {
	if (!ene.GetShow())
	{
		return;
	}
	CSphere ps = GetSphere();
	CSphere es = ene.GetSphere();
	if (ps.CollisionSphere(es))
	{
		m_bDead = true;
	}
	// 弾との判定
	for (int i = 0; i < PLAYERSHOT_COUNT; i++)
	{
		if (!m_SArray[i].GetShow())
		{
			continue;
		}
		CSphere ss = m_SArray[i].GetSphere();
		if (ss.CollisionSphere(es))
		{
			ene.Damage(1);
			m_SArray[i].SetShow(false);
			break;
		}
	}
}

void CPlayer::CollisionEnemyShot(CEnemyShot& shot) {
	CSphere ps = GetSphere();
	if (!shot.GetShow())
	{
		return;
	}
	CSphere ss = shot.GetSphere();
	if (ss.CollisionSphere(ps))
	{
		m_bDead = true;
		shot.SetShow(false);
	}
}
/**
 * 描画
 */
void CPlayer::Render(void){
	if (m_bDead)
	{
		return;
	}

	CMatrix44 matWorld;
	matWorld.RotationZ(m_RotZ);
	matWorld.SetTranslation(m_Pos);
	m_Mesh.Render(matWorld);

	for (int i = 0; i < SkillTimer; i++) {
		CGraphicsUtilities::RenderString(800 + i*2, 0, MOF_COLOR_YELLOW,"|");
	}

	for (int i = 0; i < PLAYERSHOT_COUNT; i++) {
		m_SArray[i].Render();
	}
}

void CPlayer::RenderDebug(void) {
	// 当たり判定の表示
	CGraphicsUtilities::RenderSphere(GetSphere(), Vector4(0, 1, 0, 0.3f));
	// 弾の描画
	for (int i = 0; i < PLAYERSHOT_COUNT; i++)
	{
		m_SArray[i].RenderDebug();
	}
}

/**
 * デバッグ文字描画
 */
void CPlayer::RenderDebugText(void){
	// 位置の描画
	CGraphicsUtilities::RenderString(10,40,MOF_XRGB(0,0,0),
			"プレイヤー位置 X : %.1f , Y : %.1f , Z : %.1f",m_Pos.x,m_Pos.y,m_Pos.z);
	
	
}

/**
 * 解放
 */
void CPlayer::Release(void){
	m_Mesh.Release();
	m_SMesh.Release();
}