#pragma once
#include "Event.h"


namespace Yugo
{

	class WindowResize : public Event
	{
	public:
		WindowResize(int width, int height)
			: m_Width(width), m_Height(height) {}

		virtual EventType GetEventType() const override { return EventType::WindowResize; }
		int GetWidth() const { return m_Width; }
		int GetHeight() const { return m_Height; }

	private:
		int m_Width, m_Height;
	};

	class WindowCloseEvent : public Event
	{
	public:
		WindowCloseEvent() = default;

		virtual EventType GetEventType() const override { return EventType::WindowClose; }
	};

	class ImportAssetEvent : public Event
	{
	public:
		ImportAssetEvent(const std::string& assetFilePath)
			: m_AssetFilePath(assetFilePath) {}
	
		virtual EventType GetEventType() const override { return EventType::ImportAsset; }
		const std::string& GetAssetFilePath() const { return m_AssetFilePath; }

	private:
		std::string m_AssetFilePath;
	};

}