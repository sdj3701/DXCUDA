#include "pch.h"
#include "Gaussianblur.h"

void Gaussianblur::ProcessPixelDataFromDirectXImage(const DirectX::ScratchImage& img, const DirectX::TexMetadata& md)
{
    image = img.GetImage(0, 0, 0);
    if (!image) {
        std::cerr << "Failed to get image data from ScratchImage." << std::endl;
        return;
    }

    const uint8_t* pixels = image->pixels;
    width = image->width;
    height = image->height;
    size_t rowPitch = image->rowPitch;
    DXGI_FORMAT format = md.format;

    this->pixels.resize(width * height);

    size_t bytesPerPixel = BitsPerPixel(format) / 8; // 포맷으로부터 픽셀당 바이트 수 계산

    if (format == DXGI_FORMAT_R8G8B8A8_UNORM)
    {
        if (bytesPerPixel != 4) {
            std::cerr << "오류: R8G8B8A8_UNORM 포맷은 픽셀당 4바이트여야 하지만, " << bytesPerPixel << "바이트입니다." << std::endl;
            return; // 또는 적절한 오류 처리
        }
        for (size_t y = 0; y < height; ++y)
        {
            for (size_t x = 0; x < width; ++x)
            {
                size_t pixelIndex = y * rowPitch + x * bytesPerPixel; // bytesPerPixel 사용
                uint8_t r = pixels[pixelIndex];
                uint8_t g = pixels[pixelIndex + 1];
                uint8_t b = pixels[pixelIndex + 2];
                uint8_t a = pixels[pixelIndex + 3];

                this->pixels[y * width + x] = Vec4{ r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f };
            }
        }
    }
    // 다른 포맷 처리 (예: DXGI_FORMAT_B8G8R8A8_UNORM, DXGI_FORMAT_B8G8R8X8_UNORM, DXGI_FORMAT_R8G8B8X8_UNORM 등) 추가
    else if (format == DXGI_FORMAT_B8G8R8A8_UNORM) // 예시: B8G8R8A8_UNORM 포맷 처리 추가
    {
        if (bytesPerPixel != 4) {
            std::cerr << "오류: R8G8B8A8_UNORM 포맷은 픽셀당 4바이트여야 하지만, " << bytesPerPixel << "바이트입니다." << std::endl;
            return; // 또는 적절한 오류 처리
        }
        for (size_t y = 0; y < height; ++y)
        {
            for (size_t x = 0; x < width; ++x)
            {
                size_t pixelIndex = y * rowPitch + x * bytesPerPixel;
                uint8_t b = pixels[pixelIndex]; // BGR 순서
                uint8_t g = pixels[pixelIndex + 1];
                uint8_t r = pixels[pixelIndex + 2];
                uint8_t a = pixels[pixelIndex + 3];

                this->pixels[y * width + x] = Vec4{ r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f }; // 여전히 RGB 순서로 Vec4에 저장 (필요에 따라 BGR 순서로 저장 가능)
            }
        }
    }
    else
    {
        std::cerr << "Unsupported DXGI format: " << format << std::endl;
        // 또는 다른 포맷 처리 로직 구현 (예: 포맷 변환 후 처리)
    }
}

void Gaussianblur::RightUpDownEffect(ComPtr<ID3D11ShaderResourceView> _shaderResourceView, ComPtr<ID3D11DeviceContext> _deviceContext)
{
    // 이렇게 코드를 짠 이유
    // 지금 Update에서 상수 버퍼를 Map/Umap에서 관리를 하기 때문에
    // 여기서 Texture2D를 생성 하고 UpdateSubresource에 직접 넘겨서
    // 작업하는 과정으로 변경함

    if (!image) {
        cout << "Failed to get image data from ScratchImage." << endl;
        return;
    }

    // 밝게
    //for (int i = 0; i < width * height; i++)
    //{
    //    // 1.01을 곱해 점점 밝아지도록 하고, 1.0f를 넘지 않도록 제한
    //    pixels[i].x = min(pixels[i].x * 1.01f, 1.0f);
    //    pixels[i].y = min(pixels[i].y * 1.01f, 1.0f);
    //    pixels[i].z = min(pixels[i].z * 1.01f, 1.0f);
    //}

    // 어둡게
    //for (int i = 0; i < width * height; i++)
    //{
    //    pixels[i].x = max(pixels[i].x * 0.99f, 0.0f);
    //    pixels[i].y = max(pixels[i].y * 0.99f, 0.0f);
    //    pixels[i].z = max(pixels[i].z * 0.99f, 0.0f);
    //}

    // 반 나눠서 밝기 조절
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            const int idx = j + width * i;
            if (j < width / 2)
            {
                // 왼쪽 어둡게 - 모든 채널을 균등하게 감소
                pixels[idx].x = max(pixels[idx].x * 0.99f, 0.0f);
                pixels[idx].y = max(pixels[idx].y * 0.99f, 0.0f);
                pixels[idx].z = max(pixels[idx].z * 0.99f, 0.0f);
            }
            else
            {
                // 오른쪽 밝게 - 색상 균형 유지하며 밝기 증가
                // 현재 값과 흰색(1.0) 사이의 거리를 줄이는 방식
                pixels[idx].x = min(pixels[idx].x + (1.0f - pixels[idx].x) * 0.01f, 1.0f);
                pixels[idx].y = min(pixels[idx].y + (1.0f - pixels[idx].y) * 0.01f, 1.0f);
                pixels[idx].z = min(pixels[idx].z + (1.0f - pixels[idx].z) * 0.01f, 1.0f);

                // 또는 노란색/빨간색 문제를 직접 해결하려면
                // 파란색 채널을 더 크게 증가시켜 색상 균형 조정
                //pixels[idx].z = min(pixels[idx].z + 0.015f, 1.0f);  // 파란색 추가 증가
            }
        }
    }

    // 1. 기존 텍스처를 가져오기
    ComPtr<ID3D11Resource> resource;
    _shaderResourceView->GetResource(resource.GetAddressOf());

    ComPtr<ID3D11Texture2D> texture;
    resource.As(&texture);

    // 2. 수정된 픽셀 데이터로 텍스처 업데이트
    D3D11_TEXTURE2D_DESC desc;
    texture->GetDesc(&desc);

    // 픽셀 데이터를 RGBA 형식으로 변환
    std::vector<uint8_t> rawPixels(width * height * 4);

    for (size_t i = 0; i < pixels.size(); i++)
    {
        rawPixels[i * 4 + 0] = static_cast<uint8_t>(pixels[i].x * 255.0f);
        rawPixels[i * 4 + 1] = static_cast<uint8_t>(pixels[i].y * 255.0f);
        rawPixels[i * 4 + 2] = static_cast<uint8_t>(pixels[i].z * 255.0f);
        rawPixels[i * 4 + 3] = static_cast<uint8_t>(pixels[i].w * 255.0f);
    }

    // 텍스처 업데이트
    D3D11_BOX box;
    box.left = 0;
    box.right = width;
    box.top = 0;
    box.bottom = height;
    box.front = 0;
    box.back = 1;

    _deviceContext->UpdateSubresource(
        texture.Get(), 0, &box,
        rawPixels.data(),
        width * 4,
        width * height * 4
    );

}

void Gaussianblur::GaussianblurEffect(ComPtr<ID3D11ShaderResourceView> _shaderResourceView, ComPtr<ID3D11DeviceContext> _deviceContext)
{

    const float weights[5] = { 0.0545f, 0.2442f, 0.4026f, 0.2442f, 0.0545f };

    vector<Vec4> pixelsBuffer(pixels.size());

    // 가로 x 방향
    for (int j = 0; j < height; j++)
    {
        for (int i = 0; i < width; i++)
        {
            Vec4 neighborColorSum{ 0.0f,0.0f,0.0f,1.0f };
            for (int si = 0; si < 5; si++)
            {
                Vec4 neighborColor = GetPixel(i + si - 2, j);
                neighborColorSum.x += neighborColor.x * weights[si];
                neighborColorSum.y += neighborColor.y * weights[si];
                neighborColorSum.z += neighborColor.z * weights[si];
            }

            pixelsBuffer[i + width * j].x = neighborColorSum.x;
            pixelsBuffer[i + width * j].y = neighborColorSum.y;
            pixelsBuffer[i + width * j].z = neighborColorSum.z;
        }
    }

    std::swap(pixels, pixelsBuffer);

    for (int j = 0; j < height; j++)
    {
        for (int i = 0; i < width; i++)
        {
            Vec4 neighborColorSum{ 0.0f,0.0f,0.0f,1.0f };
            for (int si = 0; si < 5; si++)
            {
                Vec4 neighborColor = GetPixel(i, j + si - 2);
                neighborColorSum.x += neighborColor.x * weights[si];
                neighborColorSum.y += neighborColor.y * weights[si];
                neighborColorSum.z += neighborColor.z * weights[si];
            }

            pixelsBuffer[i + width * j].x = neighborColorSum.x;
            pixelsBuffer[i + width * j].y = neighborColorSum.y;
            pixelsBuffer[i + width * j].z = neighborColorSum.z;
        }
    }

    std::swap(pixels, pixelsBuffer);

    // 1. 기존 텍스처를 가져오기
    ComPtr<ID3D11Resource> resource;
    _shaderResourceView->GetResource(resource.GetAddressOf());

    ComPtr<ID3D11Texture2D> texture;
    resource.As(&texture);

    // 2. 수정된 픽셀 데이터로 텍스처 업데이트
    D3D11_TEXTURE2D_DESC desc;
    texture->GetDesc(&desc);

    // 픽셀 데이터를 RGBA 형식으로 변환
    std::vector<uint8_t> rawPixels(width * height * 4);

    for (size_t i = 0; i < pixels.size(); i++)
    {
        rawPixels[i * 4 + 0] = static_cast<uint8_t>(pixels[i].x * 255.0f);
        rawPixels[i * 4 + 1] = static_cast<uint8_t>(pixels[i].y * 255.0f);
        rawPixels[i * 4 + 2] = static_cast<uint8_t>(pixels[i].z * 255.0f);
        rawPixels[i * 4 + 3] = static_cast<uint8_t>(pixels[i].w * 255.0f);
    }

    // 텍스처 업데이트
    D3D11_BOX box;
    box.left = 0;
    box.right = width;
    box.top = 0;
    box.bottom = height;
    box.front = 0;
    box.back = 1;

    _deviceContext->UpdateSubresource(
        texture.Get(), 0, &box,
        rawPixels.data(),
        width * 4,
        width * height * 4
    );

}

void Gaussianblur::Bloom(ComPtr<ID3D11ShaderResourceView> _shaderResourceView, ComPtr<ID3D11DeviceContext> _deviceContext)
{
    vector<Vec4> pixelsBackup(pixels.size());

    for (int j = 0; j < height; j++)
    {
        for (int i = 0; i < width; i++)
        {
            auto& c = GetPixel(i, j);
            const float relativeLuminance = 0.2126 * c.x + 0.7152 * c.y + 0.0722 * c.z;
            
            if (relativeLuminance < 0.3f)
            {
                c.x = 0.0f;
                c.y = 0.0f;
                c.z = 0.0f;
            }
        }
    }


    for (int i = 0; i < 1000; i++)
    {
        GaussianblurEffect(_shaderResourceView, _deviceContext);
    }

    for (int i = 0; i < pixelsBackup.size(); i++)
    {
        pixels[i].x = min(max(pixels[i].x * 1.0f + pixelsBackup[i].x, 0.0f), 1.0f);
        pixels[i].y = min(max(pixels[i].y * 1.0f + pixelsBackup[i].y, 0.0f), 1.0f);
        pixels[i].z = min(max(pixels[i].z * 1.0f + pixelsBackup[i].z, 0.0f), 1.0f);
    }

}

Vec4& Gaussianblur::GetPixel(int i, int j)
{
    i = min(max(i, 0), width - 1);
    j = min(max(j, 0), height - 1);

    return pixels[i + width * j];
}

