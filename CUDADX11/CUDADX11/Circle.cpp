#include "pch.h"
#include "Circle.h"

void Circle::DrawCircle(int width, int height, ComPtr<ID3D11ShaderResourceView>& _shaderResourceView, ComPtr<ID3D11DeviceContext>& _deviceContext, ComPtr<ID3D11Buffer>& constantBuffer)
{

    UpdateCircleData();

    // ��� ���ۿ� ������ ����
    D3D11_MAPPED_SUBRESOURCE subResource;
    ZeroMemory(&subResource, sizeof(subResource));

    // ��� ���� ����
    HRESULT hr = _deviceContext->Map(constantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
    if (SUCCEEDED(hr))
    {
        // ������ ����
        ::memcpy(subResource.pData, &circleData, sizeof(CircleData));

        // ��� ���� �����
        _deviceContext->Unmap(constantBuffer.Get(), 0);
    }

    // ��� ���۸� �ȼ� ���̴��� ����
    _deviceContext->PSSetConstantBuffers(0, 1, constantBuffer.GetAddressOf());

    // ���̴� ���ҽ� �� ���� (�ؽ�ó)
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
    // ���� �Ӽ��� ����Ǿ��� �� ��� ���� ������ ������Ʈ
    circleData.center = center;
    circleData.radius = radius;
    circleData.color = _color;
}
