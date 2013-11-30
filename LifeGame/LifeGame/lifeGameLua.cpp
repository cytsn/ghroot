#include "lifeGameLua.h"

int _tmain(int argc, CHAR* argv[])
{
	lua_State *L = luaL_newstate();
	lua_close(L);

	return 0;
}