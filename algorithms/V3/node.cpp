#include "node.h"

Node::Node(int p_l, int p_w) {
    l = p_l;
    w = p_w;

    low_b = 0;
    upp_b = 0;

    x1 = 0;
    x2 = 0;
    y1 = 0;
    y2 = 0;

    reached_limit = false;
}
