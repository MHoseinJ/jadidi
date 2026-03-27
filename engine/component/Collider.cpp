#include "Collider.h"
#include <cmath>
#include "scene/GameObject.h"

void BoxCollider::DeSerialize(const json &j) {
    if (j.contains("x")) {
        size.x = j["x"].get<float>();
    } else {
        size.x = 0;
    }
    if (j.contains("y")) {
        size.y = j["y"].get<float>();
    } else {
        size.y = 0;
    }
}

bool IsColliding(const BoxCollider *a, const BoxCollider *b)
{
    const float ax = a->owner->transform.position.x;
    const float ay = a->owner->transform.position.y;
    const float bx = b->owner->transform.position.x;
    const float by = b->owner->transform.position.y;

    const float dx = std::abs(ax - bx);
    const float dy = std::abs(ay - by);

    const float halfWidthA = a->size.x / 2.0f;
    const float halfHeightA = a->size.y / 2.0f;
    const float halfWidthB = b->size.x / 2.0f;
    const float halfHeightB = b->size.y / 2.0f;

    return (dx < (halfWidthA + halfWidthB)) &&
           (dy < (halfHeightA + halfHeightB));

}

// bool IsColliding(const Vector2 *a, const BoxCollider *b) {
//     const float bx = b->owner->transform.position.x;
//     const float by = b->owner->transform.position.y;
//
//     const float halfW = b->size.x / 2.0f;
//     const float halfH = b->size.y / 2.0f;
//
//     std::cout << "starting coordinate of box: x -> " << bx - halfW << std::endl;
//     std::cout << "starting coordinate of box: y -> " << by - halfH << std::endl;
//     std::cout << "ending coordinate of box: x -> " << halfW + bx << std::endl;
//     std::cout << "ending coordinate of box: y -> " << halfH + by << std::endl;
//     std::cout << "position on mouse: x -> " << a->x << std::endl;
//     std::cout << "position on mouse: y -> " << a->y << std::endl;
//
//     return (a->x >= bx && a->x <= bx + halfW) &&
//            (a->y >= by - halfH && a->y <= by + halfH);
// }

// bool IsColliding(const Vector2 a, const BoxCollider b) {
//     const int bx = static_cast<int>(b.owner->transform.position.x);
//     const int by = static_cast<int>(b.owner->transform.position.y);
//
//     const int bw = b.size.x / 2;
//     const int bh = b.size.y / 2;
//     const int dx = abs(a.x - bx);
//     const int dy = abs(a.y - by);
//
//     std::cout << "distance of mouse to box: x -> " << dx << std::endl;
//     std::cout << "distance of mouse to box: y -> " << dy << std::endl;
//     std::cout << "center of box: x -> " <<  bx << std::endl;
//     std::cout << "center of box: y -> " << by << std::endl;
//     std::cout << "position on mouse: x -> " << a.x << std::endl;
//     std::cout << "position on mouse: y -> " << a.y << std::endl;
//
//     if (dx <= bw && dy <= bh) {
//         return true;
//     }
//
//     return false;
// }

bool IsColliding(const Vector2* a, const BoxCollider* b) {

    const float bx = b->owner->transform.position.x;
    const float by = b->owner->transform.position.y;
    const float bw = b->size.x / 2.0f;
    const float bh = b->size.y / 2.0f;

    const float dx = std::abs(a->x - bx);
    const float dy = std::abs(a->y - by);

    std::cout << "distance of mouse to box: x -> " << dx << std::endl;
    std::cout << "distance of mouse to box: y -> " << dy << std::endl;
    std::cout << "center of box: x -> " <<  bx << std::endl;
    std::cout << "center of box: y -> " << by << std::endl;
    std::cout << "position on mouse: x -> " << a->x << std::endl;
    std::cout << "position on mouse: y -> " << a->y << std::endl;

    if (dx <= bw && dy <= bh) {
        return true;
    }
    return false;
}