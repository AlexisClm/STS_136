#include "init.h"

//Initialize global settings
void InitSettings(struct GameData* _gd)
{
	sfVideoMode mode = { _gd->gp.screenW, _gd->gp.screenH, 32 };

	_gd->settings.window = sfRenderWindow_create(mode, "STS 136", sfResize | sfClose, NULL);
	_gd->settings.dtClock = sfClock_create();
	_gd->settings.level = 0;
	_gd->gameState = GAME;
	srand((unsigned int)time(NULL));
}

//Load the sprite and center it if needed
sfSprite* LoadSprite(const char* _filename, int _isCentered)
{
	sfSprite* tempSprite;
	sfTexture* tempText;

	tempText = sfTexture_createFromFile(_filename, NULL);
	tempSprite = sfSprite_create();
	sfSprite_setTexture(tempSprite, tempText, sfTrue);

	if (_isCentered == 1)
	{
		sfVector2u tempSize = sfTexture_getSize(tempText);
		sfVector2f tempOrigin = { (float)tempSize.x / 2, (float)tempSize.y / 2 };
		sfSprite_setOrigin(tempSprite, tempOrigin);
	}

	return tempSprite;
}

//Initialize the sprites needed for the game
void InitSprites(struct GameData* _gd)
{
	char images[50] = "../Assets/Images/";
	char fileNameBackground[50];
	char fileNameShip[50];
	char fileNameSatelliteOff[50];
	char fileNameSatelliteOn[50];
	char fileNameMine[50];
	char fileNamePlayer[50];

	//Concatenation of the pictures' pathname and the sprite's filename
	sprintf(fileNameBackground, "%s%s", images, _gd->gp.fileNameBackground);
	sprintf(fileNameShip, "%s%s", images, _gd->gp.fileNameShip);
	sprintf(fileNameSatelliteOff, "%s%s", images, _gd->gp.fileNameSatelliteOff);
	sprintf(fileNameSatelliteOn, "%s%s", images, _gd->gp.fileNameSatelliteOn);
	sprintf(fileNameMine, "%s%s", images, _gd->gp.fileNameMine);
	sprintf(fileNamePlayer, "%s%s", images, _gd->gp.fileNamePlayer);

	_gd->spr.background = LoadSprite(fileNameBackground, 0);
	_gd->spr.ship = LoadSprite(fileNameShip, 1);
	_gd->spr.satelliteOff = LoadSprite(fileNameSatelliteOff, 1);
	_gd->spr.satelliteOn = LoadSprite(fileNameSatelliteOn, 1);
	_gd->spr.mine = LoadSprite(fileNameMine, 1);
	_gd->spr.player = LoadSprite(fileNamePlayer, 1);
	_gd->spr.smoke = LoadSprite("../Assets/Images/Smoke.png", 1);
	_gd->spr.star = LoadSprite("../Assets/Images/Star.png", 1);
	_gd->spr.repair = LoadSprite("../Assets/Images/Repair.png", 1);
	_gd->spr.hud = LoadSprite("../Assets/Images/Hud.png", 0);
	_gd->spr.hp = LoadSprite("../Assets/Images/Hp.png", 0);
	_gd->spr.fuel = LoadSprite("../Assets/Images/Fuel.png", 0);
	_gd->spr.win = LoadSprite("../Assets/Images/Win.png", 1);
	_gd->spr.lose = LoadSprite("../Assets/Images/Lose.png", 1);
}

//Load the sound, set its volume and loop it if needed
sfSound* LoadSound(const char* _filename, float _volume, sfBool _loop)
{
	sfSound* tempSound;
	sfSoundBuffer* tempSoundBuffer;

	tempSound = sfSound_create();
	tempSoundBuffer = sfSoundBuffer_createFromFile(_filename);
	sfSound_setBuffer(tempSound, tempSoundBuffer);
	sfSound_setVolume(tempSound, _volume);
	sfSound_setLoop(tempSound, _loop);

	return tempSound;
}

//Initialize the sounds needed for the game
void InitSounds(struct GameData* _gd)
{
	float volume = 100.0f;

	_gd->sounds.name[SOUND_MAIN_GAME] = LoadSound("../Assets/Sounds/MainGame.ogg", volume, sfTrue);
	_gd->sounds.name[SOUND_HIT0] = LoadSound("../Assets/Sounds/Hit0.wav", volume, sfFalse);
	_gd->sounds.name[SOUND_HIT1] = LoadSound("../Assets/Sounds/Hit1.wav", volume, sfFalse);
	_gd->sounds.name[SOUND_HIT2] = LoadSound("../Assets/Sounds/Hit2.wav", volume, sfFalse);
	_gd->sounds.name[SOUND_HIT3] = LoadSound("../Assets/Sounds/Hit3.wav", volume, sfFalse);
	_gd->sounds.name[SOUND_HIT4] = LoadSound("../Assets/Sounds/Hit4.wav", volume, sfFalse);
	_gd->sounds.name[SOUND_HIT5] = LoadSound("../Assets/Sounds/Hit5.wav", volume, sfFalse);
	_gd->sounds.name[SOUND_HIT6] = LoadSound("../Assets/Sounds/Hit6.wav", volume, sfFalse);
	_gd->sounds.name[SOUND_HIT7] = LoadSound("../Assets/Sounds/Hit7.wav", volume, sfFalse);
	_gd->sounds.name[SOUND_REPAIR] = LoadSound("../Assets/Sounds/Repair.wav", volume, sfFalse);
	_gd->sounds.name[SOUND_LOSE] = LoadSound("../Assets/Sounds/Lose.wav", volume, sfFalse);

	sfSound_play(_gd->sounds.name[SOUND_MAIN_GAME]);
}

//Sort all the stars in order to have the closest stars in the foreground
void SortStars(struct GameData* _gd)
{
	for (int j = 0; j < NB_STARS_MAX - 1; j++)
	{
		for (int i = 0; i < NB_STARS_MAX; i++)
		{
			if (_gd->star[i].z < _gd->star[i + 1].z)
			{
				sfVector3f tempStar = _gd->star[i];
				_gd->star[i] = _gd->star[i + 1];
				_gd->star[i + 1] = tempStar;
			}
		}
	}
}

//Initialize the stars
void InitStars(struct GameData* _gd)
{
	for (int i = 0; i < NB_STARS_MAX; i++)
	{
		_gd->star[i].x = (float)(rand() % 16000 - 8000);
		_gd->star[i].y = (float)(rand() % 16000 - 8000);
		_gd->star[i].z = 0.2f + 0.001f * (rand() % 4000);
	}

	SortStars(_gd);
}

//Create a particle of smoke
void CreateSmoke(struct GameData* _gd)
{
	if (_gd->nbSmokes < NB_SMOKES_MAX)
	{
		_gd->smoke[_gd->nbSmokes].pos.x = _gd->player.pos.x + rand() % 20 - 10;
		_gd->smoke[_gd->nbSmokes].pos.y = _gd->player.pos.y + rand() % 20 - 10;
		_gd->smoke[_gd->nbSmokes].angle = (float)rand();
		_gd->smoke[_gd->nbSmokes].timer = 0;
		_gd->smoke[_gd->nbSmokes].timerMax = (rand() / (float)RAND_MAX) + 1;
		_gd->nbSmokes++;
	}
}

//Initialize the ship
void InitShip(struct GameData* _gd)
{
	_gd->ship.size.x = sfTexture_getSize(sfSprite_getTexture(_gd->spr.ship)).x;
	_gd->ship.size.y = sfTexture_getSize(sfSprite_getTexture(_gd->spr.ship)).y;
	_gd->ship.pos.x = (float)_gd->ship.size.x / 2;
	_gd->ship.pos.y = (float)(rand() % (_gd->gp.screenH - _gd->ship.size.y) + _gd->ship.size.y / 2);
	_gd->ship.speedY = 100;
}

//Initialize the satellite
void InitSatellite(struct GameData* _gd)
{
	_gd->satellite.size.x = sfTexture_getSize(sfSprite_getTexture(_gd->spr.satelliteOff)).x;
	_gd->satellite.size.y = sfTexture_getSize(sfSprite_getTexture(_gd->spr.satelliteOff)).y;
	_gd->satellite.pos.x = _gd->gp.screenW - (float)_gd->satellite.size.x / 2;
	_gd->satellite.pos.y = (float)(rand() % (_gd->gp.screenH - _gd->satellite.size.y) + _gd->satellite.size.y / 2);
	_gd->satellite.speedY = -100;
	_gd->satellite.timer = 0;
	_gd->satellite.timerMax = 3;
	_gd->satellite.timerBlinkMax = 0.2f;
	_gd->satellite.timerBlink = _gd->satellite.timerBlinkMax;
	_gd->satellite.isRepaired = 0;
	_gd->satellite.isBlinking = 0;
}

//Initialize the player
void InitPlayer(struct GameData* _gd)
{
	_gd->player.size.x = sfTexture_getSize(sfSprite_getTexture(_gd->spr.player)).x;
	_gd->player.size.y = sfTexture_getSize(sfSprite_getTexture(_gd->spr.player)).y;
	_gd->player.pos.x = _gd->ship.pos.x;
	_gd->player.pos.y = _gd->ship.pos.y;
	_gd->player.speed.x = (float)(rand() % 40 - 20);
	_gd->player.speed.y = (float)(rand() % 40 - 20);
	_gd->player.radius = _gd->player.size.y / 2;
	_gd->player.hp = _gd->gp.playerHpMax;
	_gd->player.fuel = _gd->gp.playerFuelMax;
	_gd->player.isRepairing = 0;
}

//Initialize the mines
void InitMines(struct GameData* _gd)
{
	if (_gd->nbMines < NB_MINES_MAX)
	{
		_gd->mine[_gd->nbMines].size.x = sfTexture_getSize(sfSprite_getTexture(_gd->spr.mine)).x;
		_gd->mine[_gd->nbMines].size.y = sfTexture_getSize(sfSprite_getTexture(_gd->spr.mine)).y;
		_gd->mine[_gd->nbMines].pos.x = (float)(rand() % (_gd->gp.screenW - _gd->mine[_gd->nbMines].size.x / 2) + _gd->mine[_gd->nbMines].size.x / 2);
		_gd->mine[_gd->nbMines].pos.y = (float)(rand() % (_gd->gp.screenH - _gd->mine[_gd->nbMines].size.y / 2) + _gd->mine[_gd->nbMines].size.y / 2);
		_gd->mine[_gd->nbMines].speed.x = (float)(rand() % 200 - 100);
		_gd->mine[_gd->nbMines].speed.y = (float)(rand() % 200 - 100);
		_gd->mine[_gd->nbMines].radius = _gd->mine[_gd->nbMines].size.x / 2;
		_gd->mine[_gd->nbMines].angle = (float)(rand());
		_gd->mine[_gd->nbMines].angleSpeed = (float)(rand() % 200 - 100);
		_gd->nbMines++;
	}
}

//Initialize the win
void InitWin(struct GameData* _gd)
{
	_gd->win.timer = 0;
	_gd->win.timerMax = 2;
}

//Initialize everything the game needs
void InitGame(struct GameData* _gd)
{
	_gd->nbMines = 0;
	_gd->nbSmokes = 0;
	InitStars(_gd);
	InitShip(_gd);
	InitSatellite(_gd);
	InitPlayer(_gd);

	for (int i = 0; i < _gd->gp.nbStartMines; i++)
	{
		InitMines(_gd);
	}

	InitWin(_gd);
}

//Initialize all
void Init(struct GameData* _gd)
{
	InitSettings(_gd);
	InitSprites(_gd);
	InitSounds(_gd);
	InitGame(_gd);
}