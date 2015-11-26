#include <stdio.h>
#include "circle_queue.h"

using type::CircleQueue;

void test()
{
    CircleQueue<int> cq(10);
    for (int i = 0; i < 30; ++i) {
        cq.push(i);
        cq.PrintQueue();
    }
    for (int i = 0; i < 30; ++i) {
        printf("%d ", cq.front());
        cq.pop();
    }
    printf("\n");
}

int main()
{
    test();
    return 0;
}
