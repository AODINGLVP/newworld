#pragma once
#include "ShadersManager.h"
#include "PSOManager.h"

struct PRIM_VERTEX
{
	Vec3 position;
	Colour colour;
};
class Objects
{
};

class Collider {
public:
	vector<ANIMATED_VERTEX> animatebox;
	vector<STATIC_VERTEX> staticbox;
	Vec3 realminpoint;
	Vec3 realmaxpoint;
	Vec3 minpoint;
	Vec3 maxpoint;
	void staticinit(vector<STATIC_VERTEX> _staticbox, Vec3 _position) {
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
		minpoint = Vec3(0, 0, 0);
		maxpoint = minpoint + 1;
		realminpoint = position + minpoint;
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
	void update(Vec3 position, Matrix scv) {
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
		realminpoint = position + minpoint;
		realmaxpoint = position + maxpoint;
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
	void draw(Core* core, Matrix* w, Matrix* vp, Shader* shader, PSOManager* psos)
	{




		shader->vs_constantBuffer["staticMeshBuffer"].update("W", w);
		shader->vs_constantBuffer["staticMeshBuffer"].update("VP", vp);

		//shader.ps_constantBuffer["bufferName"].update("time", &cb->time);
		//shader.ps_constantBuffer["bufferName"].update("lights", &cb->lights);

		apply(core, shader);
		psos->bind(core, "Cube");
		mesh.draw(core);
	}

};

