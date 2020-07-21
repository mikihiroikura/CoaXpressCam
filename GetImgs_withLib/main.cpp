#include <HSC/KAYACoaXpressClass.hpp>

#ifdef _DEBUG
#define LIB_EXT "d.lib"
#else
#define LIB_EXT ".lib"
#endif


#pragma comment(lib,"KAYACoaXpressLib" LIB_EXT)

int main() {
	kayacoaxpress cam;

	return 0;
}