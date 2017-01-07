#pragma once

namespace Tool
{
	const	int	minScale = 0;
	const	int maxScale = 20;
	const	int defaultScale = 10;

	const	int minAniSpeed = 0;
	const	int maxAniSpeed = 20;
	const	int	defaultAniSpeed = 10;

	const	int	minFps = 8;
	const	int	maxFps = 60;
	const	int	defaultFps = 8;

	const	int	minMoveSpeed = 5;
	const	int	maxMoveSpeed = 100;
	const	int	defaultMoveSpeed = 40;

	const	char* const g_path = "res/animations";
	const	char* const g_strRun = "Run";
	const	char* const g_strRun1 = "run";
	const	char* const g_strAttack = "Attack";
	const	char* const g_strAttack1 = "attack";
	const	char* const g_strStand = "stand";
	const	char* const g_strIdel = "Idle";
	const	char* const g_custom = "res/custom";

	const	char* const keyScale = "scale";
	const	char* const keyAniSpeed = "aniSpeed";
	const	char* const keyFps = "fps";
	const	char* const keyMoveSpeed = "moveSpeed";

	//json file keys
	const char* const jsonAni = "animations";
	const char* const jsonActName = "actName";
	const char* const jsonSubActs = "subActions";
	const char* const jsonPlistPath = "plist";
	const char* const jsonImgName = "image";
	const char* const jsonDelayTimes = "time";
}