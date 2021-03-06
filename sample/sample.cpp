#include <cstdio>
#include <unordered_map>
#include <vector>
#include "../lent.h"
#include <Windows.h>


struct profile {
	profile(const char* msg) : msg(msg) { QueryPerformanceCounter(&start); }
	
	~profile()
	{
		LARGE_INTEGER end, freq;
		QueryPerformanceCounter(&end);
		QueryPerformanceFrequency(&freq);

		const float t = float((end.QuadPart - start.QuadPart) / double(freq.QuadPart) * 1000);
		printf("%s : %.2f ms\n", msg, t);
	}

	LARGE_INTEGER start;
	const char* msg;
};


void test_create_components()
{
	profile p("create & destroy 2'000'000 components");

	struct cmp { float x, y, z; };

	std::vector<cmp> cmps;

	cmps.resize(2'000'000);
}


void test_iterate()
{
	struct cmp { int a; };
	std::vector<cmp> cmps;
	cmps.resize(2'000'000);

	profile p("iterate 2'000'000 components, single type");
	
	int sum = 0;
	for(cmp* c = &cmps[0], *e = c + cmps.size(); c < e; ++c) {
		sum += c->a;
	}
	printf("sum %d\n", sum);
}


void test_update()
{
	struct cmp_a { int a; };
	struct cmp_b { int a; };
	std::vector<cmp_a> a_cmps;
	std::vector<cmp_b> b_cmps;
	a_cmps.resize(2'000'000);
	b_cmps.resize(2'000'000);

	profile p("update 2'000'000 components, two component type");
	
	cmp_a* a = &a_cmps[0];
	cmp_b* b = &b_cmps[0];
	cmp_a* a_end = &a_cmps[0] + a_cmps.size();
	cmp_b* b_end = &b_cmps[0] + b_cmps.size();
	while(a != a_end) {
		a->a += b->a;
		++a;
		++b;
	}
}


void test_memory()
{
	profile p("create 10'000'000 entities, create component in one of them, destroy everything");

	struct cmp { int v; };

	std::unordered_map<lent_entity, cmp> cmps;

	cmp c;
	c.v = 10;
	cmps[{5'000'000}] = c;
}


void test_create_entities()
{
	profile p("create & destroy 2'000'000 entities");

	// DONE
}


int main(int argc, char* argv[])
{
	test_create_entities();
	test_create_components();
	test_iterate();
	test_update();
	test_memory();

	getchar();
}