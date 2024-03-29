#include <optional>
#include "lines.hpp"

/*
 * Using the cross product 'x', we must find an intersection point such that:
 *
 * P + t(R) = Q + u(S)
 * here:
 * P = line1.start
 * R = line1.end - line1.start
 * Q = line2.start
 * S = line2.end - line2.start
 * line1.start + t(line1.end - line1.start) = line2.start + u(line2.end - line2.start)
 * with t and u in [0, 1]
 *
 * There might be no intersection, in which case we return std::nullopt
 * */
std::optional<sf::Vector2f>
get_segments_intersection(Line line1, Line line2) {
  /*
   * We want to eliminate one of the variables (either t or u) to solve for the other.
   * To solve for "t" we want to eliminate "u" by taking the cross product of both sides with vector S:
   *
   * (P + tR) x S = (Q + uS) x S
   *
   * Giving:
   * (P + tR) x S = Q x S + u(S x S)
   *
   * Since S x S = 0 (cross product rule), we get:
   *
   * (P + tR) x S = Q x S + 0
   *
   * At this point, we have eliminated the variable "u" from the equation.
   * In the next steps, we rearrange the equation to isolate "t" and find its symbol.
   *
   * Rewriting left side of the equation:
   *
   * (P x S) + (t * (R x S)) = Q x S
   *
   * Next, we want to isolate the term containing the variable t, which is (tR x S).
   * To do this, we subtract (P x S) from both sides of the equation:
   *
   * (t * (R x S)) = (Q x S) - (P x S)
   *
   * Notice that both sides of the equation have a common factor, S.
   * Let's rewrite the equation to highlight this factor:
   *
   * (t * (R x S)) = (Q - P) x S
   *
   * Now, we have an equation that relates the variable t to the given line horizontal_segments through the cross product.
   * To solve for t, we divide both sides of the equation by the term (R x S):
   *
   * t = ((Q - P) x S) / (R x S)
   *
   * */

  /*
   * Now, to solve for "u", we want to eliminate "t" by taking the cross product of both sides with vector R:
   *
   * (P + tR) x R = (Q + uS) x R
   *
   * Giving:
   * (P x R) + t(R x R) = (Q + uS) x R
   *
   * Since R x R = 0 (cross product rule), we get:
   * (P x R) + 0 = (Q + uS) x R
   *
   * At this point, we have eliminated the variable "t" from the equation.
   * In the next steps, we rearrange the equation to isolate "u" and find its symbol.
   *
   * Rewriting right side of the equation:
   * (P x R) = (Q x R) + u(S x R)
   *
   * Next, we want to isolate the term containing the variable u, which is u(S x R).
   * To do this, we subtract (Q x R) from both sides of the equation:
   *
   * (P x R) - (Q x R) = u(S x R)
   *
   * Notice that both sides of the equation have a common factor, R.
   * Let's rewrite the equation to highlight this factor:
   *
   * (P - Q) x R = u(S x R)
   *
   * Now, we have an equation that relates the variable u to the given line horizontal_segments through the cross product.
   * To solve for u, we divide both sides of the equation by the term (S x R):
   *
   * u = ((P - Q) x R) / (S x R)
   *
   * To reduce the number of computation steps, it's convenient to rewrite this as follows:
   *
   * u = ((Q - P) x R) / (R x S)
   * (remembering that s × r = − r × s) (cross product rule)
   *
   * */
  sf::Vector2f P = line1.start;
  sf::Vector2f R = line1.end - line1.start;
  sf::Vector2f Q = line2.start;
  sf::Vector2f S = line2.end - line2.start;

  /*
   * Now, we need to compute the cross products for:
   * t = ((Q - P) x S) / (R x S)
   * u = ((Q - P) x R) / (R x S)
   *
   * Recall that the cross product of two 2D vectors V = (Vx, Vy) and W = (Wx, Wy) is calculated as:
   * V x W = Vx * Wy - Vy * Wx.
   * */

  // for t
  sf::Vector2f q_minus_p = Q - P;
  float q_minus_p_cross_s = q_minus_p.x * S.y - q_minus_p.y * S.x;
  float r_cross_s = R.x * S.y - R.y * S.x;

  // for u
  float q_minus_p_cross_r = q_minus_p.x * R.y - q_minus_p.y * R.x;
  // sf::Vector2f q_minus_p = Q - P; // already computed
  // float r_cross_s = R.x * S.y - R.y * S.x; // already computed


  /*
   * a) If R x S = 0 and (Q - P) x R = 0, the lines are collinear (lying on the same straight line).
   * b) If R x S = 0 and (Q - P) x R ≠ 0, the lines are parallel and NON-intersecting.
   * c) If R x S ≠ 0 and both 0 ≤ t ≤ 1 and 0 ≤ u ≤ 1, the lines intersect at the point P + tR = Q + uS.
   * d) If R x S ≠ 0 and either t or u is not between 0 and 1, the lines are not parallel but don't intersect.
   *
   * The method above works for lines in two-dimensional space.
   * It is a simplified version of a more general method that works for lines in three-dimensional space as well.
   *
   * */

  if (r_cross_s == 0 && q_minus_p_cross_r == 0) {
    // lines are collinear
    // TODO: handle this case
    return std::nullopt;
  } else if (r_cross_s == 0 && q_minus_p_cross_r != 0) {
    // lines are parallel and NON-intersecting
    return std::nullopt;
  }

  // r_cross_s != 0
  float t = q_minus_p_cross_s / r_cross_s;
  float u = q_minus_p_cross_r / r_cross_s;

  if (t >= 0 && t <= 1 && u >= 0 && u <= 1) {
    // lines intersect at the point P + t(R) = Q + u(S)
    return P + t * R;
  } else {
    // lines are not parallel but don't intersect
    return std::nullopt;
  }
}
