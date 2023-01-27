#include "definitions.h"
#include "loadConfig.h"
#include "init.h"
#include "update.h"
#include "display.h"

void main()
{
	struct GameData gd;

	LoadConfig(&gd);
	Init(&gd);

	while (sfRenderWindow_isOpen(gd.settings.window))
	{
		Update(&gd);
		Display(&gd);
	}
}