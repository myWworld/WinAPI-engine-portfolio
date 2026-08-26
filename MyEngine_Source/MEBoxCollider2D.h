#pragma once
#include "MECollider.h"
#include <array>
#include <Windows.h>

namespace ME
{
    class BoxCollider2D : public Collider
    {
    public:
        BoxCollider2D();
        virtual ~BoxCollider2D();

        virtual void Initialize() override;
        virtual void Update() override;
        virtual void LateUpdate() override;
        virtual void Render(HDC hdc) override;

        void SetRotation(float rot);
        float GetRotation() const { return mRot; }

        bool IsRotate() const { return mbIsRotate; }

        Vector2 GetCentralPoint() const { return mCentralPoint; }
        float GetWidth() const { return mWidth; }
        float GetHeight() const { return mHeight; }

    private:
        void RefreshBounds();
        Vector2 RotateCollider(float rot, float x, float y, float width, float height);

    private:
        float mRot;
        bool mbIsRotate;

        float mWidth;
        float mHeight;

        Vector2 mCentralPoint;

        // 회전된 꼭짓점 저장용
        std::array<POINT, 4> mPoints;
    };
}