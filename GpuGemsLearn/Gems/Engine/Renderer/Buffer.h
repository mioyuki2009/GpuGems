#pragma once
#include <string>
#include <vector>

enum class ShaderDataType {
	None = 0, Float, Float2, Float3, Float4, Mat3, Mat4, Int, Int2, Int3, Int4, Bool
};

static uint32_t ShaderDataTypeSize(ShaderDataType type) {
	switch (type)
	{
	case ShaderDataType::None:
		break;
	case ShaderDataType::Float:
		return 1 * sizeof(float);
		break;
	case ShaderDataType::Float2:
		return 2 * sizeof(float);
		break;
	case ShaderDataType::Float3:
		return 3 * sizeof(float);
		break;
	case ShaderDataType::Float4:
		return 4 * sizeof(float);
		break;
	case ShaderDataType::Mat3:
		return 3 * 3 * sizeof(float);
		break;
	case ShaderDataType::Mat4:
		return 4 * 4 * sizeof(float);
		break;
	case ShaderDataType::Int:
		return 1 * sizeof(int);
		break;
	case ShaderDataType::Int2:
		return 2 * sizeof(int);
		break;
	case ShaderDataType::Int3:
		return 3 * sizeof(int);
		break;
	case ShaderDataType::Int4:
		return 4 * sizeof(int);
		break;
	case ShaderDataType::Bool:
		return sizeof(bool);
		break;
	default:
		break;
	}
	return 0;
}


struct  BufferElement
{
	std::string Name;
	ShaderDataType Type;
			
	uint32_t Offset;
	uint32_t Size;

	bool Normalized;

	BufferElement(ShaderDataType type, const std::string& name, bool normalized = false)
		:Name(name), Type(type), Size(ShaderDataTypeSize(type)), Offset(0), Normalized(normalized){}

	uint32_t GetComponentCount() const {
		switch (Type)
		{
		case ShaderDataType::Float:		return 1;
		case ShaderDataType::Float2:	return 2;
		case ShaderDataType::Float3:	return 3;
		case ShaderDataType::Float4:	return 4;
		case ShaderDataType::Mat3:		return 3 * 3;
		case ShaderDataType::Mat4:		return 4 * 4;
		case ShaderDataType::Int:		return 1;
		case ShaderDataType::Int2:		return 2;
		case ShaderDataType::Int3:		return 3;
		case ShaderDataType::Int4:		return 4;
		case ShaderDataType::Bool:		return 1;
		}
		return 0;
	}
};

class BufferLayout {
public:
	BufferLayout() {};
	BufferLayout(const std::initializer_list<BufferElement>& elements):
		m_Elements(elements){
		CalculateOffsetAndStride();
	}

	inline uint32_t GetStride() const { return m_Stride; }
	inline const std::vector<BufferElement>& GetElements() const { return m_Elements; }

	std::vector<BufferElement>::iterator begin() { return m_Elements.begin(); }
	std::vector<BufferElement>::iterator end() { return m_Elements.end(); }

	std::vector<BufferElement>::const_iterator begin() const { return m_Elements.begin(); }
	std::vector<BufferElement>::const_iterator end() const { return m_Elements.end(); }

private:
	void CalculateOffsetAndStride() {
		uint32_t offset = 0;
		m_Stride = 0;
		for (auto& element : m_Elements) {
			element.Offset = offset;
			offset += element.Size;
			m_Stride += element.Size;
		}
	}

private:
	std::vector<BufferElement> m_Elements;
	uint32_t m_Stride = 0;
};

class VertexBuffer {

public:
	VertexBuffer(uint32_t size);
	VertexBuffer(float* vertices, uint32_t size);
	virtual ~VertexBuffer();

	virtual void Bind() const;
	virtual void Unbind() const;

	virtual void SetData(const void* data, uint32_t size);

	virtual const BufferLayout& GetLayout() const { return m_Layerout; }
	virtual void SetLayout(const BufferLayout& layout) { m_Layerout = layout; }

private:
	uint32_t m_RendererID;
	BufferLayout m_Layerout;
};

class IndexBuffer
{
public:
	IndexBuffer(uint32_t* indices, uint32_t size);
	virtual ~IndexBuffer();
	virtual void Bind() const;
	virtual void Unbind() const;

	virtual uint32_t GetCount() const { return m_Count; }

private:
	uint32_t m_RendererID;
	uint32_t m_Count;
};
