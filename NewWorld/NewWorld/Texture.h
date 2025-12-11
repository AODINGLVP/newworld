#pragma once

#include "stb_image.h"
#include "core.h"
#include <string>
#include <map>

class Texture
{
public:

	ID3D12Resource* tex = nullptr;
	int heapOffset = -1;



	void init(Core* core, int width, int height, int channels, void* data) {
		DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
		D3D12_HEAP_PROPERTIES heapDesc;
		memset(&heapDesc, 0, sizeof(D3D12_HEAP_PROPERTIES));
		heapDesc.Type = D3D12_HEAP_TYPE_DEFAULT;

		D3D12_RESOURCE_DESC textureDesc;
		memset(&textureDesc, 0, sizeof(D3D12_RESOURCE_DESC));
		textureDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
		textureDesc.Width = width;
		textureDesc.Height = height;
		textureDesc.DepthOrArraySize = 1;
		textureDesc.MipLevels = 1;
		textureDesc.Format = format;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;
		textureDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
		textureDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

		core->device->CreateCommittedResource(&heapDesc, D3D12_HEAP_FLAG_NONE, &textureDesc,
			D3D12_RESOURCE_STATE_COPY_DEST, NULL, IID_PPV_ARGS(&tex));

		D3D12_RESOURCE_DESC desc = tex->GetDesc();
		unsigned long long size;
		D3D12_PLACED_SUBRESOURCE_FOOTPRINT footprint;
		core->device->GetCopyableFootprints(&desc, 0, 1, 0, &footprint, NULL, NULL, &size);

		unsigned int alignedWidth = ((width * channels) + 255) & ~255;
		core->uploadResource(tex, data, alignedWidth * height,
			D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, &footprint);

		D3D12_CPU_DESCRIPTOR_HANDLE srvHandle = core->srvHeap.getNextCPUHandle();
		D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		srvDesc.Format = format;
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = 1;
		core->device->CreateShaderResourceView(tex, &srvDesc, srvHandle);
		heapOffset = core->srvHeap.used - 1;


	}

	void load(Core* core, string filename) {

		int width = 0;
		int height = 0;
		int channels = 0;


		unsigned char* texels = stbi_load(filename.c_str(), &width, &height, &channels, 0);
		if (!texels) return;
		if (channels == 3) {
			channels = 4;
			unsigned char* texelsWithAlpha = new unsigned char[width * height * channels];
			for (int i = 0; i < (width * height); i++) {
				texelsWithAlpha[i * 4] = texels[i * 3];
				texelsWithAlpha[(i * 4) + 1] = texels[(i * 3) + 1];
				texelsWithAlpha[(i * 4) + 2] = texels[(i * 3) + 2];
				texelsWithAlpha[(i * 4) + 3] = 255;
			}
			// Initialize texture using width, height, channels, and texelsWithAlpha
			init(core, width, height, channels, texelsWithAlpha);
			delete[] texelsWithAlpha;
		}
		else {
			// Initialize texture using width, height, channels, and texels
			init(core, width, height, channels, texels);
		}
		stbi_image_free(texels);
	}


};


class TextureManager {
public:
	map<string, vector<Texture*> > textures;
	map<string, vector<Texture*> > NHtextures;

	void load(Core* core, vector<string> name, string texturename)
	{
		if (textures.find(texturename) != textures.end()) {

		}
		else {
			vector <Texture*> scv;

			for (int i = 0; i < name.size(); i++) {
				

				Texture* texture = new Texture;

				texture->load(core, name[i]);
				scv.push_back(texture);
			}
			textures.insert({ texturename, scv });
		}
		
		
	}
	void load(Core* core, vector<string> name,vector<string>NHname, string texturename)
	{
		if (textures.find(texturename) != textures.end()) {

		}
		else {
			vector <Texture*> scv;
			vector <Texture*> scv1;
			for (int i = 0; i < name.size(); i++) {


				Texture* texture = new Texture;

				texture->load(core, name[i]);
				scv.push_back(texture);


				Texture* texture1 = new Texture;

				texture1->load(core, NHname[i]);
				scv1.push_back(texture1);

			}
			textures.insert({ texturename, scv });
			NHtextures.insert({ texturename,scv1 });
		}


	}
	vector<Texture*> find(string name)
	{
		if (textures.find(name) != textures.end())
			return textures[name];
		else
			return {};
	}
	vector<Texture*> findNH(string name)
	{
		if (NHtextures.find(name) != NHtextures.end())
			return NHtextures[name];
		else
			return {};
	}
};