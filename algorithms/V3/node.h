#ifndef PRACA_INZYNIERSKA_NODE_H
#define PRACA_INZYNIERSKA_NODE_H


class Node {
public:
    int l, w;

    int upp_b, low_b;
    int x1, x2, y1, y2;

    bool reached_limit;

    Node(int l, int w);
};


#endif //PRACA_INZYNIERSKA_NODE_H
