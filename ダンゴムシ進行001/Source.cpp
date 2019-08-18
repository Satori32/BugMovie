#include <iostream>
#include <cstdint>
#include <vector>
#include <tuple>
#include <algorithm>
#include <cmath>


//http://mevius.5ch.net/test/read.cgi/tech/1564310397/137

namespace Dir {
	enum {
		Top = 0,
		Right = 1,
		Bottom = 2,
		Left = 3,
	};
}
typedef std::tuple<std::int64_t, std::int64_t, std::int8_t, bool> Data;//x,y,Dir,IsLeft.
typedef std::tuple < std::int64_t, std::int64_t> Point;
typedef std::vector<Point> Walls;

Data MakeBug(const Point& P) {

	return { std::get<0>(P) ,std::get<1>(P) ,Dir::Right,true };
}
Walls MakeWalls(Data& D, std::int8_t TD) {
	std::int64_t Y[] = { 1,0,-1,0 };
	std::int64_t X[] = { 0,1,0,-1 };
	Walls W;
	if (std::get<3>(D)) {	
		for (std::int64_t i = std::get<2>(D); ((i+(4*7))%4) != TD; i--) {
			W.push_back({ std::get<0>(D) + X[((i + (4*7)) % 4)], std::get<1>(D) + Y[(i + (4*7)) % 4] });
		}
	}
	else {
		for (std::int64_t i = std::get<2>(D); ((i+(4*7))%4) != TD; i++) {
			W.push_back({ std::get<0>(D) + X[((i+(4*7)) % 4)], std::get<1>(D) + Y[(i +(4*7)) % 4] });
		}

	}

	if(W.size())std::get<3>(D) = !std::get<3>(D);

	return W;

}
std::int8_t SearchDir(const Point& D) {
	std::int64_t Y[] = { 1,0,-1,0 };
	std::int64_t X[] = { 0,1,0,-1 };

	for (std::size_t i = 0; i < 4; i++) {
		if (std::get<0>(D) == X[i] && std::get<1>(D) == Y[i]) return i;
	}

	return -1;
}

Walls MakeHoge(const Point& Start, const Point& End) {
	Data B = MakeBug(Start);
	Walls W;


	std::int64_t PX = std::get<0>(End) - std::get<0>(B);
	std::int64_t PY= std::get<1>(End) - std::get<1>(B);

	std::int8_t SX = std::signbit(static_cast<double>(PX))?-1:1;
	std::int8_t Dir = SearchDir({ SX,0 });
	Walls TW = MakeWalls(B, Dir);

	W.insert(W.end(),TW.begin(), TW.end());
	TW.clear();
	std::get<0>(B) += PX;
	std::get<2>(B) = Dir;

	std::int8_t SY = std::signbit(static_cast<double>(PY))?-1:1;
	Dir = SearchDir({ 0,SY });
	TW = MakeWalls(B, Dir);

	W.insert(W.end(),TW.begin(), TW.end());
	std::get<1>(B) += PY;
	std::get<2>(B) = Dir;

	return W;
}
typedef std::vector<std::vector<char>> Screen;

bool Show(const Point& S, const Point& E, const Walls& W) {

	std::int64_t SCXMin = std::min(std::get<0>(E), std::get<0>(S));
	std::int64_t SCXMax = std::max(std::get<0>(E), std::get<0>(S));
	std::int64_t SCYMin = std::min(std::get<1>(E), std::get<1>(S));
	std::int64_t SCYMax = std::max(std::get<1>(E), std::get<1>(S));

	std::int64_t Width = (SCXMax - SCXMin);
	std::int64_t Height = (SCYMax - SCYMin);
	Screen Sc(Height + 4);

	for (auto& o : Sc) { o.resize(Width + 4, ' '); }
		Sc[-SCYMin+2][-SCXMin+2] = 'X';
	Sc[(std::get<1>(S) - SCYMin) + 2][(std::get<0>(S) - SCXMin) + 2] = 'S';
	Sc[(std::get<1>(E) - SCYMin) + 2][(std::get<0>(E) - SCXMin) + 2] = 'E';
	for (auto& o : W) {
		std::int64_t Y = (std::get<1>(o) - SCYMin) + 2;
		std::int64_t X = (std::get<0>(o) - SCXMin) + 2;
		Sc[Y][X] = '#';
	}

	for (std::int64_t i = Sc.size() - 1; i > 0; i--) {
		for (auto& o : Sc[i]) {
			std::cout << o;
		}
		std::cout << std::endl;
	}

	std::cout << '[' << std::get<0>(S) << ',' << std::get<1>(S) << ']' << std::endl;

	for (auto& o : W) {
		std::cout << '[' << std::get<0>(o) << ',' << std::get<1>(o) << ']';
	}
	std::cout << std::endl;

	std::cout << '[' << std::get<0>(E) << ',' << std::get<1>(E) << ']' << std::endl;

	return true;

}


int main() {
	//MakeHoge({ 1,1 }, { 2, 2 });
	Point S;
	Point E;
	Walls W;
	S = { 1,1 };
	E = { 2,2 };
	W = MakeHoge(S, E);
	Show(S, E, W);
	std::cout << "///////////" << std::endl;
	S = { 1,1 };
	E = { -2,2 };
	W = MakeHoge(S, E);
	Show(S, E, W);
	std::cout << "///////////" << std::endl;
	S = { 1,1 };
	E = { -2,-2 };
	W = MakeHoge(S, E);
	Show(S, E, W);
	std::cout << "///////////" << std::endl;
	S = { 1,1 };
	E = { 2,-2 };
	W = MakeHoge(S, E);
	Show(S, E, W);
	std::cout << "///////////" << std::endl;
	return 0;
}
