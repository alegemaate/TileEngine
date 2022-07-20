#ifndef UTIL_BOUNDS_H_
#define UTIL_BOUNDS_H_

/**
 * @brief Utility struct for representing a bounding box
 *
 */
struct BoundingBox {
  int x{0};
  int y{0};
  int width{0};
  int height{0};

  bool collidesPoint(int x, int y) {
    BoundingBox other = {x, y, x, y};
    return collides(other);
  };

  bool collides(BoundingBox other) {
    return x < other.x + other.width && y < other.y + other.height &&
           other.x < x + width && other.y < y + height;
  }
};

#endif  // UTIL_BOUNDS_H_