#pragma once
#include <wincrypt.h>
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
	void RightUpDownEffect(ComPtr<ID3D11ShaderResourceView> _shaderResourceView, ComPtr<ID3D11DeviceContext> _deviceContext);
	
	void GaussianblurEffect(ComPtr<ID3D11ShaderResourceView> _shaderResourceView, ComPtr<ID3D11DeviceContext> _deviceContext);
	void Bloom(ComPtr<ID3D11ShaderResourceView> _shaderResourceView, ComPtr<ID3D11DeviceContext> _deviceContext);
	Vec4& GetPixel(int i, int j);

};

