#pragma once
#include <stdint.h>
#include <string>
#include "glad/glad.h"
class Texture2D 
{
public:
	enum class Filter
	{
		LINEAR,
		NEAREST,
	};
	
	Texture2D(uint32_t width, uint32_t height);
	Texture2D(const std::string& path, Filter filter = Filter::LINEAR);
	virtual ~Texture2D();

	virtual uint32_t GetWidth() const { return m_Width; }
	virtual uint32_t GetHeight() const { return m_Height; }
	virtual uint32_t GetRendererID() const { return m_RendererID; }

	virtual void SetData(void* data, uint32_t size);

	virtual void Bind(uint32_t slot = 0) const;

	virtual bool IsLoaded() const { return m_IsLoaded; }

	virtual bool operator==(const Texture2D& other) const {
		return m_RendererID == ((Texture2D&)other).m_RendererID;
	};

private:
	std::string m_Path;
	bool m_IsLoaded = false;
	uint32_t m_Width = 0;
	uint32_t m_Height = 0;
	uint32_t m_RendererID = 0;
	GLenum m_InternalFormat = 0;
	GLenum m_DataFormat = 0;
};

