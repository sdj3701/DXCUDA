#pragma once

class Object;

class Hit
{
public:
    float d;            // ������ ���ۺ��� �浹 ���������� �Ÿ�
    glm::vec3 point;    // �浹�� ��ġ
    glm::vec3 normal;   // �浹�� ��ġ���� ǥ���� ���� ����

    std::shared_ptr<Object> obj; // ��ü�� ���� ���� �������� ���� ������

};