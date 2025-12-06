// NewWorld.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#define kuan 1920.f
#define gao 1080.f
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
#include <Vector>
#include "GEMLoader.h"
#include "Animation.h"
#include "Enums.h"
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
struct ANIMATED_VERTEX
{
	Vec3 pos;
	Vec3 normal;
	Vec3 tangent;
	float tu;
	float tv;
	unsigned int bonesIDs[4];
	float boneWeights[4];
};
class collider;
map<std::string, ConstantBufferVariable> scvvv;

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
	void init(Core* core, std::vector<ANIMATED_VERTEX> vertices, std::vector<unsigned int> indices)
	{
		init(core, &vertices[0], sizeof(ANIMATED_VERTEX), vertices.size(), &indices[0], indices.size());
		inputLayoutDesc = VertexLayoutCache::getAnimatedLayout();
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
	void Compile(Core* core,string vs,string ps) {
		debug = 1;
		OutputDebugStringA(to_string(debug).c_str());
		//Compile vertex shader
		ID3DBlob* status;
		string vertexShadersStr = ReadShader(vs);
		HRESULT hr = D3DCompile(vertexShadersStr.c_str(), strlen(vertexShadersStr.c_str()), NULL,NULL, NULL, "VS", "vs_5_0", 0, 0, &vertexShader, &status);
		string pixelShaderStr = ReadShader(ps);
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
	void init(Core* core, string vs, string ps) {

		Compile(core,vs,ps);
	
		
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

class Cube {
public:
	

	
	PRIM_VERTEX vertices[3];
	
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
	void init(Core* core, PSOManager* psos, Shader* shader) {

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

	
		//shader->init(core,"ShaderVertices.hlsl","ShaderPixel.hlsl");

		psos->createPSO(core, "Cube", shader->vertexShader, shader->pixelShader, mesh.inputLayoutDesc);
	}
	void apply(Core* core, Shader* shader) {
		// Bind VS buffers
		unsigned int slot = 0;



		/*for (auto i : shader.ps_constantBuffer)
		{
			core->getCommandList()->SetGraphicsRootConstantBufferView(1, shader.ps_constantBuffer[i.first].getGPUAddress());
			shader.ps_constantBuffer[i.first].next();
			slot++;

		}*/

		for (auto& pair : shader->vs_constantBuffer)
		{

			core->getCommandList()->SetGraphicsRootConstantBufferView(0, pair.second.getGPUAddress());
			pair.second.next();
			//core->rootSignature.
			slot++;

		}

	}
	void draw(Core* core, Matrix* w, Matrix* vp, Shader* shader,PSOManager* psos)
	{


		
	
		shader->vs_constantBuffer["staticMeshBuffer"].update("W", w);
		shader->vs_constantBuffer["staticMeshBuffer"].update("VP",vp);
		
		//shader.ps_constantBuffer["bufferName"].update("time", &cb->time);
		//shader.ps_constantBuffer["bufferName"].update("lights", &cb->lights);

		apply(core,shader);
		psos->bind(core, "Cube");
		mesh.draw(core);
	}

};




class Collider {
public:
	vector<ANIMATED_VERTEX> animatebox;
	vector<STATIC_VERTEX> staticbox;
	Vec3 realminpoint;
	Vec3 realmaxpoint;
	Vec3 minpoint;
	Vec3 maxpoint;
	void staticinit(vector<STATIC_VERTEX> _staticbox,Vec3 _position) {
		staticbox = _staticbox;

		minpoint = Vec3(11111, 11111, 11111);
		maxpoint = Vec3(-11111, -11111, -11111);
		staticcalculateBox();
		realminpoint = minpoint + _position;
		realmaxpoint = maxpoint + _position;

	}
	void animateinit(vector<ANIMATED_VERTEX> _animatebox, Vec3 _position) {
		animatebox = _animatebox;
		minpoint = Vec3(11111, 11111, 11111);
		maxpoint = Vec3(-11111, -11111, -11111);
		animatecalculateBox();
		realminpoint = minpoint + _position;
		realmaxpoint = maxpoint + _position;
		
	}
	void hero(Vec3 position) {
		minpoint =Vec3(0,0,0);
		maxpoint = minpoint + 1;
		realminpoint = position+ minpoint;
		realmaxpoint = maxpoint + position;

	}
	

	void staticcalculateBox() {
		for (int i = 0; i < staticbox.size(); i++) {

			if (staticbox[i].pos.x > maxpoint.x) {
				maxpoint.x = staticbox[i].pos.x;
			}
			if (staticbox[i].pos.y > maxpoint.y) {
				maxpoint.y = staticbox[i].pos.y;
			}
			if (staticbox[i].pos.z > maxpoint.z) {
				maxpoint.z = staticbox[i].pos.z;
			}
			if (staticbox[i].pos.x < minpoint.x) {
				minpoint.x = staticbox[i].pos.x;
			}
			if (staticbox[i].pos.y < minpoint.y) {
				minpoint.y = staticbox[i].pos.y;
			}
			if (staticbox[i].pos.z < minpoint.z) {
				minpoint.z = staticbox[i].pos.z;
			}
		}
		minpoint = minpoint * 0.01f;
		maxpoint = maxpoint * 0.01f;
	}
	void update(Vec3 position,Matrix scv) {
		vector<Vec3>corner;
		corner.push_back(Vec3(minpoint.x, minpoint.y, minpoint.z));
		corner.push_back(Vec3(minpoint.x, minpoint.y, maxpoint.z));
		corner.push_back(Vec3(minpoint.x, maxpoint.y, minpoint.z));
		corner.push_back(Vec3(minpoint.x, maxpoint.y, maxpoint.z));
		corner.push_back(Vec3(maxpoint.x, minpoint.y, minpoint.z));
		corner.push_back(Vec3(maxpoint.x, minpoint.y, maxpoint.z));
		corner.push_back(Vec3(maxpoint.x, maxpoint.y, minpoint.z));
		corner.push_back(Vec3(maxpoint.x, maxpoint.y, maxpoint.z));
		for (int i = 0; i < 8; i++) {
			corner[i] = scv.MulVec3(scv, corner[i]);
		}
		realminpoint = Vec3(11111, 11111, 11111);
		realmaxpoint = Vec3(-11111, -11111, -11111);
		for (int i = 0; i < 8; i++) {
			if (realmaxpoint.x < corner[i].x) {
				realmaxpoint.x = corner[i].x;
			}
			if (realmaxpoint.y < corner[i].y) {
				realmaxpoint.y = corner[i].y;
			}
			if (realmaxpoint.z < corner[i].z) {
				realmaxpoint.z = corner[i].z;
			}
			if (realminpoint.x > corner[i].x) {
				realminpoint.x = corner[i].x;
			}
			if (realminpoint.y > corner[i].y) {
				realminpoint.y = corner[i].y;
			}
			if (realminpoint.z > corner[i].z) {
				realminpoint.z = corner[i].z;
			}
		}
		realminpoint += position;
		realmaxpoint += position;

		OutputDebugStringA(to_string(realminpoint.x).c_str());
		OutputDebugStringA("      ");
		OutputDebugStringA(to_string(realminpoint.y).c_str());
		OutputDebugStringA("      ");
		OutputDebugStringA(to_string(realminpoint.z).c_str());
		OutputDebugStringA("\n");
		OutputDebugStringA(to_string(realmaxpoint.x).c_str());
		OutputDebugStringA("      ");
		OutputDebugStringA(to_string(realmaxpoint.y).c_str());
		OutputDebugStringA("      ");
		OutputDebugStringA(to_string(realmaxpoint.z).c_str());
		OutputDebugStringA("\n");
	}
	void updatehero(Vec3 position) {
		realminpoint = position+minpoint;
		realmaxpoint = position +maxpoint;
	}
	void animatecalculateBox() {
		for (int i = 0; i < animatebox.size(); i++) {

			if (animatebox[i].pos.x > maxpoint.x) {
				maxpoint.x = animatebox[i].pos.x;
			}
			if (animatebox[i].pos.y > maxpoint.y) {
				maxpoint.y = animatebox[i].pos.y;
			}
			if (animatebox[i].pos.z > maxpoint.z) {
				maxpoint.z = animatebox[i].pos.z;
			}
			if (animatebox[i].pos.x < minpoint.x) {
				minpoint.x = animatebox[i].pos.x;
			}
			if (animatebox[i].pos.y < minpoint.y) {
				minpoint.y = animatebox[i].pos.y;
			}
			if (animatebox[i].pos.z < minpoint.z) {
				minpoint.z = animatebox[i].pos.z;
			}
		}
		minpoint = minpoint * 0.01f;
		maxpoint = maxpoint * 0.01f;
	}

	bool AABBtest(const Vec3& minB, const Vec3& maxB)
	{

		if (realmaxpoint.x < minB.x || realminpoint.x > maxB.x)
			return false;
		if (realmaxpoint.y < minB.y || realminpoint.y > maxB.y)
			return false;
		if (realmaxpoint.z < minB.z || realminpoint.z > maxB.z)
			return false;

		return true;
	}

};

class StaticModle {
public:
	Collider collision;
	Staticmodels scv;
	Vec3 position;
	Vec3 scale;
	Matrix realshow;
	vector<STATIC_VERTEX> verticescout;
	vector<GeneralMesh *> meshes;
	//GeneralMesh mesh;
	std::vector<std::string> textureFilenames;
	void load(Core* core, std::string filename, Shaders* shaders, PSOManager* psos, Staticmodels _name,Vec3 _position)
	{
		
		scv = _name;
		position = _position;
		scale = Vec3(0.01f, 0.01f, 0.01f);
		realshow = Matrix::translation(position) * Matrix::scaling(scale);
		GEMLoader::GEMModelLoader loader;
		std::vector<GEMLoader::GEMMesh> gemmeshes;
		loader.load(filename, gemmeshes);
		for (int i = 0; i < gemmeshes.size(); i++)
		{
			GeneralMesh* mesh = new GeneralMesh();
			std::vector<STATIC_VERTEX> vertices;
			for (int j = 0; j < gemmeshes[i].verticesStatic.size(); j++)
			{
				STATIC_VERTEX v;
				memcpy(&v, &gemmeshes[i].verticesStatic[j], sizeof(STATIC_VERTEX));
				vertices.push_back(v);
				verticescout.push_back(v);
			}
			mesh->init(core, vertices, gemmeshes[i].indices);
			meshes.push_back(mesh);
		}

		psos->createPSO(core, "StaticModelPSO", shaders->shaders["shader1"].vertexShader, shaders->shaders["shader1"].pixelShader, VertexLayoutCache::getStaticLayout());
		collision.staticinit(verticescout,position);
	}

	void apply(Core* core, Shader* shader) {
		// Bind VS buffers
		unsigned int slot = 0;

		for (auto& pair : shader->vs_constantBuffer)
		{

			core->getCommandList()->SetGraphicsRootConstantBufferView(0, pair.second.getGPUAddress());
			pair.second.next();
			//core->rootSignature.
			slot++;

		}

	}
	void draw(Core* core, Matrix* w, Matrix* vp, Shader* shader, PSOManager* psos)
	{
		
		
		
		

		
		realshow = Matrix::translation(position) * Matrix::scaling(scale);
		shader->vs_constantBuffer["staticMeshBuffer"].update("W", w);
		shader->vs_constantBuffer["staticMeshBuffer"].update("VP", vp);

		//shader.ps_constantBuffer["bufferName"].update("time", &cb->time);
		//shader.ps_constantBuffer["bufferName"].update("lights", &cb->lights);

		apply(core, shader);
		psos->bind(core, "StaticModelPSO");
		for (int i = 0; i < meshes.size(); i++)
		{
			meshes[i]->draw(core);
		}

	}

};
class AnimatedModel {
public:
	Animatemodels scv;
	Collider collision;
	Vec3 position;
	Vec3 scale;
	Vec3 forward;
	Matrix realshow;
	
	
	vector<GeneralMesh*> meshes;
	vector<ANIMATED_VERTEX> verticescout;
	Animation animation;
	//GeneralMesh mesh;
	std::vector<std::string> textureFilenames;
	void load(Core* core, std::string filename, Shaders* shaders, PSOManager* psos, Animatemodels _enum,Vec3 _position)
	{
		scv = _enum;
	
		position = _position;
		scale = Vec3(0.01f, 0.01f, 0.01f);
		realshow = Matrix::translation(position) * Matrix::scaling(scale);

		GEMLoader::GEMModelLoader loader;
		std::vector<GEMLoader::GEMMesh> gemmeshes;
		GEMLoader::GEMAnimation gemanimation;
		loader.load(filename, gemmeshes, gemanimation);
		for (int i = 0; i < gemmeshes.size(); i++)
		{
			GeneralMesh* mesh = new GeneralMesh();
			std::vector<ANIMATED_VERTEX> vertices;
			for (int j = 0; j < gemmeshes[i].verticesAnimated.size(); j++)
			{
				ANIMATED_VERTEX v;
				memcpy(&v, &gemmeshes[i].verticesAnimated[j], sizeof(ANIMATED_VERTEX));
				vertices.push_back(v);
				verticescout.push_back(v);
			}
			mesh->init(core, vertices, gemmeshes[i].indices);
		
			meshes.push_back(mesh);
		}

		psos->createPSO(core, "AnimatedModelPSO", shaders->shaders["shaderAnim"].vertexShader, shaders->shaders["shaderAnim"].pixelShader, VertexLayoutCache::getAnimatedLayout());
		memcpy(&animation.skeleton.globalInverse, &gemanimation.globalInverse, 16 * sizeof(float));
		for (int i = 0; i < gemanimation.bones.size(); i++)
		{
			Bone bone;
			bone.name = gemanimation.bones[i].name;
			memcpy(&bone.offset, &gemanimation.bones[i].offset, 16 * sizeof(float));
			bone.parentIndex = gemanimation.bones[i].parentIndex;
			animation.skeleton.bones.push_back(bone);
		}
		for (int i = 0; i < gemanimation.animations.size(); i++)
		{
			std::string name = gemanimation.animations[i].name;
			AnimationSequence aseq;
			aseq.ticksPerSecond = gemanimation.animations[i].ticksPerSecond;
			for (int j = 0; j < gemanimation.animations[i].frames.size(); j++)
			{
				AnimationFrame frame;
				for (int index = 0; index < gemanimation.animations[i].frames[j].positions.size(); index++)
				{
					Vec3 p;
					Quaternion q;
					Vec3 s;
					memcpy(&p, &gemanimation.animations[i].frames[j].positions[index], sizeof(Vec3));
					frame.positions.push_back(p);
					memcpy(&q, &gemanimation.animations[i].frames[j].rotations[index], sizeof(Quaternion));
					frame.rotations.push_back(q);
					memcpy(&s, &gemanimation.animations[i].frames[j].scales[index], sizeof(Vec3));
					frame.scales.push_back(s);
				}
				aseq.frames.push_back(frame);
			}
			animation.animations.insert({ name, aseq });
		}
		collision.animateinit(verticescout,position);
	}

	void apply(Core* core, Shader* shader) {
		// Bind VS buffers
		unsigned int slot = 0;

		for (auto& pair : shader->vs_constantBuffer)
		{

			core->getCommandList()->SetGraphicsRootConstantBufferView(0, pair.second.getGPUAddress());
			pair.second.next();
			//core->rootSignature.
			slot++;

		}

	}
	void draw(Core* core, Matrix* w, Matrix* vp, Shader* shader, PSOManager* psos, AnimationInstance* instance,Matrix &roation)
	{
		
		realshow =  Matrix::translation(position)* roation * Matrix::scaling(scale);
	
		shader->vs_constantBuffer["staticMeshBuffer"].update("W", w);
		shader->vs_constantBuffer["staticMeshBuffer"].update("VP", vp);
		shader->vs_constantBuffer["staticMeshBuffer"].update("bones", instance->matrices);

		//shader.ps_constantBuffer["bufferName"].update("time", &cb->time);
		//shader.ps_constantBuffer["bufferName"].update("lights", &cb->lights);
		
		apply(core, shader);
		psos->bind(core, "AnimatedModelPSO");
		for (int i = 0; i < meshes.size(); i++)
		{
			meshes[i]->draw(core);
		}

	}

};
class Hero {
public:
	AnimatedModel heromodel;
	AnimationInstance heromodelinstace;
	float movespeed = 10.f;
	Vec3 position;
	Vec3 forward;
	Vec3 right;
	Vec3 to;
	float cooldown = 0.2f;
	float timecount = 0.f;
	void init(Core* core, Shaders* shaders, PSOManager* psos, Vec3 position) {
		heromodel.load(core, "../Resources/UZI/Uzi.gem", shaders, psos, Animatemodels::UZI, position);
		heromodelinstace.init(&heromodel.animation, 0);
	}
	
};

class Enemies {
public:
	AnimatedModel enemymodel;
	AnimationInstance enemymodelinstace;
	float health = 100.f;
	Vec3 forward;
	Vec3 position;
	float cooldown = 5.f;
	float timecount = 0.f;
	float movespeed = 0.5f;
	void init(Core* core, Shaders* shaders, PSOManager* psos,Vec3 position) {
		enemymodel.load(core, "../Resources/TRex.gem", shaders, psos, Animatemodels::TRex, position);
		enemymodelinstace.init(&enemymodel.animation, 0);
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
	PSOManager psos;
	vector<StaticModle> staticmodles;
	vector<AnimatedModel>animateModels;
	vector<AnimationInstance>animationinstances;
	vector<Enemies> enemies;
	Window win;
	Core core;
	core.init(window->hwnd, kuan, gao);
	
	Shader shader;


	Shaders shaders;
	shaders.load(&core, "shader1", "ShaderVertices.hlsl", "ShaderPixel.hlsl");
	shaders.load(&core, "shaderAnim", "ShaderVerticesAnim.hlsl", "ShaderPixel.hlsl");

	
	Cube cube;
	cube.init(&core,&psos, &shaders.shaders["shader1"]);


	/*
	StaticModle tree;
	tree.load(&core, "../Resources/acacia_003.gem", &shaders, &psos, Staticmodels::Tree,Vec3(0,0,0));
	staticmodles.push_back(tree);
	StaticModle tree1;
	tree1.load(&core, "../Resources/acacia_003.gem", &shaders, &psos, Staticmodels::Tree, Vec3(0, 0, 5));
	staticmodles.push_back(tree1);
	*/

	AnimatedModel animatedModel;
	animatedModel.load(&core, "../Resources/TRex.gem", &shaders, &psos,Animatemodels::TRex, Vec3(0, 0, 0));
	animateModels.push_back(animatedModel);
	AnimationInstance animatedInstance;
	animatedInstance.init(&animatedModel.animation, 0);
	animationinstances.push_back(animatedInstance);

	//AnimatedModel UZI;
	//UZI.load(&core, "../Resources/UZI/Uzi.gem", &shaders, &psos, Animatemodels::UZI, Vec3(0, 0, 10));
	//AnimationInstance UZIInstance;
	//UZIInstance.init(&UZI.animation, 0);
	//animateModels.push_back(UZI);
	//animationinstances.push_back(UZIInstance);


	Hero hero;
	hero.init(&core, &shaders, &psos, Vec3(0, 0, 0));
	hero.heromodel.collision.hero(hero.position);

	Enemies enemy;
	enemy.init(&core, &shaders, &psos, Vec3(0, 0, 10));
	enemies.push_back(enemy);
	
	//Collider hero;
	//hero.hero(Vec3(16, 0, 4));








	Matrix world;
	world = world.scale(0.01f, 0.01f, 0.01f);
	Matrix prespection;
	prespection = prespection.Perspective(M_PI / 4, kuan / gao, 0.1f, 100.0f);
	Matrix lookat;
	ConstantBuffer3 constBufferCPU3;
	Matrix vp;
	constBufferCPU3.VP= prespection.Perspective(M_PI / 4, kuan / gao, 0.1f, 100.0f);
	
	GamesEngineeringBase::Timer timer;
	
	win.create(kuan, gao, "My Window");
	float dt=0;
	float rexdt;
	float cameramovespeed = 5.f;
	Vec4 to = Vec4(0, 0, 0, 0);
	Vec4 up = Vec4(0, 1, 0, 0);
	Vec4 from = Vec4(16,0,4, 0);
	Vec4 forward = Vec4(0, 0,1, 0);
	Vec4 right;
	float yaw = 0.0f;    // ÈÆYÖáÐý×ª
	float pitch = 0.0f;
	static int lastMouseX = window->mousex;
	static int lastMouseY = window->mousey;

	int deltaX = window->mousex - lastMouseX;
	int deltaY = window->mousey - lastMouseY;
	float mouseSensitivity = 0.01f;
	while (1) {
		rexdt = timer.dt();
		dt += rexdt;
		
		deltaX = window->mousex - lastMouseX;
		deltaY = window->mousey - lastMouseY;
		yaw -= deltaX * mouseSensitivity;
		pitch -= deltaY * mouseSensitivity;

		float limit = 1.5f; 
		if (pitch > limit) pitch = limit;
		if (pitch < -limit) pitch = -limit;

		forward.x = cosf(pitch) * cosf(yaw);
		forward.y = sinf(pitch);
		forward.z = cosf(pitch) * sinf(yaw);
		forward.w = 0;
		lastMouseX = window->mousex;
		lastMouseY = window->mousey;
		to = forward + from;
		right = forward.Cross(Vec4(0, 1, 0, 0));
		if (win.keys[VK_ESCAPE] == 1)
		{
			break;
		}
		if (win.keys['A']) {
			from = from + Vec4(right.x, 0, right.z, 0) * cameramovespeed * rexdt;
			hero.heromodel.collision.updatehero(Vec3(from.x, from.y, from.z));
			
			for (int i = 0; i < enemies.size(); i++) {
				if (hero.heromodel.collision.AABBtest(enemies[i].enemymodel.collision.realminpoint, enemies[i].enemymodel.collision.realmaxpoint)) {
					from = from - Vec4(right.x, 0, right.z, 0) * cameramovespeed * rexdt;
					hero.heromodel.collision.updatehero(Vec3(from.x, from.y, from.z));
				}
			}
			for (int i = 0; i < staticmodles.size(); i++) {
				if (hero.heromodel.collision.AABBtest(staticmodles[i].collision.realminpoint, staticmodles[i].collision.realmaxpoint)) {
					from = from - Vec4(right.x, 0, right.z, 0) * cameramovespeed * rexdt;
					hero.heromodel.collision.updatehero(Vec3(from.x, from.y, from.z));
				}
			}
		}
		if (win.keys['D']) {
			from = from - Vec4(right.x, 0, right.z, 0) * cameramovespeed * rexdt;
			hero.heromodel.collision.updatehero(Vec3(from.x, from.y, from.z));
			for (int i = 0; i < enemies.size(); i++) {
				if (hero.heromodel.collision.AABBtest(enemies[i].enemymodel.collision.realminpoint, enemies[i].enemymodel.collision.realmaxpoint)) {
					from = from + Vec4(right.x, 0, right.z, 0) * cameramovespeed * rexdt;
					hero.heromodel.collision.updatehero(Vec3(from.x, from.y, from.z));
				}
			}
			for (int i = 0; i < staticmodles.size(); i++) {
				if (hero.heromodel.collision.AABBtest(staticmodles[i].collision.realminpoint, staticmodles[i].collision.realmaxpoint)) {
					from = from + Vec4(right.x, 0, right.z, 0) * cameramovespeed * rexdt;
					hero.heromodel.collision.updatehero(Vec3(from.x, from.y, from.z));
				}
			}
		}
		if (win.keys['W']) {
			from = from+Vec4(forward.x,0, forward.z,0) * cameramovespeed * rexdt;
			hero.heromodel.collision.updatehero(Vec3(from.x, from.y, from.z));
			for (int i = 0; i < enemies.size(); i++) {
				if (hero.heromodel.collision.AABBtest(enemies[i].enemymodel.collision.realminpoint, enemies[i].enemymodel.collision.realmaxpoint)) {
					from = from - Vec4(forward.x, 0, forward.z, 0) * cameramovespeed * rexdt;
					hero.heromodel.collision.updatehero(Vec3(from.x, from.y, from.z));
					
				}
			}
			for (int i = 0; i < staticmodles.size(); i++) {
				if (hero.heromodel.collision.AABBtest(staticmodles[i].collision.realminpoint, staticmodles[i].collision.realmaxpoint)) {
					from = from - Vec4(forward.x, 0, forward.z, 0) * cameramovespeed * rexdt;
					hero.heromodel.collision.updatehero(Vec3(from.x, from.y, from.z));

				}
			}




		//OutputDebugStringA(to_string(hero.heromodel.position.x).c_str());
		//OutputDebugStringA("\n");
		//OutputDebugStringA(to_string(hero.heromodel.collision.realmaxpoint.y).c_str());
		//OutputDebugStringA("\n");
		//OutputDebugStringA("x::");
		//OutputDebugStringA(to_string(to.x).c_str());
		//OutputDebugStringA("\n");
		//OutputDebugStringA("z::");
		//OutputDebugStringA(to_string(to.z).c_str());
		//OutputDebugStringA("\n");
		//OutputDebugStringA(to_string(to.y).c_str());
		//OutputDebugStringA("\n");











		}
		if (win.keys['S']) {
			from = from - Vec4(forward.x, 0, forward.z, 0) * cameramovespeed * rexdt;
			hero.heromodel.collision.updatehero(Vec3(from.x, from.y, from.z));
			for (int i = 0; i < enemies.size(); i++) {
				if (hero.heromodel.collision.AABBtest(enemies[i].enemymodel.collision.realminpoint, enemies[i].enemymodel.collision.realmaxpoint)) {
					from = from + Vec4(forward.x, 0, forward.z, 0) * cameramovespeed * rexdt;
					hero.heromodel.collision.updatehero(Vec3(from.x, from.y, from.z));
				}
			}
			for (int i = 0; i < staticmodles.size(); i++) {
				if (hero.heromodel.collision.AABBtest(staticmodles[i].collision.realminpoint, staticmodles[i].collision.realmaxpoint)) {
					from = from + Vec4(forward.x, 0, forward.z, 0) * cameramovespeed * rexdt;
					hero.heromodel.collision.updatehero(Vec3(from.x, from.y, from.z));
				}
			}
		}
		hero.heromodel.position = Vec3(from.x,from.y,from.z);





		//OutputDebugStringA(to_string(from.x).c_str());
		//OutputDebugStringA("\n");
		//OutputDebugStringA(to_string(from.z).c_str());
		//OutputDebugStringA("\n");
		//OutputDebugStringA("x::");
		//OutputDebugStringA(to_string(to.x).c_str());
		//OutputDebugStringA("\n");
		//OutputDebugStringA("z::");
		//OutputDebugStringA(to_string(to.z).c_str());
		//OutputDebugStringA("\n");
		//OutputDebugStringA(to_string(to.y).c_str());
		//OutputDebugStringA("\n");

		//Vec4 from = Vec4(11 * cos(dt), 5, 11 * sin(dt), 0);
		//constBufferCPU1.time += dt;
		
		lookat=lookat.LookatMatrix(from, to, up);
		vp = prespection.mul(lookat);
		//constBufferCPU3.w = constBufferCPU3.w.lookAtMatrix(from.TransToVec3(), to.TransToVec3(), up.TransToVec3());
		core.beginFrame();
		win.processMessages();
		

		core.beginRenderPass();



		for (int i = 0; i < staticmodles.size(); i++) {
			
			staticmodles[i].draw(&core, &staticmodles[i].realshow, &vp, &shaders.shaders["shader1"], &psos);
		}

	
		//tree.position = Vec3(10, 0, 0);
		
		//tree.draw(&core, &tree.realshow, &vp, &shaders.shaders["shader1"], &psos);
		//cube.draw(&core, &constBufferCPU3.w, &constBufferCPU3.VP, &shaders.shaders["shader1"], &psos);
		

		

		for (int i = 0; i < enemies.size(); i++) {
			
			enemies[i].enemymodelinstace.update("run", rexdt);
			if (enemies[i].enemymodelinstace.animationFinished()) {
				enemies[i].enemymodelinstace.resetAnimationTime();
			}

			
			Vec3 scv;
			enemies[i].enemymodel.position = enemies[i].enemymodel.position + enemies[i].enemymodel.forward * enemies[i].movespeed * rexdt;
			scv =  hero.heromodel.position- enemies[i].enemymodel.position;
			scv = scv.normalize();
			enemies[i].enemymodel.forward = scv;
			Matrix R = Matrix::ForwardtoOnlyTRex(scv);
			enemies[i].enemymodel.collision.update(enemies[i].enemymodel.position, R);
			enemies[i].enemymodel.draw(&core, &enemies[i].enemymodel.realshow, &vp, &shaders.shaders["shaderAnim"], &psos, &enemies[i].enemymodelinstace, R);
				
			
		}
		
		hero.heromodelinstace.update("08 fire", rexdt);
		if (hero.heromodelinstace.animationFinished()) {
			hero.heromodelinstace.resetAnimationTime();
		}
		
		Matrix R;
		R = Matrix::ForwardtoTOnlyHero(forward.TransToVec3RemoveW());
		
		hero.heromodel.draw(&core, &hero.heromodel.realshow, &vp, &shaders.shaders["shaderAnim"], &psos, &hero.heromodelinstace,R);
	/*	animatedInstance.update("run", rexdt);
		if (animatedInstance.animationFinished() == true)
		{
			animatedInstance.resetAnimationTime();
		}
		animatedModel.draw(&core, &animatedModel.realshow, &vp,&shaders.shaders["shaderAnim"],&psos,&animatedInstance);

	*/
		/*UZI.position = Vec3(0, 0, 20);
		
		UZIInstance.update("08 fire",rexdt);
		if (UZIInstance.animationFinished() == true)
		{
			UZIInstance.resetAnimationTime();
		}
		UZI.draw(&core, &UZI.realshow, &vp, &shaders.shaders["shaderAnim"], &psos, &UZIInstance);

		*/

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
