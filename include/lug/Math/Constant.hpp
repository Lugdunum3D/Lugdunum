#pragma once

#include <limits>

namespace lug {
namespace Math {

// TODO: Add pi, etc

template <typename T>
T epsilon();

template <typename T>
T pi();

template <typename T>
T twoPi();

template <typename T>
T rootPi();

template <typename T>
T halfPi();

template <typename T>
T threeOverTwoPi();

template <typename T>
T quarterPi();

template <typename T>
T oneOverPi();

template <typename T>
T oneOverTwoPi();

template <typename T>
T twoOverPi();

template <typename T>
T fourOverPi();

template <typename T>
T twoOverRootPi();

template <typename T>
T oneOverRootPi();

template <typename T>
T rootHalfPi();

template <typename T>
T rootTwoPi();

template <typename T>
T rootLnFour();

template <typename T>
T e();

template <typename T>
T euler();

template <typename T>
T rootTwo();

template <typename T>
T rootThree();

template <typename T>
T rootFive();

template <typename T>
T lnTwo();

template <typename T>
T lnTen();

template <typename T>
T lnLnTwo();

template <typename T>
T goldenRatio();

#include <lug/Math/Constant.inl>

}
}
