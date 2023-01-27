#include "loadConfig.h"

//Load the configuration of the txt file
void LoadConfig(struct GameData* _gd)
{
	FILE* ptrFile;
	char c = '\0';

	if (!(ptrFile = fopen("../GameParameters.txt", "r"))) 
	{
		printf("Error open file\n");
		exit(EXIT_FAILURE);
	}

	do
	{
		int i = 0;
		char line[255] = { '\0' };
		char buffer[255];
		do
		{
			c = fgetc(ptrFile);
			if (c != '\n' && c != EOF)
			{
				line[i] = c;
			}
			i++;
		} while (c != '\n' && c != EOF);

		if (*line == '#')
		{
			if (!_stricmp(line + 1, "Resolution"))
			{
				fscanf(ptrFile, "%d", &_gd->gp.screenW);
				fscanf(ptrFile, "%d", &_gd->gp.screenH);
			}
			if (!_stricmp(line + 1, "NbStartMines"))
			{
				fscanf(ptrFile, "%d", &_gd->gp.nbStartMines);
			}
			if (!_stricmp(line + 1, "NbNewMines"))
			{
				fscanf(ptrFile, "%d", &_gd->gp.nbNewMines);
			}
			if (!_stricmp(line + 1, "Player02Max"))
			{
				fscanf(ptrFile, "%d", &_gd->gp.playerHpMax);
			}
			if (!_stricmp(line + 1, "PlayerPropMax"))
			{
				fscanf(ptrFile, "%d", &_gd->gp.playerFuelMax);
			}
			if (!_stricmp(line + 1, "SpriteFileName"))
			{
				fscanf(ptrFile, "%s", _gd->gp.fileNameBackground);
				fscanf(ptrFile, "%s", _gd->gp.fileNamePlayer);
				fscanf(ptrFile, "%s", _gd->gp.fileNameShip);
				fscanf(ptrFile, "%s", _gd->gp.fileNameSatelliteOff);
				fscanf(ptrFile, "%s", _gd->gp.fileNameSatelliteOn);
				fscanf(ptrFile, "%s", _gd->gp.fileNameMine);
			}
		}
	} while (c != EOF);

	fclose(ptrFile);
}