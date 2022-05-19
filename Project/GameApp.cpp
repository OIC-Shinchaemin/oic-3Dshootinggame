/*************************************************************************//*!
					
					@file	GameApp.cpp
					@brief	基本ゲームアプリ。

															@author	濱田　享
															@date	2014.05.14
*//**************************************************************************/

//INCLUDE
#include	"GameApp.h"
#include	"Player.h"
#include	"Stage.h"

CCamera				gCamera;
CVector3			gCameraPosition;
CVector3			gTargetPosition;
CVector3			gUpVector;
float				gRotUp;

CDirectionalLight	gLight;
CPlayer				gPlayer;
CStage				gStage;

bool				gbDebug = false;




/*************************************************************************//*!
		@brief			アプリケーションの初期化
		@param			None
				
		@return			TRUE		成功<br>
						それ以外	失敗、エラーコードが戻り値となる
*//**************************************************************************/
MofBool CGameApp::Initialize(void){
	//リソース配置ディレクトリの設定
	CUtilities::SetCurrentDirectory("Resource");

	gCameraPosition = Vector3(0, 6.0f, -17.0f); // カメラポジション
	gTargetPosition = Vector3(0, 0, -10);		// ターゲットのポジション
	gUpVector		= Vector3(0, 1, 0);			// アップベクトル

	gCamera.SetViewPort();

	gCamera.LookAt(
		gCameraPosition,	// カメラポジション
		gTargetPosition,	// ターゲットのポジション
		gUpVector);			// アップベクトル

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

	gPlayer.Initialize();
	gStage.Initialize();

	return TRUE;
}
/*************************************************************************//*!
		@brief			アプリケーションの更新
		@param			None
				
		@return			TRUE		成功<br>
						それ以外	失敗、エラーコードが戻り値となる
*//**************************************************************************/

MofBool CGameApp::Update(void){
	//キーの更新
	g_pInput->RefreshKey();
	gPlayer.Update();
	gStage.Update();

	if (g_pInput->IsKeyPush(MOFKEY_F1)) {
		gbDebug = ((gbDebug) ? false : true);
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
		@brief			アプリケーションの描画
		@param			None
				
		@return			TRUE		成功<br>
						それ以外	失敗、エラーコードが戻り値となる
*//**************************************************************************/
MofBool CGameApp::Render(void){
	//描画処理
	g_pGraphics->RenderStart();
	// 画面のクリア
	g_pGraphics->ClearTarget(0.65f,0.65f,0.67f,0.0f,1.0f,0);

	g_pGraphics->SetDepthEnable(true);

	gStage.Render();

	gPlayer.Render();
	

	if (gbDebug) {
		CMatrix44 matworld;
		matworld.Scaling(FIELD_HALF_X * 2, 1, FIELD_HALF_Z * 2);
		CGraphicsUtilities::RenderPlane(matworld, Vector4(1, 1, 1, 0.4f));
	}
	g_pGraphics->SetDepthEnable(false);

	if (gbDebug) {
//		CGraphicsUtilities::RenderString(10, 40, MOF_XRGB(0, 0, 0),"%f , rot : %f / dest : %f",v,rot, dest);

//gPlayer.RenderDebugText();
//		gStage.RenderDebugText();
	}

	// 描画の終了
	g_pGraphics->RenderEnd();
	return TRUE;
}
/*************************************************************************//*!
		@brief			アプリケーションの解放
		@param			None
				
		@return			TRUE		成功<br>
						それ以外	失敗、エラーコードが戻り値となる
*//**************************************************************************/
MofBool CGameApp::Release(void){
	gPlayer.Release();
	gStage.Release();
	return TRUE;
}