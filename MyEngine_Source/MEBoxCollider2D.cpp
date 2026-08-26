#include "MEBoxCollider2D.h"
#include "METransform.h"
#include "MEGameObject.h"
#include "MERenderer.h"
#include "MECamera.h"

#include <algorithm>
#include <cmath>

namespace
{
    const float kPI = 3.14159265f;
    const float DEG_TO_RAD = kPI / 180.0f;
    const float COLLIDER_SCALE = 100.0f;
    const float ROT_EPSILON = 0.001f;

    LONG ToLong(float value)
    {
        return static_cast<LONG>(std::round(value));
    }
}

namespace ME
{
    BoxCollider2D::BoxCollider2D()
        : Collider(enums::eColliderType::Rect2D)
        , mRot(0.0f)
        , mbIsRotate(false)
        , mWidth(0.0f)
        , mHeight(0.0f)
        , mCentralPoint(Vector2(0.0f, 0.0f))
    {
        mPoints = {};
    }

    BoxCollider2D::~BoxCollider2D()
    {
    }

    void BoxCollider2D::Initialize()
    {
    }

    void BoxCollider2D::Update()
    {
        RefreshBounds();
    }

    void BoxCollider2D::LateUpdate()
    {
    }

    void BoxCollider2D::Render(HDC hdc)
    {
        Transform* tr = GetOwner()->GetComponent<Transform>();
        if (tr == nullptr)
            return;

        Vector2 pos = tr->GetPosition();
        Vector2 offset = GetOffset();

        HBRUSH transparentBrush = static_cast<HBRUSH>(GetStockObject(NULL_BRUSH));
        HBRUSH oldBrush = static_cast<HBRUSH>(SelectObject(hdc, transparentBrush));

        HPEN greenPen = CreatePen(PS_SOLID, 2, RGB(0, 255, 0));
        HPEN oldPen = static_cast<HPEN>(SelectObject(hdc, greenPen));

        if (mbIsRotate)
        {
            std::array<POINT, 4> renderPoints = mPoints;

            if (renderer::mainCamera)
            {
                for (int i = 0; i < 4; ++i)
                {
                    Vector2 worldPoint(
                        static_cast<float>(mPoints[i].x),
                        static_cast<float>(mPoints[i].y)
                    );

                    Vector2 screenPoint = renderer::mainCamera->CalculatePosition(worldPoint);

                    renderPoints[i].x = ToLong(screenPoint.x);
                    renderPoints[i].y = ToLong(screenPoint.y);
                }
            }

            // 디버그용 회전 Collider 표시
             Polygon(hdc, renderPoints.data(), 4);
        }
        else
        {
            if (renderer::mainCamera)
                pos = renderer::mainCamera->CalculatePosition(pos);

            float left = pos.x + offset.x;
            float top = pos.y + offset.y;
            float right = left + COLLIDER_SCALE * GetSize().x;
            float bottom = top + COLLIDER_SCALE * GetSize().y;

            // 디버그용 일반 Collider 표시
             Rectangle(hdc, ToLong(left), ToLong(top), ToLong(right), ToLong(bottom));
        }

        SelectObject(hdc, oldBrush);
        SelectObject(hdc, oldPen);
        DeleteObject(greenPen);
    }

    void BoxCollider2D::SetRotation(float rot)
    {
        mRot = rot;
        mbIsRotate = std::fabs(mRot) > ROT_EPSILON;

        // 회전값이 바뀌는 즉시 Collider Bounds도 갱신
        RefreshBounds();
    }

    void BoxCollider2D::RefreshBounds()
    {
        Transform* tr = GetOwner()->GetComponent<Transform>();
        if (tr == nullptr)
            return;

        Vector2 pos = tr->GetPosition();
        Vector2 offset = GetOffset();

        float x = pos.x + offset.x;
        float y = pos.y + offset.y;
        float width = COLLIDER_SCALE * GetSize().x;
        float height = COLLIDER_SCALE * GetSize().y;

        mbIsRotate = std::fabs(mRot) > ROT_EPSILON;

        if (mbIsRotate)
        {
            RotateCollider(mRot, x, y, width, height);
        }
        else
        {
            mWidth = width;
            mHeight = height;
            mCentralPoint = Vector2(x + width * 0.5f, y + height * 0.5f);

            mPoints[0] = POINT{ ToLong(x), ToLong(y) };
            mPoints[1] = POINT{ ToLong(x + width), ToLong(y) };
            mPoints[2] = POINT{ ToLong(x + width), ToLong(y + height) };
            mPoints[3] = POINT{ ToLong(x), ToLong(y + height) };
        }
    }

    Vector2 BoxCollider2D::RotateCollider(float rot, float x, float y, float width, float height)
    {
        float radian = rot * DEG_TO_RAD;

        float cosValue = std::cos(radian);
        float sinValue = std::sin(radian);

        Vector2 points[4];

        // 왼쪽 위
        points[0] = Vector2(x, y);

        // 오른쪽 위
        points[1] = Vector2(
            x + width * cosValue,
            y + width * sinValue
        );

        // 오른쪽 아래
        points[2] = Vector2(
            x + width * cosValue - height * sinValue,
            y + width * sinValue + height * cosValue
        );

        // 왼쪽 아래
        points[3] = Vector2(
            x - height * sinValue,
            y + height * cosValue
        );

        float minX = (std::min)({ points[0].x, points[1].x, points[2].x, points[3].x });
        float maxX = (std::max)({ points[0].x, points[1].x, points[2].x, points[3].x });

        float minY = (std::min)({ points[0].y, points[1].y, points[2].y, points[3].y });
        float maxY = (std::max)({ points[0].y, points[1].y, points[2].y, points[3].y });

        // 회전된 사각형을 감싸는 AABB 크기
        mWidth = maxX - minX;
        mHeight = maxY - minY;

        // AABB 중심점
        mCentralPoint = Vector2(
            (minX + maxX) * 0.5f,
            (minY + maxY) * 0.5f
        );

        // 디버그 렌더링용 회전 꼭짓점 저장
        for (int i = 0; i < 4; ++i)
        {
            mPoints[i].x = ToLong(points[i].x);
            mPoints[i].y = ToLong(points[i].y);
        }

        return mCentralPoint;
    }
}