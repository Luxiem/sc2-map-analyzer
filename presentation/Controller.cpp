#include "Controller.h"


SettingsMap* NewSettingsMap()
{
	SettingsMap* settings = new SettingsMap();

	settings->layer_pathable = false;
	settings->layer_openness = true;
	settings->layer_influence = false;
	settings->layer_height = false;
	settings->layer_buildable = false;
	settings->layer_siegeable = false;
	settings->layer_gas_balance = true;
	settings->layer_bases = true;
	settings->layer_los_blockers = true;
	settings->layer_photon_overcharge = false;
	settings->layer_watchtowers = false;
	settings->layer_destructable = true;
	settings->layer_pathing = false;

	// Colours
	settings->colours_influence = { 0xff555555, 0xff555555, 0xff555555 };
	settings->colours_openness = { 0xff555555, 0xff555555, 0xff555555 };
	settings->colour_default = 0xff555555;
	settings->colour_path_ground = 0xff555555;
	settings->colour_path_cliff = 0xff555555;
	settings->colour_path_air = 0xff555555;
	settings->colour_background = 0xff000000;
	settings->colour_base = 0xff555555;
	settings->colour_los_blockers = 0xff555555;
	settings->colour_height1 = 0xff555555;
	settings->colour_height2 = 0xff555555;
	settings->colour_height3 = 0xff5555553;

	// Base specific settings
	settings->spawn_influence = 0;

	// Paths

	// Unit settings
	float speed_worker;
	float speed_reaper;
	float speed_overlord;



	return settings;
}





