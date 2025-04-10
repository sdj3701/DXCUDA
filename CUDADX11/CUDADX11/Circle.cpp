#include "pch.h"
#include "Circle.h"

void Circle::DrawCircle(int width, int height, ComPtr<ID3D11ShaderResourceView>& _shaderResourceView, ComPtr<ID3D11DeviceContext>& _deviceContext, ComPtr<ID3D11Buffer>& constantBuffer)
{

    UpdateCircleData();

    // 상수 버퍼에 데이터 매핑
    D3D11_MAPPED_SUBRESOURCE subResource;
    ZeroMemory(&subResource, sizeof(subResource));

    // 상수 버퍼 매핑
    HRESULT hr = _deviceContext->Map(constantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
    if (SUCCEEDED(hr))
    {
        // 데이터 복사
        ::memcpy(subResource.pData, &circleData, sizeof(CircleData));

        // 상수 버퍼 언매핑
        _deviceContext->Unmap(constantBuffer.Get(), 0);
    }

    // 상수 버퍼를 픽셀 셰이더에 설정
    _deviceContext->PSSetConstantBuffers(0, 1, constantBuffer.GetAddressOf());

    // 셰이더 리소스 뷰 설정 (텍스처)
    if (_shaderResourceView)
    {
        _deviceContext->PSSetShaderResources(0, 1, _shaderResourceView.GetAddressOf());
    }
}

bool Circle::IsInside(const glm::vec2& x)
{
	{
		float distance = glm::length(center - x);

		if (radius >= distance)
			return true;
		else
			return false;
	}
}

void Circle::UpdateCircleData()
{
    // 원의 속성이 변경되었을 때 상수 버퍼 데이터 업데이트
    circleData.center = center;
    circleData.radius = radius;
    circleData.color = _color;
}
