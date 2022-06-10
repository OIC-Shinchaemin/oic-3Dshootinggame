/*************************************************************************//*!
					
					@file	GameApp.cpp
					@brief	��{�Q�[���A�v���B

															@author	�_�c�@��
															@date	2014.05.14
*//**************************************************************************/

//INCLUDE
#include	"GameApp.h"
#include	"Player.h"
#include	"Stage.h"
#include	"Stage1.h"

CCamera				gCamera;
CVector3			gCameraPosition;
CVector3			gTargetPosition;
CVector3			gUpVector;
float				gRotUp;

CDirectionalLight	gLight;
CPlayer				gPlayer;
CEnemy				gEnemyArray[ENEMY_MAX_COUNT];
CEnemyShot			gShotArray[ENEMYSHOT_COUNT];
CMeshContainer		gEnemyShotMesh;

CStage				gStage;

bool				gbDebug = false;

/*************************************************************************//*!
		@brief			�A�v���P�[�V�����̏�����
		@param			None
				
		@return			TRUE		����<br>
						����ȊO	���s�A�G���[�R�[�h���߂�l�ƂȂ�
*//**************************************************************************/
MofBool CGameApp::Initialize(void){
	//���\�[�X�z�u�f�B���N�g���̐ݒ�
	CUtilities::SetCurrentDirectory("Resource");

	gCameraPosition = Vector3(0, 6.0f, -17.0f); // �J�����|�W�V����
	gTargetPosition = Vector3(0, 0, -10);		// �^�[�Q�b�g�̃|�W�V����
	gUpVector		= Vector3(0, 1, 0);			// �A�b�v�x�N�g��

	gCamera.SetViewPort();

	gCamera.LookAt(
		gCameraPosition,	// �J�����|�W�V����
		gTargetPosition,	// �^�[�Q�b�g�̃|�W�V����
		gUpVector);			// �A�b�v�x�N�g��

	gCamera.PerspectiveFov(MOF_ToRadian(60), 1024.0f / 768.0f, 0.01f, 1000.0f);
	CGraphicsUtilities::SetCamera(&gCamera);
	gRotUp = 0;

	gLight.SetDirection(Vector3(-1, -2, 1.5f));
	gLight.SetDiffuse(MOF_COLOR_HGREEN);
	gLight.SetAmbient(MOF_COLOR_HBLUE);
	gLight.SetSpeculer(MOF_COLOR_WHITE);
	CGraphicsUtilities::SetDirectionalLight(&gLight);

	gPlayer.Load();
	gStage.Load();

	// �G�e�̑f�ޓǂݍ���
	gEnemyShotMesh.Load("eshot.mom");

	gPlayer.Initialize();
	// �X�e�[�W�̏�ԏ�����
	gStage.Initialize(&gStg1EnemyStart);
	// �G�̏�����
	for (int i = 0; i < ENEMY_MAX_COUNT; i++)
	{
		gEnemyArray[i].Initialize();
	}

	// �G�e�̏�����
	for (int i = 0; i < ENEMYSHOT_COUNT; i++)
	{
		gShotArray[i].Initialize();
		gShotArray[i].SetMesh(&gEnemyShotMesh);
	}

	return TRUE;
}
/*************************************************************************//*!
		@brief			�A�v���P�[�V�����̍X�V
		@param			None
				
		@return			TRUE		����<br>
						����ȊO	���s�A�G���[�R�[�h���߂�l�ƂȂ�
*//**************************************************************************/

MofBool CGameApp::Update(void){
	//�L�[�̍X�V
	g_pInput->RefreshKey();
	
	gStage.Update(gEnemyArray, ENEMY_MAX_COUNT);

	gPlayer.Update();

	// �G�̍X�V
	for (int i = 0; i < ENEMY_MAX_COUNT; i++)
	{
		gEnemyArray[i].SetTargetPos(gPlayer.GetPosition());
		gEnemyArray[i].Update(gShotArray, ENEMYSHOT_COUNT);
	}
	// �G�e�̍X�V
	for (int i = 0; i < ENEMYSHOT_COUNT; i++)
	{
		gShotArray[i].Update(gPlayer.GetPosition());
	}

	// �G�Ƃ̓����蔻��
	for (int i = 0; i < ENEMY_MAX_COUNT; i++)
	{
		gPlayer.CollisionEnemy(gEnemyArray[i]);
	}
	// �G�e�Ƃ̓����蔻��
	for (int i = 0; i < ENEMYSHOT_COUNT; i++)
	{
		gPlayer.CollisionEnemyShot(gShotArray[i]);
	}

	if (g_pInput->IsKeyPush(MOFKEY_F1)) {
		gbDebug = ((gbDebug) ? false : true);
	}
	// �Q�[���I�[�o�[�\����� Enter �ŏ��������s��
	if (g_pInput->IsKeyPush(MOFKEY_RETURN) && gPlayer.IsDead())
	{
		// �Q�[�����̃I�u�W�F�N�g�����ׂď���������
		gPlayer.Initialize();
		gStage.Initialize(&gStg1EnemyStart);
		for (int i = 0; i < ENEMY_MAX_COUNT; i++)
		{
			gEnemyArray[i].Initialize();
		}
		for (int i = 0; i < ENEMYSHOT_COUNT; i++)
		{
			gShotArray[i].Initialize();
		}

	}

	float posx = gPlayer.GetPosition().x * 0.4f;
	
	gCameraPosition.x = gTargetPosition.x = posx;
	gCamera.LookAt(gCameraPosition, gTargetPosition, gUpVector);
	
	gUpVector = Vector3(0, 1, 0);
	
	if (gPlayer.GetMove() != PlayerMove::IDLE){
		float v = 0;
		v = (gPlayer.GetMove() == PlayerMove::RIGHT) ? 0.1f : -0.1f;
		gRotUp = MOF_LERP(gRotUp, v, 0.01f);
		gUpVector.RotationZ(gRotUp);
	}
	else {
		gRotUp = MOF_LERP(gRotUp, 0, 0.1f);
		gUpVector.RotationZ(gRotUp);
	}

	gCamera.Update();
	return TRUE;
}

/*************************************************************************//*!
		@brief			�A�v���P�[�V�����̕`��
		@param			None
				
		@return			TRUE		����<br>
						����ȊO	���s�A�G���[�R�[�h���߂�l�ƂȂ�
*//**************************************************************************/
MofBool CGameApp::Render(void){
	//�`�揈��
	g_pGraphics->RenderStart();
	// ��ʂ̃N���A
	g_pGraphics->ClearTarget(0.65f,0.65f,0.67f,0.0f,1.0f,0);

	g_pGraphics->SetDepthEnable(true);

	gStage.Render();

	gPlayer.Render();
	
	// �G�̕`��
	for (int i = 0; i < ENEMY_MAX_COUNT; i++)
	{
		gEnemyArray[i].Render();
	}
	// �G�e�̕`��
	for (int i = 0; i < ENEMYSHOT_COUNT; i++)
	{
		gShotArray[i].Render();
	}

	if (gbDebug) {
		// �v���C���[�̃f�o�b�O�`��
		gPlayer.RenderDebug();
		// �G�̃f�o�b�O�`��
		for (int i = 0; i < ENEMY_MAX_COUNT; i++)
		{
			gEnemyArray[i].RenderDebug();
		}
		// �G�e�̃f�o�b�O�`��
		for (int i = 0; i < ENEMYSHOT_COUNT; i++)
		{
			gShotArray[i].RenderDebug();
		}

		CMatrix44 matworld;
		matworld.Scaling(FIELD_HALF_X * 2, 1, FIELD_HALF_Z * 2);
		CGraphicsUtilities::RenderPlane(matworld, Vector4(1, 1, 1, 0.4f));		
	}
	g_pGraphics->SetDepthEnable(false);

	if (gbDebug) {
		gPlayer.RenderDebugText();
		gStage.RenderDebugText();
		// �G�̃f�o�b�O�����`��
		for (int i = 0; i < ENEMY_MAX_COUNT; i++)
		{
			gEnemyArray[i].RenderDebugText(i);
		}
	}

	// �Q�[���I�[�o�[�\��
	if (gPlayer.IsDead())
	{
		CGraphicsUtilities::RenderString(240, 350, MOF_COLOR_RED,
			"�Q�[���I�[�o�[ �F Enter �L�[�ł�����x�ŏ�����");
	}

	// �`��̏I��
	g_pGraphics->RenderEnd();
	return TRUE;
}
/*************************************************************************//*!
		@brief			�A�v���P�[�V�����̉��
		@param			None
				
		@return			TRUE		����<br>
						����ȊO	���s�A�G���[�R�[�h���߂�l�ƂȂ�
*//**************************************************************************/
MofBool CGameApp::Release(void){
	gPlayer.Release();
	gStage.Release();
	gEnemyShotMesh.Release();

	return TRUE;
}