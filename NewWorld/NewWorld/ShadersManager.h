#pragma once
#include"GeneralMesh.h"
#include <fstream>
#include <sstream>
#include <map>
#include <unordered_map>
using namespace std;
struct ConstantBufferVariable
{
	unsigned int offset;
	unsigned int size;
};
class ShadersManager
{
};
class ConstantBuffer {
public:
	std::string name;
	std::map<std::string, ConstantBufferVariable> constantBufferData;
	ID3D12Resource* constantBuffer;
	unsigned char* buffer;
	unsigned int cbSizeInBytes;
	unsigned int maxDrawCalls;
	unsigned int offsetIndex;

	void init(Core* core, unsigned int _maxDrawCalls = 1024)
	{
		cbSizeInBytes = (cbSizeInBytes + 255) & ~255;
		maxDrawCalls = _maxDrawCalls;
		unsigned int cbSizeInBytesAligned = cbSizeInBytes * maxDrawCalls;

		offsetIndex = 0;
		HRESULT hr;
		D3D12_HEAP_PROPERTIES heapprops = {};
		heapprops.Type = D3D12_HEAP_TYPE_UPLOAD;
		heapprops.CreationNodeMask = 1;
		heapprops.VisibleNodeMask = 1;
		D3D12_RESOURCE_DESC cbDesc = {};
		cbDesc.Width = cbSizeInBytesAligned;
		cbDesc.Height = 1;
		cbDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		cbDesc.DepthOrArraySize = 1;
		cbDesc.MipLevels = 1;
		cbDesc.SampleDesc.Count = 1;
		cbDesc.SampleDesc.Quality = 0;
		cbDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
		core->device->CreateCommittedResource(&heapprops, D3D12_HEAP_FLAG_NONE, &cbDesc, D3D12_RESOURCE_STATE_GENERIC_READ, NULL,
			IID_PPV_ARGS(&constantBuffer));
		constantBuffer->Map(0, NULL, (void**)&buffer);
	}
	//Update via a memcpy
	void update(std::string name, void* data)
	{
		for (const auto& pair : constantBufferData)
		{
			//OutputDebugStringA((pair.first + "\n").c_str());
		}

		if (constantBufferData.find(name) != constantBufferData.end())
		{
			ConstantBufferVariable cbVariable = constantBufferData[name];
			unsigned int offset = offsetIndex * cbSizeInBytes;
			memcpy(&buffer[offset + cbVariable.offset], data, cbVariable.size);
		}
		else
		{
			return;
		}

	}


	//Will need GPU address of contents

	// Get address of constant buffer in use
	D3D12_GPU_VIRTUAL_ADDRESS getGPUAddress() const
	{
		return (constantBuffer->GetGPUVirtualAddress() + (offsetIndex * cbSizeInBytes));
	}
	void next()
	{
		offsetIndex++;
		if (offsetIndex >= maxDrawCalls)
		{
			offsetIndex = 0;
		}
	}
};
class Shader {
public:
	ID3DBlob* vertexShader;
	ID3DBlob* pixelShader;
	map<string, int> textureBindPoints;
	std::string name;
	std::unordered_map<std::string, ConstantBuffer> vs_constantBuffer;
	std::unordered_map<std::string, ConstantBuffer> ps_constantBuffer;


	int debug = 0;

	string ReadShader(string filename) {
		std::ifstream file(filename);
		std::stringstream buffer;
		buffer << file.rdbuf();
		return buffer.str();
	}
	void Compile(Core* core, string vs, string ps) {
		debug = 1;
		OutputDebugStringA(to_string(debug).c_str());
		//Compile vertex shader
		ID3DBlob* status;
		string vertexShadersStr = ReadShader(vs);
		HRESULT hr = D3DCompile(vertexShadersStr.c_str(), strlen(vertexShadersStr.c_str()), NULL, NULL, NULL, "VS", "vs_5_0", 0, 0, &vertexShader, &status);
		string pixelShaderStr = ReadShader(ps);
		hr = D3DCompile(pixelShaderStr.c_str(), strlen(pixelShaderStr.c_str()), NULL, NULL, NULL, "PS", "ps_5_0", 0, 0, &pixelShader, &status);
		getConstantBuffer(pixelShader, ps_constantBuffer);
		getConstantBuffer(vertexShader, vs_constantBuffer);

		for (auto& pair : vs_constantBuffer)
		{
			pair.second.init(core);
		}
		for (auto& pair : ps_constantBuffer)
		{
			pair.second.init(core);
		}



		if (FAILED(hr)) {
			if (status) {
				// Print the error to the Visual Studio Output window
				OutputDebugStringA((char*)status->GetBufferPointer());
				status->Release();
			}
			return;
		}



	}
	void init(Core* core, string vs, string ps) {

		Compile(core, vs, ps);


	}
	void getConstantBuffer(ID3DBlob* shader, std::unordered_map<std::string, ConstantBuffer>& _constantbuffer) {
		ID3D12ShaderReflection* reflection;
		D3DReflect(shader->GetBufferPointer(), shader->GetBufferSize(), IID_PPV_ARGS(&reflection));
		D3D12_SHADER_DESC desc;
		reflection->GetDesc(&desc);
		//read constant buffers in shader (may have more than 1 constant buffers)
		for (int i = 0; i < desc.ConstantBuffers; i++) {
			ConstantBuffer buffer;
			ID3D12ShaderReflectionConstantBuffer* constantBuffer = reflection->GetConstantBufferByIndex(i);
			D3D12_SHADER_BUFFER_DESC cbDesc;
			constantBuffer->GetDesc(&cbDesc);
			unsigned int totalSize = 0;
			for (int j = 0; j < cbDesc.Variables; j++) {
				ID3D12ShaderReflectionVariable* var = constantBuffer->GetVariableByIndex(j);
				D3D12_SHADER_VARIABLE_DESC vDesc;
				var->GetDesc(&vDesc);
				ConstantBufferVariable bufferVariable;
				bufferVariable.offset = vDesc.StartOffset;
				bufferVariable.size = vDesc.Size;
				buffer.constantBufferData.insert({ vDesc.Name, bufferVariable });
				
				totalSize += bufferVariable.size;
			}
			//add
			buffer.name = cbDesc.Name;
			buffer.cbSizeInBytes = totalSize;
			_constantbuffer.insert(std::pair<std::string, ConstantBuffer>(buffer.name, buffer));


		}
		for (int i = 0; i < desc.BoundResources; i++)
		{
			D3D12_SHADER_INPUT_BIND_DESC bindDesc;
			reflection->GetResourceBindingDesc(i, &bindDesc);
			if (bindDesc.Type == D3D_SIT_TEXTURE)
			{
				textureBindPoints.insert({ bindDesc.Name, bindDesc.BindPoint });
			}
		}
	}
	void updateTexturePS(Core* core, std::string name, int heapOffset) {

		UINT bindPoint = textureBindPoints[name];
		D3D12_GPU_DESCRIPTOR_HANDLE handle = core->srvHeap.gpuHandle;
		handle.ptr = handle.ptr + (UINT64)(heapOffset - bindPoint) * (UINT64)core->srvHeap.incrementSize;
		core->getCommandList()->SetGraphicsRootDescriptorTable(2, handle);
	}

};


class Shaders {
public:
	map<string, Shader>shaders;
	std::string readFile(std::string filename)
	{
		std::ifstream file(filename);
		std::stringstream buffer;
		buffer << file.rdbuf();
		return buffer.str();
	}
	void load(Core* core, std::string shadername, std::string vsfilename, std::string psfilename)
	{
		std::map<std::string, Shader>::iterator it = shaders.find(shadername);
		if (it != shaders.end())
		{
			return;
		}
		Shader shader;
		shader.Compile(core, vsfilename, psfilename);

		shaders.insert({ shadername, shader });
	}
};

