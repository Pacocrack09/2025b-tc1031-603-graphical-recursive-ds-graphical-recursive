#include <bits/stdc++.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <utility>
#include <algorithm>
#include <string>
using namespace std;

struct Turtle {
    long long x, y;   // current position (integer)
    int dir;          // 0=→, 1=↑, 2=←, 3=↓
    long long step;   // step size in pixels
    vector<pair<pair<long long,long long>, pair<long long,long long>>> segments;

    Turtle(long long x0, long long y0, long long s) : x(x0), y(y0), dir(0), step(s) {}

    void turnLeft()  { dir = (dir + 1) & 3; }
    void turnRight() { dir = (dir + 3) & 3; }

    void forward() {
        static const int dx[4] = {1, 0, -1, 0};
        static const int dy[4] = {0, 1, 0, -1};
        long long nx = x + dx[dir] * step;
        long long ny = y + dy[dir] * step;
        segments.push_back({{x,y}, {nx,ny}});
        x = nx; y = ny;
    }
};

void A(int n, Turtle& T);
void B(int n, Turtle& T);

// Axiom A; rules (90° turns between + and -)
void A(int n, Turtle& T) {
    if (n == 0) return;
    T.turnLeft();        // +
    B(n-1, T);
    T.forward();         // F
    T.turnRight();       // -
    A(n-1, T);
    T.forward();         // F
    A(n-1, T);
    T.turnRight();       // -
    T.forward();         // F
    B(n-1, T);
    T.turnLeft();        // +
}

void B(int n, Turtle& T) {
    if (n == 0) return;
    T.turnRight();       // -
    A(n-1, T);
    T.forward();         // F
    T.turnLeft();        // +
    B(n-1, T);
    T.forward();         // F
    B(n-1, T);
    T.turnLeft();        // +
    T.forward();         // F
    A(n-1, T);
    T.turnRight();       // -
}

static long long clamp_ll(long long v, long long lo, long long hi){
    return max(lo, min(hi, v));
}

int main(int argc, char* argv[]) {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    if (argc < 2) {
        cerr << "Usage: ./hilbert <order> [canvas=1024] [margin=10]\n";
        return 1;
    }
    int order = stoi(argv[1]);
    long long canvas = (argc >= 3) ? stoll(argv[2]) : 1024LL;
    long long margin = (argc >= 4) ? stoll(argv[3]) : 10LL;

    if (order < 1) {
        cerr << "Error: <order> must be >= 1\n";
        return 1;
    }
    if (canvas < 16) {
        cerr << "Error: canvas too small (min 16)\n";
        return 1;
    }
    if (margin < 0 || margin*2 >= canvas) {
        cerr << "Error: invalid margin\n";
        return 1;
    }

    long long gridSide = 1LL << order;           // 2^n
    long long stepsPerSide = gridSide - 1;       // steps between edges
    if (stepsPerSide == 0) stepsPerSide = 1;     // safety

    long long usable = (canvas - 1) - 2*margin;  // usable pixels
    if (usable <= 0) {
        cerr << "Error: margin too large for the canvas\n";
        return 1;
    }
    long long step = usable / stepsPerSide;
    if (step <= 0) {
        cerr << "Error: order too large for this canvas/margin (step=0)\n";
        return 1;
    }

    long long startX = margin;
    long long startY = margin;

    Turtle T(startX, startY, step);
    A(order, T);

    long long maxCoord = canvas - 1;
    for (auto& seg : T.segments) {
        seg.first.first  = clamp_ll(seg.first.first,  0, maxCoord);
        seg.first.second = clamp_ll(seg.first.second, 0, maxCoord);
        seg.second.first = clamp_ll(seg.second.first, 0, maxCoord);
        seg.second.second= clamp_ll(seg.second.second,0, maxCoord);
    }

    ofstream out("lines.txt");
    if (!out) {
        cerr << "Could not create lines.txt\n";
        return 1;
    }
    for (const auto& s : T.segments) {
        out << "(" << s.first.first  << "," << s.first.second  << ")"
            << "(" << s.second.first << "," << s.second.second << ")\n";
    }
    out.close();

    cerr << "OK: lines.txt generated. Segments: " << T.segments.size()
         << "  | order=" << order
         << "  | canvas=" << canvas
         << "  | margin=" << margin
         << "  | step=" << step << "\n";
    return 0;
}