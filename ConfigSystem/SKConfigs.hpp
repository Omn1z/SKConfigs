#pragma once

#include "Singleton.hpp"

enum class SKConfigHandle
{
	LOAD_CONFIG,
	SAVE_CONFIG,
};
enum class SKConfigFilePathType
{
	DEFAULT,
	HWID_PATH,
	//BUILD_DATE_PATH
};
class SKConfigs : public Singleton<SKConfigs>
{
public:
	void Inizialize(SKConfigFilePathType configtype, std::wstring Custom = L"");
	void Handle(SKConfigHandle request);
	
private:
	std::wstring FilePath;
	const std::wstring FileName = L"Settings.SKConfig";
	std::string SKConfig;



	void Save(bool Value, std::string name);
	void Save(int Value, std::string name);
	void Save(float Value, std::string name);
	void Save(std::string Value, std::string name);

	void Load(bool& Value, std::string name);
	void Load(int& Value, std::string name);
	void Load(float& Value, std::string name);
	void Load(std::string& Value, std::string name);

	std::string Parse(std::string szBefore, std::string szAfter);
	void ReadFile();
	void WriteFile();
protected:
	void SaveSettings();
	void LoadSettings();
};