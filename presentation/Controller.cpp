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
	settings->colour_height3 = 0xff555555;

	// Base specific settings
	settings->spawn_influence = 0;

	// Paths

	// Unit settings
	float speed_worker;
	float speed_reaper;
	float speed_overlord;



	return settings;
}



bool SerialToFile(SettingsMap* a_settings, const char* a_filename)
{
	/*
	Set working directory

	Windows:
	
	int bytes = GetModuleFileName(NULL, pBuf, len);
	if(bytes == 0)
	return -1;
	else
	return bytes;

	Linux:

	char szTmp[32];
	sprintf(szTmp, "/proc/%d/exe", getpid());
	int bytes = MIN(readlink(szTmp, pBuf, len), len - 1);
	if(bytes >= 0)
	pBuf[bytes] = '\0';
	return bytes;
	*/

	char pBuf[MAX_PATH];
	int len = MAX_PATH;

	int bytes = GetModuleFileName(NULL, pBuf, len);
	if (bytes == 0) return false;

	std::string::size_type pos = std::string(pBuf).find_last_of("\\/");
	std::string dir = std::string(pBuf).substr(0, pos + 1);

	std::string fullfilename = dir + std::string(a_filename);
	
	FILE* f = fopen(fullfilename.c_str(), "wb");
	int version = 1;
	fwrite(&version, sizeof(int), 1, f);

	fwrite(&(a_settings->layer_pathable), 1, 1, f);
	fwrite(&(a_settings->layer_openness), 1, 1, f);
	fwrite(&(a_settings->layer_influence), 1, 1, f);
	fwrite(&(a_settings->layer_height), 1, 1, f);
	fwrite(&(a_settings->layer_buildable), 1, 1, f);
	fwrite(&(a_settings->layer_siegeable), 1, 1, f);
	fwrite(&(a_settings->layer_gas_balance), 1, 1, f);
	fwrite(&(a_settings->layer_bases), 1, 1, f);
	fwrite(&(a_settings->layer_los_blockers), 1, 1, f);
	fwrite(&(a_settings->layer_photon_overcharge), 1, 1, f);
	fwrite(&(a_settings->layer_watchtowers), 1, 1, f);
	fwrite(&(a_settings->layer_destructable), 1, 1, f);
	fwrite(&(a_settings->layer_pathing), 1, 1, f);

	fflush(f);
	fclose(f);

	return true;
}


bool SerialFromFile(SettingsMap* a_settings, const char* a_filename)
{

	char pBuf[MAX_PATH];
	int len = MAX_PATH;

	int bytes = GetModuleFileName(NULL, pBuf, len);
	if (bytes == 0) return false;

	std::string::size_type pos = std::string(pBuf).find_last_of("\\/");
	std::string dir = std::string(pBuf).substr(0, pos + 1);

	std::string fullfilename = dir + std::string(a_filename);

	FILE* f = fopen(fullfilename.c_str(), "rb");
	if (f == 0) return false;

	int version = 0;
	fread(&version, sizeof(int), 1, f);

	fread(&(a_settings->layer_pathable), 1, 1, f);
	fread(&(a_settings->layer_openness), 1, 1, f);
	fread(&(a_settings->layer_influence), 1, 1, f);
	fread(&(a_settings->layer_height), 1, 1, f);
	fread(&(a_settings->layer_buildable), 1, 1, f);
	fread(&(a_settings->layer_siegeable), 1, 1, f);
	fread(&(a_settings->layer_gas_balance), 1, 1, f);
	fread(&(a_settings->layer_bases), 1, 1, f);
	fread(&(a_settings->layer_los_blockers), 1, 1, f);
	fread(&(a_settings->layer_photon_overcharge), 1, 1, f);
	fread(&(a_settings->layer_watchtowers), 1, 1, f);
	fread(&(a_settings->layer_destructable), 1, 1, f);
	fread(&(a_settings->layer_pathing), 1, 1, f);

	fclose(f);

	return true;
}

