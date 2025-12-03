// NewWorld.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#define kuan 1024.f
#define gao 768.f
#define WINDOW_GET_X_LPARAM(lp) ((int)(short)LOWORD(lp))
#define WINDOW_GET_Y_LPARAM(lp) ((int)(short)HIWORD(lp))
#include <Windows.h>
#include <iostream>
#include <string>
#include "Core.h"
#include "MathTool.h"
#include <fstream>
#include <sstream>
#include "PSOManager.h"
#include"VertexLayoutCache.h"
#include "GEMLoader.h"

using namespace MathTool;
using namespace std;
extern "C" {
	_declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
}
using namespace std;
struct PRIM_VERTEX
{
	Vec3 position;
	Colour colour;
};
struct alignas(16) ConstantBuffer3 {
	Matrix w;
	Matrix VP;
};
struct alignas(16) ConstantBuffer1
{
	float time;
};
struct STATIC_VERTEX
{
	Vec3 pos;
	Vec3 normal;
	Vec3 tangent;
	float tu;
	float tv;
};
struct alignas(16) ConstantBuffer2
{
	float time;
	float padding[3];
	Vec4 lights[4];

};
struct ConstantBufferVariable
{
	unsigned int offset;
	unsigned int size;
};
std::map<std::string, ConstantBufferVariable> scvvv;

class GeneralMesh {
public:
	ID3D12Resource* vertexBuffer;
	ID3D12Resource* indexBuffer;
	D3D12_VERTEX_BUFFER_VIEW vbView;
	D3D12_INDEX_BUFFER_VIEW ibView;
	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc;
	unsigned int numMeshIndices;

	void init(Core* core, void* vertices, int vertexSizeInBytes, int numVertices,
		unsigned int* indices, int numIndices) {
		//Specify vertex buffer will be in GPU memory heap
		D3D12_HEAP_PROPERTIES heapprops = {};
		heapprops.Type = D3D12_HEAP_TYPE_DEFAULT;
		heapprops.CreationNodeMask = 1;
		heapprops.VisibleNodeMask = 1;

		//Create vertex buffer on heap
		D3D12_RESOURCE_DESC vbDesc = {};
		vbDesc.Width = numVertices * vertexSizeInBytes;
		vbDesc.Height = 1;
		vbDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		vbDesc.DepthOrArraySize = 1;
		vbDesc.MipLevels = 1;
		vbDesc.SampleDesc.Count = 1;
		vbDesc.SampleDesc.Quality = 0;
		vbDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

		//Allocate memory
		core->device->CreateCommittedResource(&heapprops, D3D12_HEAP_FLAG_NONE, &vbDesc,
			D3D12_RESOURCE_STATE_COMMON, NULL, IID_PPV_ARGS(&vertexBuffer));
		//Copy vertices using our helper function
		core->uploadResource(vertexBuffer, vertices, numVertices * vertexSizeInBytes,
			D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER);

		//Fill in view in helper function
		vbView.BufferLocation = vertexBuffer->GetGPUVirtualAddress();
		vbView.StrideInBytes = vertexSizeInBytes;
		vbView.SizeInBytes = numVertices * vertexSizeInBytes;

		D3D12_RESOURCE_DESC ibDesc;
		memset(&ibDesc, 0, sizeof(D3D12_RESOURCE_DESC));
		ibDesc.Width = numIndices * sizeof(unsigned int);
		ibDesc.Height = 1;
		ibDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		ibDesc.DepthOrArraySize = 1;
		ibDesc.MipLevels = 1;
		ibDesc.SampleDesc.Count = 1;
		ibDesc.SampleDesc.Quality = 0;
		ibDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
		HRESULT hr = core->device->CreateCommittedResource(&heapprops, D3D12_HEAP_FLAG_NONE, &ibDesc,
			D3D12_RESOURCE_STATE_COMMON, NULL, IID_PPV_ARGS(&indexBuffer));
		core->uploadResource(indexBuffer, indices, numIndices * sizeof(unsigned int),
			D3D12_RESOURCE_STATE_INDEX_BUFFER);

		ibView.BufferLocation = indexBuffer->GetGPUVirtualAddress();
		ibView.Format = DXGI_FORMAT_R32_UINT;
		ibView.SizeInBytes = numIndices * sizeof(unsigned int);
		numMeshIndices = numIndices;
	}
	void init(Core* core, std::vector<STATIC_VERTEX> vertices, std::vector<unsigned int> indices)
	{
		init(core, &vertices[0], sizeof(STATIC_VERTEX), vertices.size(), &indices[0], indices.size());
		inputLayoutDesc = VertexLayoutCache::getStaticLayout();
	}
	
	void draw(Core* core)
	{
		core->getCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		core->getCommandList()->IASetVertexBuffers(0, 1, &vbView);
		core->getCommandList()->IASetIndexBuffer(&ibView);
		core->getCommandList()->DrawIndexedInstanced(numMeshIndices, 1, 0, 0, 0);
	}

};



class Mesh {
public:
	ID3D12Resource* vertexBuffer;
	D3D12_VERTEX_BUFFER_VIEW vbView;

	//Define layout
	D3D12_INPUT_ELEMENT_DESC inputLayout[2];
	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc;
	void init(Core* core, void* vertices, int vertexSizeInBytes, int numVertices) {
		//Specify vertex buffer will be in GPU memory heap
		D3D12_HEAP_PROPERTIES heapprops = {};
		heapprops.Type = D3D12_HEAP_TYPE_DEFAULT;
		heapprops.CreationNodeMask = 1;
		heapprops.VisibleNodeMask = 1;

		//Create vertex buffer on heap
		D3D12_RESOURCE_DESC vbDesc = {};
		vbDesc.Width = numVertices * vertexSizeInBytes;
		vbDesc.Height = 1;
		vbDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		vbDesc.DepthOrArraySize = 1;
		vbDesc.MipLevels = 1;
		vbDesc.SampleDesc.Count = 1;
		vbDesc.SampleDesc.Quality = 0;
		vbDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
		//Allocate memory
		core->device->CreateCommittedResource(&heapprops, D3D12_HEAP_FLAG_NONE, &vbDesc,
			D3D12_RESOURCE_STATE_COMMON, NULL, IID_PPV_ARGS(&vertexBuffer));
		//Allocate memory
		core->uploadResource(vertexBuffer, vertices, numVertices * vertexSizeInBytes,
			D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER);
		//Fill in view in helper function
		vbView.BufferLocation = vertexBuffer->GetGPUVirtualAddress();
		vbView.StrideInBytes = vertexSizeInBytes;
		vbView.SizeInBytes = numVertices * vertexSizeInBytes;
		//Fill in Layout
		inputLayout[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT,
D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
		inputLayout[1] = { "COLOUR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT,
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
		inputLayoutDesc.NumElements = 2;
		inputLayoutDesc.pInputElementDescs = inputLayout;
	}
	void draw(Core* core)
	{
		core->getCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		core->getCommandList()->IASetVertexBuffers(0, 1, &vbView);
		core->getCommandList()->DrawInstanced(3, 1, 0, 0);
	}
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
	
	void init(Core* core,  unsigned int _maxDrawCalls = 1024)
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
			OutputDebugStringA((pair.first + "\n").c_str());
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
	void Compile(Core* core) {
		debug = 1;
		OutputDebugStringA(to_string(debug).c_str());
		//Compile vertex shader
		ID3DBlob* status;
		string vertexShadersStr = ReadShader("ShaderVertices.hlsl");
		HRESULT hr = D3DCompile(vertexShadersStr.c_str(), strlen(vertexShadersStr.c_str()), NULL,NULL, NULL, "VS", "vs_5_0", 0, 0, &vertexShader, &status);
		string pixelShaderStr = ReadShader("ShaderPixel.hlsl");
		hr = D3DCompile(pixelShaderStr.c_str(), strlen(pixelShaderStr.c_str()), NULL, NULL,NULL, "PS", "ps_5_0", 0, 0, &pixelShader, &status);
		//getConstantBuffer(pixelShader, ps_constantBuffer);
		getConstantBuffer(vertexShader, vs_constantBuffer);
		






		for (auto& pair : vs_constantBuffer)
		{
			pair.second.init(core, 2);
		}
		for (auto& pair : ps_constantBuffer)
		{
			pair.second.init(core, 2);
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
	void init(Core* core) {

		Compile(core);
	
		
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
				scvvv.insert({ vDesc.Name, bufferVariable });
				totalSize += bufferVariable.size;
			}
			//add
			buffer.name = cbDesc.Name;
			buffer.cbSizeInBytes = totalSize;
			_constantbuffer.insert(std::pair<std::string, ConstantBuffer>(buffer.name, buffer));
			
			
		}
	}
	
};

class ScreenSpaceTriangle {
public:
	std::vector<ConstantBuffer*> vsConstantBuffers; // Vertex Shader Buffers
	std::vector<ConstantBuffer*> psConstantBuffers; // Pixel Shader Buffers

	Shader shader;
	PRIM_VERTEX vertices[3];
	PSOManager psos;
	Mesh mesh;
	void init(Core* core) {
		
		vertices[0].position = Vec3(0, 1.0f, 0);
		vertices[0].colour = Colour(0, 1.0f, 0);
		vertices[1].position = Vec3(-1.0f, -1.0f, 0);
		vertices[1].colour = Colour(1.0f, 0, 0);
		vertices[2].position = Vec3(1.0f, -1.0f, 0);
		vertices[2].colour = Colour(0, 0, 1.0f);
		mesh.init(core, &vertices[0], sizeof(PRIM_VERTEX), 3);
		shader.init(core);
		psos.createPSO(core, "Triangle", shader.vertexShader, shader.pixelShader, mesh.inputLayoutDesc);
	}
	void apply(Core* core) {
		// Bind VS buffers
		unsigned int slot = 0;



		for (auto i : shader.ps_constantBuffer)
		{



			core->getCommandList()->SetGraphicsRootConstantBufferView(1, shader.ps_constantBuffer[i.first].getGPUAddress());
			shader.ps_constantBuffer[i.first].next();
			slot++;

		}

		/*for (auto& pair : shader.vs_constantBuffer)
		{

			core->getCommandList()->SetGraphicsRootConstantBufferView(slot, pair.second.getGPUAddress());
			pair.second.next();
			//core->rootSignature.
			slot++;

		}*/
		
	}
	void draw(Core* core, ConstantBuffer2* cb)
	{
		core->beginRenderPass();
		
		shader.ps_constantBuffer["bufferName"].update("time",&cb->time );
		shader.ps_constantBuffer["bufferName"].update("lights", &cb->lights);
		
		apply(core);
		psos.bind(core, "Triangle");
		mesh.draw(core);
	}

};

class Cube {
public:
	

	Shader shader;
	PRIM_VERTEX vertices[3];
	PSOManager psos;
	GeneralMesh mesh;
	STATIC_VERTEX addVertex(Vec3 p, Vec3 n, float tu, float tv)
	{
		STATIC_VERTEX v;
		v.pos = p;
		v.normal = n;
		v.tangent = Vec3(0, 0, 0); // For now
		v.tu = tu;
		v.tv = tv;
		return v;
	}
	void init(Core* core) {

		std::vector<STATIC_VERTEX> vertices;
		Vec3 p0 = Vec3(-1.0f, -1.0f, -1.0f);
		Vec3 p1 = Vec3(1.0f, -1.0f, -1.0f);
		Vec3 p2 = Vec3(1.0f, 1.0f, -1.0f);
		Vec3 p3 = Vec3(-1.0f, 1.0f, -1.0f);
		Vec3 p4 = Vec3(-1.0f, -1.0f, 1.0f);
		Vec3 p5 = Vec3(1.0f, -1.0f, 1.0f);
		Vec3 p6 = Vec3(1.0f, 1.0f, 1.0f);
		Vec3 p7 = Vec3(-1.0f, 1.0f, 1.0f);

		vertices.push_back( addVertex(p0, Vec3(0.0f, 0.0f, -1.0f), 0.0f, 1.0f));
		vertices.push_back( addVertex(p1, Vec3(0.0f, 0.0f, -1.0f), 1.0f, 1.0f));
		vertices.push_back( addVertex(p2, Vec3(0.0f, 0.0f, -1.0f), 1.0f, 0.0f));
		vertices.push_back( addVertex(p3, Vec3(0.0f, 0.0f, -1.0f), 0.0f, 0.0f));
		vertices.push_back( addVertex(p5, Vec3(0.0f, 0.0f, 1.0f), 0.0f, 1.0f));
		vertices.push_back( addVertex(p4, Vec3(0.0f, 0.0f, 1.0f), 1.0f, 1.0f));
		vertices.push_back( addVertex(p7, Vec3(0.0f, 0.0f, 1.0f), 1.0f, 0.0f));
		vertices.push_back( addVertex(p6, Vec3(0.0f, 0.0f, 1.0f), 0.0f, 0.0f));
		vertices.push_back( addVertex(p4, Vec3(-1.0f, 0.0f, 0.0f), 0.0f, 1.0f));
		vertices.push_back( addVertex(p0, Vec3(-1.0f, 0.0f, 0.0f), 1.0f, 1.0f));
		vertices.push_back( addVertex(p3, Vec3(-1.0f, 0.0f, 0.0f), 1.0f, 0.0f));
		vertices.push_back( addVertex(p7, Vec3(-1.0f, 0.0f, 0.0f), 0.0f, 0.0f));

		vertices.push_back( addVertex(p1, Vec3(1.0f, 0.0f, 0.0f), 0.0f, 1.0f));
		vertices.push_back( addVertex(p5, Vec3(1.0f, 0.0f, 0.0f), 1.0f, 1.0f));
		vertices.push_back( addVertex(p6, Vec3(1.0f, 0.0f, 0.0f), 1.0f, 0.0f));
		vertices.push_back( addVertex(p2, Vec3(1.0f, 0.0f, 0.0f), 0.0f, 0.0f));
		vertices.push_back( addVertex(p3, Vec3(0.0f, 1.0f, 0.0f), 0.0f, 1.0f));
		vertices.push_back( addVertex(p2, Vec3(0.0f, 1.0f, 0.0f), 1.0f, 1.0f));
		vertices.push_back( addVertex(p6, Vec3(0.0f, 1.0f, 0.0f), 1.0f, 0.0f));
		vertices.push_back( addVertex(p7, Vec3(0.0f, 1.0f, 0.0f), 0.0f, 0.0f));
		vertices.push_back( addVertex(p4, Vec3(0.0f, -1.0f, 0.0f), 0.0f, 1.0f));
		vertices.push_back( addVertex(p5, Vec3(0.0f, -1.0f, 0.0f), 1.0f, 1.0f));
		vertices.push_back( addVertex(p1, Vec3(0.0f, -1.0f, 0.0f), 1.0f, 0.0f));
		vertices.push_back( addVertex(p0, Vec3(0.0f, -1.0f, 0.0f), 0.0f, 0.0f));

		std::vector<unsigned int> indices;
		indices.push_back(0); indices.push_back(1); indices.push_back(2);
		indices.push_back(0); indices.push_back(2); indices.push_back(3);
		indices.push_back(4); indices.push_back(5); indices.push_back(6);
		indices.push_back(4); indices.push_back(6); indices.push_back(7);
		indices.push_back(8); indices.push_back(9); indices.push_back(10);
		indices.push_back(8); indices.push_back(10); indices.push_back(11);
		indices.push_back(12); indices.push_back(13); indices.push_back(14);
		indices.push_back(12); indices.push_back(14); indices.push_back(15);
		indices.push_back(16); indices.push_back(17); indices.push_back(18);
		indices.push_back(16); indices.push_back(18); indices.push_back(19);
		indices.push_back(20); indices.push_back(21); indices.push_back(22);
		indices.push_back(20); indices.push_back(22); indices.push_back(23);
		
		mesh.init(core, vertices, indices);

	
		shader.init(core);

		psos.createPSO(core, "Triangle", shader.vertexShader, shader.pixelShader, mesh.inputLayoutDesc);
	}
	void apply(Core* core) {
		// Bind VS buffers
		unsigned int slot = 0;



		/*for (auto i : shader.ps_constantBuffer)
		{
			core->getCommandList()->SetGraphicsRootConstantBufferView(1, shader.ps_constantBuffer[i.first].getGPUAddress());
			shader.ps_constantBuffer[i.first].next();
			slot++;

		}*/

		for (auto& pair : shader.vs_constantBuffer)
		{

			core->getCommandList()->SetGraphicsRootConstantBufferView(0, pair.second.getGPUAddress());
			pair.second.next();
			//core->rootSignature.
			slot++;

		}

	}
	void draw(Core* core, Matrix* w, Matrix* vp)
	{


		core->beginRenderPass();
	
		shader.vs_constantBuffer["staticMeshBuffer"].update("W", w);
		shader.vs_constantBuffer["staticMeshBuffer"].update("VP",vp);
		
		//shader.ps_constantBuffer["bufferName"].update("time", &cb->time);
		//shader.ps_constantBuffer["bufferName"].update("lights", &cb->lights);

		apply(core);
		psos.bind(core, "Triangle");
		mesh.draw(core);
	}

};
class GEMtest {
public:


	Shader shader;
	PRIM_VERTEX vertices[3];
	PSOManager psos;
	GeneralMesh mesh;
	STATIC_VERTEX addVertex(Vec3 p, Vec3 n, float tu, float tv)
	{
		STATIC_VERTEX v;
		v.pos = p;
		v.normal = n;
		v.tangent = Vec3(0, 0, 0); // For now
		v.tu = tu;
		v.tv = tv;
		return v;
	}
	void init(Core* core) {

		std::vector<STATIC_VERTEX> vertices;
		Vec3 p0 = Vec3(-1.0f, -1.0f, -1.0f);
		Vec3 p1 = Vec3(1.0f, -1.0f, -1.0f);
		Vec3 p2 = Vec3(1.0f, 1.0f, -1.0f);
		Vec3 p3 = Vec3(-1.0f, 1.0f, -1.0f);
		Vec3 p4 = Vec3(-1.0f, -1.0f, 1.0f);
		Vec3 p5 = Vec3(1.0f, -1.0f, 1.0f);
		Vec3 p6 = Vec3(1.0f, 1.0f, 1.0f);
		Vec3 p7 = Vec3(-1.0f, 1.0f, 1.0f);

		vertices.push_back(addVertex(p0, Vec3(0.0f, 0.0f, -1.0f), 0.0f, 1.0f));
		vertices.push_back(addVertex(p1, Vec3(0.0f, 0.0f, -1.0f), 1.0f, 1.0f));
		vertices.push_back(addVertex(p2, Vec3(0.0f, 0.0f, -1.0f), 1.0f, 0.0f));
		vertices.push_back(addVertex(p3, Vec3(0.0f, 0.0f, -1.0f), 0.0f, 0.0f));
		vertices.push_back(addVertex(p5, Vec3(0.0f, 0.0f, 1.0f), 0.0f, 1.0f));
		vertices.push_back(addVertex(p4, Vec3(0.0f, 0.0f, 1.0f), 1.0f, 1.0f));
		vertices.push_back(addVertex(p7, Vec3(0.0f, 0.0f, 1.0f), 1.0f, 0.0f));
		vertices.push_back(addVertex(p6, Vec3(0.0f, 0.0f, 1.0f), 0.0f, 0.0f));
		vertices.push_back(addVertex(p4, Vec3(-1.0f, 0.0f, 0.0f), 0.0f, 1.0f));
		vertices.push_back(addVertex(p0, Vec3(-1.0f, 0.0f, 0.0f), 1.0f, 1.0f));
		vertices.push_back(addVertex(p3, Vec3(-1.0f, 0.0f, 0.0f), 1.0f, 0.0f));
		vertices.push_back(addVertex(p7, Vec3(-1.0f, 0.0f, 0.0f), 0.0f, 0.0f));

		vertices.push_back(addVertex(p1, Vec3(1.0f, 0.0f, 0.0f), 0.0f, 1.0f));
		vertices.push_back(addVertex(p5, Vec3(1.0f, 0.0f, 0.0f), 1.0f, 1.0f));
		vertices.push_back(addVertex(p6, Vec3(1.0f, 0.0f, 0.0f), 1.0f, 0.0f));
		vertices.push_back(addVertex(p2, Vec3(1.0f, 0.0f, 0.0f), 0.0f, 0.0f));
		vertices.push_back(addVertex(p3, Vec3(0.0f, 1.0f, 0.0f), 0.0f, 1.0f));
		vertices.push_back(addVertex(p2, Vec3(0.0f, 1.0f, 0.0f), 1.0f, 1.0f));
		vertices.push_back(addVertex(p6, Vec3(0.0f, 1.0f, 0.0f), 1.0f, 0.0f));
		vertices.push_back(addVertex(p7, Vec3(0.0f, 1.0f, 0.0f), 0.0f, 0.0f));
		vertices.push_back(addVertex(p4, Vec3(0.0f, -1.0f, 0.0f), 0.0f, 1.0f));
		vertices.push_back(addVertex(p5, Vec3(0.0f, -1.0f, 0.0f), 1.0f, 1.0f));
		vertices.push_back(addVertex(p1, Vec3(0.0f, -1.0f, 0.0f), 1.0f, 0.0f));
		vertices.push_back(addVertex(p0, Vec3(0.0f, -1.0f, 0.0f), 0.0f, 0.0f));

		std::vector<unsigned int> indices;
		indices.push_back(0); indices.push_back(1); indices.push_back(2);
		indices.push_back(0); indices.push_back(2); indices.push_back(3);
		indices.push_back(4); indices.push_back(5); indices.push_back(6);
		indices.push_back(4); indices.push_back(6); indices.push_back(7);
		indices.push_back(8); indices.push_back(9); indices.push_back(10);
		indices.push_back(8); indices.push_back(10); indices.push_back(11);
		indices.push_back(12); indices.push_back(13); indices.push_back(14);
		indices.push_back(12); indices.push_back(14); indices.push_back(15);
		indices.push_back(16); indices.push_back(17); indices.push_back(18);
		indices.push_back(16); indices.push_back(18); indices.push_back(19);
		indices.push_back(20); indices.push_back(21); indices.push_back(22);
		indices.push_back(20); indices.push_back(22); indices.push_back(23);

		//mesh.init(core, vertices, indices);


		shader.init(core);

		psos.createPSO(core, "Triangle", shader.vertexShader, shader.pixelShader, mesh.inputLayoutDesc);
	}
	void apply(Core* core) {
		// Bind VS buffers
		unsigned int slot = 0;



		/*for (auto i : shader.ps_constantBuffer)
		{
			core->getCommandList()->SetGraphicsRootConstantBufferView(1, shader.ps_constantBuffer[i.first].getGPUAddress());
			shader.ps_constantBuffer[i.first].next();
			slot++;

		}*/

		for (auto& pair : shader.vs_constantBuffer)
		{

			core->getCommandList()->SetGraphicsRootConstantBufferView(0, pair.second.getGPUAddress());
			pair.second.next();
			//core->rootSignature.
			slot++;

		}

	}
	void draw(Core* core, Matrix* w, Matrix* vp)
	{


		core->beginRenderPass();

		shader.vs_constantBuffer["staticMeshBuffer"].update("W", w);
		shader.vs_constantBuffer["staticMeshBuffer"].update("VP", vp);

		//shader.ps_constantBuffer["bufferName"].update("time", &cb->time);
		//shader.ps_constantBuffer["bufferName"].update("lights", &cb->lights);

		apply(core);
		psos.bind(core, "Triangle");
		mesh.draw(core);
	}

};
class Window;
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
Window* window;
class Window
{
public:
	bool keys[256];
	int mousex;
	int mousey;
	bool mouseButtons[3];
	std::wstring wname;
	HWND hwnd;
	HINSTANCE hinstance;
	string name;
	int __width = 800;
	int __height = 600;
	DWORD style = WS_OVERLAPPEDWINDOW | WS_VISIBLE;

	Window() {
		__width = kuan;
		__height = gao;

		WNDCLASSEX wc = {};
		hinstance = GetModuleHandle(NULL);
		name = "window_name";
		wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
		wc.lpfnWndProc = WndProc;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = hinstance;
		wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
		wc.hIconSm = wc.hIcon;
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
		wc.lpszMenuName = NULL;
		wname = std::wstring(name.begin(), name.end());
		wc.lpszClassName = wname.c_str();
		wc.cbSize = sizeof(WNDCLASSEX);
		RegisterClassEx(&wc);


		hwnd = CreateWindowEx(WS_EX_APPWINDOW, wname.c_str(), wname.c_str(), style,
			0, 0, __width, __height, NULL, NULL, hinstance, this);
		window = this;
	}

	void updateMouse(int x, int y)
	{
		mousex = x;
		mousey = y;
	}
	void processMessages() {
		MSG msg;
		ZeroMemory(&msg, sizeof(MSG));
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	void create(int w, int h, string n) {
		__width = w;
		__height = h;
		name = n;
		SetWindowTextA(hwnd, name.c_str());
		SetWindowPos(hwnd, 0, 0, 0, __width, __height, SWP_NOMOVE | SWP_NOZORDER);
	}
};
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{

	switch (msg)
	{
	case WM_CLOSE:
		PostQuitMessage(0);
		exit(0);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		exit(0);
		return 0;


	case WM_KEYDOWN:
	{
		window->keys[(unsigned int)wParam] = true;
		return 0;
	}
	case WM_KEYUP:
	{
		window->keys[(unsigned int)wParam] = false;
		return 0;
	}
	case WM_LBUTTONDOWN:
	{
		window->updateMouse(WINDOW_GET_X_LPARAM(lParam), WINDOW_GET_Y_LPARAM(lParam));
		window->mouseButtons[0] = true;
		return 0;
	}
	case WM_LBUTTONUP:
	{
		window->updateMouse(WINDOW_GET_X_LPARAM(lParam), WINDOW_GET_Y_LPARAM(lParam));
		window->mouseButtons[0] = false;
		return 0;
	}
	case WM_MOUSEMOVE:
	{
		window->updateMouse(WINDOW_GET_X_LPARAM(lParam), WINDOW_GET_Y_LPARAM(lParam));
		return 0;
	}
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
}

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	PSTR lpCmdLine, int nCmdShow) {



	


	Window win;
	Core core;
	core.init(window->hwnd, kuan, gao);
	
	Cube cube;
	cube.init(&core);


	Matrix prespection;
	prespection = prespection.Perspective(M_PI / 4, kuan / gao, 0.3f, 100.0f);

	ConstantBuffer3 constBufferCPU3;
	
	constBufferCPU3.VP= prespection.Perspective(M_PI / 4, kuan / gao, 0.3f, 100.0f);
	
	GamesEngineeringBase::Timer timer;
	
	win.create(kuan, gao, "My Window");
	float dt=0;

	std::vector<GEMtest> meshes;
	GEMLoader::GEMModelLoader loader;
	std::vector<GEMLoader::GEMMesh> gemmeshes;
	loader.load("../Resources/acacia_003.gem", gemmeshes);
	for (int i = 0; i < gemmeshes.size(); i++) {
		GEMtest mesh;
		std::vector<STATIC_VERTEX> vertices;
		for (int j = 0; j < gemmeshes[i].verticesStatic.size(); j++) {
			STATIC_VERTEX v;
			memcpy(&v, &gemmeshes[i].verticesStatic[j], sizeof(STATIC_VERTEX));
			vertices.push_back(v);
		}
		mesh.mesh.init(&core, vertices, gemmeshes[i].indices);
		mesh.init(&core);
		meshes.push_back(mesh);
	}
	Matrix world;
	
	world.scale(0.01f, 0.01f, 0.01f);

	while (1) {
		

		dt += timer.dt();
		//constBufferCPU1.time += dt;
		Vec4 from = Vec4(11 * cos(dt), 5, 11 * sin(dt), 0);
		Vec4 to = Vec4(0, 1, 0, 0);
		Vec4 up = Vec4(0, 1, 0, 0);
		constBufferCPU3.w = constBufferCPU3.w.LookatMatrix(from, to, up);
		Matrix scv111 = constBufferCPU3.VP.mul(constBufferCPU3.w);
		//constBufferCPU3.w = constBufferCPU3.w.lookAtMatrix(from.TransToVec3(), to.TransToVec3(), up.TransToVec3());
		core.beginFrame();
		win.processMessages();
		if (win.keys[VK_ESCAPE] == 1)
		{
			break;
		}

		//cube.draw(&core, &constBufferCPU3.w, &constBufferCPU3.VP);
		for (int i = 0; i < meshes.size(); i++)
		{
			meshes[i].draw(&core, &world, &scv111);
		}
		core.finishFrame();
	}
	core.flushGraphicsQueue();

}
// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
