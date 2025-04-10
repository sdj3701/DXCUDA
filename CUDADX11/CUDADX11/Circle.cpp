#include "pch.h"
#include "Circle.h"



void Circle::DrawCircle(int width, int height, ComPtr<ID3D11ShaderResourceView>& _shaderResourceView, ComPtr<ID3D11DeviceContext>& _deviceContext, ComPtr<ID3D11Buffer>& constantBuffer)
{
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
