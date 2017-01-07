#pragma 
#include <string>

namespace Tool{

	class Preference
	{
	public:
		static void init();
		static std::string getResRootPath() { return g_rootPath; };
		static void setScale(int);
		static void setAniSpeed(int);
		static void setFps(int);
		static void setMoveSpeed(int);
		static int getScale()  { return g_scale; };
		static int getAniSpeed()  { return g_AniSpeed; };
		static int getFps()  { return g_fps; };
		static int getMoveSpeed(){ return g_moveSpeed; };

	private:
		static int g_scale;
		static int g_AniSpeed;
		static int g_fps;
		static int g_moveSpeed;
		static std::string g_rootPath;
	};
}