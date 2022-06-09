#pragma once

//ステージ１の情報を定義する

//敵の出現位置定義
float			gStg1EnemyStartPosX[ENEMY_COUNT] = {
	-6.0f,
	-6.0f,
	-6.0f,
	-6.0f,
	-6.0f,
	
	6.0f,
	6.0f,
	6.0f,
	6.0f,
	6.0f,

	-5.0f, 5.0f,
	-5.0f, 5.0f,
	-5.0f, 5.0f,
	-5.0f, 5.0f,
	-5.0f, 5.0f,
};
float			gStg1EnemyStartScroll[ENEMY_COUNT] = {
	100.0f,
	150.0f,
	200.0f,
	250.0f,
	300.0f,
	
	500.0f,
	550.0f,
	600.0f,
	650.0f,
	700.0f,

	1000.0f, 1000.0f,
	1050.0f, 1050.0f,
	1100.0f, 1100.0f,
	1150.0f, 1150.0f,
	1200.0f, 1200.0f,
};

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
{ 4.0f,FIELD_HALF_Z - 10,EASE_OUT_SINE },
{ 5.0f,-FIELD_HALF_Z,EASE_LINEAR },
};

//敵の開始情報の構造体を作成する
ENEMYSTART gStg1EnemyStart = {
	ENEMY_COUNT,
	gStg1EnemyStartPosX,
	gStg1EnemyStartScroll,
	g_EnemyAnimPosY,
	g_EnemyAnimPosZ,
};