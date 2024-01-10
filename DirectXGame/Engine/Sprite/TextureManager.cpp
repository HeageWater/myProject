#include "TextureManager.h"

TextureManager* TextureManager::sTextureManager_ = nullptr;
std::vector<std::string>TextureManager::sFilePaths_;
std::unordered_map<std::string, std::unique_ptr<TextureData>> TextureManager::sTextureDatas_;

void TextureManager::LoadFile(const std::string& path, DirectX::TexMetadata& metadata, DirectX::ScratchImage& scratchImg)
{
	wchar_t wfilepath[256];
	HRESULT result = 0;

	MultiByteToWideChar(CP_ACP, 0, path.c_str(), -1, wfilepath, _countof(wfilepath));
	// WICテクスチャのロード
	result = LoadFromWICFile(
		wfilepath,
		DirectX::WIC_FLAGS_NONE,
		&metadata, scratchImg);
	assert(SUCCEEDED(result));
}

TextureData* TextureManager::FromTextureData(const std::string& path)
{
	TextureData* result = new TextureData();

	DirectX::TexMetadata metadata{};
	DirectX::ScratchImage scratchImg{};
	DirectX::ScratchImage mipChain{};

	result->srvHeap = directX_->GetDescriptorHeap()->GetHeap();

	LoadFile(path, metadata, scratchImg);

	//ミップマップ生成
	HRESULT hr = GenerateMipMaps(
		scratchImg.GetImages(), scratchImg.GetImageCount(), scratchImg.GetMetadata(),
		DirectX::TEX_FILTER_DEFAULT, 0, mipChain);

	if (SUCCEEDED(hr))
	{
		scratchImg = std::move(mipChain);
		metadata = scratchImg.GetMetadata();
	}

	//読み込んだディフューズテクスチャをSRGBとして扱う
	metadata.format = DirectX::MakeSRGB(metadata.format);

	result->texBuff = CreateTexBuff(metadata, scratchImg);

	result->gpuHandle = CreateShaderResourceView(result->texBuff.Get(), metadata);

	result->width = metadata.width;
	result->height = metadata.height;

	return result;
}

uint32_t TextureManager::LoadTexture(const std::string& path)
{
	if (nextTexture_ > 2024)
	{
		assert(0);
	}

	//一回読み込んだことがあるファイルはそのまま返す
	auto itr = find_if(sTextureDatas_.begin(), sTextureDatas_.end(), [&](std::pair<const std::string, std::unique_ptr<TextureData, std::default_delete<TextureData>>>& p)
		{
			return p.second->path == path;
		});

	if (itr == sTextureDatas_.end())
	{
		std::unique_ptr<TextureData> data;

		data.reset(FromTextureData(path));
		data->textureHandle = nextTexture_;
		data->path = path;

		sTextureDatas_[path] = std::move(data);
		sFilePaths_[nextTexture_] = path;
		uint32_t handl = nextTexture_;

		nextTexture_++;

		return handl;
	}
	else
	{

		uint32_t modelHandle = sTextureDatas_[path]->textureHandle;

		return modelHandle;
	}
}

void TextureManager::StaticInitialize()
{
	directX_ = MyDirectX::GetInstance();

	// ヒープ設定
	textureHeapProp_.Type = D3D12_HEAP_TYPE_CUSTOM;
	textureHeapProp_.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	textureHeapProp_.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;

	sFilePaths_.resize(2024);
}

TextureManager* TextureManager::GetInstance()
{
	static TextureManager instance;
	return &instance;
}

uint32_t TextureManager::Load(const std::string& path)
{
	return TextureManager::GetInstance()->LoadTexture(path);
}

TextureData* TextureManager::GetTextureData(uint32_t handle)
{
	return sTextureDatas_[sFilePaths_[handle]].get();
}

Microsoft::WRL::ComPtr<ID3D12Resource> TextureManager::CreateTexBuff(DirectX::TexMetadata& metadata, DirectX::ScratchImage& scratchImg)
{
	Microsoft::WRL::ComPtr<ID3D12Resource> result;
	// リソース設定
	D3D12_RESOURCE_DESC textureResourceDesc{};
	textureResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	textureResourceDesc.Format = metadata.format;
	textureResourceDesc.Width = metadata.width;//幅
	textureResourceDesc.Height = (uint32_t)metadata.height;//高さ
	textureResourceDesc.DepthOrArraySize = (UINT16)metadata.arraySize;
	textureResourceDesc.MipLevels = (UINT16)metadata.mipLevels;
	textureResourceDesc.SampleDesc.Count = 1;

	// テクスチャバッファの生成
	HRESULT hr = directX_->GetDev()->CreateCommittedResource(
		&textureHeapProp_,
		D3D12_HEAP_FLAG_NONE,
		&textureResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(result.ReleaseAndGetAddressOf()));

	//テクスチャバッファにデータ転送
	for (size_t i = 0; i < metadata.mipLevels; i++)
	{
		// ミップマップレベルを指定してイメージを取得
		const DirectX::Image* img = scratchImg.GetImage(i, 0, 0);
		// テクスチャバッファにデータ転送
		hr = result->WriteToSubresource(
			static_cast<uint32_t>(i),
			nullptr,              // 全領域へコピー
			img->pixels,          // 元データアドレス
			static_cast<uint32_t>(img->rowPitch),  // 1ラインサイズ
			static_cast<uint32_t>(img->slicePitch) // 1枚サイズ
		);
		assert(SUCCEEDED(hr));
	}

	return result;
}

D3D12_GPU_DESCRIPTOR_HANDLE TextureManager::CreateShaderResourceView(ID3D12Resource* texBuff, DirectX::TexMetadata& metadata)
{
	// シェーダリソースビュー設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = metadata.format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = static_cast<uint32_t>(metadata.mipLevels);

	D3D12_GPU_DESCRIPTOR_HANDLE result{};

	result.ptr = directX_->GetDescriptorHeap()->CreateSRV(srvDesc, texBuff);

	return result;
}