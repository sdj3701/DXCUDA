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

    size_t bytesPerPixel = BitsPerPixel(format) / 8; // �������κ��� �ȼ��� ����Ʈ �� ���

    if (format == DXGI_FORMAT_R8G8B8A8_UNORM)
    {
        if (bytesPerPixel != 4) {
            std::cerr << "����: R8G8B8A8_UNORM ������ �ȼ��� 4����Ʈ���� ������, " << bytesPerPixel << "����Ʈ�Դϴ�." << std::endl;
            return; // �Ǵ� ������ ���� ó��
        }
        for (size_t y = 0; y < height; ++y)
        {
            for (size_t x = 0; x < width; ++x)
            {
                size_t pixelIndex = y * rowPitch + x * bytesPerPixel; // bytesPerPixel ���
                uint8_t r = pixels[pixelIndex];
                uint8_t g = pixels[pixelIndex + 1];
                uint8_t b = pixels[pixelIndex + 2];
                uint8_t a = pixels[pixelIndex + 3];

                this->pixels[y * width + x] = Vec4{ r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f };
            }
        }
    }
    // �ٸ� ���� ó�� (��: DXGI_FORMAT_B8G8R8A8_UNORM, DXGI_FORMAT_B8G8R8X8_UNORM, DXGI_FORMAT_R8G8B8X8_UNORM ��) �߰�
    else if (format == DXGI_FORMAT_B8G8R8A8_UNORM) // ����: B8G8R8A8_UNORM ���� ó�� �߰�
    {
        if (bytesPerPixel != 4) {
            std::cerr << "����: R8G8B8A8_UNORM ������ �ȼ��� 4����Ʈ���� ������, " << bytesPerPixel << "����Ʈ�Դϴ�." << std::endl;
            return; // �Ǵ� ������ ���� ó��
        }
        for (size_t y = 0; y < height; ++y)
        {
            for (size_t x = 0; x < width; ++x)
            {
                size_t pixelIndex = y * rowPitch + x * bytesPerPixel;
                uint8_t b = pixels[pixelIndex]; // BGR ����
                uint8_t g = pixels[pixelIndex + 1];
                uint8_t r = pixels[pixelIndex + 2];
                uint8_t a = pixels[pixelIndex + 3];

                this->pixels[y * width + x] = Vec4{ r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f }; // ������ RGB ������ Vec4�� ���� (�ʿ信 ���� BGR ������ ���� ����)
            }
        }
    }
    else
    {
        std::cerr << "Unsupported DXGI format: " << format << std::endl;
        // �Ǵ� �ٸ� ���� ó�� ���� ���� (��: ���� ��ȯ �� ó��)
    }
}

void Gaussianblur::RightUpEffect()
{
    if (!image) {
        cout << "Failed to get image data from ScratchImage." << endl;
        return;
    }
    
    for (int i = 0; i < width * height; i++)
    {
        pixels[i].x = max(pixels[i].x * 0.99f, 0.0f);
        pixels[i].y = max(pixels[i].y * 0.99f, 0.0f);
        pixels[i].z = max(pixels[i].z * 0.99f, 0.0f);
    }
    
}

//void Gaussianblur::OldRightUpEffect(const DirectX::ScratchImage& img, const DirectX::TexMetadata& md)
//{
//    image = img.GetImage(0, 0, 0);
//    if (!image) {
//        std::cerr << "Failed to get image data from ScratchImage." << std::endl;
//        return;
//    }
//    uint8_t* pixels = const_cast<uint8_t*>(image->pixels); // const ���� (����: const_cast ���)
//    size_t rowPitch = image->rowPitch;
//    DXGI_FORMAT format = md.format;
//    size_t bytesPerPixel = BitsPerPixel(format) / 8;
//
//    if (format == DXGI_FORMAT_R8G8B8A8_UNORM)
//    {
//        for (size_t y = 0; y < height; ++y)
//        {
//            for (size_t x = 0; x < width; ++x)
//            {
//                size_t pixelIndex = y * rowPitch + x * bytesPerPixel;
//                float r = pixels[pixelIndex] / 255.0f;
//                float g = pixels[pixelIndex + 1] / 255.0f;
//                float b = pixels[pixelIndex + 2] / 255.0f;
//
//                r += 0.005f; // ��� �����
//                g += 0.005f;
//                b += 0.005f;
//
//
//                pixels[pixelIndex] = static_cast<uint8_t>(r * 255.0f); // �ٽ� uint8_t�� ��ȯ�Ͽ� ����
//                pixels[pixelIndex + 1] = static_cast<uint8_t>(g * 255.0f);
//                pixels[pixelIndex + 2] = static_cast<uint8_t>(b * 255.0f);
//            }
//        }
//    }
//
//}