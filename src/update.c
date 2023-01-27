#include "update.h"

//Player actions on the keyboard
void InputPlayer(struct GameData* _gd, sfEvent _event)
{
	if (_gd->player.fuel > 0 && !_gd->player.isRepairing)
	{
		if (_event.type == sfEvtKeyPressed)
		{
			float prop = 30.0f;

			//Press 'Z' or 'up' to go up
			if (_event.key.code == sfKeyZ || _event.key.code == sfKeyUp)
			{
				_gd->player.speed.y -= prop;
				_gd->player.fuel--;
			}

			//Press 'S' or 'down' to go down
			if (_event.key.code == sfKeyS || _event.key.code == sfKeyDown)
			{
				_gd->player.speed.y += prop;
				_gd->player.fuel--;
			}

			//Press 'Q' or 'left' to go left
			if (_event.key.code == sfKeyQ || _event.key.code == sfKeyLeft)
			{
				_gd->player.speed.x -= prop;
				_gd->player.fuel--;
			}

			//Press 'D' or 'right' to go right
			if (_event.key.code == sfKeyD || _event.key.code == sfKeyRight)
			{
				_gd->player.speed.x += prop;
				_gd->player.fuel--;
			}
		}
	}
}

//Update the ship's movements
void UpdateShip(struct GameData* _gd)
{
	if (_gd->settings.level >= 2)
	{
		//Move the ship
		_gd->ship.pos.y += _gd->ship.speedY * _gd->settings.dt;

		//Change the ship's direction at the edge of the screen
		if (_gd->ship.pos.y - _gd->ship.size.y / 2 < 0)
		{
			_gd->ship.pos.y = (float)_gd->ship.size.y / 2;
			_gd->ship.speedY = -_gd->ship.speedY;
		}
		else if (_gd->ship.pos.y + _gd->ship.size.y / 2 > _gd->gp.screenH)
		{
			_gd->ship.pos.y = _gd->gp.screenH - (float)_gd->ship.size.y / 2;
			_gd->ship.speedY = -_gd->ship.speedY;
		}
	}
}

//Update the satellite's movements
void UpdateSatellite(struct GameData* _gd)
{
	if (_gd->settings.level >= 1)
	{
		//Move the satellite
		_gd->satellite.pos.y += _gd->satellite.speedY * _gd->settings.dt;

		//Change the satellite's direction at the edge of the screen
		if (_gd->satellite.pos.y - _gd->satellite.size.y / 2 < 0)
		{
			_gd->satellite.pos.y = (float)_gd->satellite.size.y / 2;
			_gd->satellite.speedY = -_gd->satellite.speedY;
		}
		else if (_gd->satellite.pos.y + _gd->satellite.size.y / 2 > _gd->gp.screenH)
		{
			_gd->satellite.pos.y = _gd->gp.screenH - (float)_gd->satellite.size.y / 2;
			_gd->satellite.speedY = -_gd->satellite.speedY;
		}
	}
}

//Update the player's movements and hp
void UpdatePlayer(struct GameData* _gd)
{
	//Move the player
	_gd->player.pos.x += _gd->player.speed.x * _gd->settings.dt;
	_gd->player.pos.y += _gd->player.speed.y * _gd->settings.dt;

	CreateSmoke(_gd);

	//Remove the player hp according to the time
	if (!_gd->player.isRepairing)
	{
		_gd->player.hp -= _gd->settings.dt;
	}
}

//Update the mines' movements and hp
void UpdateMines(struct GameData* _gd)
{
	for (int i = 0; i < _gd->nbMines; i++)
	{
		//Move and rotate the mines
		_gd->mine[i].pos.x += _gd->mine[i].speed.x * _gd->settings.dt;
		_gd->mine[i].pos.y += _gd->mine[i].speed.y * _gd->settings.dt;
		_gd->mine[i].angle += _gd->mine[i].angleSpeed * _gd->settings.dt;

		//Change the mines's direction at the edge of the screen
		if (_gd->mine[i].pos.x - _gd->mine[i].size.x / 2 < 0)
		{
			_gd->mine[i].pos.x = (float)_gd->mine[i].size.x / 2;
			_gd->mine[i].speed.x = -_gd->mine[i].speed.x;
		}
		else if (_gd->mine[i].pos.x + _gd->mine[i].size.x / 2 > _gd->gp.screenW)
		{
			_gd->mine[i].pos.x = _gd->gp.screenW - (float)_gd->mine[i].size.x / 2;
			_gd->mine[i].speed.x = -_gd->mine[i].speed.x;
		}
		else if (_gd->mine[i].pos.y - _gd->mine[i].size.y / 2 < 0)
		{
			_gd->mine[i].pos.y = (float)_gd->mine[i].size.y / 2;
			_gd->mine[i].speed.y = -_gd->mine[i].speed.y;
		}
		else if (_gd->mine[i].pos.y + _gd->mine[i].size.y / 2 > _gd->gp.screenH)
		{
			_gd->mine[i].pos.y = _gd->gp.screenH - (float)_gd->mine[i].size.y / 2;
			_gd->mine[i].speed.y = -_gd->mine[i].speed.y;
		}
	}
}

//Update the entities of the game
void UpdateEntities(struct GameData* _gd)
{
	UpdateShip(_gd);
	UpdateSatellite(_gd);
	UpdatePlayer(_gd);
	UpdateMines(_gd);
}

//Update particles of smoke
void UpdateSmoke(struct GameData* _gd)
{
	for (int i = _gd->nbSmokes - 1; i >= 0; i--)
	{
		_gd->smoke[i].timer += _gd->settings.dt;

		//Remove the particle when the timer is over
		if (_gd->smoke[i].timer >= _gd->smoke[i].timerMax)
		{
			_gd->smoke[i] = _gd->smoke[_gd->nbSmokes - 1];
			_gd->nbSmokes--;
		}
	}
}

//Check if there is an AABB collision
int IsCollidingRectRect(sfVector2f _pos1, sfVector2f _pos2, sfVector2u _size2)
{
	return (_pos1.x > _pos2.x - _size2.x / 2
		&& _pos1.x < _pos2.x + _size2.x / 2
		&& _pos1.y > _pos2.y - _size2.y / 2
		&& _pos1.y < _pos2.y + _size2.y / 2);
}

//Check if there is a circle circle collision
int IsCollidingCircleCircle(sfVector2f _pos1, unsigned char _rad1, sfVector2f _pos2, unsigned char _rad2)
{
	return (_pos1.x - _pos2.x) * (_pos1.x - _pos2.x) + (_pos1.y - _pos2.y) * (_pos1.y - _pos2.y) < (_rad1 + _rad2) * (_rad1 + _rad2);
}

//Get the squared length of a vector
float GetSquaredVectorLength(sfVector2f _pos)
{
	return _pos.x * _pos.x + _pos.y * _pos.y;
}

//Check if the player is slow enough to hang on something
int IsPlayerSlowEnough(struct GameData* _gd)
{
	float playerSquaredSpeed = GetSquaredVectorLength(_gd->player.speed);
	float speedMax = 150.0f;

	if (playerSquaredSpeed < speedMax * speedMax)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

//Check if there is a collision between the player and the ship
void CheckCollisionPlayerShip(struct GameData* _gd)
{
	if (IsCollidingRectRect(_gd->player.pos, _gd->ship.pos, _gd->ship.size))
	{
		//Fill the player hp and fuel bars
		if (IsPlayerSlowEnough(_gd))
		{
			_gd->player.hp = _gd->gp.playerHpMax;
			_gd->player.fuel = _gd->gp.playerFuelMax;
		}
	}
}

//Blink the sprite repair
void BlinkRepair(struct GameData* _gd)
{
	_gd->satellite.timerBlink += _gd->settings.dt;
	if (_gd->satellite.timerBlink > _gd->satellite.timerBlinkMax)
	{
		//Start the blinking effect
		if (_gd->satellite.isBlinking)
		{
			_gd->satellite.isBlinking = 0;
		}
		else
		{
			_gd->satellite.isBlinking = 1;
		}
		_gd->satellite.timerBlink = 0;
	}
}

//Check if there is a collision between the player and the satellite
void CheckCollisionPlayerSatellite(struct GameData* _gd)
{
	if (IsCollidingRectRect(_gd->player.pos, _gd->satellite.pos, _gd->satellite.size))
	{
		if (IsPlayerSlowEnough(_gd))
		{
			if (!_gd->player.isRepairing)
			{
				sfSound_play(_gd->sounds.name[SOUND_REPAIR]);
			}

			//Set the player at the satellite position while the time is not over
			_gd->player.isRepairing = 1;
			_gd->satellite.timer += _gd->settings.dt;
			_gd->player.pos.x = _gd->satellite.pos.x;
			_gd->player.pos.y = _gd->satellite.pos.y;

			BlinkRepair(_gd);

			//Move the player towards the left when the time is over
			if (_gd->satellite.timer > _gd->satellite.timerMax)
			{
				_gd->player.isRepairing = 0;
				_gd->satellite.isRepaired = 1;
				_gd->satellite.timer = 0;
				_gd->player.speed.x = -(rand() % 60) - 50.0f;
				_gd->player.speed.y = (float)(rand() % 40 - 20);
			}
		}
	}
}

//Play a random hit sound
void PlayRandomHitSound(struct GameData* _gd)
{
	sfSoundStatus stateHit0 = sfSound_getStatus(_gd->sounds.name[SOUND_HIT0]);
	sfSoundStatus stateHit1 = sfSound_getStatus(_gd->sounds.name[SOUND_HIT1]);
	sfSoundStatus stateHit2 = sfSound_getStatus(_gd->sounds.name[SOUND_HIT2]);
	sfSoundStatus stateHit3 = sfSound_getStatus(_gd->sounds.name[SOUND_HIT3]);
	sfSoundStatus stateHit4 = sfSound_getStatus(_gd->sounds.name[SOUND_HIT4]);
	sfSoundStatus stateHit5 = sfSound_getStatus(_gd->sounds.name[SOUND_HIT5]);
	sfSoundStatus stateHit6 = sfSound_getStatus(_gd->sounds.name[SOUND_HIT6]);
	sfSoundStatus stateHit7 = sfSound_getStatus(_gd->sounds.name[SOUND_HIT7]);

	//Check if a sound isn't playing
	if (stateHit0 != sfPlaying && stateHit1 != sfPlaying && stateHit2 != sfPlaying && stateHit3 != sfPlaying &&
		stateHit4 != sfPlaying && stateHit5 != sfPlaying && stateHit6 != sfPlaying && stateHit7 != sfPlaying)
	{
		int randomHit = rand() % SOUND_HIT7 + SOUND_HIT0;
		sfSound_play(_gd->sounds.name[randomHit]);
	}
}

//Check if there is a collision between the player and the mines
void CheckCollisionPlayerMines(struct GameData* _gd)
{
	for (int i = 0; i < _gd->nbMines; i++)
	{
		if (IsCollidingCircleCircle(_gd->player.pos, _gd->player.radius, _gd->mine[i].pos, _gd->mine[i].radius))
		{
			//Remove the player hp bar harder
			PlayRandomHitSound(_gd);
			_gd->player.hp -= 5 * _gd->settings.dt;
		}
	}
}

//Check if there is a collision between all entities
void CheckCollisionEntities(struct GameData* _gd)
{
	CheckCollisionPlayerShip(_gd);

	if (!_gd->satellite.isRepaired)
	{
		CheckCollisionPlayerSatellite(_gd);
	}

	if (!_gd->player.isRepairing)
	{
		CheckCollisionPlayerMines(_gd);
	}
}

//Check if the game is won
void CheckWin(struct GameData* _gd)
{
	if (_gd->satellite.isRepaired)
	{
		if (IsCollidingRectRect(_gd->player.pos, _gd->ship.pos, _gd->ship.size))
		{
			//Set the game state to win
			if (IsPlayerSlowEnough(_gd))
			{
				_gd->gameState = WIN;
			}
		}
	}
}

//Check if the game is lost
void CheckLose(struct GameData* _gd)
{
	if (_gd->player.hp <= 0)
	{
		sfSound_play(_gd->sounds.name[SOUND_LOSE]);
		_gd->gameState = LOSE;
	}
}

//Update the win
void UpdateWin(struct GameData* _gd)
{
	_gd->win.timer += _gd->settings.dt;
	if (_gd->win.timer > _gd->win.timerMax)
	{
		//Increase the level
		_gd->settings.level++;

		//Increase the number of the mines
		if (_gd->gp.nbStartMines < NB_MINES_MAX)
		{
			_gd->gp.nbStartMines += _gd->gp.nbNewMines;
		}

		//Reset the game
		InitGame(_gd);
		_gd->gameState = GAME;
	}
}

//Update all following the game state
void Update(struct GameData* _gd)
{
	sfEvent event;
	_gd->settings.dt = sfTime_asSeconds(sfClock_restart(_gd->settings.dtClock));

	while (sfRenderWindow_pollEvent(_gd->settings.window, &event))
	{
		if (event.type == sfEvtKeyPressed && event.key.code == sfKeyEscape)
		{
			exit(0);
		}

		else if (event.type == sfEvtClosed)
		{
			exit(0);
		}

		InputPlayer(_gd, event);
	}

	switch (_gd->gameState)
	{
	case GAME:
		UpdateEntities(_gd);
		UpdateSmoke(_gd);
		CheckCollisionEntities(_gd);
		CheckWin(_gd);
		CheckLose(_gd);
		break;

	case WIN:
		UpdateWin(_gd);
		break;

	default:
		break;
	}
}