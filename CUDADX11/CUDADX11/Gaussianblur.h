#pragma once
class Gaussianblur
{
private:
	const DirectX::Image* image;

public:
	int width = 0, height = 0, channels = 0;
	// ÇÈ¼¿
	vector<Vec4> pixels;

public:
	void ProcessPixelDataFromDirectXImage(const DirectX::ScratchImage& img, const DirectX::TexMetadata& md);
	void RightUpEffect(ComPtr<ID3D11ShaderResourceView> _shaderResourceView, ComPtr<ID3D11DeviceContext> _deviceContext);
};

