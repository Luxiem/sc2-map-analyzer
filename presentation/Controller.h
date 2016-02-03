#pragma once


#include <vector>
#include <Windows.h>


struct SettingsMap;


// Create a new, initialised, settings map instance.
SettingsMap* NewSettingsMap();


// Drawing settings for the SC2Map display
struct SettingsMap
{
public:

	// Layers and objects
	bool layer_pathable;
	bool layer_openness;
	bool layer_influence;
	bool layer_height;
	bool layer_buildable;
	bool layer_siegeable;
	bool layer_gas_balance;
	bool layer_bases;
	bool layer_los_blockers;
	bool layer_photon_overcharge;
	bool layer_watchtowers;
	bool layer_destructable;
	bool layer_pathing;

	// Colours
	std::vector<DWORD> colours_influence;
	std::vector<DWORD> colours_openness;
	DWORD colour_default;
	DWORD colour_path_ground;
	DWORD colour_path_cliff;
	DWORD colour_path_air;
	DWORD colour_background;
	DWORD colour_base;
	DWORD colour_los_blockers;
	DWORD colour_height1;
	DWORD colour_height2;
	DWORD colour_height3;

	// Base specific settings
	int spawn_influence;

	// Paths

	// Unit settings
	float speed_worker;
	float speed_reaper;
	float speed_overlord;
};
