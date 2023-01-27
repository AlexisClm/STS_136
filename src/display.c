#include "display.h"

//Display a sprite
void BlitSprite(sfSprite* _sprite, sfVector2f _pos, float _angle, sfRenderWindow* _window)
{
	sfVector2f tempPos = { _pos.x, _pos.y };
	sfSprite_setPosition(_sprite, tempPos);
	sfSprite_setRotation(_sprite, _angle);
	sfRenderWindow_drawSprite(_window, _sprite, NULL);
}

//Display the background and the stars
void DisplayBackground(struct GameData* _gd)
{
	//Display the background
	sfSprite_setScale(_gd->spr.background, (sfVector2f) { (float)_gd->gp.screenW / sfTexture_getSize(sfSprite_getTexture(_gd->spr.background)).x, (float)_gd->gp.screenH / sfTexture_getSize(sfSprite_getTexture(_gd->spr.background)).y });
	BlitSprite(_gd->spr.background, (sfVector2f) { 0, 0 }, 0, _gd->settings.window);

	//Display the stars
	for (int i = 0; i < NB_STARS_MAX; i++)
	{
		sfVector2f posStars = { (_gd->star[i].x - _gd->player.pos.x) / _gd->star[i].z + _gd->gp.screenW / 2, (_gd->star[i].y - _gd->player.pos.y) / _gd->star[i].z + _gd->gp.screenH / 2 };
		sfSprite_setScale(_gd->spr.star, (sfVector2f) { 1.0f / _gd->star[i].z, 1.0f / _gd->star[i].z });
		BlitSprite(_gd->spr.star, posStars, 0, _gd->settings.window);
	}
}

//Display the sprites needed for the game
void DisplayGame(struct GameData* _gd)
{
	//Display the ship
	BlitSprite(_gd->spr.ship, _gd->ship.pos, 0, _gd->settings.window);

	//Display the satellite
	if (!_gd->satellite.isRepaired)
	{
		BlitSprite(_gd->spr.satelliteOff, _gd->satellite.pos, 0, _gd->settings.window);
	}
	else
	{
		BlitSprite(_gd->spr.satelliteOn, _gd->satellite.pos, 0, _gd->settings.window);
	}

	//Display the particles of smoke
	for (int i = 0; i < _gd->nbSmokes; i++)
	{
		sfSprite_setColor(_gd->spr.smoke, (sfColor) { 255, 255, 255, (1 - powf(_gd->smoke[i].timer / _gd->smoke[i].timerMax, 0.5f)) * 255 });
		BlitSprite(_gd->spr.smoke, _gd->smoke[i].pos, _gd->smoke[i].angle, _gd->settings.window);
	}

	//Display the player
	BlitSprite(_gd->spr.player, _gd->player.pos, 0, _gd->settings.window);

	//Display the mines
	for (int i = 0; i < _gd->nbMines; i++)
	{
		BlitSprite(_gd->spr.mine, _gd->mine[i].pos, _gd->mine[i].angle, _gd->settings.window);
	}

	//Display the hud
	BlitSprite(_gd->spr.hud, (sfVector2f) { 25, 25 }, 0, _gd->settings.window);

	//Display the player hp
	for (float i = 0; i < _gd->player.hp * 10; i++)
	{
		BlitSprite(_gd->spr.hp, (sfVector2f) { 120 + i, 32 }, 0, _gd->settings.window);
	}

	//Display the player fuel
	for (float i = 0; i < _gd->player.fuel * 10; i++)
	{
		BlitSprite(_gd->spr.fuel, (sfVector2f) { 120 + i, 66 }, 0, _gd->settings.window);
	}

	//Display the repair
	if (_gd->player.isRepairing && _gd->satellite.isBlinking)
	{
		BlitSprite(_gd->spr.repair, (sfVector2f) { _gd->satellite.pos.x - sfTexture_getSize(sfSprite_getTexture(_gd->spr.repair)).x / 3, _gd->satellite.pos.y }, 0, _gd->settings.window);
	}
}

//Display all following the game state
void Display(struct GameData* _gd)
{
	sfRenderWindow_clear(_gd->settings.window, sfBlack);

	DisplayBackground(_gd);

	switch (_gd->gameState)
	{
	case GAME:
		DisplayGame(_gd);
		break;

	case WIN:
		BlitSprite(_gd->spr.win, (sfVector2f) { (float)_gd->gp.screenW / 2, (float)_gd->gp.screenH / 2 }, 0, _gd->settings.window);
		break;

	case LOSE:
		BlitSprite(_gd->spr.lose, (sfVector2f) { (float)_gd->gp.screenW / 2, (float)_gd->gp.screenH / 2 }, 0, _gd->settings.window);
		break;

	default:
		break;
	}

	sfRenderWindow_display(_gd->settings.window);
}