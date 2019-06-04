#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <iosfwd>
#include <sstream> 
#include <ShlObj_core.h>

#include "SKConfigs.hpp"



void SKConfigs::Inizialize(SKConfigFilePathType configtype, std::wstring Custom)
{
	wchar_t path[MAX_PATH];
	SHGetFolderPathW(NULL, CSIDL_COMMON_APPDATA, NULL, 0, path);
	switch (configtype)
	{
		case SKConfigFilePathType::DEFAULT: {
			this->FilePath = std::wstring(path) + L"\\" + Custom + L"\\";
			break;
		};
		case SKConfigFilePathType::HWID_PATH: {
			DWORD serial;
			GetVolumeInformationW(L"C:\\", 0, 0, &serial, 0, 0, 0, 0);
			this->FilePath = std::wstring(path) + L"\\" + std::to_wstring(serial) + L"\\";
			break;
		};
	};
	CreateDirectoryW(this->FilePath.c_str(), 0);
}
void SKConfigs::Handle(SKConfigHandle request)
{
	this->SKConfig.clear();
	switch (request)
	{
		case SKConfigHandle::SAVE_CONFIG: {
			SaveSettings();
			break;
		}
		case SKConfigHandle::LOAD_CONFIG: {
			LoadSettings();
			break;
		}
	};
}

float Test[50];
void SKConfigs::SaveSettings() {
	Save(Test, ARRAYSIZE(Test),  "Test");
	this->WriteFile();
}
void SKConfigs::LoadSettings() {
	this->ReadFile();
	Load(Test, ARRAYSIZE(Test), "Test");
}



void SKConfigs::Load(bool& Value, std::string name) {
	std::string Before = "<" + name + ">";
	std::string After_ = "</" + name + ">";
	auto result = this->Parse(Before, After_);
	if (result == "")
		return;
	Value = result.find("true") != std::string::npos;
}
void SKConfigs::Load(int& Value, std::string name) {
	std::string Before = "<" + name + ">";
	std::string After_ = "</" + name + ">";
	auto result = this->Parse(Before, After_);
	if (result == "")
		return;
	Value = atoi(result.c_str());
}
void SKConfigs::Load(float& Value, std::string name) {
	std::string Before = "<" + name + ">";
	std::string After_ = "</" + name + ">";
	auto result = this->Parse(Before, After_);
	if (result == "")
		return;
	Value = (float)atof(result.c_str());
}
void SKConfigs::Load(std::string& Value, std::string name) {
	std::string Before = "<" + name + ">";
	std::string After_ = "</" + name + ">";
	auto result = this->Parse(Before, After_);
	if (result == "")
		return;
	Value = result;
}
void SKConfigs::Load(float* Value, int size, std::string name) {
	for (int i = 0; i < size; i++)
	{
		std::string Before = "<" + name + "_" + std::to_string(i) + ">";
		std::string After_ = "</" + name + "_" + std::to_string(i) + ">";
		auto result = this->Parse(Before, After_);
		if (result == "")
			continue;
		Value[i] = (float)atof(result.c_str());
	}
}
void SKConfigs::Load(int* Value, int size, std::string name) {
	for (int i = 0; i < size; i++)
	{
		std::string Before = "<" + name + "_" + std::to_string(i) + ">";
		std::string After_ = "</" + name + "_" + std::to_string(i) + ">";
		auto result = this->Parse(Before, After_);
		if (result == "")
			continue;
		Value[i] = atoi(result.c_str());
	}
}

void SKConfigs::Save(bool Value, std::string name){
	this->SKConfig += "<" + name + ">";
	this->SKConfig += Value ? "true" : "false";
	this->SKConfig += "</" + name + ">\n";
}
void SKConfigs::Save(int Value, std::string name){
	this->SKConfig += "<" + name + ">";
	this->SKConfig += std::to_string(Value);
	this->SKConfig += "</" + name + ">\n";
}
void SKConfigs::Save(float Value, std::string name){
	this->SKConfig += "<" + name + ">";
	this->SKConfig += std::to_string(Value);
	this->SKConfig += "</" + name + ">\n";
}
void SKConfigs::Save(std::string Value, std::string name){
	this->SKConfig += "<" + name + ">";
	this->SKConfig += Value;
	this->SKConfig += "</" + name + ">\n";
}
void SKConfigs::Save(float* Value, int size, std::string name) {
	for (int i = 0; i < size; i++)
	{
		this->SKConfig += "<" + name + "_" + std::to_string(i) +  ">";
		this->SKConfig += std::to_string(Value[i]);
		this->SKConfig += "</" + name + "_" + std::to_string(i) + ">\n";
	}
}
void SKConfigs::Save(int* Value, int size, std::string name) {
	for (int i = 0; i < size; i++)
	{
		this->SKConfig += "<" + name + "_" + std::to_string(i) + ">";
		this->SKConfig += std::to_string(Value[i]);
		this->SKConfig += "</" + name + "_" + std::to_string(i) + ">\n";
	}
}


std::string SKConfigs::Parse(std::string szBefore, std::string szAfter)
{
	if (!szBefore.empty() && !szAfter.empty() && !this->SKConfig.empty() && (this->SKConfig.find(szBefore) != std::string::npos) && (this->SKConfig.find(szAfter) != std::string::npos))
	{
		std::string t = strstr(this->SKConfig.c_str(), szBefore.c_str());
		t.erase(0, szBefore.length());
		std::string::size_type loc = t.find(szAfter, 0);
		t = t.substr(0, loc);
		return t;
	}
	else
		return "";
}

void SKConfigs::ReadFile() {
	char line[128];
	std::ifstream file(this->FilePath + this->FileName);
	if (!file.good())
		return;
	while (file.getline(line, 128))
		this->SKConfig += line;
	file.close();
}
void SKConfigs::WriteFile() {
	std::ofstream file(this->FilePath + this->FileName);
	file << this->SKConfig;
	file.close();
}