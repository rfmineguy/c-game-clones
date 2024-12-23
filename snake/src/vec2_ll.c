#include "vec2_ll.h"

LL_FREE_IMPL(Vector2);
LL_CMP_VALUE_IMPL(Vector2) {
	return val1.x == val2.x && val1.y == val2.y;
}
LL_PUSH_BACK_IMPL(Vector2);
LL_PUSH_FRONT_IMPL(Vector2);
LL_POP_BACK_IMPL(Vector2);
LL_POP_FRONT_IMPL(Vector2);
LL_DELETE_IMPL(Vector2);
LL_CONTAINS_IMPL(Vector2);
