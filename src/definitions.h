#ifndef definitions__H
#define definitions__H

#include <SFML/Window.h>
#include <SFML/Graphics.h>
#include <SFML/System.h>
#include <SFML/OpenGL.h>
#include <SFML/Network.h>
#include <SFML/Audio.h>

#include <stdio.h>
#include <stdlib.h>
#include <tchar.h>
#include <string.h>
#include "time.h"
#include "windows.h"
#include "math.h"

#define NB_MINES_MAX 20
#define NB_STARS_MAX 10000
#define NB_SMOKES_MAX 10000

enum GameState
{
	GAME,
	WIN,
	LOSE
};

enum Sound
{
	SOUND_MAIN_GAME,
	SOUND_HIT0,
	SOUND_HIT1,
	SOUND_HIT2,
	SOUND_HIT3,
	SOUND_HIT4,
	SOUND_HIT5,
	SOUND_HIT6,
	SOUND_HIT7,
	SOUND_REPAIR,
	SOUND_LOSE,
	NB_SOUNDS
};

struct GameParameters
{
	unsigned int screenW;
	unsigned int screenH;
	unsigned char nbStartMines;
	unsigned char nbNewMines;
	unsigned char playerHpMax;
	unsigned char playerFuelMax;
	char fileNameBackground[100];
	char fileNamePlayer[100];
	char fileNameShip[100];
	char fileNameSatelliteOff[100];
	char fileNameSatelliteOn[100];
	char fileNameMine[100];
};

struct GameSettings 
{
	sfRenderWindow* window;
	sfClock* dtClock;
	float dt;
	unsigned char level;
};

struct GameSprites
{
	sfSprite* background;
	sfSprite* star;
	sfSprite* ship;
	sfSprite* satelliteOff;
	sfSprite* satelliteOn;
	sfSprite* repair;
	sfSprite* mine;
	sfSprite* player;
	sfSprite* smoke;
	sfSprite* hud;
	sfSprite* hp;
	sfSprite* fuel;
	sfSprite* win;
	sfSprite* lose;
};

struct GameSounds
{
	sfSound* name[NB_SOUNDS];
	enum Sound typeSound;
};

struct Ship
{
	sfVector2u size;
	sfVector2f pos;
	float speedY;
};

struct Satellite
{
	sfVector2u size;
	sfVector2f pos;
	float speedY;
	float timer;
	float timerMax;
	float timerBlink;
	float timerBlinkMax;
	unsigned char isRepaired;
	unsigned char isBlinking;
};

struct Mines
{
	sfVector2u size;
	sfVector2f pos;
	sfVector2f speed;
	float angle;
	float angleSpeed;
	unsigned char radius;
};

struct Player
{
	sfVector2u size;
	sfVector2f pos;
	sfVector2f speed;
	float hp;
	unsigned char radius;
	char fuel;
	unsigned char isRepairing;
};

struct Win
{
	float timer;
	float timerMax;
};

struct Smoke
{
	sfVector2f pos;
	float timer;
	float timerMax;
	float angle;
};

struct GameData
{
	enum GameState gameState;
	struct GameParameters gp;
	struct GameSettings settings;
	struct GameSprites spr;
	struct GameSounds sounds;
	struct Ship ship;
	struct Satellite satellite;
	struct Mines mine[NB_MINES_MAX];
	struct Player player;
	struct Win win;
	struct Smoke smoke[NB_SMOKES_MAX];
	sfVector3f star[NB_STARS_MAX];
	int nbMines;
	int nbSmokes;
};

#endif