#include "CBController.h"

#include <UniDx/D3DManager.h>
#include <UniDx/Input.h>


using namespace UniDx;

namespace
{
    // 定数バッファ（型定義自体はシェーダーに伝わらないことに注意）
    struct CBParam // register(b0)
    {
        Color baseColor;
        float Speed;
        float padding[3];
    };
    constexpr int CBSlot = 0;

    Color colorTable[] =
    {
        Color(1, 1, 1),
        Color(1, 1, 0.5),
        Color(1, 1, 0),
        Color(1, 0.5, 1),
        Color(1, 0.5, 0.5),
        Color(1, 0.5, 0),
        Color(1, 0, 1),
        Color(1, 0, 0.5),
        Color(1, 0, 0),
        Color(0.5, 1, 1),
        Color(0.5, 1, 0.5),
        Color(0.5, 1, 0),
        Color(0.5, 0.5, 1),
        Color(0.5, 0.5, 0.5),
        Color(0.5, 0.5, 0),
        Color(0.5, 0, 1),
        Color(0.5, 0, 0.5),
        Color(0.5, 0, 0),
        Color(0, 1, 1),
        Color(0, 1, 0.5),
        Color(0, 1, 0),
        Color(0, 0.5, 1),
        Color(0, 0.5, 0.5),
        Color(0, 0.5, 0),
        Color(0, 0, 1),
        Color(0, 0, 0.5),
        Color(0, 0, 0),
    };
    constexpr int tableSize = sizeof(colorTable) / sizeof(Color);
    float speed = 1.0f;
}


void CBController::Start()
{
    D3D11_BUFFER_DESC desc{};
    desc.ByteWidth = sizeof(CBParam);       // 構造体のサイズを指定
    desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    desc.CPUAccessFlags = 0;
    desc.Usage = D3D11_USAGE_DEFAULT;

    // 定数バッファを確保
    D3DManager::getInstance()->GetDevice()->CreateBuffer(&desc, nullptr, constantBuffer.GetAddressOf());
}


void CBController::Update()
{
    if (Input::GetKeyUp(Keyboard::Up))
    {
        colorIndex = (colorIndex - 1 + tableSize) % tableSize;
    }
    if(Input::GetKeyDown(Keyboard::Down))
    {
        colorIndex = (colorIndex + 1) % tableSize;
    }
    if (Input::GetKey(Keyboard::Left))
    {
        speed -= UniDx::Time::deltaTime;
    }
    if (Input::GetKey(Keyboard::Right))
    {
        speed += UniDx::Time::deltaTime;
    }

    CBParam cb;

    // カラーテーブルから現在のインデクスの色を設定
    cb.baseColor = colorTable[colorIndex];
    cb.Speed = speed;

    // 定数バッファを更新
    D3DManager::getInstance()->GetContext()->UpdateSubresource(constantBuffer.Get(), 0, nullptr, &cb, 0, 0);

    // シェーダーへ設定
    ID3D11Buffer* cbs[1] = { constantBuffer.Get() };
    D3DManager::getInstance()->GetContext()->VSSetConstantBuffers(CBSlot, 1, cbs);
    D3DManager::getInstance()->GetContext()->PSSetConstantBuffers(CBSlot, 1, cbs);

}