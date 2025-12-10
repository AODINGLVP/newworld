#pragma once
#include "ShadersManager.h"
#include "PSOManager.h"
#include "Enums.h"
#include "Texture.h"
#include "stb_image.h"
#include "GEMLoader.h"
#include "Animation.h"
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




class Sphere {
public:

	PRIM_VERTEX vertices[3];
	GeneralMesh mesh;
	Vec3 position;
	Vec3 scale = Vec3(1, 1, 1);
	Matrix realshow;
	STATIC_VERTEX addVertex(Vec3 p, Vec3 n, float tu, float tv, int tiling)
	{
		STATIC_VERTEX v;
		v.pos = p;
		v.normal = n;
		
		v.tangent = Vec3(0, 0, 0); 
		v.tu = tu * tiling;
		v.tv = tv * tiling;
		return v;
	}

	void init(Core* core, PSOManager* psos, Shader* shader, Vec3 _position) {
		int rings = 64;
		int segments = 128;
		float radius = 98.f;
		int tiling = 1;
		position = _position;
		realshow = Matrix::translation(position) * Matrix::scaling(scale);
		std::vector<STATIC_VERTEX> vertices;
		//Calculate the vertices of the circle using latitude and longitude. The normal of each vertex is the position normalize, and the UV values are filled based on the vertex positions.
		for (int lat = 0; lat <= rings; lat++) {//¦È
			float theta = lat * M_PI / rings;
			float sinTheta = sinf(theta);
			float cosTheta = cosf(theta);
			for (int lon = 0; lon <= segments; lon++) {////¦Õ
				float phi = lon * 2.0f * M_PI / segments;
				float sinPhi = sinf(phi);
				float cosPhi = cosf(phi);
				Vec3 position(radius * sinTheta * cosPhi, radius * cosTheta, radius * sinTheta * sinPhi);//turn sphere to 3 dimension
				Vec3 normal = position.normalize();
				float tu = 1.0f - (float)lon / segments;//¦Õ
				float tv = 1.0f - (float)lat / rings;//¦È

				vertices.push_back(addVertex(position, normal, tu, tv, tiling));
			}
		}

		std::vector<unsigned int> indices;
		for (int lat = 0; lat < rings; lat++)
		{
			for (int lon = 0; lon < segments; lon++)
			{
				int current = lat * (segments + 1) + lon;
				int next = current + segments + 1;
				indices.push_back(current);
				indices.push_back(next);
				indices.push_back(current + 1);
				//topleft,bottonleft,topright
				indices.push_back(current + 1);
				indices.push_back(next);
				indices.push_back(next + 1);
				//topright,bottonleft,botton,right
			}
		}

		mesh.init(core, vertices, indices);


		//shader->init(core,"ShaderVertices.hlsl","ShaderPixel.hlsl");

		psos->createPSO(core, "Sphere", shader->vertexShader, shader->pixelShader, mesh.inputLayoutDesc);
	}
	void apply(Core* core, Shader* shader) {
		

		for (int i = 0; i < shader->vsConstantBuffers.size(); i++)
		{
			core->getCommandList()->SetGraphicsRootConstantBufferView(0, shader->vsConstantBuffers[i].getGPUAddress());
			shader->vsConstantBuffers[i].next();
		}
		for (int i = 0; i < shader->psConstantBuffers.size(); i++)
		{
			core->getCommandList()->SetGraphicsRootConstantBufferView(1, shader->psConstantBuffers[i].getGPUAddress());
			shader->psConstantBuffers[i].next();
		}

	}

	void draw(Core* core, Matrix* w, Matrix* vp, Shader* shader, PSOManager* psos, vector<Texture*> texture)
	{






		realshow = Matrix::translation(position) * Matrix::scaling(scale);
		shader->updateVSConstantBuffer(core, "staticMeshBuffer", "W", w);
		shader->updateVSConstantBuffer(core, "staticMeshBuffer", "VP", vp);
		

		//shader.ps_constantBuffer["bufferName"].update("time", &cb->time);
		//shader.ps_constantBuffer["bufferName"].update("lights", &cb->lights);

		apply(core, shader);
		psos->bind(core, "cube");

		shader->updateTexturePS(core, "tex", texture[0]->heapOffset);
		mesh.draw(core);


	}

};




class Cube {
public:

	PRIM_VERTEX vertices[3];
	GeneralMesh mesh;
	Vec3 position ;
	Vec3 scale = Vec3(1, 1, 1);
	Matrix realshow;
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
	void init(Core* core, PSOManager* psos, Shader* shader,Vec3 _position) {
		float tilling = 50.f;

		position = _position;
		realshow = Matrix::translation(position) * Matrix::scaling(scale);
		int width = 25;
		int height = 25;
		std::vector<STATIC_VERTEX> vertices;
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				float fi = i / 1.f;
				float fj = j / 1.f;
				vertices.push_back(addVertex(Vec3(fi, -5, fj), Vec3(0.0f, 1.0f, 0.0f), (float)i / height, (float)j / width));

			}
		}
		

		std::vector<unsigned int> indices;
		for (int i = 0; i < height - 1; i++) {
			for (int j = 0; j < width - 1; j++) {

				int current = i * width + j;
				int right = current + 1;
				int down = current + width;
				int downR = down + 1;

				indices.push_back(current);
				indices.push_back(down);
				indices.push_back(right);

				
				indices.push_back(right);
				indices.push_back(down);
				indices.push_back(downR);
			}
		}
		
		

		mesh.init(core, vertices, indices);


		//shader->init(core,"ShaderVertices.hlsl","ShaderPixel.hlsl");

		psos->createPSO(core,"cube", shader->vertexShader, shader->pixelShader, mesh.inputLayoutDesc);
	}
	void apply(Core* core, Shader* shader) {
		for (int i = 0; i < shader->vsConstantBuffers.size(); i++)
		{
			core->getCommandList()->SetGraphicsRootConstantBufferView(0, shader->vsConstantBuffers[i].getGPUAddress());
			shader->vsConstantBuffers[i].next();
		}
		for (int i = 0; i < shader->psConstantBuffers.size(); i++)
		{
			core->getCommandList()->SetGraphicsRootConstantBufferView(1, shader->psConstantBuffers[i].getGPUAddress());
			shader->psConstantBuffers[i].next();
		}

		

	}
	
	void draw(Core* core, Matrix* w, Matrix* vp, Shader* shader, PSOManager* psos, vector<Texture*> texture)
	{

		realshow = Matrix::translation(position) * Matrix::scaling(scale);


		shader->updateVSConstantBuffer(core, "staticMeshBuffer", "W", w);
		shader->updateVSConstantBuffer(core, "staticMeshBuffer", "VP", vp);

		

		//shader.ps_constantBuffer["bufferName"].update("time", &cb->time);
		//shader.ps_constantBuffer["bufferName"].update("lights", &cb->lights);

		apply(core, shader);
		psos->bind(core, "cube");
		
			shader->updateTexturePS(core, "tex", texture[0]->heapOffset);
			mesh.draw(core);
		

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
	vector<GeneralMesh*> meshes;
	string texturename;
	//GeneralMesh mesh;
	vector<string> textureFilenames;
	void load(Core* core, std::string filename, Shaders* shaders, PSOManager* psos, Staticmodels _name, Vec3 _position,TextureManager *textures,string selftexturename)
	{
		texturename = selftexturename;
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
			std::string tex_root = gemmeshes[i].material.find("albedo").getValue();
			tex_root = "../Resources/" + tex_root;
			textureFilenames.push_back(tex_root);

			

			mesh->init(core, vertices, gemmeshes[i].indices);
			meshes.push_back(mesh);
		
		}
		textures->load(core, textureFilenames, selftexturename);
		psos->createPSO(core, "StaticModelPSO", shaders->shaders["shader1"].vertexShader, shaders->shaders["shader1"].pixelShader, VertexLayoutCache::getStaticLayout());
		collision.staticinit(verticescout, position);
	}

	void apply(Core* core, Shader* shader) {
		for (int i = 0; i < shader->vsConstantBuffers.size(); i++)
		{
			core->getCommandList()->SetGraphicsRootConstantBufferView(0, shader->vsConstantBuffers[i].getGPUAddress());
			shader->vsConstantBuffers[i].next();
		}
		for (int i = 0; i < shader->psConstantBuffers.size(); i++)
		{
			core->getCommandList()->SetGraphicsRootConstantBufferView(1, shader->psConstantBuffers[i].getGPUAddress());
			shader->psConstantBuffers[i].next();
		}


	}
	void draw(Core* core, Matrix* w, Matrix* vp, Shader* shader, PSOManager* psos, vector<Texture*> texture)
	{






		realshow = Matrix::translation(position) * Matrix::scaling(scale);
		shader->updateVSConstantBuffer(core, "staticMeshBuffer", "W", w);
		shader->updateVSConstantBuffer(core, "staticMeshBuffer", "VP", vp);


		//shader.ps_constantBuffer["bufferName"].update("time", &cb->time);
		//shader.ps_constantBuffer["bufferName"].update("lights", &cb->lights);

		apply(core, shader);
		psos->bind(core, "StaticModelPSO");
		for (int i = 0; i < meshes.size(); i++)
		{
			if (i >= texture.size()) {
				shader->updateTexturePS(core, "tex", texture[texture.size()-1]->heapOffset);
				meshes[i]->draw(core);
			}
			else {
				shader->updateTexturePS(core, "tex", texture[i]->heapOffset);
				meshes[i]->draw(core);
			}
			
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
	string texturename;



	vector<GeneralMesh*> meshes;
	vector<ANIMATED_VERTEX> verticescout;
	Animation animation;
	vector<string>textureFilenames;
	//GeneralMesh mesh;

	void load(Core* core, string filename, Shaders* shaders, PSOManager* psos, Animatemodels _enum, Vec3 _position,TextureManager* textures, string selftexturename)
	{
		texturename = selftexturename;
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

			//std::string tex_root = "../" + gemmeshes[i].material.find("albedo").getValue();
			//textureFilenames.push_back("../Resources/Textures/T-rex_Base_Color_alb.png");
			// Load texture with filename: gemmeshes[i].material.find("albedo").getValue()
			//textures.load(core, "../Resources/Textures/T-rex_Base_Color_alb.png");

			mesh->init(core, vertices, gemmeshes[i].indices);

			meshes.push_back(mesh);
			std::string tex_root = gemmeshes[i].material.find("albedo").getValue();
			tex_root = "../Resources/" + tex_root;
			textureFilenames.push_back(tex_root);
		}
		textures->load(core, textureFilenames, selftexturename);
		psos->createPSO(core, "AnimatedModelPSO", shaders->shaders["shaderTexture"].vertexShader, shaders->shaders["shaderTexture"].pixelShader, VertexLayoutCache::getAnimatedLayout());
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
		collision.animateinit(verticescout, position);
	}

	void apply(Core* core, Shader* shader) {
		for (int i = 0; i < shader->vsConstantBuffers.size(); i++)
		{
			core->getCommandList()->SetGraphicsRootConstantBufferView(0, shader->vsConstantBuffers[i].getGPUAddress());

			shader->vsConstantBuffers[i].next();
		}
		for (int i = 0; i < shader->psConstantBuffers.size(); i++)
		{
			core->getCommandList()->SetGraphicsRootConstantBufferView(1, shader->psConstantBuffers[i].getGPUAddress());
			shader->psConstantBuffers[i].next();
		}


	}
	void draw(Core* core, Matrix* w, Matrix* vp, Shader* shader, PSOManager* psos, AnimationInstance* instance, Matrix& roation, vector<Texture*> texture)
	{

		realshow = Matrix::translation(position) * roation * Matrix::scaling(scale);
		shader->updateVSConstantBuffer(core, "staticMeshBuffer", "W", w);
		shader->updateVSConstantBuffer(core, "staticMeshBuffer", "VP", vp);
		shader->updateVSConstantBuffer(core, "staticMeshBuffer", "bones", instance->matrices);
		

		//shader.ps_constantBuffer["bufferName"].update("time", &cb->time);
		//shader.ps_constantBuffer["bufferName"].update("lights", &cb->lights);

		apply(core, shader);
		psos->bind(core, "AnimatedModelPSO");
		for (int i = 0; i < meshes.size(); i++)
		{
			if (i >= texture.size()) {
				shader->updateTexturePS(core, "tex", texture[texture.size() - 1]->heapOffset);
				meshes[i]->draw(core);
			}
			else {
				shader->updateTexturePS(core, "tex", texture[i]->heapOffset);
				meshes[i]->draw(core);
			}
		}

	}

};

