#pragma once


#define REGISTER_CLASS(className, filePath) \
className::className() { m_ScriptFilePath = filePath; } \
static ScriptRegister<className> s_ScriptRegister;


#define SCRIPT_API __declspec(dllexport)